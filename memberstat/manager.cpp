/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : manager.cpp
 *
 ****************************************/

#include "manager.h"
#include <QMessageBox>

manager::manager()
{
    if(!global::init())
    {
        QMessageBox::critical(nullptr, APP_NAME" - Fatal Error", "Unable to Start MemberStat");
        exit(EXIT_SUCCESS);
    }

    if(global::settings->app_translation=="autoSystem")
    {
        translator.load(QLocale(), "memberstat", "_", APP_TRANSLATION_FOLDER, ".qm");
    }
    else
    {
        translator.load(APP_TRANSLATION_FOLDER"/memberstat_"+global::settings->app_translation);
    }
    QApplication::installTranslator(&translator);

    if(!engine_db::connect(global::settings))
    {
        exit(EXIT_SUCCESS);
        return;
    }

    if(APP_DB_VERSION!=engine_db::getVersion())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(nullptr, tr("Start Configuration Tool ?"), tr("MemberStat is incompatible with the current Database version.\nMemberStat will be closed.\nDid you want to start the Configuration Tool ?"), QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            //QProcess::startDetached(QCoreApplication::applicationDirPath()+"/ConfigTool.exe");
            QDesktopServices::openUrl(QUrl("file:///"+QCoreApplication::applicationDirPath()+"/ConfigTool.exe", QUrl::TolerantMode));
        }
        exit(EXIT_SUCCESS);
        return;
    }

    user::updateDB();
    user::logout();
    winLogin = new window_login();
    connect(winLogin, SIGNAL(loginSuccess()), this, SLOT(openMain()));
    winLogin->openshow(true);
}

void manager::openLogin()
{
    user::logout();
    winLogin = new window_login();
    connect(winLogin, SIGNAL(loginSuccess()), this, SLOT(openMain()));
    winMain->close();
    winLogin->openshow();
}

void manager::openMain()
{
    winMain = new window_main();
    connect(winMain, SIGNAL(logoutRequest()), this, SLOT(openLogin()));
    winLogin->close();
    winMain->show();
}
