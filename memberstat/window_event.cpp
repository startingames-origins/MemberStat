/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : window_event.cpp
 *
 ****************************************/

#include "window_event.h"

window_event::window_event(uint64_t _eventID, QMdiArea *_mainMdi, QWidget *_parent, bool _create) : QMainWindow(_parent)
{
    eventID=_eventID;
    mainMdi=_mainMdi;
    setWindowTitle(APP_NAME" - "+tr("Event"));

    QLabel *label_ID = new QLabel("ID", this);
    QLabel *label_ref = new QLabel("Reference", this);
    QLabel *label_name = new QLabel("Name", this);
    QLabel *label_desc = new QLabel("Description", this);
    QLabel *label_start = new QLabel("Start", this);
    QLabel *label_end = new QLabel("End", this);
    QLabel *label_createdAt = new QLabel("Created At", this);
    QLabel *label_editedAt = new QLabel("Edited At", this);

    view_ID = new QLineEdit(this);
        view_ID->setDisabled(true);
    view_ref = new QLineEdit(this);
        view_ref->setMaxLength(250);
    view_name = new QLineEdit(this);
        view_name->setMaxLength(250);
    view_desc = new QLineEdit(this);
        view_desc->setMaxLength(250);
    view_start = new QDateTimeEdit(this);
        view_start->setCalendarPopup(true);
    view_end = new QDateTimeEdit(this);
        view_end->setCalendarPopup(true);
    view_createdAt = new QDateTimeEdit(this);
        view_createdAt->setDisabled(true);
    view_editedAt = new QDateTimeEdit(this);
        view_editedAt->setDisabled(true);

    group_member = new QWidget(this);
        group_member_add = new QPushButton(QIcon(ICON_ADD_PEOPLE), tr("Add"), this);
            connect(group_member_add, SIGNAL(released()), this, SLOT(groupMemberAdd()));
        group_member_edit = new QPushButton(QIcon(ICON_USEREDIT), tr("Edit"), this);
            connect(group_member_edit, SIGNAL(released()), this, SLOT(groupMemberEdit()));
        group_member_remove = new QPushButton(QIcon(ICON_USERREMOVE), tr("Remove"), this);
            connect(group_member_remove, SIGNAL(released()), this, SLOT(groupMemberRemove()));
        group_member_table = new QTableWidget(0, 7, this);
            group_member_table->setAlternatingRowColors(true);
            group_member_table->setStyleSheet("alternate-background-color: rgb(210,240,250);");
            group_member_table->setColumnHidden(0, true);
            group_member_table->verticalHeader()->setHidden(true);
            group_member_table->setSelectionMode(QAbstractItemView::SingleSelection);
            group_member_table->setSelectionBehavior(QAbstractItemView::SelectRows);
            group_member_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
            QStringList horizontalHeaders;
            horizontalHeaders << "id" << tr("Reg Num") << tr("First Name") << tr("Last Name") << tr("Member Type") << tr("Age") << tr("Comment");
            group_member_table->setHorizontalHeaderLabels(horizontalHeaders);
            group_member_table->resizeColumnsToContents();
            group_member_table->sortByColumn(1, Qt::AscendingOrder);
            group_member_table->setSortingEnabled(true);
            group_member_table->verticalHeader()->setDefaultSectionSize(40);
            connect(group_member_table, SIGNAL(itemSelectionChanged()), this, SLOT(groupMemberSelChange()));
            connect(group_member_table, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(openMember(QTableWidgetItem*)));
        QLabel *group_member_label = new QLabel(tr("Actions in this group are auto-saved !"), this);
        QGridLayout *group_member_layout = new QGridLayout(this);
            group_member_layout->addWidget(group_member_label, 0, 0, 1, 3);
            group_member_layout->addWidget(group_member_add, 1, 0);
            group_member_layout->addWidget(group_member_edit, 1, 1);
            group_member_layout->addWidget(group_member_remove, 1, 2);
            group_member_layout->addWidget(group_member_table, 2, 0, 1, 3);
    group_member->setLayout(group_member_layout);

    view_events_comm = new QWidget(this);

    view_events_comm_new = new QPushButton(QIcon(ICON_PLUS), tr("New"), this);
        view_events_comm_new->setVisible(user::can(USER_PERM_EVENT_COMMENT));
        connect(view_events_comm_new, SIGNAL(released()), this, SLOT(evtNewButton()));

    view_events_comm_edit = new QPushButton(QIcon(ICON_EDIT), tr("Edit"), this);
        view_events_comm_edit->setVisible(user::can(USER_PERM_EVENT_COMMENT));
        connect(view_events_comm_edit, SIGNAL(released()), this, SLOT(evtEditButton()));

    view_events_comm_delete = new QPushButton(QIcon(ICON_TRASH), tr("Delete"), this);
        view_events_comm_delete->setVisible(user::can(USER_PERM_EVENT_COMMENT));
        connect(view_events_comm_delete, SIGNAL(released()), this, SLOT(evtDeleteButton()));

    view_events_comm_table = new QListWidget(this);
        view_events_comm_table->setAlternatingRowColors(true);
        view_events_comm_table->setStyleSheet("alternate-background-color: rgb(210,240,250);");
        connect(view_events_comm_table, SIGNAL(itemSelectionChanged()), this, SLOT(evtButtonsTable()));

    view_events_comm_text = new QTextEdit(this);
        view_events_comm_text->setVisible(user::can(USER_PERM_EVENT_COMMENT));
        view_events_comm_text->setContextMenuPolicy(Qt::NoContextMenu);
        view_events_comm_text->setStyleSheet("background-color: rgb(250,250,200);");

    //Add button
    view_events_comm_confirm_add = new QPushButton(QIcon(ICON_PLUS), tr("Add this Comment"), this);
        view_events_comm_confirm_add->setVisible(user::can(USER_PERM_EVENT_COMMENT));
        connect(view_events_comm_confirm_add, SIGNAL(released()), this, SLOT(commentAdd()));

    //Edit button
    view_events_comm_confirm_edit = new QPushButton(QIcon(ICON_EDIT), tr("Edit this Comment"), this);
        view_events_comm_confirm_edit->setVisible(user::can(USER_PERM_EVENT_COMMENT));
        connect(view_events_comm_confirm_edit, SIGNAL(released()), this, SLOT(commentEdit()));


    QGridLayout *view_events_comm_event = new QGridLayout(this);
        view_events_comm_event->addWidget(view_events_comm_new, 0, 0);
        view_events_comm_event->addWidget(view_events_comm_edit, 0, 1);
        view_events_comm_event->addWidget(view_events_comm_delete, 0, 2);
        view_events_comm_event->addWidget(view_events_comm_table, 1,0,1,3);
        view_events_comm_event->addWidget(view_events_comm_text, 2,0,1,3);
        view_events_comm_event->addWidget(view_events_comm_confirm_add, 3,0,1,3);
        view_events_comm_event->addWidget(view_events_comm_confirm_edit, 4,0,1,3);
    view_events_comm->setLayout(view_events_comm_event);

    tab_widget = new QTabWidget(this);
        tab_widget->addTab(group_member, tr("Participants"));
        tab_widget->addTab(view_events_comm, tr("Comments"));

    QHBoxLayout *baseinfoLayout = new QHBoxLayout(this);
        baseinfoLayout->addWidget(label_ID);
        baseinfoLayout->addWidget(view_ID);
        baseinfoLayout->addWidget(label_ref);
        baseinfoLayout->addWidget(view_ref);
        baseinfoLayout->addWidget(label_createdAt);
        baseinfoLayout->addWidget(view_createdAt);
        baseinfoLayout->addWidget(label_editedAt);
        baseinfoLayout->addWidget(view_editedAt);

    QGridLayout *nameinfoLayout = new QGridLayout(this);
        nameinfoLayout->addWidget(label_name, 0, 0);
        nameinfoLayout->addWidget(label_start, 0, 1);
        nameinfoLayout->addWidget(label_end, 0, 2);
        nameinfoLayout->addWidget(view_name, 1, 0);
        nameinfoLayout->addWidget(view_start, 1, 1);
        nameinfoLayout->addWidget(view_end, 1, 2);

    QHBoxLayout *descinfoLayout = new QHBoxLayout(this);
        descinfoLayout->addWidget(label_desc);
        descinfoLayout->addWidget(view_desc);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->addLayout(baseinfoLayout);
        mainLayout->addLayout(nameinfoLayout);
        mainLayout->addLayout(descinfoLayout);
        mainLayout->addWidget(tab_widget);


    QWidget *centralWidget = new QWidget(this);
        centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    QToolBar *toolBar = new QToolBar(this);
        toolBar->addAction(QIcon(ICON_REFRESH), tr("Refresh"), this, &window_event::updateView);
        toolBar->addSeparator();
        action_edit = new QAction(QIcon(ICON_EDIT), tr("Edit"), this);
            action_edit->setCheckable(true);
            connect(action_edit, SIGNAL(triggered(bool)), this, SLOT(editTrig(bool)));
        toolBar->addAction(action_edit);
        action_save = new QAction(QIcon(ICON_SAVE), tr("Save"), this);
            connect(action_save, SIGNAL(triggered()), this, SLOT(editSave()));
        toolBar->addAction(action_save);
        action_remove = new QAction(QIcon(ICON_TRASH), tr("Remove"), this);
            connect(action_remove, SIGNAL(triggered()), this, SLOT(editRemove()));
        toolBar->addAction(action_remove);

        action_edit->setEnabled(user::can(USER_PERM_EVENT_EDIT));
        action_save->setVisible(user::can(USER_PERM_EVENT_EDIT));
        action_remove->setVisible(user::can(USER_PERM_EVENT_EDIT) && user::can(USER_PERM_EVENT_REMOVE));

    addToolBar(toolBar);
    lock();

    if(_create)
    {
        eventID=0;
        QSqlQuery query;
        QSqlQuery query2;
        QString uuidText = QUuid::createUuid().toString();
        query.prepare("INSERT INTO events (name, `desc`) VALUES (? ,?)");
        query.addBindValue(tr("New Event - Unamed"));
        query.addBindValue(uuidText);
        if(query.exec())
        {
            query.prepare("SELECT id FROM events WHERE `desc`=? ORDER BY id DESC");
            query.addBindValue(uuidText);
            if(query.exec())
            {
                if(query.next())
                {
                    eventID = query.record().value("id").toUInt();
                }
            }
            else
            {
                engine_db::dispERR(this, query);
            }
        }
        else
        {
            engine_db::dispERR(this, query);
        }
        action_edit->setChecked(true);
        unlock();
    }

    view_ID->setText(QString::number(eventID));
    updateView();
}


