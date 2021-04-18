#include "header/handlers/scoreshandler.h"

bool ScoresHandler::getIsEmpty() const
{
    return is_empty;
}

void ScoresHandler::start(const QString semester, const QString student_id)
{
    _semester = semester;
    _student_id = student_id;
//    requestTokens();

    QDir::setCurrent("/home/moorko/cpp/boostan/boostan/test/");
    QFile file("res2.html");
    if (file.open(QIODevice::ReadOnly)) {
        QString rr {file.readAll()};
        extractScores(rr);
        extractBirefScores(rr);
    } else {
        qDebug() << file.errorString();
    }
    setErrorCode(Errors::ExtractError);
    setSuccess(true);
    setFinished(true);

}

void ScoresHandler::getScoresOf(const int semester)
{
    _semester = semester;
    requestScores();

}

void ScoresHandler::requestTokens()
{
    connect(&request, &Network::complete, this, &ScoresHandler::parseTokens);
    QString tck_token {getTckToken()};
    request.setUrl(root_url + _scores_url + tck_token);
    request.addHeader("Cookie", getCookies().toUtf8());
    request.get();
}

void ScoresHandler::parseTokens(QNetworkReply &reply)
{
    disconnect(&request, &Network::complete, this, &ScoresHandler::parseTokens);
    QString data;
    if (!verifyResponse(reply, data)) {
        reply.deleteLater();
        setSuccess(false);
        setFinished(true);
    }

    reply.deleteLater();
    request_validators.insert(extractFormValidators(data));
    requestScores();
}

void ScoresHandler::requestScores()
{
    connect(&request, &Network::complete, this, &ScoresHandler::parseScores);
    setFinished(false);
    setIsEmpty(true);
    _need_custom_avg = false;
    _scores.clear();

    request.setUrl(root_url + _scores_url + request_validators["tck"]);
    request.addHeader("Content-Type", "application/x-www-form-urlencoded");
    request.addHeader("Cookie", getCookies().toUtf8());

    QString ticket_tbox {getTckToken()};
    QString param_txtmiddle {QString(QStringLiteral("<r F41251=\"%1\" F43501=\"%2\"/>")).arg(_student_id, _semester)};
    QString data{
            QStringLiteral("__VIEWSTATE=")             % QUrl::toPercentEncoding(request_validators["__VIEWSTATE"])
            % QStringLiteral("&__VIEWSTATEGENERATOR=")  % request_validators["__VIEWSTATEGENERATOR"]
            % QStringLiteral("&__EVENTVALIDATION=")     % QUrl::toPercentEncoding(request_validators["__EVENTVALIDATION"])
            % QStringLiteral("&TicketTextBox=")         % ticket_tbox
            % QStringLiteral("&TxtMiddle=")             % QUrl::toPercentEncoding(param_txtmiddle)
            % QStringLiteral("&Fm_Action=80&Frm_Type=&Frm_No=&XMLStdHlp=&ex=")
                };

    request.post(data.toUtf8());
}

void ScoresHandler::parseScores(QNetworkReply &reply)
{
    disconnect(&request, &Network::complete, this, &ScoresHandler::parseScores);
    bool parse_success {true};

    QString data;
    if (!verifyResponse(reply, data))
        parse_success = false;

    if (parse_success && (!extractScores(data) || !extractBirefScores(data))) {
        setErrorCode(Errors::ExtractError);
        parse_success = false;
    }

    reply.deleteLater();
    if (!parse_success) {
        setSuccess(false);
        setFinished(true);
        return;
    }
    setSuccess(true);
    setFinished(true);
}

void ScoresHandler::normalizeName(QString &name)
{
    name.replace(QStringLiteral("ك"), QStringLiteral("ک"));
    name.replace(QStringLiteral("ي"), QStringLiteral("ی"));
    name = name.simplified();
}

