/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : engine_settings.cpp
 *
 ****************************************/

#include "engine_settings.h"

engine_settings::engine_settings()
{
    loaded=false;
}

bool engine_settings::open(QString filePath)
{
    if(!QFile::exists(filePath)) { return false; }

    QFile *file = new QFile(filePath);
    file->open(QFile::ReadOnly);
    int versionFile;
    QByteArray dataFile = file->readAll();
    file->close();

    QDataStream out(&dataFile, QIODevice::ReadWrite);
        out >> versionFile;
        out >> password;
        switch(versionFile)
        {
        case 1:
            QByteArray dataCrypt;
            out >> dataCrypt;
            SETTINGS_DECRYPT_MACRO;
            QDataStream out2(&data, QIODevice::ReadWrite);
            out2 >> app_translation;
            out2 >> db_local;
            out2 >> db_hostName;
            out2 >> db_userName;
            out2 >> db_password;
            out2 >> db_dataBase;
            out2 >> db_driver;
            out2 >> db_path;
            out2 >> login_noLogout;
            out2 >> login_getUser;
            out2 >> login_auto;
            out2 >> login_userName;
            out2 >> login_password;
            break;
        }

    loaded=true;
    return true;
}

void engine_settings::save(QString filePath)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::ReadWrite);

        out << app_translation;
        out << db_local;
        out << db_hostName;
        out << db_userName;
        out << db_password;
        out << db_dataBase;
        out << db_driver;
        out << db_path;
        out << login_noLogout;
        out << login_getUser;
        out << login_auto;
        out << login_userName;
        out << login_password;

    SETTINGS_ENCRYPT_MACRO;
    QByteArray dataFile;
    QDataStream out2(&dataFile, QIODevice::ReadWrite);
        out2 << (int)1;
        out2 << password;
        out2 << dataCrypt;

    QFile *file = new QFile(filePath);
    file->open(QFile::WriteOnly);
    file->write(dataFile);
    file->close();
}

void engine_settings::setDefault()
{
    loaded=false;
    db_local = true;
    app_translation="autoSystem";
    if(QFile::exists(APP_FILEFLAG_DESKTOPINSTALL))
    {
        db_path = "C:/ProgramData/Startingames/MemberStat/localDB.db";
    }
    else
    {
        db_path = "localDB.db";
    }
    db_hostName = "localHost";
    db_userName = "root";
    db_password = "root";
    db_dataBase = "memberstat";
    db_driver = "{MySQL ODBC 8.0 Unicode Driver}";

    login_noLogout = false;
    login_getUser = false;
    login_auto = false;
    login_userName = "";
    login_password = "";
}
