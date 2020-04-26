#ifndef DB_MIGRATION_ENGINE_H
#define DB_MIGRATION_ENGINE_H

#include <QObject>
#include "db_migration_generic.h"

class db_migration_engine : public QObject
{
    Q_OBJECT
public:
    explicit db_migration_engine(QObject *parent = nullptr);
    bool migrate(int (*getDBVer)(), int targetVer);

signals:
    void updateMsg(QString);
    void updateStep(int, int);

public slots:
    void updateRequestMsg(QString msg, int red=0, int green=0, int blue=0);
    void updateRequestStep(int step, int stepMax=-1);

private:
    void addMigration(db_migration_generic *newMigration);

    QVector<db_migration_generic*> migrationList;
    int stepExec;

};

#endif // DB_MIGRATION_ENGINE_H
