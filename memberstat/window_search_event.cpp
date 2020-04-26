/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : window_search_event.cpp
 *
 ****************************************/

#include "window_search_event.h"

window_search_event::window_search_event(QMdiArea *_mainMdi, QWidget *_parent) : QMainWindow(_parent)
{
    mainMdi=_mainMdi;
    setWindowTitle(APP_NAME" - "+tr("Search Events"));

    module_search_event *moduleSearch = new module_search_event();
        connect(moduleSearch, SIGNAL(doubleCliked(uint64_t)), this, SLOT(openEvent(uint64_t)));

    setCentralWidget(moduleSearch);

    QToolBar *toolBar = new QToolBar(this);
       toolBar->addAction(QIcon(ICON_SEARCH), tr("Search"), moduleSearch, &module_search_event::searchExec);
       toolBar->addSeparator();
       toolBar->addAction(QIcon(ICON_ADD_EVENT), tr("Add Event"), this, &window_search_event::addEvent);
       toolBar->setEnabled(user::can(USER_PERM_EVENT_ADD));

    addToolBar(toolBar);
}

void window_search_event::openEvent(uint64_t eventID)
{
    window_event *child = new window_event(eventID, mainMdi, nullptr);
    mainMdi->addSubWindow(child);
    child->show();
}

void window_search_event::addEvent()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Create a new Event ?"), tr("Are you sure to create a new event ?"), QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        window_event *child = new window_event(0, mainMdi, nullptr, true);
        mainMdi->addSubWindow(child);
        child->show();
    }
}
