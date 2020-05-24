/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : window_search_member.cpp
 *
 ****************************************/

#include "window_search_member.h"

window_search_member::window_search_member(QMdiArea *_mainMdi, QWidget *_parent) : QMainWindow(_parent)
{
    mainMdi=_mainMdi;
    setWindowTitle(APP_NAME" - "+tr("Search Members"));

    moduleSearch = new module_search_member();
        connect(moduleSearch, SIGNAL(doubleCliked(uint64_t)), this, SLOT(openMember(uint64_t)));

    setCentralWidget(moduleSearch);

    QToolBar *toolBar = new QToolBar(this);
        toolBar->addAction(QIcon(ICON_SEARCH), tr("Search"), moduleSearch, &module_search_member::searchExec);
        toolBar->addSeparator();
        toolBar->addAction(QIcon(ICON_ADD_PEOPLE), tr("Add Member"), this, &window_search_member::addMember);
        toolBar->setEnabled(user::can(USER_PERM_MEMBER_ADD));
        toolBar->addSeparator();
        toolBar->addAction(QIcon(ICON_SAVE), tr("Save"), this, &window_search_member::saveSearch);
        toolBar->addAction(QIcon(ICON_OPEN), tr("Open"), this, &window_search_member::loadSearch);

    addToolBar(toolBar);
}

void window_search_member::openMember(uint64_t memeberID)
{
    window_member *child = new window_member(memeberID, mainMdi, nullptr);
    QMdiSubWindow *sub = mainMdi->addSubWindow(child);
    child->subContainer = sub;
    child->show();
}

void window_search_member::addMember()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Create a new Member ?"), tr("Are you sure to create a new member ?"), QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        window_member *child = new window_member(0, mainMdi, nullptr, true);
        QMdiSubWindow *sub = mainMdi->addSubWindow(child);
        child->subContainer = sub;
        child->show();
    }
}

void window_search_member::saveSearch()
{
    QDir dir; dir.mkpath(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/Startingames/MemberStat");
    QString fname = QFileDialog::getSaveFileName(this, tr("Save Memberstat search"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/Startingames/MemberStat/search-save-" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".memberstat", "Memberstat (*.memberstat)" );

    QSettings settings(fname, QSettings::IniFormat);

    settings.setValue("ageMax", moduleSearch->ageMax->value());
    settings.setValue("ageMin", moduleSearch->ageMin->value());
    settings.setValue("gender", moduleSearch->gender->currentIndex());
    settings.setValue("regNum", moduleSearch->regNum->text());
    settings.setValue("autoLast", moduleSearch->autoLast->isChecked());
    settings.setValue("lastName", moduleSearch->lastName->text());
    settings.setValue("firstName", moduleSearch->firstName->text());
    settings.setValue("otherName", moduleSearch->otherName->isChecked());
    settings.setValue("memberType", moduleSearch->memberType->currentIndex());
    settings.setValue("option_child_box", moduleSearch->option_child_box->isChecked());
    settings.setValue("option_child_ageMax", moduleSearch->option_child_ageMax->value());
    settings.setValue("option_event_box", moduleSearch->option_event_box->isChecked());
    settings.setValue("option_event_dateMin", moduleSearch->option_event_dateMin->date());
    settings.setValue("option_event_dateMax", moduleSearch->option_event_dateMax->date());

    for(int i=0; i<moduleSearch->selectDisplay->count(); i++)
    {
        settings.setValue("selectDisplay"+QString::number(i), moduleSearch->selectDisplay->getState(i));
    }

    settings.sync();
}

void window_search_member::loadSearch()
{
    QString fname = QFileDialog::getOpenFileName(this, tr("Load Memberstat search"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/Startingames/MemberStat", "Memberstat (*.memberstat)" );

    QSettings settings(fname, QSettings::IniFormat);

    moduleSearch->ageMax->setValue(settings.value("ageMax",                     moduleSearch->ageMax->value()).toInt());
    moduleSearch->ageMin->setValue(settings.value("ageMin",                     moduleSearch->ageMin->value()).toInt());
    moduleSearch->gender->setCurrentIndex(settings.value("gender",              moduleSearch->gender->currentIndex()).toInt());
    moduleSearch->regNum->setText(settings.value("regNum",                      moduleSearch->regNum->text()).toString());
    moduleSearch->autoLast->setChecked(settings.value("autoLast",               moduleSearch->autoLast->isChecked()).toBool());
    moduleSearch->lastName->setText(settings.value("lastName",                  moduleSearch->firstName->text()).toString());
    moduleSearch->firstName->setText(settings.value("firstName",                moduleSearch->firstName->text()).toString());
    moduleSearch->otherName->setChecked(settings.value("otherName",             moduleSearch->otherName->isChecked()).toBool());
    moduleSearch->memberType->setCurrentIndex(settings.value("memberType",      moduleSearch->memberType->currentIndex()).toInt());
    moduleSearch->option_child_box->setChecked(settings.value("option_child_box",      moduleSearch->option_child_box->isChecked()).toBool());
    moduleSearch->option_child_ageMax->setValue(settings.value("option_child_ageMax",      moduleSearch->option_child_ageMax->value()).toInt());
    moduleSearch->option_event_box->setChecked(settings.value("option_event_box",      moduleSearch->option_event_box->isChecked()).toBool());
    moduleSearch->option_event_dateMin->setDate(settings.value("option_event_dateMin",      moduleSearch->option_event_dateMin->date()).toDate());
    moduleSearch->option_event_dateMax->setDate(settings.value("option_event_dateMax",      moduleSearch->option_event_dateMax->date()).toDate());

    for(int i=0; i<moduleSearch->selectDisplay->count(); i++)
    {
        moduleSearch->selectDisplay->setState(i, settings.value("selectDisplay"+QString::number(i), moduleSearch->selectDisplay->getState(i)).toBool());
    }

    moduleSearch->searchExec();
}
