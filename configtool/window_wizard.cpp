#include "window_wizard.h"

window_wizard::window_wizard(QWidget *_parent) : QDialog(_parent)
{
    int id = QFontDatabase::addApplicationFont(APP_FONT);
    if(id<0)
    {
        QMessageBox::critical(nullptr, APP_NAME" - Missing File", "Unable to found MemberStat Font !\nIf the problem persist, try to re-install.");
        exit(EXIT_SUCCESS);
    }
    fontName = QFontDatabase::applicationFontFamilies(id).at(0);

    if(!QFile::exists(APP_LICENSE) || !QFile::exists(APP_LICENSE_BRAND))
    {
        QMessageBox::warning(nullptr, APP_NAME" - Missing File", "Unable to found Licenses File.\nIf the problem persist, try to re-install.");
        exit(EXIT_SUCCESS);
    }

    translator.load(QLocale(), "configtool", "_", APP_TRANSLATION_FOLDER, ".qm");
    QApplication::installTranslator(&translator);


    canClose = false;

    wizard.setupUi(this);

    wizard.progressBarWelcome->setRange(0, 2);
        wizard.progressBarDatabase->setRange(0,3);
        wizard.progressBarSettings->setRange(0,1);
        wizard.progressBarFinish->setRange(0,1);
    wizard.progressBarWelcome->setValue(0);
        wizard.progressBarDatabase->setValue(0);
        wizard.progressBarSettings->setValue(0);
        wizard.progressBarFinish->setValue(0);

    wizard.label_icon_title->setPixmap(QPixmap(APP_ICON));
        wizard.label_icon_title->setMargin(5);
    QFont titleFont(fontName);
        titleFont.setPixelSize(45);
        wizard.label_title->setFont(titleFont);

    stepWidget = new QWidget();
    wizard.horizontalLayout_widget->addWidget(stepWidget);

    settings = new engine_settings();

    setWindowTitle(APP_NAME);
    setWindowFlag(Qt::WindowMaximizeButtonHint);
    resize(0,0);

    show();

    setup_welcome1();
}

void window_wizard::closeEvent(QCloseEvent *event)
{
    if(canClose)
    {
        event->accept();
        qApp->quit();
        exit(EXIT_SUCCESS);
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Quit ?"), tr("Are you sure to quit the Settings Tool ?\nConfiguration will be aborted and not saved !"), QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        event->accept();
        qApp->quit();
        exit(EXIT_SUCCESS);
    }
    else
    {
        event->ignore();
    }
}

void window_wizard::setup_welcome1()
{
    settings->setDefault();
    if(!settings->open(APP_SETTINGS))
    {
        QMessageBox::information(this, tr("Settings File not Found"), tr("Unable to found settings file.\nDefaults settings are loaded. \nSettings password disabled !"));
    }
    else
    {
        if(settings->app_translation=="autoSystem")
        {
            translator.load(QLocale(), "configtool", "_", APP_TRANSLATION_FOLDER, ".qm");
        }
        else
        {
            translator.load(APP_TRANSLATION_FOLDER"/configtool_"+settings->app_translation);
        }
        QApplication::installTranslator(&translator);
    }

    stepWidget->deleteLater();
    stepWidget = new QWidget();
    step_welcome1.setupUi(stepWidget);

    if(settings->password.isEmpty())
    {
        step_welcome1.label_configPwd->setVisible(false);
        step_welcome1.lineEdit_configPwd->setVisible(false);
        step_welcome1.label->setVisible(false);
    }
    else
    {
        step_welcome1.label_2->setVisible(false);
    }

    connect(step_welcome1.button_import, SIGNAL(released()), this, SLOT(setup_welcome1_import()));
    connect(step_welcome1.button_export, SIGNAL(released()), this, SLOT(setup_welcome1_export()));
    connect(step_welcome1.pushButton, SIGNAL(released()), this, SLOT(setup_welcome1_nextButton()));

    wizard.horizontalLayout_widget->addWidget(stepWidget);
    if(settings->loaded)    { wizard.progressBarWelcome->setValue(2); }
    else                    { wizard.progressBarWelcome->setValue(1); }
}

