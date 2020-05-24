#ifndef DB_MIGRATION_2_H
#define DB_MIGRATION_2_H

#include "db_migration_generic.h"
#include "../../common/engine_db.h"
#include <QMessageBox>

class db_migration_2 : public db_migration_generic
{
    Q_OBJECT
public:
    int getStepCount() { return 6; }
    int getOriginVer() { return 2; }

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

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(nullptr, "Update DB ?", "Member Genre Edition was removed !\nYou will have to check Genre for All Member !\nSure to migrate ?", QMessageBox::Yes|QMessageBox::No);
        if (reply != QMessageBox::Yes)
        {
            emit updateMsg("Aborted by User !", 255, 0, 0);
            return -1;
        }

        //Table structure for table member_phone
        emit updateMsg("Create Table member_phone", 0, 0, 0);
        query.prepare("DROP TABLE IF EXISTS member_phone");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        query.prepare("CREATE TABLE `member_phone` ("
                            "`id` INTEGER PRIMARY KEY NOT NULL AUTO_INCREMENT,"
                            "`member` INT UNSIGNED NOT NULL,"
                            "`phone` TINYTEXT,"
                            "`comment` TINYTEXT)");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(1);

        //Table structure for table member_email
        emit updateMsg("Create Table member_email", 0, 0, 0);
        query.prepare("DROP TABLE IF EXISTS member_email");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        query.prepare("CREATE TABLE `member_email` ("
                            "`id` INTEGER PRIMARY KEY NOT NULL AUTO_INCREMENT,"
                            "`member` INT UNSIGNED NOT NULL,"
                            "`email` TINYTEXT,"
                            "`comment` TINYTEXT)");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(2);

        //Table structure for table member_address
        emit updateMsg("Create Table member_address", 0, 0, 0);
        query.prepare("DROP TABLE IF EXISTS member_address");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        query.prepare("CREATE TABLE `member_address` ("
                            "`id` INTEGER PRIMARY KEY NOT NULL AUTO_INCREMENT,"
                            "`member` INT UNSIGNED NOT NULL,"
                            "`street` TINYTEXT,"
                            "`postcode` TINYTEXT,"
                            "`city` TINYTEXT,"
                            "`comment` TINYTEXT)");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(3);

        //Table structure for table add id to events_comments
        emit updateMsg("Add id to events_comments", 0, 0, 0);
        query.prepare("ALTER TABLE events_comments "
                          "ADD COLUMN `id` INT NOT NULL AUTO_INCREMENT FIRST,"
                          "ADD PRIMARY KEY (`id`)");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(4);

        emit updateMsg("Member Genre editing have been removed. All Genres will be lost ! Please check genre for all member !", 255, 127, 0);

        //Table structure for
        emit updateMsg("Delete Table member_gender", 0, 0, 0);
        query.prepare("DROP TABLE IF EXISTS member_gender");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(5);

        emit updateMsg("Update DB Version", 0, 0, 0);
        query.prepare("UPDATE config SET `number` = '3' WHERE (`id` = '1')");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(6);

        return 0;
    }

    int setupDatabaseLocal(QSqlQuery &query)
    {
        emit updateMsg("Migrate for Server Mode", 0, 0, 128);

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(nullptr, "Update DB ?", "Member Genre Edition was removed !\nYou will have to check Genre for All Member !\nAll comments on event will be removed !\nSure to migrate ?", QMessageBox::Yes|QMessageBox::No);
        if (reply != QMessageBox::Yes)
        {
            emit updateMsg("Aborted by User !", 255, 0, 0);
            return -1;
        }

        //Table structure for table member_phone
        emit updateMsg("Create Table member_phone", 0, 0, 0);
        query.prepare("DROP TABLE IF EXISTS member_phone");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        query.prepare("CREATE TABLE `member_phone` ("
                            "`id` INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "`member` INT UNSIGNED NOT NULL,"
                            "`phone` TINYTEXT,"
                            "`comment` TINYTEXT)");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(1);

        //Table structure for table member_email
        emit updateMsg("Create Table member_email", 0, 0, 0);
        query.prepare("DROP TABLE IF EXISTS member_email");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        query.prepare("CREATE TABLE `member_email` ("
                            "`id` INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "`member` INT UNSIGNED NOT NULL,"
                            "`email` TINYTEXT,"
                            "`comment` TINYTEXT)");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(2);

        //Table structure for table member_address
        emit updateMsg("Create Table member_address", 0, 0, 0);
        query.prepare("DROP TABLE IF EXISTS member_address");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        query.prepare("CREATE TABLE `member_address` ("
                            "`id` INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "`member` INT UNSIGNED NOT NULL,"
                            "`street` TINYTEXT,"
                            "`postcode` TINYTEXT,"
                            "`city` TINYTEXT,"
                            "`comment` TINYTEXT)");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(3);

        emit updateMsg("All comment on Event Will be lost !", 255, 127, 0);

        //Table structure for table add id to events_comments
        emit updateMsg("Add id to events_comments", 0, 0, 0);
        query.prepare("DROP TABLE IF EXISTS events_comments");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        query.prepare("CREATE TABLE `events_comments` ("
                            "`id` INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "`event` INTEGER UNSIGNED NOT NULL,"
                            "`user` INTEGER UNSIGNED NOT NULL,"
                            "`createdAt` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
                            "`updatedAt` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
                            "`comment` TINYTEXT NOT NULL)");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(4);

        emit updateMsg("Member Genre editing have been removed. All Genres will be lost ! Please check genre for all member !", 255, 127, 0);

        //Table structure for
        emit updateMsg("Delete Table member_gender", 0, 0, 0);
        query.prepare("DROP TABLE IF EXISTS member_gender");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(5);

        emit updateMsg("Update DB Version", 0, 0, 0);
        query.prepare("UPDATE config SET `number` = '3' WHERE (`id` = '1')");
        if(!query.exec()) { emit updateMsg(query.lastError().text(), 255, 0, 0); return -1; }
        emit updateStep(6);

        return 0;
    }

    db_migration_response rollback()
    {
        return DB_MIGRATION_MISSING;
    }
};

#endif // DB_MIGRATION_2_H
