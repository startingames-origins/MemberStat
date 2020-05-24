/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : window_search_member.h
 *
 ****************************************/

#ifndef WINDOW_SEARCH_MEMBER_H
#define WINDOW_SEARCH_MEMBER_H

#include <QtWidgets>
#include "global.h"
#include "window_member.h"
#include "module_search_member.h"

class window_search_member : public QMainWindow
{
    Q_OBJECT
public:
    explicit window_search_member(QMdiArea *_mainMdi, QWidget *_parent = nullptr);

private slots:
    void openMember(uint64_t memberID);
    void addMember();

    void saveSearch();
    void loadSearch();

private:
    QMdiArea *mainMdi;
    module_search_member *moduleSearch;
};

#endif // WINDOW_SEARCH_MEMBER_H
