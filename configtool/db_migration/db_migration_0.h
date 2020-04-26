#ifndef DB_MIGRATION_0_H
#define DB_MIGRATION_0_H

#include "db_migration_generic.h"
#include "../../common/engine_db.h"

class db_migration_0 : public db_migration_generic
{
    Q_OBJECT
public:
    int getStepCount() { return 10; }
    int getOriginVer() { return 0; }

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
        if(engine_db::local)
        {
            return setupDatabaseLocal(query);
        }

        emit updateMsg("Migrate for Server Mode", 0, 0, 128);
        //Table structure for table events
        emit updateMsg("Create Table events", 0, 0, 0);
        query.prepare("DROP TABLE IF EXISTS events");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        query.prepare("CREATE TABLE `events` ("
                            "`id` INTEGER PRIMARY KEY NOT NULL AUTO_INCREMENT,"
                            "`ref` TINYTEXT,"
                            "`name` TINYTEXT,"
                            "`desc` TINYTEXT,"
                            "`start` TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP,"
                            "`end` TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP,"
                            "`createdAt` TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP,"
                            "`updatedAt` TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP)");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(1);

        //Table structure for table events_members
        emit updateMsg("Create Table events_members", 0, 0, 0);
        query.prepare("DROP TABLE IF EXISTS events_members");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        query.prepare("CREATE TABLE events_members ("
                            "`event` INT UNSIGNED NOT NULL,"
                            "`member` INT UNSIGNED NOT NULL,"
                            "`comment` TINYTEXT);");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(2);

        //Table structure for table member_gender
        emit updateMsg("Create Table member_gender", 0, 0, 0);
        query.prepare("DROP TABLE IF EXISTS member_gender");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        query.prepare("CREATE TABLE member_gender ("
                            "`id` INTEGER PRIMARY KEY NOT NULL AUTO_INCREMENT,"
                            "`priority` INT UNSIGNED DEFAULT NULL,"
                            "`name` TINYTEXT,"
                            "`desc` TINYTEXT)");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(3);

        //Table structure for table member_type
        emit updateMsg("Create Table member_type", 0, 0, 0);
        query.prepare("DROP TABLE IF EXISTS member_type");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        query.prepare("CREATE TABLE member_type ("
                          "`id` INTEGER PRIMARY KEY NOT NULL AUTO_INCREMENT,"
                          "`priority` INT UNSIGNED DEFAULT NULL,"
                          "`name` TINYTEXT,"
                          "`desc` TINYTEXT)");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(4);

        //Table structure for
        emit updateMsg("Create Table members", 0, 0, 0);
        query.prepare("DROP TABLE IF EXISTS members");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        query.prepare("CREATE TABLE members ("
                        "`id` INTEGER PRIMARY KEY NOT NULL AUTO_INCREMENT,"
                        "`regNum` TINYTEXT,"
                        "`firstName` TINYTEXT,"
                        "`otherName` TINYTEXT,"
                        "`lastName` TINYTEXT,"
                        "`birthDate` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
                        "`birthLocation` TINYTEXT,"
                        "`gender` INT NOT NULL DEFAULT '0',"
                        "`type` INT NOT NULL DEFAULT '0',"
                        "`createdAt` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
                        "`updatedAt` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP)");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(5);

        emit updateMsg("Create Table members_relation", 0, 0, 0);
        query.prepare("DROP TABLE IF EXISTS members_relation");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        query.prepare("CREATE TABLE members_relation ("
                        "`member1` INT UNSIGNED NOT NULL,"
                        "`member2` INT UNSIGNED NOT NULL,"
                        "`type` INT UNSIGNED NOT NULL)");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(6);

        emit updateMsg("Create Table users", 0, 0, 0);
        query.prepare("DROP TABLE IF EXISTS users");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        query.prepare("CREATE TABLE users ("
                        "`id` INTEGER PRIMARY KEY NOT NULL AUTO_INCREMENT,"
                        "`username` TINYTEXT NOT NULL,"
                        "`password` TINYTEXT NOT NULL,"
                        "`rights` BIGINT UNSIGNED DEFAULT NULL)");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(7);

        emit updateMsg("Create Table events_comments", 0, 0, 0);
        query.prepare("DROP TABLE IF EXISTS events_comments");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        query.prepare("CREATE TABLE events_comments ("
                        "`event` INTEGER UNSIGNED NOT NULL,"
                        "`user` INTEGER UNSIGNED NOT NULL,"
                        "`createdAt` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
                        "`updatedAt` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
                        "`comment` TINYTEXT NOT NULL)");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(8);

        emit updateMsg("Create Admin User", 0, 0, 0);
        QString username = "Admin"; QString userpass = username;
        USER_PASSWORD_MACRO
        query.prepare("INSERT INTO users (username, password, rights) VALUES (? ,?, 1023)");
        query.addBindValue(username); query.addBindValue(QString(USER_PASSWORD_MACRO2));
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(9);

