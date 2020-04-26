#include "window_user_rights.h"

window_user_rights::window_user_rights(uint64_t _userID, QWidget *_parent) : QMainWindow(_parent)
{
    userID=_userID;
    setWindowTitle(APP_NAME" - "+tr("User Rights"));

    headerLabel = new QLabel(this);

    cbox_access = new QCheckBox(tr("Can Login"), this);
    cbox_admin_users = new QCheckBox(tr("Manage Users"), this);
    cbox_member_add = new QCheckBox(tr("Create a new member"), this);
    cbox_member_edit = new QCheckBox(tr("Edit an existing member"), this);
    cbox_member_remove = new QCheckBox(tr("Remove an existing member"), this);
    cbox_member_config = new QCheckBox(tr("Configure member type and genre"), this);
    cbox_event_add = new QCheckBox(tr("Create a new event"), this);
    cbox_event_edit = new QCheckBox(tr("Edit an existing event"), this);
    cbox_event_remove = new QCheckBox(tr("Remove an existing event"), this);
    cbox_event_comment = new QCheckBox(tr("Comment an existing event"), this);

    QGroupBox *groupMember = new QGroupBox(tr("Members"), this);
    QGridLayout *groupMemberLayout = new QGridLayout();
        groupMemberLayout->addWidget(cbox_member_add);
        groupMemberLayout->addWidget(cbox_member_edit);
        groupMemberLayout->addWidget(cbox_member_remove);
        groupMemberLayout->addWidget(cbox_member_config);
    groupMember->setLayout(groupMemberLayout);

    QGroupBox *groupEvent = new QGroupBox(tr("Members"), this);
    QGridLayout *groupEventLayout = new QGridLayout();
        groupEventLayout->addWidget(cbox_event_add);
        groupEventLayout->addWidget(cbox_event_edit);
        groupEventLayout->addWidget(cbox_event_remove);
        groupEventLayout->addWidget(cbox_event_comment);
    groupEvent->setLayout(groupEventLayout);

    QGridLayout *mainLayout = new QGridLayout(this);
        mainLayout->addWidget(headerLabel, 0, 0, 1, 2);
        mainLayout->addWidget(cbox_access, 1, 0, 1, 2);
        mainLayout->addWidget(cbox_admin_users, 2, 0, 1, 2);
        mainLayout->addWidget(groupMember, 3, 0);
        mainLayout->addWidget(groupEvent, 3, 1);
    QWidget *centralWidget = new QWidget(this);
        centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);



    QToolBar *toolBar = new QToolBar(this);
       toolBar->addAction(QIcon(ICON_REFRESH), tr("Refresh"), this, &window_user_rights::updateView);
       toolBar->addSeparator();

    if(user::can(USER_PERM_USER_ADMIN))
    {
        toolBar->addAction(QIcon(ICON_SAVE), tr("Save"), this, &window_user_rights::save);
    }
    else
    {
        cbox_access->setEnabled(false);
        cbox_admin_users->setEnabled(false);
        cbox_member_add->setEnabled(false);
        cbox_member_edit->setEnabled(false);
        cbox_member_remove->setEnabled(false);
        cbox_member_config->setEnabled(false);
        cbox_event_add->setEnabled(false);
        cbox_event_edit->setEnabled(false);
        cbox_event_remove->setEnabled(false);
        cbox_event_comment->setEnabled(false);
    }

    addToolBar(toolBar);
    updateView();
}

void window_user_rights::updateView()
{
    QString username="";
    uint64_t rights=0;

    QSqlQuery query;
    query.prepare("SELECT username, rights FROM users WHERE id = "+QString::number(userID));
    //query.addBindValue(userID);
    if(query.exec())    //exec query
    {
        if(query.next())    //Get user infos
        {
            rights=query.record().value("rights").toUInt();
            username=query.record().value("username").toString();
        }
    }
    else
    {
        engine_db::dispERR(nullptr, query);
    }


    if(user::can(USER_PERM_USER_ADMIN))
    {
        headerLabel->setText(tr("Edit Rights for : ")+username);
    }
    else
    {
        headerLabel->setText(tr("You have not permission to edit rights for : ")+username);
    }

    cbox_access->setChecked((rights>>USER_PERM_OPEN)&1);
    cbox_admin_users->setChecked((rights>>USER_PERM_USER_ADMIN)&1);
    cbox_member_add->setChecked((rights>>USER_PERM_MEMBER_ADD)&1);
    cbox_member_edit->setChecked((rights>>USER_PERM_MEMBER_EDIT)&1);
    cbox_member_remove->setChecked((rights>>USER_PERM_MEMBER_REMOVE)&1);
    cbox_member_config->setChecked((rights>>USER_PERM_MEMBER_CONFIG)&1);
    cbox_event_add->setChecked((rights>>USER_PERM_EVENT_ADD)&1);
    cbox_event_edit->setChecked((rights>>USER_PERM_EVENT_EDIT)&1);
    cbox_event_remove->setChecked((rights>>USER_PERM_EVENT_REMOVE)&1);
    cbox_event_comment->setChecked((rights>>USER_PERM_EVENT_COMMENT)&1);
}

void window_user_rights::save()
{
    uint64_t rights=0;
    rights+=(uint64_t)cbox_access->isChecked()<<USER_PERM_OPEN;
    rights+=(uint64_t)cbox_admin_users->isChecked()<<USER_PERM_USER_ADMIN;
    rights+=(uint64_t)cbox_member_add->isChecked()<<USER_PERM_MEMBER_ADD;
    rights+=(uint64_t)cbox_member_edit->isChecked()<<USER_PERM_MEMBER_EDIT;
    rights+=(uint64_t)cbox_member_remove->isChecked()<<USER_PERM_MEMBER_REMOVE;
    rights+=(uint64_t)cbox_member_config->isChecked()<<USER_PERM_MEMBER_CONFIG;
    rights+=(uint64_t)cbox_event_add->isChecked()<<USER_PERM_EVENT_ADD;
    rights+=(uint64_t)cbox_event_edit->isChecked()<<USER_PERM_EVENT_EDIT;
    rights+=(uint64_t)cbox_event_remove->isChecked()<<USER_PERM_EVENT_REMOVE;
    rights+=(uint64_t)cbox_event_comment->isChecked()<<USER_PERM_EVENT_COMMENT;

    QSqlQuery query;
    query.prepare("UPDATE users SET rights=? WHERE id = ?");
    query.addBindValue(rights);
    query.addBindValue(userID);
    if(!query.exec())    //exec query
    {
        engine_db::dispERR(nullptr, query);
    }

    updateView();
}
