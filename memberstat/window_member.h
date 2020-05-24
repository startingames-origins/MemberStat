/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : window_member.h
 *
 ****************************************/

#ifndef WINDOW_MEMBER_H
#define WINDOW_MEMBER_H

#include <QtWidgets>
#include "global.h"
#include "../common/engine_db.h"
#include "module_search_member.h"
#include "window_event.h"
#include "module_info_spetable.h"

class window_member : public QMainWindow
{
    Q_OBJECT
public:
    window_member(uint64_t _memberID, QMdiArea *_mainMdi, QWidget *_parent = nullptr, int _create = -1, uint64_t _createParent = 0);
    QMdiSubWindow *subContainer;

signals:

public slots:
    void updateView();
    void updateAge();
    void updateSpouse();
    void updateFather();
    void updateMother();
    void updateChilds();
    void updateEvents();
    void lock();
    void unlock();
    void editTrig(bool isActive);
    void editSave();
    void editRemove();

    void spouseRemove();
    void spouseSelect();
    void spouseSelectReturn(uint64_t memberID);
    void spouseOpen();
    void fatherRemove();
    void fatherSelect();
    void fatherSelectReturn(uint64_t memberID);
    void fatherOpen();
    void motherRemove();
    void motherSelect();
    void motherSelectReturn(uint64_t memberID);
    void motherOpen();

    void openMember(QTableWidgetItem *item);
    void openEvent(QTableWidgetItem *item);

    void addChild();
    void commentEdited();

private:
    QMdiArea *mainMdi;
    QWidget *centralWidget;

    uint64_t memberID;
    uint64_t spouseID;
    uint64_t fatherID;
    uint64_t motherID;

    QLineEdit *view_ID;
    QLineEdit *view_regNum;
    member_gender_widget *view_gender;
    QComboBox *view_memberType;
    QLineEdit *view_firstName;
    QLineEdit *view_otherName;
    QLineEdit *view_lastName;
    QDateEdit *view_birthDate;
    QLineEdit *view_Age;
    QLineEdit *view_birthLocation;
    QDateTimeEdit *view_createdAt;
    QDateTimeEdit *view_editedAt;

    QLabel *view_comment_label;
    QTextEdit *view_comment_edit;

    module_info_spetable *view_contact_phones_module;
    module_info_spetable *view_contact_emails_module;
    module_info_spetable *view_contact_addr_module;

    QLineEdit *view_spouse_regNum;
    QLineEdit *view_spouse_firstName;
    QLineEdit *view_spouse_lastName;
    QLineEdit *view_spouse_age;
    QPushButton *button_spouse_nobody;
    QPushButton *button_spouse_select;
    QPushButton *button_spouse_open;
    QLineEdit *view_father_regNum;
    QLineEdit *view_father_firstName;
    QLineEdit *view_father_lastName;
    QLineEdit *view_father_age;
    QPushButton *button_father_nobody;
    QPushButton *button_father_select;
    QPushButton *button_father_open;
    QLineEdit *view_mother_regNum;
    QLineEdit *view_mother_firstName;
    QLineEdit *view_mother_lastName;
    QLineEdit *view_mother_age;
    QPushButton *button_mother_nobody;
    QPushButton *button_mother_select;
    QPushButton *button_mother_open;

    QLabel *view_childs_count;
    QTableWidget *view_childs_table;

    QLabel *view_events_count;
    QTableWidget *view_events_table;

    QTabWidget *tab_widget;

    QAction *action_edit;
    QAction *action_save;
    QAction *action_remove;

    QDialog *bwin_member_select;
    module_search_member *bwin_member_select_module;
    QLayout *bwin_member_select_layout;
};

#endif // WINDOW_MEMBER_H