void window_wizard::setup_welcome1_import()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Import Config"), QStandardPaths::writableLocation(QStandardPaths::HomeLocation), "Config File (*.ini)");
    if(path.isEmpty()) { return; }
    engine_settings *localSettings = new engine_settings();
    localSettings->open(path);
    if(localSettings->password!="")
    {
        QString password = QInputDialog::getText(this, tr("Settings Password"), tr("Please, enter the configuration password"));
        if(SETTINGS_PASSWORD_MACRO!=localSettings->password)
        {
            QMessageBox::warning(this, tr("Access Denied !"), tr("Invalid Password !\nImport Aborted !"));
            return;
        }
    }
    else
    {
        QMessageBox::warning(this, tr("No Password Set"), tr("Warning !\nNo Settings Password has been setted !"));
    }
    settings->open(path);
}

void window_wizard::setup_welcome1_export()
{
    if(!settings->password.isEmpty())
    {
        QString password = step_welcome1.lineEdit_configPwd->text();
        if(SETTINGS_PASSWORD_MACRO!=settings->password)
        {
            QMessageBox::warning(this, tr("Access Denied !"), tr("Invalid Password !"));
            return;
        }
    }
    QString path = QFileDialog::getSaveFileName(this, tr("Export Config"), QStandardPaths::writableLocation(QStandardPaths::HomeLocation), "Config File (*.ini)");
    if(path.isEmpty()) { return; }
    settings->save(path);
}

void window_wizard::setup_welcome1_nextButton()
{
    if(!settings->password.isEmpty())
    {
        QString password = step_welcome1.lineEdit_configPwd->text();
        if(SETTINGS_PASSWORD_MACRO!=settings->password)
        {
            QMessageBox::warning(this, tr("Access Denied !"), tr("Invalid Password !"));
            return;
        }
    }

    if(settings->loaded)    { setup_database2(); }
    else                    { setup_welcome2(); }
}

void window_wizard::setup_welcome2()
{
    stepWidget->deleteLater();
    stepWidget = new QWidget();
    step_welcome2.setupUi(stepWidget);

    QFile *file = new QFile(APP_LICENSE_BRAND);
    file->open(QFile::ReadOnly);
    step_welcome2.textBrowser_licence->setText(file->readAll());
    file->close();

    file = new QFile(APP_LICENSE);
    file->open(QFile::ReadOnly);
    step_welcome2.textBrowser_licence->setText(step_welcome2.textBrowser_licence->toPlainText()+file->readAll());
    file->close();

    step_welcome2.pushButton_next->setEnabled(false);
        connect(step_welcome2.pushButton_next, SIGNAL(released()), this, SLOT(setup_welcome2_nextButton()));
    connect(step_welcome2.checkBox_acceptLicence, SIGNAL(stateChanged(int)), this, SLOT(setup_welcome2_checkChange(int)));

    wizard.horizontalLayout_widget->addWidget(stepWidget);
    wizard.progressBarWelcome->setValue(2);
}

void window_wizard::setup_welcome2_checkChange(int)
{
    if(step_welcome2.checkBox_acceptLicence->isChecked())   { step_welcome2.pushButton_next->setEnabled(true); }
    else                                                    { step_welcome2.pushButton_next->setEnabled(false); }
}

void window_wizard::setup_welcome2_nextButton()
{
    if(!step_welcome2.checkBox_acceptLicence->isChecked()) { return; }
    if(settings->loaded)    { setup_database2(); }
    else                    { setup_database1(); }
}

void window_wizard::setup_database1()
{
    stepWidget->deleteLater();
    stepWidget = new QWidget();
    step_database1.setupUi(stepWidget);

    step_database1.Button_local->setIcon(QIcon(ICON_LOCAL));
    step_database1.Button_remote->setIcon(QIcon(ICON_SERVER));

    //step_database1.Button_local->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    //step_database1.Button_remote->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    connect(step_database1.Button_local, SIGNAL(released()), this, SLOT(setup_database1_local()));
    connect(step_database1.Button_remote, SIGNAL(released()), this, SLOT(setup_database1_server()));

    wizard.horizontalLayout_widget->addWidget(stepWidget);
    wizard.progressBarDatabase->setValue(1);
}

void window_wizard::setup_database1_local()
{
    settings->db_local=true;
    if(!settings->loaded)
    {
        settings->login_auto=true;
        settings->login_noLogout=true;
        settings->login_password="Admin";
        settings->login_userName="Admin";
    }
    setup_database2();
}

