/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : window_login.h
 *
 ****************************************/

#ifndef WINDOW_LOGIN_H
#define WINDOW_LOGIN_H

#include <QtWidgets>
#include "user.h"
#include "global.h"

class window_login : public QWidget
{
    Q_OBJECT
public:
    window_login(QWidget *_parent = nullptr);
    void openshow(bool _firstopen = false);

signals:
    void loginSuccess();

private slots:
    void loginButton();
    void okButton();

private:

    QLabel *label_memberstat1;
    QLabel *label_memberstat2;
    QLabel *label_brand;

    QLabel *label_text;
    QLineEdit *ledit_UserName;
    QLineEdit *ledit_Password;
    QPushButton *button_login;
    QPushButton *button_ok;
};

#endif // WINDOW_LOGIN_H
