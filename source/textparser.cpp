#include "header/textparser.h"
#include <QFile>

QHashString TextParser::Validators::extractFormValidators(const QString& response)
{
    QHashString result;
    QRegularExpression re(Validators::viewstate_pattern);
    QRegularExpressionMatch match = re.match(response);
    QString capture;
    if (!match.hasMatch()) return QHashString {};
    capture = match.captured();
    capture.remove(capture.size() -1 , 1);
    capture.remove("__VIEWSTATE\" value=\"");
    result["__VIEWSTATE"] = capture;

    re.setPattern(Validators::viewstate_gen_pattern);
    match = re.match(response);
    if (!match.hasMatch()) return QHashString {};
    capture = match.captured();
    capture.remove("__VIEWSTATEGENERATOR\" value=\"");
    result["__VIEWSTATEGENERATOR"] = capture;

    re.setPattern(Validators::event_val_pattern);
    match = re.match(response);
    if (!match.hasMatch()) return QHashString {};
    capture = match.captured();
    capture.remove(capture.size() -1 , 1);
    capture.remove("__EVENTVALIDATION\" value=\"");
    result["__EVENTVALIDATION"] = capture;

//    qDebug() << result;
    return result;
}

QHashString TextParser::Validators::extractTokens(const QString& response)
{
    if (Errors::hasError(response)) return QHashString {};

    QHashString tokens {{"u", ""}, {"su", ""}, {"ft", ""}, {"f", ""}, {"lt", ""}, {"ctck", ""}, {"seq", ""}, {"tck", ""}};
    QString capture;
    QRegularExpression re {Validators::tokens_pattern};
    QRegularExpressionMatch match {re.match(response)};

    if (!match.hasMatch()) return QHashString {};
    capture = match.captured().remove("SavAut(").remove("'");
    QStringList splited = capture.split(",");
    if (splited.size() < 7) return QHashString {};
    tokens["u"] = splited[0];
    tokens["su"] = splited[1];
    tokens["ft"] = splited[2];
    tokens["f"] = splited[3];
    tokens["lt"] = splited[4];
    tokens["ctck"] = splited[5];
    tokens["seq"] = splited[6];

    if (!response.contains("SetOpenerTck(") || response.contains("SetOpenerTck('')")) {
        tokens["tck"] = splited[5];
        tokens.remove("ctck");
    } else {
        re.setPattern(Validators::tck_pattern);
        match = re.match(response);
        if (!match.hasMatch()) return QHashString {};
        capture = match.captured().remove("SetOpenerTck('");
        tokens["tck"] = capture;
    }
    return tokens;
}

int TextParser::Errors::hasError(const QString& response)
{
    if (response.contains("ErrorArr = new Array()")) return Constants::Errors::NoError;
    int code {Errors::extractErrorCode(response)};
    if (code != Errors::NoCodeFound) return code;
    QHash<int, QString>::const_iterator it {Errors::error_keywords.cbegin()};
    for (; it != Errors::error_keywords.cend(); ++it) {
        if (response.contains(it.value())) {
            return it.key();
        }
    }
    return Constants::Errors::UnknownError;

}

int TextParser::Errors::extractErrorCode(const QString& response)
{
    int code_position {response.indexOf("کد")};
    QString code;
    if (code_position == -1) return Errors::NoCodeFound;

    int i = code_position + 2;
    while (response[i] != " ") {
        code.append(response[i]);
        ++i;
    }

    return code.toInt();
}

void TextParser::extractOfferedCourses(const QString& response)
{
//    QRegularExpression re {offered_course_pattern, QRegularExpression::UseUnicodePropertiesOption};
//    QRegularExpressionMatch match {re.match(response)};
//    if (match.hasMatch()) {
//        QFile f("courses.xml");
//        f.open(QIODevice::WriteOnly);
//        f.write(match.captured().toUtf8());
//        f.close();
//    }

    QXmlStreamReader reader(response);
    if (reader.readNextStartElement()) {
        if (reader.name() == "Root") {
            while(reader.readNextStartElement()) {
//                Q_ASSERT(reader.isStartElement() && reader.name() != QLatin1String("xbel"));
                if(reader.name() == "row") {
                    qDebug() << reader.attributes().value("C2").toString();
                    reader.skipCurrentElement();
                }
            }
        }
    }
    qDebug() << "end";
}

QString TextParser::extractStudentNumber(const QString &response)
{
    int position {response.indexOf("=&quot;")};
    QString stu_number;
    if (position == -1) return QString(); // return error
    // 7 is the lentgh of string we searched. we need to skip this string.
    int char_position {position + 7};
    while (response[char_position] != "&") {
        stu_number.append(response[char_position]);
        ++char_position;
    }
    return stu_number;
}
