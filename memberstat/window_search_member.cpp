/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : window_search_member.cpp
 *
 ****************************************/

#include "window_search_member.h"

window_search_member::window_search_member(QMdiArea *_mainMdi, QWidget *_parent) : QMainWindow(_parent)
{
    mainMdi=_mainMdi;
    setWindowTitle(APP_NAME" - "+tr("Search Members"));

    module_search_member *moduleSearch = new module_search_member();
        connect(moduleSearch, SIGNAL(doubleCliked(uint64_t)), this, SLOT(openMember(uint64_t)));

    setCentralWidget(moduleSearch);

    QToolBar *toolBar = new QToolBar(this);
       toolBar->addAction(QIcon(ICON_SEARCH), tr("Search"), moduleSearch, &module_search_member::searchExec);
       toolBar->addSeparator();
       toolBar->addAction(QIcon(ICON_ADD_PEOPLE), tr("Add Member"), this, &window_search_member::addMember);
       toolBar->setEnabled(user::can(USER_PERM_MEMBER_ADD));

    addToolBar(toolBar);
}

void window_search_member::openMember(uint64_t memeberID)
{
    window_member *child = new window_member(memeberID, mainMdi, nullptr);
    QMdiSubWindow *sub = mainMdi->addSubWindow(child);
    child->subContainer = sub;
    child->show();
}

void window_search_member::addMember()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Create a new Member ?"), tr("Are you sure to create a new member ?"), QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        window_member *child = new window_member(0, mainMdi, nullptr, true);
        mainMdi->addSubWindow(child);
        child->show();
    }
}
