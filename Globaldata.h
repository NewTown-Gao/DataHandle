#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <QDebug>
#include <QFile>
#include <QDir>
#include <QDate>
#include <QThread>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDriver>
#include <QQueue>
#include <QMutex>

#define AUTHON_DB_IP  "192.168.1.100"
#define AUTHON_DB_ACCOUNT "root"
#define AUTHON_DB_PASSWORD "password"
#define AUTHON_DB_NAME "db_name"
#define AUTHON_DB_PORT 3306

#define CONFIG_FILE "config"
#define FILE_DIR "/home/gxc/DataFile/"

class writeLog
{
public:
    writeLog():webdata(""),webdata_linNum(0),detaildata(""),detaildata_linNum(0),flowdata(""),flowdata_linNum(0){}
    QString webdata;
    int webdata_linNum;
    QString detaildata;
    int detaildata_linNum;
    QString flowdata;
    int flowdata_linNum;

    friend QDataStream & operator << (QDataStream &qos, const writeLog &log)
    {
        qos <<log.webdata<<log.webdata_linNum<<log.detaildata<<log.detaildata_linNum
              <<log.flowdata<<log.flowdata_linNum;
        return qos;
    }
    friend QDataStream & operator >> (QDataStream &qis, writeLog &log)
    {
        qis >>log.webdata>>log.webdata_linNum>>log.detaildata>>log.detaildata_linNum
              >>log.flowdata>>log.flowdata_linNum;
        return qis;
    }
};

class Global
{
public:
   static QMutex *threadLock;
   static writeLog *mylog;
   static void saveLog();
};



#endif // GLOBALDATA_H
