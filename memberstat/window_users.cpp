/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : window_users.cpp
 *
 ****************************************/

#include "window_users.h"

window_users::window_users(QMdiArea *_mainMdi, QWidget *_parent) : QMainWindow(_parent)
{
    mainMdi=_mainMdi;
    setWindowTitle(APP_NAME" - "+tr("Manage Users"));

    filter = new QLineEdit(this);
        filter->setPlaceholderText(tr("Username Filter"));
        connect(filter, SIGNAL(textChanged(QString)), this, SLOT(filterChanged(QString)));
    table = new QTableWidget(0, 3, this);
        table->setAlternatingRowColors(true);
        table->setStyleSheet("alternate-background-color: rgb(210,240,250);");
        table->setColumnHidden(0, true);
        table->verticalHeader()->setHidden(true);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        QStringList horizontalHeaders;
        horizontalHeaders << "id" << "" << tr("Username");
        table->setHorizontalHeaderLabels(horizontalHeaders);
        table->resizeColumnsToContents();
        table->verticalHeader()->setDefaultSectionSize(40);
        connect(table, SIGNAL(itemSelectionChanged()), this, SLOT(tableSelChange()));
        //connect(table, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(openMember(QTableWidgetItem*)));
    QGridLayout *layout = new QGridLayout(this);
        layout->addWidget(filter);
        layout->addWidget(table);
    QWidget *centralWidget = new QWidget(this);
        centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    QToolBar *toolBar = new QToolBar(this);
       toolBar->addAction(QIcon(ICON_REFRESH), tr("Refresh"), this, &window_users::updateView);
       toolBar->addSeparator();
       toolBar->addAction(QIcon(ICON_ADD_PEOPLE), tr("Add User"), this, &window_users::userAdd);
       action_remove = new QAction(QIcon(ICON_USERREMOVE), tr("Remove User"), this);
           connect(action_remove, SIGNAL(triggered()), this, SLOT(userRemove()));
       toolBar->addAction(action_remove);
       toolBar->addSeparator();
       action_edit = new QAction(QIcon(ICON_USEREDIT), tr("Edit UserName"), this);
           connect(action_edit, SIGNAL(triggered()), this, SLOT(editUserName()));
       toolBar->addAction(action_edit);
       action_password = new QAction(QIcon(ICON_KEY), tr("Change Password"), this);
           connect(action_password, SIGNAL(triggered()), this, SLOT(editPassword()));
       toolBar->addAction(action_password);
       action_rights = new QAction(QIcon(ICON_USERSETTING), tr("Edit Rights"), this);
           connect(action_rights, SIGNAL(triggered()), this, SLOT(editRights()));
       toolBar->addAction(action_rights);


    addToolBar(toolBar);

    updateView();
}

void window_users::filterChanged(QString filter)
{
    table->clearSelection();
    filter=filter.toLower();
    for(int i=0; i<table->rowCount(); i++)
    {
        if(table->item(i, 2)->text().toLower().contains(filter))
        {
            table->setRowHidden(i, false);
        }
        else
        {
            table->setRowHidden(i, true);
        }
    }
}

void window_users::tableSelChange()
{
    if(table->selectedItems().isEmpty())
    {
        action_remove->setEnabled(false);
        action_edit->setEnabled(false);
        action_password->setEnabled(false);
        action_rights->setEnabled(false);
    }
    else
    {
        action_remove->setEnabled(true);
        action_edit->setEnabled(true);
        action_password->setEnabled(true);
        action_rights->setEnabled(true);
    }
}

void window_users::updateView()
{
    int count=0;
    table->setSortingEnabled(false);
    table->setRowCount(0);


    QSqlQuery query;
    query.prepare("SELECT id, username, rights FROM users ORDER BY username");
    if(query.exec())    //exec query
    {
        while(query.next())    //Get user infos
        {
            table->setRowCount(count+1);
            table->setItem(count, 0, new QTableWidgetItem(QString::number(query.record().value("id").toInt())));
            if(!(query.record().value("rights").toInt()&1))
            {
                table->setItem(count, 1, new QTableWidgetItem(QIcon(ICON_USERLOCK),""));
            }
            table->setItem(count, 2, new QTableWidgetItem(query.record().value("username").toString()));
            count++;
        }
    }
    else
    {
        engine_db::dispERR(nullptr, query);
    }

    table->resizeColumnsToContents();
    table->resizeRowsToContents();
    tableSelChange();
    filter->setText("");
}