void window_event::evtNewButton()
{
    evtButtonsTable();
    view_events_comm_text->setVisible(true);
    view_events_comm_confirm_add->setVisible(true);


}

void window_event::evtEditButton()
{
    evtButtonsTable();
    view_events_comm_text->setVisible(true);
    view_events_comm_confirm_edit->setVisible(true);

    QString temp = view_events_comm_table->selectedItems().first()->text();
    int pause = temp.indexOf('\n');
    pause = temp.indexOf('\n', pause+1);
    //TODO: Rappatrie l'information
    view_events_comm_text->setText(temp.mid(pause + 1));

}

void window_event::evtDeleteButton()
{
    if(view_events_comm_table->selectedItems().isEmpty()) { return; }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Remove a comment ?"), tr("Are you sure to remove this comment ?"), QMessageBox::Yes|QMessageBox::No);

    if(reply == QMessageBox::Yes)
    {
        QSqlQuery query;
        query.prepare("DELETE FROM events_comments WHERE id = ?");
        query.addBindValue(view_events_comm_listID.at(view_events_comm_table->row(view_events_comm_table->selectedItems().takeFirst())));
        if(!query.exec())    //exec query
        {
            engine_db::dispERR(nullptr, query);
        }
    }
    commentUpdate();
}

void window_event::commentEdit()
{
    QSqlQuery query;
    query.prepare("UPDATE events_comments SET comment=?, updatedAt= CURRENT_TIMESTAMP WHERE id = ?");
    query.addBindValue(view_events_comm_text->toPlainText());
    query.addBindValue(view_events_comm_listID.at(view_events_comm_table->row(view_events_comm_table->selectedItems().takeFirst())));
    if(!query.exec())
    {
        engine_db::dispERR(this, query);
    }

    commentUpdate();
}

