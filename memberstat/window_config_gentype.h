#ifndef WINDOW_CONFIG_GENTYPE_H
#define WINDOW_CONFIG_GENTYPE_H

#include <QtWidgets>
#include "global.h"
#include "../common/engine_db.h"

class window_config_gentype : public QMainWindow
{
    Q_OBJECT

public:
    window_config_gentype(QString _genType, QWidget *_parent = nullptr);

private slots:
    void updateView();
    void selectionChanged();
    void add();
    void edit();
    void editExec();
    void remove();

private:
    QString genType;

    QAction *actionRefresh;
    QAction *actionAdd;
    QAction *actionEdit;
    QAction *actionDelete;

    QTableWidget *table;

    QDialog *bwin_edit;
    int bwin_edit_id;
    QSpinBox *bwin_edit_priority;
    QLineEdit *bwin_edit_name;
    QLineEdit *bwin_edit_desc;
    QPushButton *bwin_edit_save;

};

#endif // WINDOW_CONFIG_GENTYPE_H
