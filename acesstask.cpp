#include "acesstask.h"


AcessTask::AcessTask(QObject *parent) :
    QObject(parent),linNum(1),isRun(false)
{
    authon_db = QSqlDatabase::addDatabase("QMYSQL","mysql_authon_acess");
    authon_db.setHostName(AUTHON_DB_IP);
    authon_db.setDatabaseName(AUTHON_DB_NAME);
    authon_db.setUserName(AUTHON_DB_ACCOUNT);
    authon_db.setPassword(AUTHON_DB_PASSWORD);
    authon_db.setPort(AUTHON_DB_PORT);

    sqlHandle = new QSqlQuery(authon_db);

    QTimer::singleShot(10000,this,SLOT(initHandle()));
    runTimer = new QTimer(this);
    runTimer->setInterval(60*1000);
    connect(runTimer,SIGNAL(timeout()),SLOT(run()));
}

AcessTask::~AcessTask()
{
    authon_db.commit();
    authon_db.close();
    if(QSqlDatabase::contains("mysql_authon_acess"))
        QSqlDatabase::removeDatabase("mysql_authon_acess");
}

void AcessTask::open_authonSql()
{
    if(QSqlDatabase::contains("mysql_authon_acess"))
    {
        authon_db.commit();
        authon_db.close();
    }
    if(!authon_db.open())
    {
        printf("authon_db%s\n",qPrintable(authon_db.lastError().text()));
        open_authonSql();
    }

    authon_db.exec("SET NAMES 'UTF-8'");
    qDebug() << Q_FUNC_INFO << tr("authon_db 连接成功");
}

void AcessTask::openDataFile(QString filename)
{
    if(dataFile.isOpen())
        dataFile.close();
    QString filepath = QString(FILE_DIR)+filename;
    dataFile.setFileName(filepath);
    if(!dataFile.open(QIODevice::ReadWrite | QIODevice::Text))
        qDebug()<<"open file fail:"<<filepath;
}

//每隔一段时间扫描fileQueue队列 取出第一项处理
void AcessTask::run()
{
    if(isRun)
        return;
    isRun = true;
    qDebug()<<Q_FUNC_INFO<<Global::mylog->webdata<<Global::mylog->webdata_linNum<<fileQueue.size();
    if(!authon_db.isOpen())
        open_authonSql();
    if(!fileQueue.isEmpty())
    {
        openDataFile(fileQueue.at(0));
        while(!dataFile.atEnd())
        {
            QByteArray line = dataFile.readLine();
            if(linNum <= Global::mylog->webdata_linNum)
            {
                linNum++;
                if(Global::mylog->webdata_linNum == 0)
                {
                    process_line(line);
                    Global::mylog->webdata_linNum++;
                    linNum++;
                }
                continue;
            }
            process_line(line);
            Global::mylog->webdata_linNum++;
            linNum++;
        }
        Global::mylog->webdata=QFileInfo(dataFile.fileName()).fileName();
        Global::saveLog();
        linNum =1;

        if(fileQueue.size()>1)
        {
            fileQueue.dequeue();
            if(!fileQueue.isEmpty())
            {
                qDebug()<<"1111111111";
                Global::mylog->webdata = fileQueue.at(0);
                Global::mylog->webdata_linNum =0;
            }
        }
    }
    isRun = false;
}

//启动初始化
void AcessTask::initHandle()
{
    qDebug()<<Q_FUNC_INFO<<Global::mylog->webdata<<Global::mylog->webdata_linNum;
    if(Global::mylog->webdata.isEmpty())
    {
        if(!fileQueue.isEmpty())
            Global::mylog->webdata = fileQueue.at(0);
    }
    else
    {
        if(fileQueue.isEmpty())
            qDebug()<<"等待推送文件。。。。。";
        else
        {
            while(!fileQueue.isEmpty())
            {
                if(fileQueue.at(0) == Global::mylog->webdata)
                    break;
                else
                    fileQueue.dequeue();
            }
        }
    }

    runTimer->start();
}

//向monitor_access表插入一条记录
void AcessTask::process_line(QByteArray line)
{
    QString data = QString(line);
    QStringList lineList = data.split(" ");
    lineList.removeAll("");
    if(lineList.size()<6)
        return;
    sqlHandle->clear();
    sqlHandle->prepare("INSERT INTO monitor_access(`date_time`,`srcip`,`dstip`,"
                       "`mac`,`host`,`url`) VALUES(:date_time,:srcip,:dstip,:mac,:host,:url);");
    sqlHandle->bindValue(":date_time",lineList.at(0)+" "+lineList.at(1));
    sqlHandle->bindValue(":srcip",lineList.at(2));
    sqlHandle->bindValue(":dstip",lineList.at(3));
    sqlHandle->bindValue(":mac",lineList.at(4));
    sqlHandle->bindValue(":host",lineList.at(5));
    sqlHandle->bindValue(":url",lineList.at(6));
    if(sqlHandle->exec())
    {
        sqlHandle->clear();
        qDebug()<<"insert acess ok ";
    }
    else
    {
        qDebug()<<"insert acess fail : "<<lineList.at(0)+" "+lineList.at(1);
    }

}

//文件按日期从小到大插入队列
void AcessTask::fileEnqueue(QString filename)
{
    if(fileQueue.contains(filename))
       return;
    QDate fileDate = QDate::fromString(filename.left(10),"yyyy-MM-dd");
    if(!Global::mylog->webdata.isEmpty())
    {
        QDate webdate = QDate::fromString(Global::mylog->webdata.left(10),"yyyy-MM-dd");
        if(fileDate <= webdate)     //推送的文件日期小于当前已经处理的文件日期
            return;
    }
    int pos=0;
    for(int i=0;i<fileQueue.size();i++)
    {
        QString date = fileQueue.at(i).left(10);
        QDate qdate = QDate::fromString(date,"yyyy-MM-dd");
        if(qdate > fileDate)
        {
            pos = i;
            break;
        }
    }
    if(pos > 0)
        fileQueue.insert(pos,filename);
    else
        fileQueue.enqueue(filename);

    qDebug()<<"access fileQueue :";
    for(int i=0;i<fileQueue.size();i++)
    {
        qDebug()<<fileQueue.at(i);
    }

}
