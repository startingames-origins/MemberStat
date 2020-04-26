/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : window_login.h
 *
 ****************************************/

#include "window_login.h"

window_login::window_login(QWidget *_parent) : QWidget(_parent)
{
    setWindowTitle(APP_NAME" - "+tr("Login"));
    setWindowIcon(QIcon(APP_ICON));
    resize(640, 480);

    label_memberstat1 = new QLabel(this);
        label_memberstat1->setPixmap(QPixmap(APP_ICON));
        label_memberstat1->setMargin(5);
    label_memberstat2 = new QLabel(APP_NAME, this);
        QFont monospace(global::fontName);
        monospace.setPixelSize(90);
        label_memberstat2->setFont(monospace);

    label_brand = new QLabel(this);
        label_brand->setPixmap(QPixmap(IMG_BRAND));
        label_brand->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        label_brand->setAlignment(Qt::AlignBottom);

    label_text = new QLabel(this);
        label_text->setAlignment(Qt::AlignCenter);
        QFont Update_stateFont = label_text->font();
        Update_stateFont.setPointSize(12);
        Update_stateFont.setBold(true);
        label_text->setFont(Update_stateFont);
    ledit_UserName = new QLineEdit("", this);
        ledit_UserName->setPlaceholderText(tr("User name..."));
    ledit_Password = new QLineEdit("", this);
        ledit_Password->setPlaceholderText(tr("Password..."));
        ledit_Password->setEchoMode(QLineEdit::Password);
        connect(ledit_UserName, SIGNAL(returnPressed()), ledit_Password, SLOT(setFocus()));
        connect(ledit_Password, SIGNAL(returnPressed()), this, SLOT(loginButton()));
    button_login = new QPushButton(QIcon(ICON_LOGIN),"", this); button_login->setToolTip(tr("Login"));
        button_login->setDefault(false);
        button_login->setIconSize(QSize(button_login->size().height(),button_login->size().height()));
        button_login->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        button_login->setAutoDefault(false);
        connect(button_login, SIGNAL(released()), this, SLOT(loginButton()));
    button_ok = new QPushButton(tr("OK"), this);
        connect(button_ok, SIGNAL(released()), this, SLOT(okButton()));

    QSpacerItem *Spacer3 = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *SpacerTop = new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem *SpacerBottom = new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem *Spacer6 = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *Spacer7 = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *Spacer8 = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *Spacer9 = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    QGridLayout *lay = new QGridLayout();
    lay->addWidget(button_login,0,0);
    lay->addItem(Spacer7, 0, 1);

    QGridLayout *LoginLayout = new QGridLayout();
        LoginLayout->addWidget(label_text, 2, 0, 1, 3);
        LoginLayout->addWidget(ledit_UserName, 3, 1);
        LoginLayout->addWidget(ledit_Password, 4, 1);
        LoginLayout->addWidget(button_ok, 5, 1);
        LoginLayout->addLayout(lay, 3, 2, 2, 1);
        LoginLayout->addItem(Spacer3, 1, 0);
        LoginLayout->addItem(Spacer6, 1, 2);

    QLabel *version = new QLabel(tr("Version")+" "+APP_VERSION, this);
        version->setAlignment(Qt::AlignBottom | Qt::AlignRight);

    QHBoxLayout *TopLayout = new QHBoxLayout();
        TopLayout->addItem(Spacer8);
        TopLayout->addWidget(label_memberstat1);
        TopLayout->addWidget(label_memberstat2);
        TopLayout->addItem(Spacer9);

    QHBoxLayout *BottomLayout = new QHBoxLayout();
        BottomLayout->addWidget(label_brand);
        BottomLayout->addWidget(version);

//creating and set main layout
    QGridLayout *MainLayout = new QGridLayout();
        MainLayout->addItem(SpacerTop, 0, 0);
        MainLayout->addLayout(TopLayout, 0, 0);
        MainLayout->addLayout(LoginLayout, 1, 0);
        MainLayout->addItem(SpacerBottom, 2, 0);
        MainLayout->addLayout(BottomLayout, 2, 0);
    setLayout(MainLayout);
}

void window_login::openshow(bool _firstopen)
{
    okButton();
    show();
    ledit_UserName->setFocus();

    if(_firstopen && global::settings->login_auto)
    {
        ledit_UserName->setText(global::settings->login_userName);
        ledit_Password->setText(global::settings->login_password);
        loginButton();
    }
    else if(global::settings->login_getUser)
    {
        ledit_UserName->setText(getenv("USERNAME"));
        ledit_Password->setFocus();
    }
    else
    {
        ledit_UserName->setText(global::settings->login_userName);
        ledit_Password->setText(global::settings->login_password);
    }
}

void window_login::loginButton()
{
    label_text->setText(tr("Please Wait..."));
    ledit_UserName->setVisible(false);
    ledit_Password->setVisible(false);
    button_login->setVisible(false);

    if(user::login(ledit_UserName->text(), ledit_Password->text()))
    {
        if(user::can(USER_PERM_OPEN))
        {
            emit loginSuccess();
            label_text->setText(tr("Welcome !"));
        }
        else
        {
            user::logout();
            label_text->setText(tr("Access Denied !\nYour Account are locked !\nPlease Contact an Users Manager."));
            button_ok->setVisible(true);
            button_ok->setDefault(true);
            button_ok->setFocus();
        }
    }
    else
    {
        label_text->setText(tr("Unknow User !\nInvalid Username or Password !"));
        button_ok->setVisible(true);
        button_ok->setDefault(true);
        button_ok->setFocus();
    }
}

void window_login::okButton()
{
    label_text->setText(tr("Please Login"));
    button_ok->setVisible(false);
    button_ok->setDefault(false);

    ledit_Password->setText("");
    ledit_UserName->setVisible(true);
    ledit_Password->setVisible(true);
    button_login->setVisible(true);
    ledit_Password->setFocus();
}