void window_wizard::setup_database1_server()
{
    settings->db_local=false;
    setup_database2();
}

void window_wizard::setup_database2()
{
    stepWidget->deleteLater();
    stepWidget = new QWidget();
    step_database2.setupUi(stepWidget);

    step_database2.pushButton_changeDatabase->setIcon(QIcon(ICON_LEFT));
    step_database2.label_validIcon->setMaximumSize(64, 64);

    step_database2.lineEdit_defaultPath->setText(settings->db_path);
    step_database2.lineEdit_hostname->setText(settings->db_hostName);
    step_database2.lineEdit_username->setText(settings->db_userName);
    step_database2.lineEdit_password->setText(settings->db_password);
    step_database2.lineEdit_database->setText(settings->db_dataBase);
    step_database2.lineEdit_driver->setText(settings->db_driver);

    step_database2.label_validIcon->setText("");
    step_database2.label_valid->setText("");

    if(settings->db_local)
    {
        step_database2.label_hostname->setVisible(false);
        step_database2.label_username->setVisible(false);
        step_database2.label_password->setVisible(false);
        step_database2.label_database->setVisible(false);
        step_database2.label_driver->setVisible(false);
        step_database2.lineEdit_hostname->setVisible(false);
        step_database2.lineEdit_username->setVisible(false);
        step_database2.lineEdit_password->setVisible(false);
        step_database2.lineEdit_database->setVisible(false);
        step_database2.lineEdit_driver->setVisible(false);
        step_database2.label_descriptionRemote->setVisible(false);
    }
    else
    {
        step_database2.label_defaultPath->setVisible(false);
        step_database2.lineEdit_defaultPath->setVisible(false);
        step_database2.label_descriptionLocal->setVisible(false);
    }

    connect(step_database2.pushButton_changeDatabase, SIGNAL(released()), this, SLOT(setup_database1()));
    connect(step_database2.button_test, SIGNAL(released()), this, SLOT(setup_database2_test()));
    connect(step_database2.button_next, SIGNAL(released()), this, SLOT(setup_database2_next()));

    wizard.horizontalLayout_widget->addWidget(stepWidget);
    wizard.progressBarDatabase->setValue(2);
}

bool window_wizard::setup_database2_test()
{
    stepWidget->setEnabled(false);
    step_database2.label_validIcon->setVisible(false);
    step_database2.label_valid->setText(tr("Testing..."));
    QCoreApplication::processEvents();
    settings->db_path=step_database2.lineEdit_defaultPath->text();
    settings->db_hostName=step_database2.lineEdit_hostname->text();
    settings->db_userName=step_database2.lineEdit_username->text();
    settings->db_password=step_database2.lineEdit_password->text();
    settings->db_dataBase=step_database2.lineEdit_database->text();
    settings->db_driver=step_database2.lineEdit_driver->text();

    if(engine_db::connect(settings))
    {
        step_database2.label_validIcon->setPixmap(QPixmap(ICON_CHECK).scaled(32, 32, Qt::KeepAspectRatio));
        step_database2.label_validIcon->setVisible(true);
        step_database2.label_valid->setText(tr("Connexion OK"));
        stepWidget->setEnabled(true);
        return true;
    }
    else
    {
        step_database2.label_validIcon->setPixmap(QPixmap(ICON_TIME).scaled(32, 32, Qt::KeepAspectRatio));
        step_database2.label_validIcon->setVisible(true);
        step_database2.label_valid->setText(tr("Unable to connect"));
        stepWidget->setEnabled(true);
        return false;
    }

}

void window_wizard::setup_database2_next()
{
    if(setup_database2_test())  { setup_database3(); }
}

void window_wizard::setup_database3()
{
    stepWidget->deleteLater();
    stepWidget = new QWidget();
    step_database3.setupUi(stepWidget);

    step_database3.button_next->setDisabled(true);
    step_database3.progressBar_infos->setRange(0, 0);
    step_database3.progressBar_infos->setValue(0);
    connect(step_database3.button_next, SIGNAL(released()), this, SLOT(setup_database3_next()));

    wizard.horizontalLayout_widget->addWidget(stepWidget);
    wizard.progressBarDatabase->setValue(3);

    step_database3.textEdit_infos->setHtml("Checking Database...");
    QCoreApplication::processEvents();

    QTimer *timer = new QTimer();
    timer->setSingleShot(true);
    timer->setInterval(2000);
    connect(timer, SIGNAL(timeout()), this, SLOT(setup_database3_exec()));
    timer->start();
}

