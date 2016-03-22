#ifndef MAINENTER_H
#define MAINENTER_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QStringList>
#include "acesstask.h"
#include "detailtask.h"
#include "sumflowtask.h"
class MainEnter : public QObject
{
    Q_OBJECT
public:
    explicit MainEnter(QObject *parent = 0);

    void readConfig();
private:
    QFileSystemWatcher watch;
    bool dirScanflag;
signals:
    void pushAcessFile(QString);
    void pushDetailFile(QString);
    void pushSumflowFile(QString);
public slots:
    void fileChange(QString);
    void dirChange(QString);
    void scanDir();

};

#endif // MAINENTER_H
