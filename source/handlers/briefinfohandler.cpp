#include "header/handlers/briefinfohandler.h"

BriefInfoHandler::BriefInfoHandler() : _locale{QLocale::Persian, QLocale::Iran}, _current_year{0}
{
    // we don't need to numbers being separated by thousands
    _locale.setNumberOptions(QLocale::OmitGroupSeparator);
    // In this occasion, Unavailability of data is not acceptable.
    // So this error is Critical in this situation.
    // 18 is the number of error code which indicate the Limited access to the content.
    _error_handler.setCriticalStatus(18, Errors::Critical);
}

int BriefInfoHandler::getCurrentYear() const
{
    return _current_year;
}

/*
 * Create and return a locale-aware QVariantMap from student_info
*/
QVariantMap BriefInfoHandler::getStudentInfo() const
{
    QVariantMap data {_student_info};
    data[_info_title[Index_Id]] = _locale.toString(data[_info_title[Index_Id]].toULongLong());
    data[_info_title[Index_Passed]] = _locale.toString(static_cast<int>(data[_info_title[Index_Passed]].toFloat()));
    data[_info_title[Index_TotalAvg]] = _locale.toString(data[_info_title[Index_TotalAvg]].toFloat());
    return data;
}

void BriefInfoHandler::start()
{
    requestTokens();
}

QStringList BriefInfoHandler::getSemesterAvgs() const
{
    return _passed_semesters_avg;
}

/*
 * Create and return a list of human readable(by removing the '3' at front and using locale) passed semesters
*/
QStringList BriefInfoHandler::getSemesterYears() const
{
    QStringList list;
    for (int year : _passed_semesters) {
        // years are like this: 3XXX. we just need the XXX part.
        list << _locale.toString(year).remove(0, 1);
    }
    return list;
}

QList<int> BriefInfoHandler::getRawSemesters() const
{
    return _passed_semesters;
}

bool BriefInfoHandler::requestTokens()
{
    connect(&_request, &Network::complete, this, &BriefInfoHandler::parseTokens);
    QString tck_token {getTckToken()};
    _request.setUrl(_root_url + _user_info_url + tck_token);
    _request.addHeader("Cookie", getCookies().toUtf8());
    return _request.get();
}

void BriefInfoHandler::parseTokens(QNetworkReply& reply)
{
    disconnect(&_request, &Network::complete, this, &BriefInfoHandler::parseTokens);
    QString data;
    if (!verifyResponse(reply, data)) {
        reply.deleteLater();
        setSuccess(false);
        setFinished(true);
        return;
    }

    reply.deleteLater();
    _request_validators.insert(extractFormValidators(data));

    requestStuId();
}

bool BriefInfoHandler::requestStuId()
{
    connect(&_request, &Network::complete, this, &BriefInfoHandler::parseStuId);
    _request.setUrl(_root_url + _user_info_url + _request_validators["tck"]);
    _request.addHeader("Content-Type", "application/x-www-form-urlencoded");
    _request.addHeader("Cookie", getCookies().toUtf8());

    QString ticket_tbox {getTckToken()};
    QString data{QStringLiteral("__VIEWSTATE=")             % QUrl::toPercentEncoding(_request_validators["__VIEWSTATE"])
                % QStringLiteral("&__VIEWSTATEGENERATOR=")  % _request_validators["__VIEWSTATEGENERATOR"]
                % QStringLiteral("&__EVENTVALIDATION=")     % QUrl::toPercentEncoding(_request_validators["__EVENTVALIDATION"])
                % QStringLiteral("&TicketTextBox=")         % ticket_tbox
                % QStringLiteral("&Fm_Action=00&Frm_Type=&Frm_No=&XMLStdHlp=&TxtMiddle=%3Cr%2F%3E&ex=")};

    return _request.post(data.toUtf8());
}

void BriefInfoHandler::parseStuId(QNetworkReply& reply)
{
    disconnect(&_request, &Network::complete, this, &BriefInfoHandler::parseStuId);
    bool parse_success {true};

    QString data, student_id;
    if (!verifyResponse(reply, data))
        parse_success = false;

    student_id = extractStuId(data);
    if (parse_success && student_id.isEmpty()) {
        setErrorCode(Errors::ExtractError);
        parse_success = false;
    }

    reply.deleteLater();
    if (!parse_success) {
        setSuccess(false);
        setFinished(true);
        return;
    }

    _request_validators.insert(extractFormValidators(data));
    _student_info["id"] = student_id;
    requestBriefInfo();
}

