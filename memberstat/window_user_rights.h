#ifndef WINDOW_USER_RIGHTS_H
#define WINDOW_USER_RIGHTS_H

#include <QtWidgets>
#include "global.h"
#include "user.h"
#include "../common/engine_db.h"

class window_user_rights : public QMainWindow
{
    Q_OBJECT
public:
    window_user_rights(uint64_t _userID, QWidget *_parent = nullptr);

private slots:
    void updateView();
    void save();

private:
    bool can(uint64_t rights, int userperm);
    uint64_t userID;

    QLabel *headerLabel;

    QCheckBox *cbox_access;
    QCheckBox *cbox_admin_users;
    QCheckBox *cbox_member_add;
    QCheckBox *cbox_member_edit;
    QCheckBox *cbox_member_remove;
    QCheckBox *cbox_member_config;
    QCheckBox *cbox_event_add;
    QCheckBox *cbox_event_edit;
    QCheckBox *cbox_event_remove;
    QCheckBox *cbox_event_comment;


};

#endif // WINDOW_USER_RIGHTS_H
