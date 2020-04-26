/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : global.h
 *
 ****************************************/

#ifndef GLOBAL_H
#define GLOBAL_H

#include <QFontDatabase>
#include <QObject>
#include <QMessageBox>
#include "../common/engine_settings.h"
#include "main.h"
#include <QDesktopServices>

class global : public QObject
{
    Q_OBJECT
public:
    explicit global(QObject *_parent = nullptr);

    static engine_settings *settings;
    static QString fontName;

    static bool init();

    static int age(QDate birthDate);

};

#endif // GLOBAL_H
