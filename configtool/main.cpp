/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat - ConfigTool
 * File : main.cpp
 *
 ****************************************/

#include <QApplication>
#include <QTextCodec>

#include "window_wizard.h"

int main(int argc, char* argv[])
{
//Accent management
#if (QT_VERSION < 0x05000)
    QTextCodec::setCodecForTr (QTextCodec::codecForName ("UTF-8"));
    QTextCodec::setCodecForCStrings (QTextCodec::codecForName ("UTF-8"));
#endif
    QTextCodec::setCodecForLocale (QTextCodec::codecForName ("UTF-8"));

//Start app
    QCoreApplication::addLibraryPath("resources/plugins");
    QApplication app(argc, argv);

    new window_wizard();

    return app.exec();
}
