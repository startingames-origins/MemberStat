/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : module_search_event.h
 *
 ****************************************/

#ifndef MODULE_SEARCH_EVENT_H
#define MODULE_SEARCH_EVENT_H

#include <QtWidgets>
#include "global.h"
#include "../common/engine_db.h"

class module_search_event : public QWidget
{
    Q_OBJECT
public:
    module_search_event(QWidget *_parent = nullptr);
    QLineEdit *ref;
    QLineEdit *name;
    QCheckBox *desc;
    QDateTimeEdit *start;
    QDateTimeEdit *end;

public slots:
    void searchExec();

private slots:
    void openEvent(QTableWidgetItem *item);

signals:
    void doubleCliked(uint64_t member_id);

private:
    QLabel *result;
    QPushButton *search;
    QTableWidget *table;
};

#endif // MODULE_SEARCH_EVENT_H
