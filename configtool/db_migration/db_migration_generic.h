#ifndef DB_MIGRATION_GENERIC_H
#define DB_MIGRATION_GENERIC_H

#include <QObject>
#include <QtSql>

typedef enum
{
    DB_MIGRATION_MISSING = -2,
    DB_MIGRATION_FAILURE = -1,
    DB_MIGRATION_SUCCESS = 0,

}db_migration_response;

//Generic class for migration
class db_migration_generic : public QObject
{
    Q_OBJECT
public:
    virtual int getStepCount() = 0;
    virtual int getOriginVer() = 0;
    virtual db_migration_response migrate() = 0;
    virtual db_migration_response rollback() = 0;

signals:
    void updateMsg(QString, int, int, int);
    void updateStep(int);
};

//Default class for migration missing
class db_migration_nomigration : public db_migration_generic
{
    Q_OBJECT
public:
    int getStepCount() { return 1; }
    int getOriginVer() { return 0; }
    db_migration_response migrate() { return DB_MIGRATION_MISSING; }
    db_migration_response rollback() { return DB_MIGRATION_MISSING; }
};

#endif // DB_MIGRATION_GENERIC_H