void window_event::evtButtonsTable()
{
    view_events_comm_text->setText("");
    view_events_comm_text->setVisible(false);
    view_events_comm_confirm_add->setVisible(false);
    view_events_comm_confirm_edit->setVisible(false);

    if(view_events_comm_table->selectedItems().isEmpty())
    {
        view_events_comm_edit->setEnabled(false);
        view_events_comm_delete->setEnabled(false);
    }
    else
    {
        view_events_comm_edit->setEnabled(true);
        view_events_comm_delete->setEnabled(true);
    }

}

void window_event::updateView()
{
    QSqlQuery query;
    query.prepare("SELECT * FROM events WHERE id = "+QString::number(eventID));
    //query.addBindValue((int)eventID);
    if(query.exec())
    {
        if(query.next())
        {
            view_ref->setText(query.record().value("ref").toString());
            view_name->setText(query.record().value("name").toString());
            view_desc->setText(query.record().value("desc").toString());
            view_start->setDateTime(query.record().value("start").toDateTime());
            view_end->setDateTime(query.record().value("end").toDateTime());
            view_createdAt->setDateTime(query.record().value("createdAt").toDateTime());
            view_editedAt->setDateTime(query.record().value("updatedAt").toDateTime());
        }
    }
    else
    {
        engine_db::dispERR(this, query);
    }

    groupMemberUpdate();
    commentUpdate();
}

