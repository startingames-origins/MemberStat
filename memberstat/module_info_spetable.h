#ifndef MODULE_INFO_SPETABLE_H
#define MODULE_INFO_SPETABLE_H

#include <QtWidgets>
#include "global.h"
#include "../common/engine_db.h"

class module_info_spetable : public QWidget
{
    Q_OBJECT
public:
    module_info_spetable(QStringList _names, QStringList _namesDB, QString _tableDB, QString _relidNameDB, int _relid, QWidget *_parent = nullptr, bool _displayMsgAutoSave = true);
    QStringList getNames();

signals:

public slots:
    void update();

private slots:
    void selChange();
    void add();
    void add_exec();
    void edit();
    void edit_exec();
    void remove();

private:
    QStringList names;
    QStringList namesDB;
    QString tableDB;
    QString relidNameDB;
    int relid;

    QTableWidget *table;
    QLabel *result;
    QPushButton *button_add;
    QPushButton *button_edit;
    QPushButton *button_remove;

    QDialog *bwin_addedit;
    QGridLayout *bwin_addedit_layout;
    QHBoxLayout *bwin_addedit_layout_button;
    QList<QLabel*> bwin_addedit_labels;
    QList<QLineEdit*> bwin_addedit_ledits;
    QPushButton *bwin_addedit_cancel;
    QPushButton *bwin_addedit_confirm;
    QSpacerItem *bwin_addedit_spacer;
    int bwin_addedit_id;
    QMetaObject::Connection bwin_addedit_confirm_connection;

};

#endif // MODULE_INFO_SPETABLE_H
