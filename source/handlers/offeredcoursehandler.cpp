#include "header/handlers/offeredcoursehandler.h"

OfferedCourseHandler::OfferedCourseHandler()
    : container {new std::vector<std::vector<QVariant>*>}
{

}

OfferedCourseHandler::~OfferedCourseHandler()
{
    for (std::vector<QVariant>* element : *container) {
        delete element;
    }
    delete container;
}

bool OfferedCourseHandler::getIsEmpty() const
{
    return is_empty;
}

bool OfferedCourseHandler::extractOfferedCourses(const QString& response)
{
    QRegularExpression re {xmldata_pattern, QRegularExpression::UseUnicodePropertiesOption};
    QRegularExpressionMatch match {re.match(response)};
    if (!match.hasMatch()) return false;

    QXmlStreamReader reader(match.captured());
    std::vector<QVariant>* row_datas;
    QStringRef column_data_ref;
    QString column_data;
    QStringList splited_data;

    if (!reader.readNextStartElement()) return false;
    if (reader.name() != QStringLiteral("Root")) return false;

    while(reader.readNextStartElement()) {
        if(reader.name() != QStringLiteral("row")) continue;
        // the struture is not empty
        setIsEmpty(false);
        row_datas = new std::vector<QVariant>;
        /*
         * the order of the values which pushed into the row_data is important
         * because the order should be equal to the order of roleNames in the OfferedCourseModel
         * otherwise the information would be false
         */
        splited_data = reader.attributes().value("C1").toString().split("-");
        row_datas->emplace_back(splited_data[1]);                                   // group
        row_datas->emplace_back(splited_data[0]);                                   // course number
        row_datas->emplace_back(reader.attributes().value("C2").toString());        // course name
        row_datas->emplace_back(reader.attributes().value("C3").toInt());           // weight
        row_datas->emplace_back(reader.attributes().value("C5").toInt());           // capacity
        column_data_ref = reader.attributes().value("C6");

        // sex
        if (column_data_ref == QStringLiteral("زن"))
            row_datas->emplace_back(QStringLiteral("female"));

        else if (column_data_ref == QStringLiteral("مرد"))
            row_datas->emplace_back(QStringLiteral("male"));

        else
            row_datas->emplace_back(QStringLiteral("none"));

        // teacher
        row_datas->emplace_back(reader.attributes().value("C7").toString().remove(QStringLiteral("<BR>")));

        // time and exam
        column_data = reader.attributes().value("C8").toString();
        column_data.remove(QStringLiteral("درس(ت): "));

        splited_data = column_data.split(QStringLiteral("<BR>"));
        splited_data.pop_back();
        column_data.clear();
        // i decreased the size by 2 because i want to add the last class time without a <br> tag
        for (int i {0}; i < splited_data.size() - 2; ++i) {
            column_data += splited_data[i] + QStringLiteral("<br>");
        }
        column_data += splited_data[splited_data.size() - 2];

        // pushing time
        row_datas->emplace_back(column_data);

        // place
        column_data_ref = reader.attributes().value("C9");
        int underline_pos {column_data_ref.lastIndexOf('_')};
        int endpos {column_data_ref.lastIndexOf("<BR>")};
        if (underline_pos != -1 && endpos != -1)
            // we skip 2 character which is '_ '
            row_datas->emplace_back(column_data_ref.mid(underline_pos + 2, endpos - underline_pos - 2).toString());
        else
            row_datas->emplace_back(column_data_ref.toString());

        // pushing exam
        row_datas->emplace_back(splited_data.last()
                                .remove(QStringLiteral("امتحان("))
                                .remove(QStringLiteral("ساعت : "))
                                .replace(QStringLiteral(")"), QStringLiteral("<br>")));

        container->push_back(row_datas);
        reader.skipCurrentElement();
    }
    return true;
}