void window_event::lock()
{
    view_ref->setReadOnly(true);
    view_name->setReadOnly(true);
    view_desc->setReadOnly(true);
    view_start->setReadOnly(true);
    view_end->setReadOnly(true);

    action_save->setEnabled(false);
    action_remove->setEnabled(false);
}

void window_event::unlock()
{
    view_ref->setReadOnly(false);
    view_name->setReadOnly(false);
    view_desc->setReadOnly(false);
    view_start->setReadOnly(false);
    view_end->setReadOnly(false);

    action_save->setEnabled(true);
    action_remove->setEnabled(true);
}

void window_event::editTrig(bool isActive)
{
    if(isActive)
    {
        unlock();
    }
    else
    {
        lock();
        updateView();
    }
}

void window_event::editSave()
{
    QSqlQuery query;
    query.prepare("UPDATE events SET ref=?, name=?, `desc`=?, start=?, end=?, updatedAt= CURRENT_TIMESTAMP WHERE id = ?");
    query.addBindValue(view_ref->text());
    query.addBindValue(view_name->text());
    query.addBindValue(view_desc->text());
    query.addBindValue(view_start->dateTime());
    query.addBindValue(view_end->dateTime());
    query.addBindValue(eventID);
    if(!query.exec())
    {
        engine_db::dispERR(this, query);
    }

    action_edit->setChecked(false);
    lock();
    updateView();
}

void window_event::editRemove()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Remove an event ?"), tr("Warning !\nRemove an event is generaly not a good idea.\nSure to remove ?"), QMessageBox::Yes|QMessageBox::No);
    if(reply != QMessageBox::Yes) { return; }

    reply = QMessageBox::question(this, tr("Remove an event ?"), tr("Association with this event will be lost !\nThis action cannot be canceled !\nSure to remove ?"), QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
        QSqlQuery query;
        query.prepare("DELETE FROM events WHERE id = ?");
        query.addBindValue(eventID);
        if(!query.exec())    //exec query
        {
            engine_db::dispERR(nullptr, query);
        }

        query.prepare("DELETE FROM events_members WHERE event = ?");
        query.addBindValue(eventID);
        if(!query.exec())    //exec query
        {
            engine_db::dispERR(nullptr, query);
        }

        this->close();
    }
}

void window_event::groupMemberSelChange()
{
    if(group_member_table->selectedItems().isEmpty())
    {
        group_member_edit->setEnabled(false);
        group_member_remove->setEnabled(false);
    }
    else
    {
        group_member_edit->setEnabled(true);
        group_member_remove->setEnabled(true);
    }
}

