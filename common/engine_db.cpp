/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : engine_db.h
 *
 ****************************************/

#include "engine_db.h"

QSqlDatabase engine_db::db;
bool engine_db::local = false;

engine_db::engine_db() : QObject()
{

}

bool engine_db::connect(engine_settings *settings)
{
    local=settings->db_local;
    if(settings->db_local)   //Local dataBase
    {
        QFileInfo finfo(settings->db_path);
        QDir dir;
        dir.mkpath(finfo.path());
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(settings->db_path);
    }
    else
    {
        db = QSqlDatabase::addDatabase("QODBC3");
        db.setHostName(settings->db_hostName);
        db.setUserName(settings->db_userName);
        db.setPassword(settings->db_password);
        db.setDatabaseName("Driver="+settings->db_driver+";DATABASE="+settings->db_dataBase+";");
    }

    if(!db.open())
    {
        QMessageBox::critical(nullptr, APP_NAME" - "+tr("Fatal Error"), tr("DB connection error !\n") + db.lastError().text());
        return false;
    }
    return true;
}

void engine_db::dispERR(QWidget *parent, QSqlQuery query)
{
    QMessageBox::warning(parent, APP_NAME" - "+tr("Query error"), query.lastError().text());
}

QString engine_db::echap(QString text)
{
    text.replace("\\", "\\\\");
    text.replace("\'", "\\\'");
    text.replace("\"", "\\\"");
    text.replace("%", "\\%");

    return text;
}

int engine_db::getVersion()
{
    QSqlQuery query;
    query.prepare("SELECT * FROM config WHERE id=1");
    if(query.exec())
    {
        if(query.next())
        {
            return query.record().value("number").toInt();
        }
    }

    return -1;
}

bool engine_db::createConfigTable()
{
    QSqlQuery query;
    //Delete config table
    query.prepare("DROP TABLE config");
    query.exec();

    //Create config table
    query.prepare("CREATE TABLE `config` ("
                  "`id` INT UNSIGNED NOT NULL,"
                  "`number` INT NULL,"
                  "`text` TINYTEXT NULL,"
                  "PRIMARY KEY (`id`))");
    if(!query.exec()) { dispERR(nullptr, query); return false; }

    //Add DB_version (ID 1)
    query.prepare("INSERT INTO config (`id`, `number`) VALUES ('1', '0');");
    if(!query.exec()) { dispERR(nullptr, query); return false; }
    return true;
}
