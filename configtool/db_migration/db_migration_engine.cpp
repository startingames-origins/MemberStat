#include "db_migration_engine.h"
#include "db_migration_register.h"

db_migration_engine::db_migration_engine(QObject *parent) : QObject(parent)
{
    migrationList.clear();
    DB_MIGRATION_REGISTER_ADD   //Add migration from register file
}

void db_migration_engine::updateRequestMsg(QString msg, int red, int green, int blue)
{
    emit updateMsg("<span style=\"color: rgb("+QString::number(red)+","+QString::number(green)+","+QString::number(blue)+");\">"+msg+"</span>");
}

void db_migration_engine::updateRequestStep(int step, int stepMax)
{
    emit updateStep(stepExec+step, stepMax);
}

void db_migration_engine::addMigration(db_migration_generic *newMigration)
{
    int originVer = newMigration->getOriginVer();               //Get origin Version of new migration
    connect(newMigration, SIGNAL(updateMsg(QString, int, int, int)), this, SLOT(updateRequestMsg(QString, int, int, int)));
    connect(newMigration, SIGNAL(updateStep(int)), this, SLOT(updateRequestStep(int)));
    while(migrationList.count()<originVer+1)                    //If no index, create it
    {
        migrationList.append(new db_migration_nomigration());
    }
    migrationList.replace(originVer, newMigration);             //Replace default migration by the new
}

bool db_migration_engine::migrate(int (*getDBVer)(), int targetVer)
{
    QEventLoop loop;
    emit updateRequestMsg("Migration System Started !", 0, 0, 255);
        QTimer::singleShot(250, &loop, SLOT(quit())); loop.exec();
    emit updateRequestMsg("Initializing...");
        QTimer::singleShot(500, &loop, SLOT(quit())); loop.exec();

    int stepNumber=0;
    int currentVersion=getDBVer();
    while(currentVersion!=targetVer)
    {
        if(currentVersion>=migrationList.count())
        {
            emit updateRequestMsg("Migration Missing", 255, 0, 0);
            return false;
        }

        if(targetVer>currentVersion)
        {
            stepNumber+=migrationList.at(currentVersion)->getStepCount();
            currentVersion++;
        }
        else
        {
            emit updateRequestMsg("Roolback is not implemented !", 255, 0, 0);
            return false;
        }
    }

    stepExec=0;
    emit updateRequestStep(0, stepNumber);

    currentVersion=getDBVer();
    while(currentVersion!=targetVer)
    {
        emit updateRequestMsg("Execute Migration "+QString::number(currentVersion), 0, 0, 255);
        switch(migrationList.at(currentVersion)->migrate())
        {
        case DB_MIGRATION_MISSING:
            emit updateRequestMsg("Migration Missing", 255, 0, 0);
            return false;
        case DB_MIGRATION_FAILURE:
            emit updateRequestMsg("Migration Error", 255, 0, 0);
            return false;
        case DB_MIGRATION_SUCCESS:
            stepExec+=migrationList.at(currentVersion)->getStepCount();
            break;
        }

        currentVersion=getDBVer();
    }

    emit updateRequestMsg("Migration Success", 0, 127, 0);

    return true;
}
