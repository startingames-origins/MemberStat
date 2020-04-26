/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : window_main.h
 *
 ****************************************/

#ifndef WINDOW_MAIN_H
#define WINDOW_MAIN_H

#include <QtWidgets>

#include "window_config_gentype.h"
#include "window_search_member.h"
#include "window_search_event.h"
#include "window_users.h"
#include "global.h"

class window_main : public QMainWindow
{
    Q_OBJECT
public:
    window_main(QWidget *parent = nullptr);

signals:
    void logoutRequest();

public slots:
    void logout();

private slots:
    void about();
    void memberSearch();
    void openEvents();
    void configMemberType();
    void configMemberGender();
    void configUsers();
    void openRights();

private:
    QMdiArea *centralZone;
};

#endif // WINDOW_MAIN_H