        emit updateMsg("Update DB Version", 0, 0, 0);
        query.prepare("UPDATE config SET `number` = '1' WHERE (`id` = '1')");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(10);

        return 0;
    }

    int setupDatabaseLocal(QSqlQuery &query)
    {
        emit updateMsg("Migrate for Local Mode", 0, 0, 128);
        //Table structure for table events
        emit updateMsg("Create Table events", 0, 0, 0);
        query.prepare("DROP TABLE IF EXISTS events");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        query.prepare("CREATE TABLE `events` ("
                            "`id` INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "`ref` TINYTEXT,"
                            "`name` TINYTEXT,"
                            "`desc` TINYTEXT,"
                            "`start` TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP,"
                            "`end` TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP,"
                            "`createdAt` TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP,"
                            "`updatedAt` TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP)");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(1);

        //Table structure for table events_members
        emit updateMsg("Create Table events_members", 0, 0, 0);
        query.prepare("DROP TABLE IF EXISTS events_members");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        query.prepare("CREATE TABLE events_members ("
                            "`event` INT UNSIGNED NOT NULL,"
                            "`member` INT UNSIGNED NOT NULL,"
                            "`comment` TINYTEXT);");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(2);

        //Table structure for table member_gender
        emit updateMsg("Create Table member_gender", 0, 0, 0);
        query.prepare("DROP TABLE IF EXISTS member_gender");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        query.prepare("CREATE TABLE member_gender ("
                            "`id` INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "`priority` INT UNSIGNED DEFAULT NULL,"
                            "`name` TINYTEXT,"
                            "`desc` TINYTEXT)");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(3);

        //Table structure for table member_type
        emit updateMsg("Create Table member_type", 0, 0, 0);
        query.prepare("DROP TABLE IF EXISTS member_type");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        query.prepare("CREATE TABLE member_type ("
                          "`id` INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "`priority` INT UNSIGNED DEFAULT NULL,"
                          "`name` TINYTEXT,"
                          "`desc` TINYTEXT)");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(4);

        //Table structure for
        emit updateMsg("Create Table members", 0, 0, 0);
        query.prepare("DROP TABLE IF EXISTS members");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        query.prepare("CREATE TABLE members ("
                        "`id` INTEGER PRIMARY KEY AUTOINCREMENT,"
                        "`regNum` TINYTEXT,"
                        "`firstName` TINYTEXT,"
                        "`otherName` TINYTEXT,"
                        "`lastName` TINYTEXT,"
                        "`birthDate` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
                        "`birthLocation` TINYTEXT,"
                        "`gender` INT NOT NULL DEFAULT '0',"
                        "`type` INT NOT NULL DEFAULT '0',"
                        "`createdAt` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
                        "`updatedAt` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP)");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(5);

        emit updateMsg("Create Table members_relation", 0, 0, 0);
        query.prepare("DROP TABLE IF EXISTS members_relation");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        query.prepare("CREATE TABLE members_relation ("
                        "`member1` INT UNSIGNED NOT NULL,"
                        "`member2` INT UNSIGNED NOT NULL,"
                        "`type` INT UNSIGNED NOT NULL)");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(6);

        emit updateMsg("Create Table users", 0, 0, 0);
        query.prepare("DROP TABLE IF EXISTS users");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        query.prepare("CREATE TABLE users ("
                        "`id` INTEGER PRIMARY KEY AUTOINCREMENT,"
                        "`username` TINYTEXT NOT NULL,"
                        "`password` TINYTEXT NOT NULL,"
                        "`rights` BIGINT UNSIGNED DEFAULT NULL)");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(7);

        emit updateMsg("Create Table events_comments", 0, 0, 0);
        query.prepare("DROP TABLE IF EXISTS events_comments");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        query.prepare("CREATE TABLE events_comments ("
                        "`event` INTEGER UNSIGNED NOT NULL,"
                        "`user` INTEGER UNSIGNED NOT NULL,"
                        "`createdAt` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
                        "`updatedAt` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
                        "`comment` TINYTEXT NOT NULL)");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(8);

        emit updateMsg("Create Admin User", 0, 0, 0);
        QString username = "Admin"; QString userpass = username;
        USER_PASSWORD_MACRO
        query.prepare("INSERT INTO users (username, password, rights) VALUES (? ,?, 1023)");
        query.addBindValue(username); query.addBindValue(QString(USER_PASSWORD_MACRO2));
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(9);

        emit updateMsg("Update DB Version", 0, 0, 0);
        query.prepare("UPDATE config SET `number` = '1' WHERE (`id` = '1')");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(10);

        return 0;
    }

    db_migration_response rollback()
    {
        return DB_MIGRATION_MISSING;
    }
};

#endif // DB_MIGRATION_0_H
