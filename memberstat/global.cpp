/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : global.cpp
 *
 ****************************************/

#include "global.h"

engine_settings* global::settings = nullptr;
QString global::fontName = "";

global::global(QObject *_parent) : QObject(_parent)
{

}

bool global::init()
{
    if(!global::fontName.isEmpty()) { return false; }   //Allready initalized

    int id = QFontDatabase::addApplicationFont(APP_FONT);
    if(id<0)
    {
        QMessageBox::critical(nullptr, APP_NAME" - Missing File", "Unable to find MemberStat Font !\nIf the problem persist, try to re-install.");
        return false;
    }
    global::fontName = QFontDatabase::applicationFontFamilies(id).at(0);

    global::settings = new engine_settings;
    if(!global::settings->open(APP_SETTINGS))
    {
        QDesktopServices::openUrl(QUrl("file:///"+QCoreApplication::applicationDirPath()+"/ConfigTool.exe", QUrl::TolerantMode));
        exit(EXIT_SUCCESS);
        return false;
    }

    return true;
}

int global::age(QDate birthDate)
{
    QDate currentDate = QDate::currentDate();
    int age = currentDate.year() - birthDate.year();

    if(currentDate.month() < birthDate.month())
    {
        age--;
    }
    else if(currentDate.day() < birthDate.day() && currentDate.month() == birthDate.month())
    {
        age--;
    }

    return age;
}
