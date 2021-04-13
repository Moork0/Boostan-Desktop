#include "header/handlers/scoreshandler.h"

bool ScoresHandler::getIsEmpty() const
{
    return is_empty;
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

    QString name, score, score_result, score_status;
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

        int status = Temporary;

        if (score_status.startsWith("حذف")) {
            status = Deleted;
            score = "-";
        } else if (score.isEmpty()) {
            status = Undefined;
            score = "-";
        } else if (score_result.startsWith(QStringLiteral("قبول"))) {
            status = Passed;
        } else if (score_result.startsWith(QStringLiteral("رد"))) {
            status = Failed;
        }

        _scores.append(
                    QVariantMap {
                        {QStringLiteral("name"), name},
                        {QStringLiteral("weight"), attribute.value(QStringLiteral("F0205")).toString()},
                        {QStringLiteral("score"), score},
                        {QStringLiteral("status"), status}
                                }
                    );

        reader.skipCurrentElement();
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
    _score_brief.insert(QStringLiteral("average"), extractXmlAttr(texts, QStringLiteral("F4360=\"")));
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

void ScoresHandler::start()
{
    _scores.clear();
    QDir::setCurrent("/home/moorko/cpp/boostan/boostan/test/");
    QFile file("res2.html");
    if (file.open(QIODevice::ReadOnly)) {
        QString rr {file.readAll()};
        extractScores(rr);
        extractBirefScores(rr);
    } else {
        qDebug() << file.errorString();
    }
    setSuccess(true);
    setFinished(true);
}

QVariantList ScoresHandler::getScores() const
{
    return _scores;
}

QVariantList ScoresHandler::getBriefScores() const
{
    return QVariantList {_score_brief};
}
