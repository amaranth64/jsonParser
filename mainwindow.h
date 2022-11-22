#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTextEdit>
#include <QList>

#include <fstream>


#include "database/mydatabaseclass.h"
#include "myjsonparser.h"
#include "printprotokol/printprotokol.h"


using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;


private slots:

    void on_pushButton_clicked();
};
#endif // MAINWINDOW_H