void window_wizard::setup_database3_exec()
{
    bool pass=false;
    int dbCurVer = engine_db::getVersion();
    if(dbCurVer<0)
    {
        setup_database3_updateMsg("<span style=\"color: rgb(255,0,0);\">Unable to read DataBase Version !</span>");
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Initialize DB ?"), tr("Unable to read version of database. Do you want to initialize the database for the first time ?"), QMessageBox::Yes|QMessageBox::No);
        if (reply != QMessageBox::Yes)
        {
            setup_database3_updateMsg("<span style=\"color: rgb(255,127,0);\">Process stoped by user !</span>");
            setup_database3_updateMsg("<span style=\"color: rgb(0,0,0);\">You can close the tool.</span>");
            setup_database3_updateStep(0, 1);
            canClose = true;
            return;
        }
        setup_database3_updateMsg("<span style=\"color: rgb(0,0,255);\">Initializing Database</span>");
        if(!engine_db::createConfigTable())
        {
            setup_database3_updateMsg("<span style=\"color: rgb(255,0,0);\">Unable to Initialize the database !</span>");
            setup_database3_updateMsg("<span style=\"color: rgb(0,0,0);\">You can close the tool.</span>");
            setup_database3_updateStep(0, 1);
            canClose = true;
            return;
        }
        setup_database3_updateMsg("<span style=\"color: rgb(0,128,0);\">Database initialization SUCCESS</span>");
        dbCurVer = engine_db::getVersion();
        pass=true;
    }

    setup_database3_updateMsg("<span style=\"color: rgb(0,0,0);\">Current Database Version : "+QString::number(dbCurVer)+"</span>");
    if(dbCurVer==APP_DB_VERSION)
    {
        setup_database3_updateMsg("<span style=\"color: rgb(0,128,0);\">Database OK !</span>");
        step_database3.button_next->setDisabled(false);
        setup_database3_updateStep(1, 1);
        return;
    }

    setup_database3_updateMsg("<span style=\"color: rgb(255,0,0);\">Needed Database Version : "+QString::number(APP_DB_VERSION)+"</span>");
    if(!pass)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Update DB ?"), tr("The current database version is incompatible, did you want to update the database ?"), QMessageBox::Yes|QMessageBox::No);
        if (reply != QMessageBox::Yes)
        {
            setup_database3_updateMsg("<span style=\"color: rgb(255,127,0);\">Process stoped by user !</span>");
            setup_database3_updateMsg("<span style=\"color: rgb(0,0,0);\">You can close the tool.</span>");
            setup_database3_updateStep(0, 1);
            canClose = true;
            return;
        }
    }

    setup_database3_updateMsg("<span style=\"color: rgb(0,0,0);\"></span>");

    migrationEngine = new db_migration_engine();
    connect(migrationEngine, SIGNAL(updateMsg(QString)), this, SLOT(setup_database3_updateMsg(QString)));
    connect(migrationEngine, SIGNAL(updateStep(int, int)), this, SLOT(setup_database3_updateStep(int, int)));

    if(migrationEngine->migrate(engine_db::getVersion, APP_DB_VERSION))
    {
        setup_database3_updateMsg("<span style=\"color: rgb(0,128,0);\">Database OK !</span>");
        step_database3.button_next->setDisabled(false);
        setup_database3_updateStep(1, 1);
    }
    else
    {
        setup_database3_updateMsg("<span style=\"color: rgb(255,0,0);\">Migration ERROR !</span>");
        setup_database3_updateMsg("<span style=\"color: rgb(0,0,0);\">You can close the tool.</span>");
        canClose = true;
    }
}

void window_wizard::setup_database3_updateMsg(QString newMsg)
{
    step_database3.textEdit_infos->setHtml(step_database3.textEdit_infos->toHtml()+newMsg);
    step_database3.textEdit_infos->verticalScrollBar()->setValue(step_database3.textEdit_infos->verticalScrollBar()->maximum());
    QCoreApplication::processEvents();
}

