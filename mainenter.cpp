#include "mainenter.h"
#include <QDebug>
MainEnter::MainEnter(QObject *parent) :
    QObject(parent),dirScanflag(false)
{
    watch.addPath(FILE_DIR);        //文件系统监视器 监测文件夹的改动
    //connect(&watch,SIGNAL(fileChanged(QString)),this,SLOT(fileChange(QString)));
    connect(&watch,SIGNAL(directoryChanged(QString)),this,SLOT(dirChange(QString)));

    readConfig();

    /******monitor_acess处理线程对应###webdata文件******/
    QThread *AcessThread = new QThread();
    AcessTask *at = new AcessTask();
    at->moveToThread(AcessThread);
    AcessThread->start();
    connect(this,SIGNAL(pushAcessFile(QString)),at,SLOT(fileEnqueue(QString)));

    /******monitor_detail处理线程对应###detailflowdata文件******/
    QThread *DetailThread = new QThread();
    DetailTask *dt = new DetailTask();
    dt->moveToThread(DetailThread);
    DetailThread->start();
    connect(this,SIGNAL(pushDetailFile(QString)),dt,SLOT(fileEnqueue(QString)));

    /******monitor_sumflow处理线程对应###sumflow文件******/
    QThread *SumFlowThread = new QThread();
    SumflowTask *st = new SumflowTask();
    st->moveToThread(SumFlowThread);
    SumFlowThread->start();
    connect(this,SIGNAL(pushSumflowFile(QString)),st,SLOT(fileEnqueue(QString)));

    scanDir();

}

//读取配置文件 获取文件操作记录
void MainEnter::readConfig()
{
    QFile file(QString(FILE_DIR)+QString(CONFIG_FILE));
    if(!file.open(QIODevice::ReadWrite))
        return;
     QDataStream in(&file);
     in>>*Global::mylog;
     qDebug()<<Q_FUNC_INFO<<Global::mylog->detaildata<<Global::mylog->detaildata_linNum;
     file.close();
}


void MainEnter::fileChange(QString path)
{
    qDebug()<<"file change:"<<path;
}

//目录监视器响应槽函数
void MainEnter::dirChange(QString path)
{
    qDebug()<<"Dir change:"<<path<<watch.files();
    if(dirScanflag)
        return;
    dirScanflag = true;
    QTimer::singleShot(5000,this,SLOT(scanDir()));
}


//扫描目录 将符合的文件推送到相应的线程处理
void MainEnter::scanDir()
{
    QDir dir(FILE_DIR);
    if (!dir.exists())
    {
        dirScanflag = false;
        return;
    }
    dir.setFilter(QDir::Files);
    QFileInfoList fileList = dir.entryInfoList();
    foreach (QFileInfo fileinfo, fileList) {
        if(fileinfo.isFile())
        {
            QString filename= fileinfo.fileName();
            if(filename.endsWith("webdata"))
            {
                qDebug()<<"推送acess ："<<filename;
                emit pushAcessFile(filename);
            }
            if(filename.endsWith("detailflowdata"))
            {
                qDebug()<<"推送detail ："<<filename;
                emit pushDetailFile(filename);
            }
            if(filename.endsWith("sumflowdata"))
            {
                qDebug()<<"推送sumflow ："<<filename;
                emit pushSumflowFile(filename);
            }
        }
    }

    dirScanflag = false;
}


