/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : manager.h
 *
 ****************************************/

#ifndef MANAGER_H
#define MANAGER_H

#include <QtCore>
#include <QMessageBox>

#include "global.h"
#include "../common/engine_db.h"
#include "window_login.h"
#include "window_main.h"

class manager : public QObject
{
    Q_OBJECT
public:
    manager();

private slots:
    void openLogin();
    void openMain();

private:
    QTranslator translator;
    window_login *winLogin;
    window_main *winMain;
};

#endif // MANAGER_H
