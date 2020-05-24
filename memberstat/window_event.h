/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : window_event.h
 *
 ****************************************/

#ifndef WINDOW_EVENT_H
#define WINDOW_EVENT_H

#include <QtWidgets>
#include "global.h"
#include "../common/engine_db.h"
#include "user.h"
#include "module_search_member.h"
#include "window_member.h"

class window_event : public QMainWindow
{
    Q_OBJECT
public:
    window_event(uint64_t _eventID, QMdiArea *_mainMdi, QWidget *_parent = nullptr, bool _create = false);

public slots:
    void updateView();
    void lock();
    void unlock();
    void editTrig(bool isActive);
    void editSave();
    void editRemove();

private slots:
    void groupMemberUpdate();
    void groupMemberSelChange();
    void groupMemberAdd();
    void groupMemberAddReturn(uint64_t memberID);
    void groupMemberEdit();
    void groupMemberEditOpen(uint64_t memberID);
    void groupMemberEditReturn();
    void groupMemberRemove();
    void openMember(QTableWidgetItem *item);
    void commentUpdate();
    void commentAdd();
    void evtButtonsTable();
    void evtNewButton();
    void evtEditButton();
    void evtDeleteButton();
    void commentEdit();


private:
    QMdiArea *mainMdi;

    uint64_t eventID;

    QLineEdit *view_ID;
    QLineEdit *view_ref;
    QLineEdit *view_name;
    QLineEdit *view_desc;
    QDateTimeEdit *view_start;
    QDateTimeEdit *view_end;
    QDateTimeEdit *view_createdAt;
    QDateTimeEdit *view_editedAt;

    QWidget *group_member;
    QPushButton *group_member_add;
    QPushButton *group_member_edit;
    QPushButton *group_member_remove;
    QTableWidget *group_member_table;

    QWidget *view_events_comm;

    QPushButton *view_events_comm_new;
    QPushButton *view_events_comm_edit;
    QPushButton *view_events_comm_delete;

    QList<int> view_events_comm_listID;
    QListWidget *view_events_comm_table;
    QTextEdit *view_events_comm_text;

    QPushButton *view_events_comm_confirm_add;
    QPushButton *view_events_comm_confirm_edit;

    QTabWidget *tab_widget;

    QAction *action_edit;
    QAction *action_save;
    QAction *action_remove;

    QDialog *bwin_member_select;
    module_search_member *bwin_member_select_module;
    QLayout *bwin_member_select_layout;

    QDialog *bwin_member_edit;
    QLabel *bwin_member_label;
    QTextEdit *bwin_member_edit_comment;
    QPushButton *bwin_member_edit_button;
    QLayout *bwin_member_edit_layout;
    uint64_t bwin_member_edit_memberID;
};

#endif // WINDOW_EVENT_H
