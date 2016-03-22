#ifndef SUMFLOWTASK_H
#define SUMFLOWTASK_H

#include <QObject>
#include "Globaldata.h"

/******处理monitor_sumflow表对应###sumflow文件******/


class SumflowTask : public QObject
{
    Q_OBJECT
public:
    explicit SumflowTask(QObject *parent = 0);
    ~SumflowTask();
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

#endif // SUMFLOWTASK_H
