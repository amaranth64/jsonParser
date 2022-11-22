#ifndef MYDATABASECLASS_H
#define MYDATABASECLASS_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDate>
#include <QtMath>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlRecord>

#include <iomanip>
#include <database/datatype.h>
#include "database/datatype.h"

template <class T>
class Singleton {
public:
  static T& instance() {
    static T instance;
    return instance;
  }
private:
  Singleton();
  ~Singleton();
  Singleton(const Singleton &);
  Singleton& operator=(const Singleton &);
};

class MyDataBaseClass : public QObject
{
    Q_OBJECT
public:
    explicit MyDataBaseClass(QObject *parent = nullptr);
    ~MyDataBaseClass();

    bool connectToBase(QList<QString> dbPath);
    bool disconnectFromBase();

    void getDataFromHistory(QString table, QString number, QString checkType, IzdelieData &result);

  //  UserData getUserByID(quint8 id);
  //  QList<UserData> getUsersFromBase();

private:
    QList<QSqlDatabase> db;
signals:

public slots:

};

#define dataBase Singleton<MyDataBaseClass>::instance()

#endif // MYDATABASECLASS_H