bool BriefInfoHandler::requestBriefInfo()
{
    connect(&_request, &Network::complete, this, &BriefInfoHandler::parseUserInfo);
    _request.setUrl(_root_url + _user_info_url + _request_validators["tck"]);
    _request.addHeader("Content-Type", "application/x-www-form-urlencoded");
    _request.addHeader("Cookie", getCookies().toUtf8());

    QString ticket_tbox {getTckToken()};
    QString data{QStringLiteral("__VIEWSTATE=")                  % QUrl::toPercentEncoding(_request_validators["__VIEWSTATE"])
                % QStringLiteral("&__VIEWSTATEGENERATOR=")       % _request_validators["__VIEWSTATEGENERATOR"]
                % QStringLiteral("&__EVENTVALIDATION=")          % QUrl::toPercentEncoding(_request_validators["__EVENTVALIDATION"])
                % QStringLiteral("&TicketTextBox=")              % ticket_tbox
                % QStringLiteral("&TxtMiddle=%3Cr+F41251%3D%22") % _student_info["id"].toString()
                % QStringLiteral("%22%2F%3E&Fm_Action=08&Frm_Type=&Frm_No=&XMLStdHlp=&ex=")};

    return _request.post(data.toUtf8());
}

void BriefInfoHandler::parseUserInfo(QNetworkReply& reply)
{
    disconnect(&_request, &Network::complete, this, &BriefInfoHandler::parseUserInfo);
    bool parse_success {true};

    QString data;
    if (!verifyResponse(reply, data))
        parse_success = false;

    if (parse_success && (!extractStudentInfo(data) || !extractStudentAvgs(data))) {
        setErrorCode(Errors::ExtractError);
        parse_success = false;
    }

    reply.deleteLater();
    if (!parse_success) {
        setSuccess(false);
        setFinished(true);
        return;
    }

    emit studentInfoChanged();
    setSuccess(true);
    setFinished(true);
    return;
}

bool BriefInfoHandler::extractStudentInfo(const QString& response)
{
    const QList<QString> keywords {QStringLiteral("F17551"),        // Field
                                   QStringLiteral("F41351"),        // StudyType
                                   QStringLiteral("F41701"),        // TotalAvg
                                   QStringLiteral("F41801")};      // Passed
    int position;
    QString value;
    /* increased Index_START by 2 because we want to skip the Id since we don't have Id in this data.
      (we did that in extractStuId) */
    for (int title_index{Index_START + 2}, keyindex{0}; title_index < Index_END; ++title_index, ++keyindex) {
        position = response.indexOf(keywords[keyindex]);
        if (position == -1) return false;

        // 10 is the lentgh of keyword which for example is "F51851 = '"
        // so we need to skip this
        position += 10;
        for (int i{position}; response[i] != "'"; ++i) {
            value.append(response[i]);
        }
        _student_info[_info_title[title_index]] = value;
        value.clear();
    }
    return true;
}

bool BriefInfoHandler::extractStudentAvgs(const QString &response)
{
    int year_position, avg_position;
    QString year_value, avg_value;
    const QString year_keyword {QStringLiteral("F4350")}, avg_keyword {QStringLiteral("F4360")};
    year_position = response.indexOf(year_keyword);
    avg_position = response.indexOf(avg_keyword);
    if (year_position == -1 || avg_position == -1) {
        return false;
    }

    while (year_position != -1 && avg_position != -1) {
        // 7 is the lentgh of actual keywords which for example is 'F4350="'
        // so we need to skip these characters
        year_position += 7;
        avg_position += 7;
        for (int i{year_position}; response[i] != '"'; ++i) {
            year_value.append(response[i]);
        }

        for (int i{avg_position}; response[i] != '"'; ++i) {
            avg_value.append(response[i]);
        }
        _passed_semesters.append(year_value.toInt());
        _passed_semesters_avg.append(avg_value);

        year_value.clear();
        avg_value.clear();
        year_position = response.indexOf(year_keyword, year_position);
        avg_position = response.indexOf(avg_keyword, avg_position);
    }
    _current_year = _passed_semesters.last();

    if (_passed_semesters_avg.last().isEmpty()) {
        _passed_semesters_avg.pop_back();
        _passed_semesters.pop_back();
    }
    emit currentYearChanged();
    return true;
}

QString BriefInfoHandler::extractStuId(const QString &response)
{
    int position {response.indexOf(QStringLiteral("=&quot;"))};
    QString stu_number;
    if (position == -1) return QString(); // return error
    // 7 is the lentgh of string we searched. we need to skip this string.
    int char_position {position + 7};
    while (response[char_position] != '&') {
        stu_number.append(response[char_position]);
        ++char_position;
    }
    return stu_number;
}
