/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : user.h
 *
 ****************************************/

#ifndef USER_H
#define USER_H

#include <QtCore>
#include <QtWidgets>
#include "../common/engine_db.h"
#include "global.h"

#define USER_PERM_OPEN          0
#define USER_PERM_USER_ADMIN    1
#define USER_PERM_MEMBER_ADD    2
#define USER_PERM_MEMBER_EDIT   3
#define USER_PERM_MEMBER_REMOVE 4
#define USER_PERM_MEMBER_CONFIG 5
#define USER_PERM_EVENT_ADD     6
#define USER_PERM_EVENT_EDIT    7
#define USER_PERM_EVENT_REMOVE  8
#define USER_PERM_EVENT_COMMENT 9

class user : public QObject
{
    Q_OBJECT

public:
    user();

    static void updateDB();
    static bool login(QString username, QString userpass);
    static void logout();
    static bool can(int userperm);

    static int id;
    static QString name;
    static int rights;

    static QList<int> gender_id;
    static QStringList gender_text;

    static QList<int> type_id;
    static QStringList type_text;

public slots:
    static void changePassword();
};

#endif // USER_H