bool ScoresHandler::extractScores(const QString& data)
{
    const QString scores_pattern {QStringLiteral("T02XML='(<Root>[\\W\\w]+<\\/Root>)")};
    QRegularExpression re {scores_pattern, QRegularExpression::UseUnicodePropertiesOption};
    QRegularExpressionMatch match {re.match(data)};
    if (!match.hasMatch()) return false;

    QXmlStreamReader reader(match.captured(1));

    if (!reader.readNextStartElement())
        return false;

    if (reader.name() != QStringLiteral("Root"))
        return false;

    int weight_sum {0}, int_weight;
    float scores_sum {0};
    QString name, score, score_result, score_status, weight;
    while(reader.readNextStartElement()) {
        if(reader.name() != QStringLiteral("N"))
            continue;

        QXmlStreamAttributes attribute {reader.attributes()};

        // course name
        name = attribute.value(QStringLiteral("F0200")).toString();
        normalizeName(name);

        // score
        score = attribute.value(QStringLiteral("F3945")).toString();
        // score result
        score_result = attribute.value(QStringLiteral("F3965")).toString();
        // score status
        score_status = attribute.value(QStringLiteral("F3955")).toString();
        weight = attribute.value(QStringLiteral("F0205")).toString();
        int_weight = weight.toInt();

        int status = Temporary;
        weight_sum += int_weight;

        if (score_status.startsWith("حذف")) {
            status = Deleted;
            score = "-";
            weight_sum -= int_weight;
        } else if (score.isEmpty()) {
            status = Undefined;
            score = "-";
            weight_sum -= int_weight;
        } else if (score_result.startsWith(QStringLiteral("قبول"))) {
            status = Passed;
        } else if (score_result.startsWith(QStringLiteral("رد"))) {
            status = Failed;
        }

        scores_sum += (score.toFloat() * int_weight);
        if (status == Temporary)
            _need_custom_avg = true;

        _scores.append(
                    QVariantMap {
                        {QStringLiteral("name"), name},
                        {QStringLiteral("weight"), weight},
                        {QStringLiteral("score"), score},
                        {QStringLiteral("status"), status}
                                }
                    );

        reader.skipCurrentElement();
    }

    if (!_scores.isEmpty()) {
        setIsEmpty(false);

        if (_need_custom_avg)
            _custom_average = scores_sum / weight_sum;
    }

    return true;
}

bool ScoresHandler::extractBirefScores(const QString& data)
{
    const QString scores_pattern {QStringLiteral("T01XML='(<Root>[\\W\\w]+<\\/Root>)")};
    QRegularExpression re {scores_pattern, QRegularExpression::UseUnicodePropertiesOption};
    QRegularExpressionMatch match {re.match(data)};
    if (!match.hasMatch()) return false;

    QString texts {match.captured(1)};
    QString average = _need_custom_avg ? QString::number(_custom_average)  : extractXmlAttr(texts, QStringLiteral("F4360=\""));
    if (average.isEmpty())
        average = QStringLiteral("-");
    _score_brief.insert(QStringLiteral("average"), average);
    _score_brief.insert(QStringLiteral("passedUnits"), extractXmlAttr(texts, QStringLiteral("F4370=\""), false));
    _score_brief.insert(QStringLiteral("semesterUnits"), extractXmlAttr(texts, QStringLiteral("F4365=\""), false));
    _score_brief.insert(QStringLiteral("totalAvg"), extractXmlAttr(texts, QStringLiteral("F4360=\""), false));
    _score_brief.insert(QStringLiteral("totalPassedUnits"), extractXmlAttr(texts, QStringLiteral("F4370=\""), false));
    return true;
}

QString ScoresHandler::extractXmlAttr(const QString &data, const QString& key, const bool search_at_start) const
{
    static int start_point {0};
    int start_index, end_index;

    if (search_at_start)
        start_index = data.indexOf(key);
    else
        start_index = data.indexOf(key, start_point);

    if (start_index == -1)
        return QString();

    start_index += 7;
    end_index = data.indexOf(QChar('"'), start_index);
    start_point = start_index;
    return data.mid(start_index, end_index - start_index);
}

ScoresHandler::ScoresHandler()
{

}

QVariantList ScoresHandler::getScores() const
{
    return _scores;
}

QVariantList ScoresHandler::getBriefScores() const
{
    return QVariantList {_score_brief};
}
