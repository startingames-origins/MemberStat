/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : window_search_event.h
 *
 ****************************************/

#ifndef WINDOW_SEARCH_EVENT_H
#define WINDOW_SEARCH_EVENT_H

#include <QtWidgets>
#include "global.h"
#include "module_search_event.h"
#include "window_event.h"

class window_search_event : public QMainWindow
{
    Q_OBJECT
public:
    explicit window_search_event(QMdiArea *_mainMdi, QWidget *_parent = nullptr);

private slots:
    void openEvent(uint64_t eventID);
    void addEvent();

private:
    QMdiArea *mainMdi;

};

#endif // WINDOW_SEARCH_EVENT_H