void window_event::groupMemberUpdate()
{
    user::updateDB();
    int count=0;
    group_member_table->setSortingEnabled(false);
    group_member_table->setRowCount(0);


    QSqlQuery query;
    query.prepare("SELECT events_members.comment, member, regNum, firstName, lastName, type, birthDate FROM members JOIN events_members ON members.id = events_members.member WHERE event = "+QString::number(eventID));
    //query.addBindValue(eventID);
    if(query.exec())    //exec query
    {
        while(query.next())    //Get user infos
        {
            group_member_table->setRowCount(count+1);
            group_member_table->setItem(count, 0, new QTableWidgetItem(QString::number(query.record().value("member").toInt())));
            group_member_table->setItem(count, 1, new QTableWidgetItem(query.record().value("regNum").toString()));
            group_member_table->setItem(count, 2, new QTableWidgetItem(query.record().value("firstName").toString()));
            group_member_table->setItem(count, 3, new QTableWidgetItem(query.record().value("lastName").toString()));
            int temp = user::type_id.indexOf(query.record().value("type").toInt());
            if(temp<1) { temp=0; }
            group_member_table->setItem(count, 4, new QTableWidgetItem(user::type_text.at(temp)));
            group_member_table->setItem(count, 5, new QTableWidgetItem(QString::number(global::age(query.record().value("birthDate").toDate()))));
            group_member_table->setItem(count, 6, new QTableWidgetItem(query.record().value("comment").toString()));

            count++;
        }
    }
    else
    {
        engine_db::dispERR(nullptr, query);
    }

    group_member_table->sortByColumn(2, Qt::AscendingOrder);
    group_member_table->setSortingEnabled(true);
    group_member_table->resizeColumnsToContents();
    group_member_table->resizeRowsToContents();

    groupMemberSelChange();
}

void window_event::groupMemberAdd()
{
    bwin_member_select = new QDialog(this, Qt::Tool);
    bwin_member_select->setWindowTitle(tr("Select a Participant (Member)"));
    bwin_member_select_module = new module_search_member();
    connect(bwin_member_select_module, SIGNAL(doubleCliked(uint64_t)), this, SLOT(groupMemberAddReturn(uint64_t)));
    bwin_member_select_layout = new QGridLayout();
    bwin_member_select_layout->addWidget(bwin_member_select_module);
    bwin_member_select->setLayout(bwin_member_select_layout);
    bwin_member_select->exec();
}

void window_event::groupMemberAddReturn(uint64_t memberID)
{
    bwin_member_select->close();

    QSqlQuery query;
    query.prepare("SELECT * FROM events_members WHERE event = ? AND member = ?");
    query.addBindValue(eventID);
    query.addBindValue(memberID);
    if(query.exec())
    {
        if(query.next())
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, tr("A member is already participant !"), tr("This member is already a participant !\nEdit Participant ?"), QMessageBox::Yes|QMessageBox::No);
            if(reply == QMessageBox::Yes)
            {
                groupMemberEditOpen(memberID);
            }
            return;
        }
    }
    else
    {
        engine_db::dispERR(this, query);
    }

    query.prepare("INSERT INTO events_members (event, member) VALUES (? ,?)");
    query.addBindValue(eventID);
    query.addBindValue(memberID);
    if(!query.exec())
    {
        engine_db::dispERR(this, query);
    }

    groupMemberUpdate();
    groupMemberEditOpen(memberID);
}

void window_event::groupMemberEdit()
{
    if(group_member_table->selectedItems().isEmpty()) { return; }
    groupMemberEditOpen(group_member_table->item(group_member_table->selectedItems().takeFirst()->row(), 0)->text().toUInt());
}

