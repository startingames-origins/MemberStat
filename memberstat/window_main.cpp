/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : window_main.cpp
 *
 ****************************************/

#include "window_main.h"

window_main::window_main(QWidget *parent) : QMainWindow(parent)
{
//config window
    setWindowTitle(APP_NAME);
    setWindowIcon(QIcon(APP_ICON));

    centralZone = new QMdiArea(this);
    setCentralWidget(centralZone);

    //dock_launcher *launch = new dock_launcher(this, CentralZone);
    //addDockWidget(Qt::TopDockWidgetArea, launch);
    //connect(launch->logout, SIGNAL(released()), this, SLOT(logout()));


    QMenu *menuFile = menuBar()->addMenu(tr("File"));
        QMenu *menuFileUser = menuFile->addMenu(tr("User"));
        menuFileUser->addAction(QIcon(ICON_USEREDIT), tr("Change Password"), &user::changePassword);
        menuFileUser->addAction(QIcon(ICON_USERSETTING), tr("Rights"), this, &window_main::openRights);
        if(!global::settings->login_noLogout)
        {
            menuFile->addAction(QIcon(ICON_LOGOUT), tr("Logout"), this, &window_main::logout);
            menuFile->addSeparator();
        }
        menuFile->addAction(QIcon(ICON_CLOSE), tr("Quit"), this, &QWidget::close);

    if(user::can(USER_PERM_USER_ADMIN) || user::can(USER_PERM_MEMBER_CONFIG))
    {
        QMenu *menuConfig = menuBar()->addMenu(tr("Config"));
        if(user::can(USER_PERM_USER_ADMIN))
        {
            menuConfig->addAction(tr("Manage Users"), this, &window_main::configUsers);
        }
        if(user::can(USER_PERM_MEMBER_CONFIG))
        {
            menuConfig->addAction(tr("Member Type"), this, &window_main::configMemberType);
        }
    }

    QMenu *menuAbout = menuBar()->addMenu(tr("?"));
        menuAbout->addAction(QIcon(ICON_QUESTION), tr("About"), this, &window_main::about);
        menuAbout->addAction(QIcon(ICON_QT), tr("About Qt"), this, &QApplication::aboutQt);

    QToolBar *toolBar = new QToolBar(this);
       toolBar->addAction(QIcon(ICON_PEOPLE), tr("Members"), this, &window_main::memberSearch);
       toolBar->addSeparator();
       toolBar->addAction(QIcon(ICON_CALENDAR), tr("Events"), this, &window_main::openEvents);

    addToolBar(toolBar);

}

void window_main::about()
{
    QMessageBox::about(this, APP_NAME" - "+tr("About"), tr("Member Stat is an application for manager members.")+tr("\nVersion")+" "+APP_VERSION+"\n\nMemberStat is open-source for community improvements but\nStartingames is the only one authorized to distribute the software !\nAll copies are prohibited !");
}

void window_main::logout()
{
    emit logoutRequest();
}

void window_main::memberSearch()
{
    window_search_member *child = new window_search_member(centralZone, nullptr);
    centralZone->addSubWindow(child);
    child->show();
}

void window_main::openEvents()
{
    window_search_event *child = new window_search_event(centralZone, nullptr);
    centralZone->addSubWindow(child);
    child->show();
}

void window_main::configMemberType()
{
    if(!user::can(USER_PERM_MEMBER_CONFIG)) { return; }
    window_config_gentype *child = new window_config_gentype("member_type");
    centralZone->addSubWindow(child);
    child->show();
}

void window_main::configUsers()
{
    if(!user::can(USER_PERM_USER_ADMIN)) { return; }
    window_users *child = new window_users(centralZone);
    centralZone->addSubWindow(child);
    child->show();
}

void window_main::openRights()
{
    window_user_rights *child = new window_user_rights(user::id);
    centralZone->addSubWindow(child);
    child->show();
}
