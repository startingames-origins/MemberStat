#ifndef DB_MIGRATION_1_H
#define DB_MIGRATION_1_H

#include "db_migration_generic.h"
#include "../../common/engine_db.h"

class db_migration_1 : public db_migration_generic
{
    Q_OBJECT
public:
    int getStepCount() { return 2; }
    int getOriginVer() { return 1; }

    db_migration_response migrate()
    {
        emit updateMsg("Open transaction", 0, 0, 0);
        if(engine_db::db.transaction())
        {
            QSqlQuery query(engine_db::db);
            if(setupDatabase(query) != 0)
            {
                emit updateMsg("rollback", 0, 0, 0);
                if(!engine_db::db.rollback()) emit updateMsg(query.lastError().text(), 255, 0, 0);
                return DB_MIGRATION_FAILURE;
            }
            else
            {
                emit updateMsg("commit", 0, 0, 0);
                if(!engine_db::db.commit()) engine_db::db.rollback(); emit updateMsg(query.lastError().text(), 255, 0, 0);
                return DB_MIGRATION_SUCCESS;
            }
        }
        else
        {
            return DB_MIGRATION_FAILURE;
        }

        return DB_MIGRATION_FAILURE;
    }

    int setupDatabase(QSqlQuery &query)
    {
        //Table structure for table events
        emit updateMsg("Alter Table members - ADD comment", 0, 0, 0);
        query.prepare("ALTER TABLE members ADD COLUMN `comment` TINYTEXT NULL");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(1);

        emit updateMsg("Update DB Version", 0, 0, 0);
        query.prepare("UPDATE config SET `number` = '2' WHERE (`id` = '1')");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(2);

        return 0;
    }

    db_migration_response rollback()
    {
        return DB_MIGRATION_MISSING;
    }
};

#endif // DB_MIGRATION_1_H