void window_event::groupMemberEditOpen(uint64_t memberID)
{
    QSqlQuery query;
    query.prepare("SELECT comment FROM events_members WHERE event = "+QString::number(eventID)+" AND member = "+QString::number(memberID));
    //query.addBindValue(eventID);
    if(query.exec())    //exec query
    {
        if(query.next())    //Get user infos
        {
            bwin_member_edit_memberID=memberID;
            bwin_member_edit = new QDialog(this, Qt::Tool);
            bwin_member_edit->setWindowTitle(tr("Edit a Participant"));
            bwin_member_label = new QLabel(tr("Comment"),bwin_member_edit);
            bwin_member_edit_comment = new QTextEdit(query.record().value("comment").toString(), bwin_member_edit);
            bwin_member_edit_button =new QPushButton(tr("Save"), bwin_member_edit);
                connect(bwin_member_edit_button, SIGNAL(released()), this, SLOT(groupMemberEditReturn()));
            bwin_member_edit_layout = new QGridLayout();
            bwin_member_edit_layout->addWidget(bwin_member_label);
            bwin_member_edit_layout->addWidget(bwin_member_edit_comment);
            bwin_member_edit_layout->addWidget(bwin_member_edit_button);
            bwin_member_edit->setLayout(bwin_member_edit_layout);
            bwin_member_edit->exec();
        }
    }
    else
    {
        engine_db::dispERR(nullptr, query);
    }
}

void window_event::groupMemberEditReturn()
{
    QSqlQuery query;
    query.prepare("UPDATE events_members SET comment=? WHERE event = "+QString::number(eventID)+" AND member = "+QString::number(bwin_member_edit_memberID));
    query.addBindValue(bwin_member_edit_comment->toPlainText());
    if(!query.exec())
    {
        engine_db::dispERR(this, query);
    }

    bwin_member_edit->close();
    groupMemberUpdate();
}

void window_event::groupMemberRemove()
{
    if(group_member_table->selectedItems().isEmpty()) { return; }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Remove a member ?"), tr("Are you sure to remove ?"), QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
        QSqlQuery query;
        query.prepare("DELETE FROM events_members WHERE event = ? AND member = ?");
        query.addBindValue(eventID);
        query.addBindValue(group_member_table->item(group_member_table->selectedItems().takeFirst()->row(), 0)->text().toUInt());
        if(!query.exec())    //exec query
        {
            engine_db::dispERR(nullptr, query);
        }
    }

    groupMemberUpdate();
}

void window_event::openMember(QTableWidgetItem *item)
{
    window_member *child = new window_member(group_member_table->item(item->row(), 0)->text().toUInt(), mainMdi, nullptr);
    mainMdi->addSubWindow(child);
    child->show();
}

void window_event::commentUpdate()
{
    view_events_comm_listID.clear();
    view_events_comm_table->clear();

    QSqlQuery query;
    query.prepare("SELECT events_comments.id, createdAt, comment, user, username FROM events_comments LEFT JOIN users ON users.id = events_comments.user WHERE event = "+QString::number(eventID)+" ORDER BY createdAt DESC");
    //query.addBindValue(eventID);
    if(query.exec())    //exec query
    {
        while(query.next())    //Get user infos
        {
            QListWidgetItem *item = new QListWidgetItem();
            //item->setData(0, query.record().value("member").toInt());
            QString username = query.record().value("username").toString();
            if(username.isEmpty()) { username=tr("Removed User"); }
            QString text = username+" at "+query.record().value("createdAt").toDateTime().toString();
            text+="\n\n"+query.record().value("comment").toString();
            item->setText(text);
            view_events_comm_table->addItem(item);
            view_events_comm_listID.append(query.record().value("id").toInt());
        }
    }
    else
    {
        engine_db::dispERR(nullptr, query);
    }

    evtButtonsTable();
}

void window_event::commentAdd()
{
    if(!user::can(USER_PERM_EVENT_COMMENT)) { return; }

    if(view_events_comm_text->toPlainText().size()<1 || view_events_comm_text->toPlainText().size()>250)
    {
        QMessageBox::information(this, tr("Comment Lenth error"), tr("Comment must have at least one caracter and a maximum of 250."));
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO events_comments (event, user, comment) VALUES (? ,?, ?)");
    query.addBindValue(eventID);
    query.addBindValue(user::id);
    query.addBindValue(view_events_comm_text->toPlainText());
    if(!query.exec())
    {
        engine_db::dispERR(this, query);
    }

    view_events_comm_text->setText("");
    commentUpdate();
}
