/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : user.cpp
 *
 ****************************************/

#include "user.h"

int user::id = 0;
QString user::name = "";
int user::rights = 0;

QList<int> user::type_id;
QStringList user::type_text;

user::user()
{
    logout();       //Init user to nothing
}

void user::updateDB()
{
    QSqlQuery query;

    user::type_id.clear();
    user::type_text.clear();
    user::type_id.append(-1);
    user::type_text.append("-");

    query.prepare("SELECT id, name FROM member_type ORDER BY priority DESC");
    if(query.exec())    //exec query
    {
        while(query.next())    //Get user infos
        {
            user::type_id.append(query.value(query.record().indexOf("id")).toInt());
            user::type_text.append(query.value(query.record().indexOf("name")).toString());
        }
    }
    else
    {
        engine_db::dispERR(nullptr, query);
    }
}

bool user::login(QString username, QString userpass)
{
    USER_PASSWORD_MACRO

    QSqlQuery query;
    query.prepare("SELECT id, rights FROM users WHERE username = ? AND password = ?");
    query.addBindValue(username);
    query.addBindValue(QString(USER_PASSWORD_MACRO2));
    if(query.exec())    //exec query
    {
        if(query.next())    //Get user infos
        {
            user::id=query.value(query.record().indexOf("id")).toInt();
            user::rights=query.value(query.record().indexOf("rights")).toInt();
            user::name=username;
            return true;
        }
    }
    else
    {
        engine_db::dispERR(nullptr, query);
    }
    return false;
}

void user::logout()
{
    user::id=0;
    user::name="";
    user::rights=0;
}

bool user::can(int userperm)
{
    if(user::rights&(1<<userperm))
    {
        return true;
    }
    return false;
}

void user::changePassword()
{
    QString username = user::name;
    QString userpass = QInputDialog::getText(nullptr, tr("Edit Password"), tr("Password for ")+user::name);

    if(!userpass.isEmpty())
    {
        QSqlQuery query;
        USER_PASSWORD_MACRO
        query.prepare("UPDATE users SET password=? WHERE id = ?");
        query.addBindValue(QString(USER_PASSWORD_MACRO2));
        query.addBindValue(user::id);
        if(!query.exec())    //exec query
        {
            engine_db::dispERR(nullptr, query);
        }
    }
}
