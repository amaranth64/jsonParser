#include "mydatabaseclass.h"

MyDataBaseClass::MyDataBaseClass(QObject *parent) : QObject(parent)
{

}

MyDataBaseClass::~MyDataBaseClass()
{
    disconnectFromBase();
}

bool MyDataBaseClass::connectToBase(QList<QString> dbPath)
{
    // на входе получаем список с путями к базам данных
    // создаем соединения и заполняем список соединений db
    // если удалось создать соединение
    foreach (QString path, dbPath) {

        QSqlDatabase dbItem;

        dbItem = QSqlDatabase::addDatabase("QODBC", path);

        dbItem.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=" + path);
        dbItem.setPassword("01031978");

        if (dbItem.open()) {
            qDebug() << "Connecting SUCCESS to " + path ;
            db.append(dbItem);
        } else{
            qDebug() << "Connecting ERROR to " + path ;
        }
    }
    return true;
}

bool MyDataBaseClass::disconnectFromBase()
{

    foreach (QSqlDatabase dbItem, db) {
        dbItem.close();
    }
    return true;
}

void MyDataBaseClass::getDataFromHistory(QString table, QString number, QString checkType, IzdelieData &result)
{

    QDate dMax(1900,01,01);
    QTime tMax(0,0,0);

    // для каждого соединения в списке (базе данных со разных стендов) db создаем запрос
    // по номеру number и виду проверки checkType где результат не ПРЕРВАНО в порядке убывания даты
    // (самое последнее измерение будет первым в ответе на запрос)
    // В цикле анализируем дату и время. В результате нужно найти из всех баз данных самое последнее по времени измерение
    // Это измерение (запись в таблице) и будет результатом работы функции

    foreach (QSqlDatabase dbItem, db) {
        QSqlQuery queryHistory(dbItem);

        queryHistory.exec("SELECT * FROM tb_History WHERE [Номер] LIKE "+ number + " and [Вид проверки]=" + checkType + "and [Результат] NOT LIKE 'ПРЕРВАНО' ORDER BY [Дата] DESC, [Время] DESC");
        queryHistory.next(); // запрос выстроен по убыванию даты

        QSqlRecord rec = queryHistory.record();

        IzdelieData data;
        data.number = queryHistory.value(rec.indexOf("Номер")).toString();
        data.checkType = queryHistory.value(rec.indexOf("Вид проверки")).toString();
        data.data = queryHistory.value(rec.indexOf("Дата")).toString();
        data.time = queryHistory.value(rec.indexOf("Время")).toString();
        data.userFIO = queryHistory.value(rec.indexOf("ФИО оператора")).toString();
        data.pzFIO = queryHistory.value(rec.indexOf("ФИО ПЗ")).toString();
        data.otkFIO = queryHistory.value(rec.indexOf("ФИО ОТК")).toString();
        data.result = queryHistory.value(rec.indexOf("Результат")).toString();
        data.linkKey = queryHistory.value(rec.indexOf("LinkKey")).toString();

        data.data = data.data.section("T", 0,0);
        data.time = data.time.section("T", 1,1);
        QString date_format = "yyyy-MM-dd";
        QString time_format = "HH:mm:ss.zzz";
        QDate d = QDate::fromString(data.data, date_format);
        QTime t = QTime::fromString(data.time, time_format);

        if (d > dMax){
            if (t > tMax) {
                dMax = d;
                tMax = t;

                qDebug() << "This is max data" << d << t;

                QSqlQuery queryParam(dbItem);
                queryParam.exec("SELECT * FROM " + table + " WHERE [Key1]=" + data.linkKey);
                queryParam.next();
                QSqlRecord recParam = queryParam.record();


                for (int i = 0; i < recParam.count(); i++){
                    data.parametr[recParam.fieldName(i)] = queryParam.value(i).toString();
                }
                result = data;
            }
        }

        qDebug() << data.data;
        qDebug() << data.time;
        qDebug() <<data.number;
        qDebug() <<data.result;
        qDebug() << data.userFIO;
        qDebug() <<data.pzFIO;
        qDebug() <<data.otkFIO;
        qDebug() <<data.checkType;
        if (data.result == "СТУ") qDebug() << "OK";
        else if (data.result == "НСТУ") qDebug() << "bad";
        else qDebug() << "Fuck";
        qDebug() <<data.linkKey;
        qDebug() <<"-----------------";

    }
}




//UserData MyDataBaseClass::getUserByID(quint8 id)
//{
//    UserData tmpUser;
//    tmpUser.name = "Неизвестно";

//    if (!QSqlDatabase::contains("KRD")){
//        return tmpUser;
//    }
//    // получаем данные о соединении с базой данных
//    QSqlDatabase db = QSqlDatabase::database("KRD");

//    if (db.isOpen()){
//        QSqlQuery query(db);

//        query.exec("SELECT userID, userName FROM users WHERE userID=" + QString::number(id));
//        QSqlRecord rec = query.record();

//        while (query.next()){
//            tmpUser.userID = query.value(rec.indexOf("userID")).toInt();
//            tmpUser.name = query.value(rec.indexOf("userName")).toString();
//        }
//    }
//    return tmpUser;
//}

//QList<UserData> MyDataBaseClass::getUsersFromBase()
//{
//    QList<UserData> tmpUser;

//    if (!QSqlDatabase::contains("KRD")){
//        return tmpUser;
//    }

//    // получаем данные о соединении с базой данных
//    QSqlDatabase db = QSqlDatabase::database("KRD");

//    if (db.isOpen()){
//        QSqlQuery query(db);

//        query.exec("SELECT * FROM users ;");
//        QSqlRecord rec = query.record();

//        while (query.next()){
//            UserData ud;
//            ud.userID = query.value(rec.indexOf("userID")).toInt();
//            ud.name = query.value(rec.indexOf("userName")).toString();
//            tmpUser.append(ud);
//        }
//    }
//    return tmpUser;
//}

//bool MyDataBaseClass::disconnectFromBase()
//{
//    if (QSqlDatabase::contains("valves")) {
//        // получаем данные о соединении с базой данных
//        QSqlDatabase db = QSqlDatabase::database("KRD");

//        if (db.isOpen()) {
//            db.close();

//        }
//        QSqlDatabase::removeDatabase("valves");
//        return true;
//    }
//    return false;
//}


