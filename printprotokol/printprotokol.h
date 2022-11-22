#ifndef PRINTPROTOKOL_H
#define PRINTPROTOKOL_H

#include <QObject>
#include <QDebug>
#include <QMap>
#include <QFile>
#include <QColor>
#include <QAxObject>
#include <QAxBase>
#include <QCoreApplication>

#include "windows.h"
#include "database/datatype.h"

class PrintProtokol : public QObject
{
    Q_OBJECT
public:
    explicit PrintProtokol(QObject *parent = nullptr);
    ~PrintProtokol();

    void setFileName(QString name);
    bool printPrirabotka(QMap<CellExcel, QString> ta);

signals:

private:
    QString fileName="";
    bool setCellText(const QString& data);

    QAxObject* wordApp;
    QAxObject* wordDoc;

    // получаем указатель на Excel
    QAxObject *mExcel;
    // указатель на Workbooks
    QAxObject *excelWB;

    // указатель на документ (файл)
    QAxObject *excelDoc;

    // указатель на листы (снизу вкладки)
    QAxObject *excelSheets;

    // указатель на текущий лист
    QAxObject *currentSheet;






};

#endif // PRINTPROTOKOL_H
