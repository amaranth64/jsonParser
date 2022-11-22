#ifndef DATATYPE_H
#define DATATYPE_H


#include <QMap>
#include <QObject>
#include <QString>


struct IzdelieData
{
    QString number = "";
    QString time = "";
    QString data = "";
    QString result = "";
    QString userFIO = "";
    QString otkFIO = "";
    QString pzFIO = "";
    QString checkType = "";
    QString linkKey = "";

    QMap<QString, QString> parametr;
};
Q_DECLARE_METATYPE(IzdelieData);


struct CellExcel
{
    QString sheet = "";
    QString row = "";
    QString col = "";
    bool operator< (const CellExcel &other) const {
        // нужно доработать
        if (qHash(sheet) == qHash(other.sheet)){
            if (col == other.col){
                if (row < other.row) return true; else return false;
            } else {
                if (col < other.col) return true; else return false;
            }
        }

        if (qHash(sheet) < qHash(other.sheet)){
            return true;
        } else {
            return false;
//            if (col == other.col){
//                if (row < other.row) return true; else return false;
//            } else {
//                if (col < other.col) return true; else return false;
//            }
        }

        return false;
    }
};
Q_DECLARE_METATYPE(CellExcel);

#endif // DATATYPE_H
