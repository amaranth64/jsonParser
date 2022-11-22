#include "myjsonparser.h"

#include <QFile>
#include <QTextCodec>

MyJSONParser::MyJSONParser()
{

}

QList<QString> MyJSONParser::getArrayText(QString fileName, QString section, QString array)
{
    QList<QString> m;

    bool bOk;
    QByteArray saveData = getFileData(fileName, bOk);
    if (!bOk) {
        qDebug() << "Can't read settings" ;
        return QList<QString>();
    }

    // parsing
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(saveData, &error);

    if (error.error != QJsonParseError::NoError) {
        qDebug() << "Error: " << error.errorString() << error.offset << error.error;
        return QList<QString>();
    }

    if (doc.isObject())
    {
        QJsonObject json = doc.object();
        QJsonValue jsonSection = json.value(section);

        if (jsonSection.isObject())
        {

            QJsonObject obj = jsonSection.toObject();
            QJsonArray ar = obj[array].toArray();

            foreach (QVariant a, ar){
                qDebug() << a.toString();
                m.append(a.toString());
            }

        }

    }
    return  m;

}

QString MyJSONParser::getParamText(QString fileName, QString section, QString param)
{
    QString m;

    bool bOk;
    QByteArray saveData = getFileData(fileName, bOk);
    if (!bOk) {
        qDebug() << "Can't read settings" ;
        return QString();
    }

    // parsing
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(saveData, &error);

    if (error.error != QJsonParseError::NoError) {
        qDebug() << "Error: " << error.errorString() << error.offset << error.error;
        return QString();
    }

    if (doc.isObject())
    {
        QJsonObject json = doc.object();
        QJsonValue jsonSection = json.value(section);

        if (jsonSection.isObject())
        {

            QJsonObject obj = jsonSection.toObject();
            m = obj[param].toString();
        }
    }
    return  m;
}




bool MyJSONParser::openFile(QString fileName)
{
    bool bOk = false;
    errorString.clear();

    fData = getFileData(fileName, bOk);
    if (!bOk) {
        qDebug() << "File is not exists or can't read." ;
        errorString = "File is not exists or can't read.";
        return false;
    }
    fName = fileName;

    // parsing
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(fData, &error);

    if (error.error != QJsonParseError::NoError) {
        qDebug() << "Error: " << error.errorString() << error.offset << error.error;
        errorString = error.errorString() + "-" + error.offset + "-" + error.error;
        return false;
    }

    if (doc.isObject())
    {
        jsonFile = doc.object();
    } else {
        errorString = "Cant find main object.";
        return false;
    }

    return true;
}

// функция для чтения "section": { "array": [["1","J"],["2","K"],["3","L"]] }
// array как QMap<QString, QString>
QMap<QString, QString> MyJSONParser::getMapTextToText(QString section, QString array)
{
    QMap<QString, QString> m;

    QJsonValue jsonSection = jsonFile.value(section);

    if (jsonSection.isObject())
    {

        QJsonObject obj = jsonSection.toObject();
        QJsonArray ar = obj[array].toArray();

        foreach (QJsonValue a, ar){
            QJsonArray b = a.toArray();
            m[b[0].toString()]= b[1].toString();
        }
    }
    return m;
}

// функция для чтения "section": { "array": [["3_1", 121], ["3_2", 122], ["3_3", 123]]}
// array как QMap<QString, quint16>
QMap<QString, quint16> MyJSONParser::getMapTextToInt(QString section, QString array)
{
    QMap<QString, quint16> m;

    QJsonValue jsonSection = jsonFile.value(section);

    if (jsonSection.isObject())
    {

        QJsonObject obj = jsonSection.toObject();
        QJsonArray ar = obj[array].toArray();

        foreach (QJsonValue a, ar){
            QJsonArray b = a.toArray();
            m[b[0].toString()]= b[1].toInt();
        }
    }
    return m;
}

// функция для чтения "section": { "array": ["text1", "text2", "text3"]} - array как список QString
// или "section": ["text1", "text2", "text3"] - section как список QString
QList<QString> MyJSONParser::getListText(QString section, QString array)
{
    QList<QString> m;

    QJsonValue jsonSection = jsonFile.value(section);

    if (jsonSection.isObject())
    {
        // если section является объектом {   }
        QJsonObject obj = jsonSection.toObject();
        QJsonArray ar = obj[array].toArray();

        foreach (QVariant a, ar){
            m.append(a.toString());
        }
    } else {
        // если section просто массив "section": ["text1", "text2", "text3"]
        QJsonArray ar = jsonSection.toArray();

        foreach (QVariant a, ar){
            m.append(a.toString());
        }
    }


    return m;

}

QList<CellExcel> MyJSONParser::getListCells(QString section, QString array)
{
    QList<CellExcel> m;

    QJsonValue jsonSection = jsonFile.value(section);

    if (jsonSection.isObject())
    {

        QJsonObject obj = jsonSection.toObject();
        QJsonArray ar = obj[array].toArray();

        foreach (QJsonValue a, ar){
            QJsonArray b = a.toArray();
            CellExcel cell;
            cell.sheet = a[0].toString();
            cell.col = a[1].toString();
            cell.row = a[2].toString();
            m.append(cell);
        }
    }
    return m;
}

// функция для чтения "section": { "strName": "text1"} или "section": "text1"
QString MyJSONParser::getStringText(QString section, QString strName)
{
    QString m;

    QJsonValue jsonSection = jsonFile.value(section);

    if (jsonSection.isObject())
    {
        QJsonObject obj = jsonSection.toObject();
        m = obj[strName].toString();
    } else {
        m = jsonSection.toString();
    }

    return m;
}


// функция для чтения "section": { "valueName": 123} или "section": 123
quint16 MyJSONParser::getInt16Text(QString section, QString valueName)
{
    quint16 m = 0;

    QJsonValue jsonSection = jsonFile.value(section);

    if (jsonSection.isObject())
    {
        QJsonObject obj = jsonSection.toObject();
        m = obj[valueName].toInt();
    } else {
        m = jsonSection.toInt();
    }


    return m;
}

// функция для чтения "section": { "valueName": 1.235} или "section": 1.235
double MyJSONParser::getFloatText(QString section, QString valueName)
{
    double m = 0.0;

    QJsonValue jsonSection = jsonFile.value(section);

    if (jsonSection.isObject())
    {
        QJsonObject obj = jsonSection.toObject();
        m = obj[valueName].toDouble();
    } else {
        m = jsonSection.toDouble();
    }

    return m;
}



QByteArray MyJSONParser::getFileData(QString fileName, bool &bOk)
{
    bOk = true;
    if (!(QFile::exists(fileName))) {
        qDebug() << "Error: File don't exists" ;
        bOk = false;
        return QByteArray();
    }


    QFile file;
    file.setFileName(fileName);

    if (!(file.open(QIODevice::ReadOnly | QIODevice::Text))) {
        qDebug() << "Error: File not open" ;
        bOk = false;
        return QByteArray();
    }


    QByteArray saveData = file.readAll();
    file.close();
    file.deleteLater();
    return saveData;
}


QString MyJSONParser::getErrorString() const
{
    return errorString;
}
