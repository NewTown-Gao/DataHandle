#include "Globaldata.h"

writeLog * Global::mylog = new writeLog();

QMutex *Global::threadLock = new QMutex();

void Global::saveLog()
{
    Global::threadLock->lock();
    QFile file(QString(FILE_DIR)+QString(CONFIG_FILE));
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        Global::threadLock->unlock();
        return;
    }
    QDataStream out(&file);
    out<<*Global::mylog;
    qDebug()<<Q_FUNC_INFO<<Global::mylog->webdata_linNum<<Global::mylog->detaildata_linNum<<Global::mylog->flowdata_linNum;
    file.close();
    Global::threadLock->unlock();
}
