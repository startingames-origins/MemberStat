/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : engine_settings.h
 *
 ****************************************/

#ifndef ENGINE_SETTINGS_H
#define ENGINE_SETTINGS_H

#include <QObject>
#include <QtCore>
#include "common.h"

class engine_settings : public QObject
{
    Q_OBJECT
public:
    engine_settings();

    bool loaded;

    QString password;

    QString app_translation;

    bool db_local;
    QString db_hostName;
    QString db_userName;
    QString db_password;
    QString db_dataBase;
    QString db_driver;
    QString db_path;

    bool login_noLogout;
    bool login_getUser;
    bool login_auto;
    QString login_userName;
    QString login_password;

    bool open(QString filePath);
    void save(QString filePath);
    void setDefault();

};

#endif // ENGINE_SETTINGS_H