void window_users::userAdd()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Create a new User ?"), tr("Are you sure to create a new user ?\nDefault Password is Username.\nUser will have no rights. Please edit rights after !"), QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        QString username = QInputDialog::getText(this, tr("Create a new User"), tr("Username :"));
        if(!username.isEmpty())
        {
            QString userpass = username;
            USER_PASSWORD_MACRO
            QSqlQuery query;
            query.prepare("INSERT INTO users (username, password) VALUES (? ,?)");
            query.addBindValue(username);
            query.addBindValue(QString(USER_PASSWORD_MACRO2));
            if(!query.exec())
            {
                engine_db::dispERR(this, query);
            }
            updateView();
        }
    }
}

void window_users::userRemove()
{
    if(table->selectedItems().isEmpty()) { return; }
    int localID=table->item(table->selectedItems().takeFirst()->row(), 0)->text().toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Remove an User ?"), tr("Warning !\nRemove an user is generaly not a good idea. Please consider to remove access rights to this user.\nSure to remove ?"), QMessageBox::Yes|QMessageBox::No);
    if(reply != QMessageBox::Yes) { return; }

    reply = QMessageBox::question(this, tr("Remove an User ?"), tr("Association with this user will be lost !\nThis action cannot be canceled !\nSure to remove ?"), QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
        QSqlQuery query;
        query.prepare("DELETE FROM users WHERE id = ?");
        query.addBindValue(localID);
        if(!query.exec())    //exec query
        {
            engine_db::dispERR(nullptr, query);
        }

        updateView();
    }
}

void window_users::editUserName()
{
    if(table->selectedItems().isEmpty()) { return; }
    int localID=table->item(table->selectedItems().takeFirst()->row(), 0)->text().toInt();
    QString result="";

    QSqlQuery query;
    query.prepare("SELECT username FROM users WHERE id = ?");
    query.addBindValue(localID);
    if(query.exec())    //exec query
    {
        if(query.next())    //Get user infos
        {
            result = QInputDialog::getText(this, tr("Edit UserName"), tr("UserName :"), QLineEdit::Normal, query.record().value("username").toString());
        }
    }
    else
    {
        engine_db::dispERR(nullptr, query);
    }

    if(!result.isEmpty())
    {
        query.prepare("UPDATE users SET username=? WHERE id = ?");
        query.addBindValue(result);
        query.addBindValue(localID);
        if(!query.exec())    //exec query
        {
            engine_db::dispERR(nullptr, query);
        }
        updateView();
    }
}

void window_users::editPassword()
{
    if(table->selectedItems().isEmpty()) { return; }
    int localID=table->item(table->selectedItems().takeFirst()->row(), 0)->text().toInt();
    QString userpass="";
    QString username="";

    QSqlQuery query;
    query.prepare("SELECT username FROM users WHERE id = ?");
    query.addBindValue(localID);
    if(query.exec())    //exec query
    {
        if(query.next())    //Get user infos
        {
            username=query.record().value("username").toString();
        }
    }
    else
    {
        engine_db::dispERR(nullptr, query);
        return;
    }

    userpass = QInputDialog::getText(this, tr("Edit Password"), tr("Password for ")+username);

    if(!userpass.isEmpty())
    {
        USER_PASSWORD_MACRO
        query.prepare("UPDATE users SET password=? WHERE id = ?");
        query.addBindValue(QString(USER_PASSWORD_MACRO2));
        query.addBindValue(localID);
        if(!query.exec())    //exec query
        {
            engine_db::dispERR(nullptr, query);
        }
        updateView();
    }
}

void window_users::editRights()
{
    if(table->selectedItems().isEmpty()) { return; }
    int localID=table->item(table->selectedItems().takeFirst()->row(), 0)->text().toInt();
    window_user_rights *child = new window_user_rights(localID);
    mainMdi->addSubWindow(child);
    child->show();
}
