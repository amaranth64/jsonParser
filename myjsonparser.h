#ifndef MYJSONPARSER_H
#define MYJSONPARSER_H

#include <QDebug>

#include <QMap>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include "database/datatype.h"


class MyJSONParser
{
public:
    MyJSONParser();
    static QList<QString> getArrayText(QString fileName, QString section, QString array);
    static QString getParamText(QString fileName, QString section, QString param);

    bool openFile(QString fileName);
    QMap<QString, QString> getMapTextToText(QString section, QString array);
    QMap<QString, quint16> getMapTextToInt(QString section, QString array);
    QList<QString> getListText(QString section, QString array = "");

    QList<CellExcel> getListCells(QString section, QString array = "");
    QString getStringText(QString section, QString strName = "");
    quint16 getInt16Text(QString section, QString valueName = "");
    double getFloatText(QString section, QString valueName = "");

    QString getErrorString() const;

private:
    QString fName;
    QByteArray fileData;
    QByteArray fData;
    QString errorString;

    QJsonObject jsonFile;


    static QByteArray getFileData(QString fileName, bool &bOk);
};

#endif // MYJSONPARSER_H
