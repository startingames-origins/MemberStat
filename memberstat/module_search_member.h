#ifndef MODULE_SEARCH_MEMBER_H
#define MODULE_SEARCH_MEMBER_H

#include <QtWidgets>
#include "global.h"
#include "../common/engine_db.h"
#include "user.h"

class module_search_member : public QWidget
{
    Q_OBJECT
public:
    module_search_member(QWidget *_parent = nullptr);

    QLineEdit *regNum;
    QComboBox *gender;
    QComboBox *memberType;
    QLineEdit *firstName;
    QCheckBox *otherName;
    QLineEdit *lastName;
    QCheckBox *autoLast;
    QSpinBox *ageMin;
    QSpinBox *ageMax;

public slots:
    void searchExec();

private slots:
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