void window_wizard::setup_database3_updateStep(int step, int max_step)
{
    if(max_step>0)
    {
        step_database3.progressBar_infos->setRange(0, max_step);
    }
    step_database3.progressBar_infos->setValue(step);
    QCoreApplication::processEvents();
}

void window_wizard::setup_database3_next()
{
    setup_settings1();
}

void window_wizard::setup_settings1()
{
    stepWidget->deleteLater();
    stepWidget = new QWidget();
    step_settings1.setupUi(stepWidget);

    step_settings1.comboBox_language->clear();
    step_settings1.comboBox_language->addItem(tr("System Default Langage"), "autoSystem");
    step_settings1.comboBox_language->addItem("en", "en");

    if(settings->login_auto || !settings->login_userName.isEmpty() || !settings->login_password.isEmpty())
    {
        step_settings1.groupBox_defaultCredentials->setChecked(true);
    }

    QStringList dataSlug;
    dataSlug.append("autoSystem");
    dataSlug.append("en");
    QDirIterator fileIt(APP_TRANSLATION_FOLDER, QStringList() << "memberstat_*.qm", QDir::Files | QDir::NoSymLinks);
        do
        {
            QString temp = fileIt.next();
            if(!temp.isEmpty())
            {
                temp.remove("ressources/translations/memberstat_");
                temp.remove(".qm");
                dataSlug.append(temp);
                step_settings1.comboBox_language->addItem(temp, temp);
            }

            qApp->processEvents();

        } while (fileIt.hasNext());

    int index = dataSlug.indexOf(settings->app_translation);
    if(index<0) { index=0; }
    step_settings1.comboBox_language->setCurrentIndex(index);
    step_settings1.checkBox_disableLogout->setChecked(settings->login_noLogout);
    step_settings1.checkBox_useWindowsUsername->setChecked(settings->login_getUser);
    step_settings1.checkBox_autoLogin->setChecked(settings->login_auto);
    step_settings1.lineEdit_usernameDefault->setText(settings->login_userName);
    step_settings1.lineEdit_passwordDefault->setText(settings->login_password);

    connect(step_settings1.button_next, SIGNAL(released()), this, SLOT(setup_settings1_next()));

    wizard.horizontalLayout_widget->addWidget(stepWidget);
    wizard.progressBarSettings->setValue(1);
}

void window_wizard::setup_settings1_next()
{
    if(!step_settings1.groupBox_defaultCredentials->isChecked())
    {
        step_settings1.checkBox_autoLogin->setChecked(false);
        step_settings1.lineEdit_usernameDefault->setText("");
        step_settings1.lineEdit_passwordDefault->setText("");
    }

    settings->app_translation=step_settings1.comboBox_language->currentData().toString();
    settings->login_noLogout=step_settings1.checkBox_disableLogout->isChecked();
    settings->login_getUser=step_settings1.checkBox_useWindowsUsername->isChecked();
    settings->login_auto=step_settings1.checkBox_autoLogin->isChecked();
    settings->login_userName=step_settings1.lineEdit_usernameDefault->text();
    settings->login_password=step_settings1.lineEdit_passwordDefault->text();

    QString password = step_settings1.lineEdit_configurationPassword->text();
    if(!password.isEmpty())
    {
        settings->password = SETTINGS_PASSWORD_MACRO;
    }

    settings->save(APP_SETTINGS);
    setup_endConfig1();
}

void window_wizard::setup_endConfig1()
{
    stepWidget->deleteLater();
    stepWidget = new QWidget();
    step_endconfig1.setupUi(stepWidget);

    connect(step_endconfig1.button_close, SIGNAL(released()), this, SLOT(setup_endConfig1_close()));
    connect(step_endconfig1.button_launch, SIGNAL(released()), this, SLOT(setup_endConfig1_start()));

    wizard.horizontalLayout_widget->addWidget(stepWidget);
    wizard.progressBarFinish->setValue(1);

    canClose = true;
}

void window_wizard::setup_endConfig1_close()
{
    qApp->quit();
}

void window_wizard::setup_endConfig1_start()
{
    //QProcess::startDetached(QCoreApplication::applicationDirPath()+"/MemberStat.exe");
    QDesktopServices::openUrl(QUrl("file:///"+QCoreApplication::applicationDirPath()+"/MemberStat.exe", QUrl::TolerantMode));
    qApp->quit();
}
