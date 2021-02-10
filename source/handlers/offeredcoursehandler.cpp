#include "header/handlers/offeredcoursehandler.h"

OfferedCourseHandler::OfferedCourseHandler()
{

}

OfferedCourseHandler::~OfferedCourseHandler()
{
    cleanUp();
}

bool OfferedCourseHandler::getIsEmpty() const
{
    return is_empty;
}

void OfferedCourseHandler::cleanUp()
{
    if (container.isEmpty()) return;
    for (QVariantList* element : qAsConst(container)) {
        delete element;
        element = nullptr;
    }
    container.clear();
}

void OfferedCourseHandler::normalizeTime(QString &time)
{
    time.replace(QStringLiteral("ك"), QStringLiteral("ک"));
    time = time.simplified();
}

bool OfferedCourseHandler::extractOfferedCourses(const QString& response)
{
    QRegularExpression re {xmldata_pattern, QRegularExpression::UseUnicodePropertiesOption};
    QRegularExpressionMatch match {re.match(response)};
    if (!match.hasMatch()) return false;

    QXmlStreamReader reader(match.captured());
    QVariantList* row_datas;
    QStringRef column_data_ref;
    QString column_data;
    QString course_id;
    QStringList splited_data;
    int column_count = OfferedCourseModel::columns.count();
    if (!reader.readNextStartElement()) return false;
    if (reader.name() != QStringLiteral("Root")) return false;

    while(reader.readNextStartElement()) {
        if(reader.name() != QStringLiteral("row")) continue;
        // the struture is not empty
        setIsEmpty(false);
        row_datas = new QVariantList;
        row_datas->reserve(column_count);
        for (int i {0}; i < column_count; ++i) row_datas->append(QVariant());
        /*
         * the order of the values which pushed into the row_data is important
         * because the order should be equal to the order of roleNames in the OfferedCourseModel
         * otherwise the information would be false
         */
        course_id = reader.attributes().value("C1").toString();
        splited_data = course_id.split("-");
        // course number
        row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::courseNumberRole), splited_data[0]);
        // group
        row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::groupRole), splited_data[1]);
        // course name
        row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::courseNameRole),
                           reader.attributes().value("C2").toString());
        // weight
        row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::weightRole),
                           reader.attributes().value("C3").toInt());
        // capacity
        row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::capacityRole),
                           reader.attributes().value("C5").toInt());

        column_data_ref = reader.attributes().value("C6");

        // sex
        if (column_data_ref == QStringLiteral("زن"))
            row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::sexRole), OfferedCourseModel::Female);

        else if (column_data_ref == QStringLiteral("مرد"))
            row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::sexRole), OfferedCourseModel::Male);

        else
            row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::sexRole), OfferedCourseModel::None);

        // teacher
        row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::teacherRole),
                           reader.attributes().value("C7").toString().remove(QStringLiteral("<BR>")));

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
        normalizeTime(column_data);
        // time
        row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::timeRole), column_data);

        // exam
        row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::examRole),
                           splited_data.last().remove(QStringLiteral("امتحان(")).remove(QStringLiteral("ساعت : "))
                                .replace(QStringLiteral(")"), QStringLiteral("<br>")));

        // place
        column_data_ref = reader.attributes().value("C9");
        int underline_pos {column_data_ref.lastIndexOf('_')};
        int endpos {column_data_ref.lastIndexOf("<BR>")};
        if (underline_pos != -1 && endpos != -1)
            // we skip 2 character which is '_ '
            row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::placeRole),
                               column_data_ref.mid(underline_pos + 2, endpos - underline_pos - 2).toString());
        else
            row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::placeRole),
                               column_data_ref.toString());

        // selected
//        row_datas->replace(false);
        row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::isChoosedRole),
                           (row_datas->at(0).toInt() % 2 == 0) ? true : false);
        container.insert(course_id, row_datas);
        reader.skipCurrentElement();
    }
    return true;
}

void OfferedCourseHandler::start()
{
        QDir::setCurrent("/home/moorko/cpp/boostan/boostan/test/");
        QFile file("res.html");
        if (file.open(QIODevice::ReadOnly)) {
            QString rr {file.readAll()};
            extractOfferedCourses(rr);
        } else {
            qDebug() << file.errorString();
        }
        setFinished(true);
        setSuccess(true);
}

void OfferedCourseHandler::sendDataTo(QObject* model)
{
    OfferedCourseModel* view_model = reinterpret_cast<OfferedCourseModel*>(model);
    view_model->setDataContainer(container);
    container.clear();
}

