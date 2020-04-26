/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : engine_db.h
 *
 ****************************************/

#ifndef ENGINE_DB_H
#define ENGINE_DB_H

#include <QObject>
#include <QtSql>
#include <QMessageBox>
#include "engine_settings.h"

#include "main.h"

class engine_db : public QObject
{
public:
    engine_db();

    static bool connect(engine_settings *settings);
    static void dispERR(QWidget *parent, QSqlQuery query);
    static QString echap(QString text);

    static int getVersion();
    static bool createConfigTable();

    static bool local;
    static QSqlDatabase db;

};

#endif // ENGINE_DB_H
