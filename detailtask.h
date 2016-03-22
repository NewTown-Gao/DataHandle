#ifndef DETAILTASK_H
#define DETAILTASK_H

#include <QObject>
#include "Globaldata.h"

/******处理monitor_detail表对应###detailflowdata文件******/


class DetailTask : public QObject
{
    Q_OBJECT
public:
    explicit DetailTask(QObject *parent = 0);
    ~DetailTask();

    void open_authonSql();
    void openDataFile(QString);
    void process_line(QByteArray);
private:
    QSqlDatabase authon_db;
    QFile dataFile;
    QSqlQuery *sqlHandle;
    int linNum;

    QQueue<QString> fileQueue;
    QTimer *runTimer;
    bool isRun;
signals:

public slots:
    void fileEnqueue(QString);
    void initHandle();
    void run();
};

#endif // DETAILTASK_H
