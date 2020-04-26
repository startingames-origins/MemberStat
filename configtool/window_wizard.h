#ifndef WINDOW_WIZARD_H
#define WINDOW_WIZARD_H

#include <QtWidgets>
#include "main.h"
#include "ui_window_wizard.h"
#include "ui_widget_welcome1.h"
#include "ui_widget_welcome2.h"
#include "ui_widget_database1.h"
#include "ui_widget_database2.h"
#include "ui_widget_database3.h"
#include "ui_widget_settings1.h"
#include "ui_widget_endConfiguration1.h"
#include "../common/engine_settings.h"
#include "../common/engine_db.h"
#include "db_migration/db_migration_engine.h"

class window_wizard : public QDialog
{
    Q_OBJECT
public:
    window_wizard(QWidget *_parent = nullptr);

public slots:
    void closeEvent(QCloseEvent *event);
    void setup_welcome1();
    void setup_welcome1_import();
    void setup_welcome1_export();
    void setup_welcome1_nextButton();
    void setup_welcome2();
    void setup_welcome2_checkChange(int);
    void setup_welcome2_nextButton();
    void setup_database1();
    void setup_database1_local();
    void setup_database1_server();
    void setup_database2();
    bool setup_database2_test();
    void setup_database2_next();
    void setup_database3();
    void setup_database3_exec();
    void setup_database3_updateMsg(QString newMsg);
    void setup_database3_updateStep(int step, int max_step);
    void setup_database3_next();
    void setup_settings1();
    void setup_settings1_next();
    void setup_endConfig1();
    void setup_endConfig1_close();
    void setup_endConfig1_start();

private:
    QString fontName;
    QTranslator translator;

    bool canClose;

    db_migration_engine *migrationEngine;

    engine_settings *settings;
    Ui::window_wizard wizard;
    QWidget *stepWidget;
    Ui::Widget_welcome1 step_welcome1;
    Ui::Widget_welcome2 step_welcome2;
    Ui::Widget_database1 step_database1;
    Ui::Widget_database2 step_database2;
    Ui::Widget_database3 step_database3;
    Ui::Widget_settings1 step_settings1;
    Ui::Widget_endConfiguration1 step_endconfig1;
};

#endif // WINDOW_WIZARD_H
