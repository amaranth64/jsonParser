#include "printprotokol.h"

PrintProtokol::PrintProtokol(QObject *parent) : QObject(parent)
{
//    wordApp = new QAxObject("Word.Application", this);
//    wordApp->dynamicCall("SetVisible(bool)", false);
//    wordApp->setProperty("DisplayAlerts", "0");
//    wordDoc = wordApp->querySubObject("Documents");

//    wordApp = new QAxObject("Excel.Application", this);
//    wordApp->dynamicCall("SetVisible(bool)", false);
//    wordApp->setProperty("DisplayAlerts", "0");
//    wordDoc = wordApp->querySubObject("Workbooks");


//    // получаем указатель на Excel
//    wordApp = new QAxObject("Excel.Application",this);
//    // невидимый
//    wordApp->dynamicCall("SetVisible(bool)", false); // невидимый
//    // без предупреждений
//    wordApp->setProperty("DisplayAlerts", "0"); // без предупреждений
//    // получаем указатель на книги
//    wordDoc = wordApp->querySubObject("Workbooks");
//    qDebug() << "create ExcelDocument()";


    // получаем указатель на Excel
    mExcel = new QAxObject("Excel.Application", this);

    // Excel невидимый
    mExcel->dynamicCall("SetVisible(bool)", false);

    // Excel без предупреждений
    mExcel->setProperty("DisplayAlerts", "0");

}

PrintProtokol::~PrintProtokol()
{
    mExcel->dynamicCall("Quit(void)");

    delete mExcel;
}

void PrintProtokol::setFileName(QString name)
{
    fileName = name;
}

// data <row, text>
bool PrintProtokol::printPrirabotka(QMap<CellExcel, QString> ta) // <col,<row, data>>
{

    QString filePath = QCoreApplication::applicationDirPath().replace("/","\\") + "\\protokols\\" + fileName;
    QString tempPath = QCoreApplication::applicationDirPath().replace("/","\\") + "\\protokols\\temp.xls";
    bool bOk = QFile::exists(filePath);
    if (!bOk) {
        return false;
    }

    if (QFile::exists(tempPath)){
        qDebug() << "Delete old temp.xlsx"; qApp->processEvents();
        QFile::remove(tempPath);
    }
    QFile::copy(filePath, tempPath);

    // указатель на Workbooks
    QAxObject *excelWB = mExcel->querySubObject("Workbooks");

    // указатель на документ
    QAxObject *excelDoc = excelWB->querySubObject("Open(const QString&)", tempPath);

    // на листы (снизу вкладки)
    QAxObject *excelSheets = excelDoc->querySubObject("Sheets");

    // указатели на листы
    QAxObject *sheet1 = excelSheets->querySubObject("Item(const QVariant&)", QVariant("Лист1"));
    QAxObject *sheet2 = excelSheets->querySubObject("Item(const QVariant&)", QVariant("Лист2"));
    QAxObject *sheet3 = excelSheets->querySubObject("Item(const QVariant&)", QVariant("Лист3"));

    QAxObject* cell;

    foreach (CellExcel c, ta.keys()){

        //foreach(QString row, ta[regime].keys()) {

            // получение указателя на ячейку [row][col] ((!)нумерация с единицы)
        if (c.sheet == "Лист1") cell = sheet1->querySubObject("Cells(QVariant,QVariant)", c.row, c.col); //row, col;
        if (c.sheet == "Лист2") cell = sheet2->querySubObject("Cells(QVariant,QVariant)", c.row, c.col); //row, col;
        if (c.sheet == "Лист3") cell = sheet3->querySubObject("Cells(QVariant,QVariant)", c.row, c.col); //row, col;
            // вставка значения переменной data (любой тип, приводимый к QVariant) в полученную ячейку
            cell->setProperty("Value", QVariant(ta[c]));
            //cell->setProperty("Value", ta[regime].value(row));
            qDebug() << c.sheet<< c.row << c.col << ta[c];

        //}
    }

    // освобождение памяти
    cell->deleteLater();
    sheet1->deleteLater();
    sheet2->deleteLater();
    sheet3->deleteLater();

//    wordOpenDoc->dynamicCall("PrintOut()");

//    Sleep(1000);
//    qApp->processEvents();

    QString path = "SaveAs(" + tempPath + ")";
    std::string stlPath = path.toStdString();
    excelDoc->dynamicCall(stlPath.c_str());

    excelDoc->dynamicCall("Close()");
    excelDoc->deleteLater();
    return true;

}

bool PrintProtokol::setCellText(const QString& data)
{
//    // получение указателя на ячейку [row][col] ((!)нумерация с единицы)
//    QAxObject* cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", row, col);
//    // вставка значения переменной data (любой тип, приводимый к QVariant) в полученную ячейку
//    cell->setProperty("Value", QVariant(data));
//    // освобождение памяти
//    delete cell;
//    return res.toBool();
}
