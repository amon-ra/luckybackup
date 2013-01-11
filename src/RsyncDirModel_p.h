/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore.  Details of those terms
** are listed in licence.txt included as part of the distribution package
** of this file. This file may not be distributed without including the
** licence.txt file.
**
** Contact info@vcreatelogic.com if any conditions of this licensing are
** not clear to you.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef Rsync_DIR_MODEL_P_H
#define Rsync_DIR_MODEL_P_H

#include <QObject>
#include <QMetaType>
#include <QUrlInfo>
#include <QList>
#include <QString>
#include <QUrl>
#include <QIcon>
#include <QProcess>
#include <QTextStream>

class RsyncEntry;
class RsyncDirLister;
class RsyncDirListerOperation;

struct RsyncEntryData;
class RsyncEntry
{
public:
    ~RsyncEntry();

    RsyncEntry* parentEntry() const;
    QList<RsyncEntry*> childEntries() const;
    bool isChildrenFetched() const;
    bool isFetching() const;

    QString path() const;
    QUrlInfo urlInfo() const;
    QIcon icon() const;

//protected:
    RsyncEntry(const QUrlInfo& info);
    RsyncEntry* addUrlInfo(const QUrlInfo& urlInfo);
    void removeUrlInfo(const QUrlInfo& urlInfo);
    void setChildrenFetched(bool val=true);
    void clear();
    void setFetching(bool val=true);

private:
    friend class RsyncDirModel;
    RsyncEntryData* d;
};
Q_DECLARE_METATYPE(RsyncEntry*);


struct RsyncDirListerOperationData;
class RsyncDirListerOperation : public QObject
{
    Q_OBJECT

public:
    RsyncDirListerOperation(RsyncDirLister* parent);
    RsyncDirListerOperation(RsyncDirLister* parent,RsyncEntry* entry);
    ~RsyncDirListerOperation();
    void setParentEntry(RsyncEntry* entry);
    RsyncEntry* parentEntry() const;




    int id() const;
    bool isExecuting() const;
    bool isFinished() const;
    bool isSuccessfullyFinished() const;

public:
    void execute();
    void abort();

protected:
    QString path() const;

signals:
    void started();
    void finished();
    void aborted();

protected slots:
    void procFinished();
    void procError();

private:
    friend class RsyncDirLister;
    RsyncDirListerOperationData* d;
};

struct RsyncDirListerData;
class RsyncDirLister : public QObject
{
    Q_OBJECT

public:
    RsyncDirLister(QObject* parent=0);
    ~RsyncDirLister();

    void setUrl(const QUrl& url);
    QUrl url() const;

    QList<RsyncEntry*> topLevelEntries() const;
    bool topLevelEntriesFetched() const;
    QString lastError() const;

    void list();
    void list(RsyncEntry* entry);
    void refresh();
    void refresh(RsyncEntry* entry);
    void abort();
    void setChildrenFetched(RsyncEntry* entry);
    void setUrl(const QUrl& url,const QString& cpath);
    void setUrl(const QUrl& url,const QString& cpath, const QStringList& cargs);
    void cleanOperations();
    QStringList arguments() const;
    QString command() const;

signals:
    void listingDone(RsyncEntry* entry=0);
    void listingAborted(RsyncEntry* entry=0);
    void listingError(const QString& error, RsyncEntry* entry=0);

//protected slots:
//    void slotOperationFinished();

protected:
    void setTopLevelEntries(const QList<RsyncEntry*>& entries);

private:
    friend class RsyncDirListerOperation;
    RsyncDirListerData* d;
};


struct FileIconCacheData;
class FileIconCache
{
public:
    static FileIconCache& instance();
    ~FileIconCache();

    QIcon icon(const QUrlInfo& info) const;

private:
    FileIconCache();

private:
    FileIconCacheData* d;
};

#endif 
