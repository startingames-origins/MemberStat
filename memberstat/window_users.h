/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : window_users.h
 *
 ****************************************/

#ifndef WINDOW_USERS_H
#define WINDOW_USERS_H

#include <QtWidgets>
#include "global.h"
#include "user.h"
#include "../common/engine_db.h"
#include "window_user_rights.h"

class window_users : public QMainWindow
{
    Q_OBJECT
public:
    window_users(QMdiArea *_mainMdi, QWidget *_parent = nullptr);

private slots:
    void filterChanged(QString filter);
    void tableSelChange();
    void updateView();
    void userAdd();
    void userRemove();
    void editUserName();
    void editPassword();
    void editRights();

private:
    QMdiArea *mainMdi;

    QAction *action_remove;
    QAction *action_edit;
    QAction *action_password;
    QAction *action_rights;

    QLineEdit *filter;
    QTableWidget *table;
};

#endif // WINDOW_USERS_H
