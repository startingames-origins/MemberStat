#ifndef MODULE_SEARCH_MEMBER_H
#define MODULE_SEARCH_MEMBER_H

#include <QtWidgets>
#include "global.h"
#include "../common/engine_db.h"
#include "user.h"
#include "spoilerwidget.h"
#include "module_select_display.h"

class module_search_member : public QWidget
{
    Q_OBJECT
public:
    module_search_member(QWidget *_parent = nullptr);

    QLineEdit *regNum;
    member_gender_widget *gender;
    QComboBox *memberType;
    QLineEdit *firstName;
    QCheckBox *otherName;
    QLineEdit *lastName;
    QCheckBox *autoLast;
    QSpinBox *ageMin;
    QSpinBox *ageMax;

    module_select_display *selectDisplay;

    QGroupBox *option_child_box;
    QSpinBox *option_child_ageMax;
    QGroupBox *option_event_box;
    QDateEdit *option_event_dateMin;
    QDateEdit *option_event_dateMax;


public slots:
    void searchExec();

private slots:
    void selDisplay();
    void autoLastChanged(int newValue);
    void openMember(QTableWidgetItem *item);

signals:
    void doubleCliked(uint64_t member_id);

private:
    QLabel *result;
    QPushButton *search;
    QTableWidget *table;

};

#endif // MODULE_SEARCH_MEMBER_H
