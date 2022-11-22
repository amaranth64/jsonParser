#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAxObject>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qRegisterMetaType<IzdelieData>("IzdelieData");
    qRegisterMetaType<CellExcel>("CellExcel");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString myPath = qApp->applicationDirPath();
    myPath.replace("/","\\");

    QMap<CellExcel, QString> dataProtokol;

    MyJSONParser m;
    m.openFile("file.json");
    QList<QString> lanName;
    QList<QString> dbName;
    lanName = m.getListText("LAN", "compPATH");
    dbName = m.getListText("DataBase", "names");

    QString sheets; // имя листа в протоколе для этого режима
    sheets = m.getStringText("RegimPSI_Termo", "sheetParam");
    qDebug() << "sheets" <<  sheets;

    QMap<QString, QString> cols; // пара номер проверки (режим проверки) - имя колонки в протоколе для этого режима
    cols = m.getMapTextToText("RegimPSI_Termo", "columns");
    qDebug() << "cols" <<  cols;

    QMap<QString, QString> rows; // пара пункт проверки - ряд в протоколе
    rows = m.getMapTextToText("RegimPSI_Termo", "rows");
    qDebug() << "rows" <<  rows;

    QList<CellExcel> numbers; // список пар колонка - ряд в протоколе для номера изделия
    numbers = m.getListCells("RegimPSI_Termo", "number");
    foreach(CellExcel c, numbers) qDebug() << "numbers" <<  c.col << c.row;

    QString cellFIO; // ряд в протоколе для оператора
    cellFIO = m.getStringText("RegimPSI_Termo", "users");
    qDebug() << "users" <<  cellFIO;


    // копируем базы данных
    QList<QString> dbPath;

    foreach (QString l, lanName){
        qDebug() << "Check connection to " + l;
        qApp->processEvents();
        QFile file(l);

        QString s = myPath+ "\\db\\" + dbName.value(lanName.indexOf(l));
        dbPath.append(s);

        if (file.exists()){
            qDebug() << "Connected. Copying is in progress..."; qApp->processEvents();
            //if (QFile::exists(s)) QFile::remove(s);
            //file.copy(s);
            qDebug() << "Copying done! " + s; qApp->processEvents();

        }
        else {
            qDebug() << "NOT EXIST";
            qDebug() << "Connection error..."; qApp->processEvents();
        }
    }

    dataBase.connectToBase(dbPath);


    //=========================================================
    dataProtokol.clear();

    QString number = "1910102280";
    // два цикла
    // цикл col с запросом к БД для получения данных для каждого режима из списка col
    // cols это QMap<QString, QString>
    // КЛЮЧ - режим проверки   ЗНАЧЕНИЕ - имя колонки в протоколе для этого режима
    foreach (QString regime, cols.keys()){

        IzdelieData hdMax;
        dataBase.getDataFromHistory("tb_Param", number, regime, hdMax); // regime - это номер проверки (режим проверки)
        // Параметры изделия для режима проверки находятся в hdMax

        CellExcel c;
        // оператор
        c.sheet = sheets;
        c.col = cols[regime];
        c.row = cellFIO;
        // заносим данные по ячейкам в dataProtokol
        dataProtokol[c] = hdMax.userFIO + "\n" + hdMax.otkFIO + "\n" + hdMax.pzFIO;


        // цикл по пунктам проверки для текущего режима regime
        // hdMax.parametr это  QMap<QString, QString> // КЛЮЧ - пункт проверки (3_1_1_10), ЗНАЧЕНИЕ - измеренное значение
        // rows это QMap<QString, QString> // КЛЮЧ - пункт проверки   ЗНАЧЕНИЕ - имя строки в протоколе для этого пункта
        foreach (QString row, hdMax.parametr.keys()){

            // проходим по всем ключам (пункт проверки) и из rows получаем номер строки в протоколе
            // из cols[regime] получаем колонку
            if (rows.contains(row)) {
                CellExcel c;
                c.sheet = sheets;
                c.col = cols[regime];
                c.row = rows[row];
                // заносим данные по ячейкам в dataProtokol
                dataProtokol[c] = hdMax.parametr[row];
            }
        }
    }

    foreach (CellExcel cell, numbers){

        dataProtokol[cell] = number;
    }

    PrintProtokol p;
    p.setFileName("7821.xls");

    p.printPrirabotka(dataProtokol);

    p.deleteLater();

}
