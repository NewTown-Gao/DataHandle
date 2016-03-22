#ifndef ACESSTASK_H
#define ACESSTASK_H

#include <QObject>
#include "Globaldata.h"

/*********处理monitor_acess表对应###webdata文件*********/

class AcessTask : public QObject
{
    Q_OBJECT
public:
    explicit AcessTask(QObject *parent = 0);
    ~AcessTask();
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

#endif // ACESSTASK_H
