/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : window_member.cpp
 *
 ****************************************/

#include "window_member.h"

#include "spoilerwidget.h"

window_member::window_member(uint64_t _memberID, QMdiArea *_mainMdi, QWidget *_parent, bool _create) : QMainWindow(_parent)
{
    memberID=_memberID;
    mainMdi=_mainMdi;
    setWindowTitle(APP_NAME" - "+tr("Member"));

    QLabel *label_ID = new QLabel("ID", this);
    QLabel *label_regNum = new QLabel("Registration Number", this);
    QLabel *label_gender = new QLabel("Gender", this);
    QLabel *label_memberType = new QLabel("Member Type", this);
    QLabel *label_firstName = new QLabel("First Name", this);
    QLabel *label_otherName = new QLabel("Other Name", this);
    QLabel *label_lastName = new QLabel("Last Name", this);
    QLabel *label_birthThe = new QLabel("Birth The", this);
    QLabel *label_birthAt = new QLabel("At", this);
    QLabel *label_createdAt = new QLabel("Created At", this);
    QLabel *label_editedAt = new QLabel("Edited At", this);

    view_ID = new QLineEdit(this);
        view_ID->setDisabled(true);
    view_regNum = new QLineEdit(this);
        view_regNum->setMaxLength(250);
    view_gender = new QComboBox(this);
    view_memberType = new QComboBox(this);
    view_firstName = new QLineEdit(this);
        view_firstName->setMaxLength(250);
    view_otherName = new QLineEdit(this);
        view_otherName->setMaxLength(250);
    view_lastName = new QLineEdit(this);
        view_lastName->setMaxLength(250);
    view_birthDate = new QDateEdit(this);
        connect(view_birthDate, SIGNAL(editingFinished()), this, SLOT(updateAge()));
    view_Age = new QLineEdit(this);
        view_Age->setDisabled(true);
    view_birthLocation = new QLineEdit(this);
        view_birthLocation->setMaxLength(250);
    view_createdAt = new QDateTimeEdit(this);
        view_createdAt->setDisabled(true);
    view_editedAt = new QDateTimeEdit(this);
        view_editedAt->setDisabled(true);
    view_comment = new QTextEdit(this);

    //view_gender->setCurrentIndex(0);


    QHBoxLayout *baseinfoLayout = new QHBoxLayout(this);
        baseinfoLayout->addWidget(label_ID);
        baseinfoLayout->addWidget(view_ID);
        baseinfoLayout->addWidget(label_regNum);
        baseinfoLayout->addWidget(view_regNum);
        baseinfoLayout->addWidget(label_memberType);
        baseinfoLayout->addWidget(view_memberType);
        baseinfoLayout->addWidget(label_createdAt);
        baseinfoLayout->addWidget(view_createdAt);
        baseinfoLayout->addWidget(label_editedAt);
        baseinfoLayout->addWidget(view_editedAt);

    QGridLayout *nameinfoLayout = new QGridLayout(this);
        nameinfoLayout->addWidget(label_firstName, 0, 0);
        nameinfoLayout->addWidget(label_otherName, 0, 1);
        nameinfoLayout->addWidget(label_lastName, 0, 2);
        nameinfoLayout->addWidget(view_firstName, 1, 0);
        nameinfoLayout->addWidget(view_otherName, 1, 1);
        nameinfoLayout->addWidget(view_lastName, 1, 2);

    QHBoxLayout *birthinfoLayout = new QHBoxLayout(this);
        birthinfoLayout->addWidget(label_gender);
        birthinfoLayout->addWidget(view_gender);
        birthinfoLayout->addWidget(label_birthThe);
        birthinfoLayout->addWidget(view_birthDate);
        birthinfoLayout->addWidget(view_Age);
        birthinfoLayout->addWidget(label_birthAt);
        birthinfoLayout->addWidget(view_birthLocation);

    view_relations = new QWidget(this);
        QLabel *label_relregnum = new QLabel(tr("Reg Num"), this);
        QLabel *label_relfirstName = new QLabel(tr("First Name"), this);
        QLabel *label_relLastName = new QLabel(tr("Last Name"), this);
        QLabel *label_relAge = new QLabel(tr("Age"), this);
        QLabel *label_spouse = new QLabel(tr("Spouse"), this);
        QLabel *label_father = new QLabel(tr("Father"), this);
        QLabel *label_mother = new QLabel(tr("Mother"), this);
        view_spouse_regNum = new QLineEdit(this);
            view_spouse_regNum->setReadOnly(true);
        view_spouse_firstName = new QLineEdit(this);
            view_spouse_firstName->setReadOnly(true);
        view_spouse_lastName = new QLineEdit(this);
            view_spouse_lastName->setReadOnly(true);
        view_spouse_age = new QLineEdit(this);
            view_spouse_age->setReadOnly(true);
        button_spouse_nobody = new QPushButton(QIcon(ICON_USERREMOVE), "", this);
            connect(button_spouse_nobody, SIGNAL(released()), this, SLOT(spouseRemove()));
        button_spouse_select = new QPushButton(QIcon(ICON_USEREDIT), "", this);
            connect(button_spouse_select, SIGNAL(released()), this, SLOT(spouseSelect()));
        button_spouse_open = new QPushButton(QIcon(ICON_OPENEXTERN), "", this);
            connect(button_spouse_open, SIGNAL(released()), this, SLOT(spouseOpen()));
        view_father_regNum = new QLineEdit(this);
            view_father_regNum->setReadOnly(true);
        view_father_firstName = new QLineEdit(this);
            view_father_firstName->setReadOnly(true);
        view_father_lastName = new QLineEdit(this);
            view_father_lastName->setReadOnly(true);
        view_father_age = new QLineEdit(this);
            view_father_age->setReadOnly(true);
        button_father_nobody = new QPushButton(QIcon(ICON_USERREMOVE), "", this);
            connect(button_father_nobody, SIGNAL(released()), this, SLOT(fatherRemove()));
        button_father_select = new QPushButton(QIcon(ICON_USEREDIT), "", this);
            connect(button_father_select, SIGNAL(released()), this, SLOT(fatherSelect()));
        button_father_open = new QPushButton(QIcon(ICON_OPENEXTERN), "", this);
            connect(button_father_open, SIGNAL(released()), this, SLOT(fatherOpen()));
        view_mother_regNum = new QLineEdit(this);
            view_mother_regNum->setReadOnly(true);
        view_mother_firstName = new QLineEdit(this);
            view_mother_firstName->setReadOnly(true);
        view_mother_lastName = new QLineEdit(this);
            view_mother_lastName->setReadOnly(true);
        view_mother_age = new QLineEdit(this);
            view_mother_age->setReadOnly(true);
        button_mother_nobody = new QPushButton(QIcon(ICON_USERREMOVE), "", this);
            connect(button_mother_nobody, SIGNAL(released()), this, SLOT(motherRemove()));
        button_mother_select = new QPushButton(QIcon(ICON_USEREDIT), "", this);
            connect(button_mother_select, SIGNAL(released()), this, SLOT(motherSelect()));
        button_mother_open = new QPushButton(QIcon(ICON_OPENEXTERN), "", this);
            connect(button_mother_open, SIGNAL(released()), this, SLOT(motherOpen()));
        QSpacerItem *relation_spacer = new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        QGridLayout *relationLayout = new QGridLayout(this);
            relationLayout->addWidget(label_relregnum, 0, 1);
            relationLayout->addWidget(label_relfirstName, 0, 2);
            relationLayout->addWidget(label_relLastName, 0, 3);
            relationLayout->addWidget(label_relAge, 0, 4);
            relationLayout->addWidget(label_spouse, 1, 0);
            relationLayout->addWidget(view_spouse_regNum, 1, 1);
            relationLayout->addWidget(view_spouse_firstName, 1, 2);
            relationLayout->addWidget(view_spouse_lastName, 1, 3);
            relationLayout->addWidget(view_spouse_age, 1, 4);
            relationLayout->addWidget(button_spouse_nobody, 1, 5);
            relationLayout->addWidget(button_spouse_select, 1, 6);
            relationLayout->addWidget(button_spouse_open, 1, 7);
            relationLayout->addWidget(label_father, 2, 0);
            relationLayout->addWidget(view_father_regNum, 2, 1);
            relationLayout->addWidget(view_father_firstName, 2, 2);
            relationLayout->addWidget(view_father_lastName, 2, 3);
            relationLayout->addWidget(view_father_age, 2, 4);
            relationLayout->addWidget(button_father_nobody, 2, 5);
            relationLayout->addWidget(button_father_select, 2, 6);
            relationLayout->addWidget(button_father_open, 2, 7);
            relationLayout->addWidget(label_mother, 3, 0);
            relationLayout->addWidget(view_mother_regNum, 3, 1);
            relationLayout->addWidget(view_mother_firstName, 3, 2);
            relationLayout->addWidget(view_mother_lastName, 3, 3);
            relationLayout->addWidget(view_mother_age, 3, 4);
            relationLayout->addWidget(button_mother_nobody, 3, 5);
            relationLayout->addWidget(button_mother_select, 3, 6);
            relationLayout->addWidget(button_mother_open, 3, 7);
            relationLayout->addItem(relation_spacer, 4, 0, 1, 8);
    view_relations->setLayout(relationLayout);

    view_childrens = new QWidget(this);
        view_childs_label = new QLabel(tr("This section show the list of childrens. Double-click to open child.\nTo edit this list, please edit parent."), this);
        view_childs_table = new QTableWidget(0, 5, this);
            view_childs_table->setAlternatingRowColors(true);
            view_childs_table->setStyleSheet("alternate-background-color: rgb(210,240,250);");
            //view_childs_table->setColumnHidden(0, true);
            view_childs_table->verticalHeader()->setHidden(true);
            view_childs_table->setSelectionMode(QAbstractItemView::SingleSelection);
            view_childs_table->setSelectionBehavior(QAbstractItemView::SelectRows);
            view_childs_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
            QStringList horizontalHeaders;
            horizontalHeaders << "id" << tr("Reg Num") << tr("Age") << tr("First Name") << tr("Last Name");
            view_childs_table->setHorizontalHeaderLabels(horizontalHeaders);
            view_childs_table->resizeColumnsToContents();
            view_childs_table->sortByColumn(1, Qt::AscendingOrder);
            view_childs_table->verticalHeader()->setDefaultSectionSize(40);
            //connect(table, SIGNAL(itemSelectionChanged()), this, SLOT(versionSelChange()));
            connect(view_childs_table, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(openMember(QTableWidgetItem*)));

        QGridLayout *childLayout = new QGridLayout(this);
            childLayout->addWidget(view_childs_label);
            childLayout->addWidget(view_childs_table);
    view_childrens->setLayout(childLayout);

    view_events = new QWidget(this);
        view_events_label = new QLabel(tr("This section show the list of events where the member are. Double-click to open event.\nTo edit this list, please edit Events."), this);
        view_events_table = new QTableWidget(0, 5, this);
            view_events_table->setAlternatingRowColors(true);
            view_events_table->setStyleSheet("alternate-background-color: rgb(210,240,250);");
            view_events_table->verticalHeader()->setHidden(true);
            view_events_table->setSelectionMode(QAbstractItemView::SingleSelection);
            view_events_table->setSelectionBehavior(QAbstractItemView::SelectRows);
            view_events_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
            //QStringList horizontalHeaders;
            horizontalHeaders.clear();
            horizontalHeaders << "id" << tr("Start") << tr("End") << tr("Name") << tr("Comment");
            view_events_table->setHorizontalHeaderLabels(horizontalHeaders);
            view_events_table->resizeColumnsToContents();
            view_events_table->sortByColumn(1, Qt::AscendingOrder);
            view_events_table->verticalHeader()->setDefaultSectionSize(40);
            //connect(table, SIGNAL(itemSelectionChanged()), this, SLOT(versionSelChange()));
            connect(view_events_table, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(openEvent(QTableWidgetItem*)));

        QGridLayout *eventsLayout = new QGridLayout(this);
            eventsLayout->addWidget(view_events_label);
            eventsLayout->addWidget(view_events_table);
    view_events->setLayout(eventsLayout);

    tab_widget = new QTabWidget(this);
        tab_widget->insertTab(0, view_relations, tr("Relations"));
        tab_widget->insertTab(1, view_childrens, tr("Childrens"));
        tab_widget->insertTab(2, view_events, tr("Events"));

   QGridLayout *spoilerLayout = new QGridLayout();
   spoilerLayout->addWidget(tab_widget);
   spoilerWidget *spoiler = new spoilerWidget(tr("Relations"), 100, this);
   spoiler->setContentLayout(*spoilerLayout);


    QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->addLayout(baseinfoLayout);
        mainLayout->addLayout(nameinfoLayout);
        mainLayout->addLayout(birthinfoLayout);
        mainLayout->addWidget(view_comment);
        mainLayout->addWidget(spoiler);

    centralWidget = new QWidget(this);
        centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    connect(spoiler, &spoilerWidget::stateChanged, [this]() {
        if(subContainer->sizeHint().height()>subContainer->size().height())
        {
            subContainer->resize(subContainer->size().width(), subContainer->sizeHint().height()-100);
        }
    });

    QToolBar *toolBar = new QToolBar(this);
        toolBar->addAction(QIcon(ICON_REFRESH), tr("Refresh"), this, &window_member::updateView);
        toolBar->addSeparator();
        action_edit = new QAction(QIcon(ICON_EDIT), tr("Edit"), this);
            action_edit->setCheckable(true);
            connect(action_edit, SIGNAL(triggered(bool)), this, SLOT(editTrig(bool)));
        toolBar->addAction(action_edit);
        action_save = new QAction(QIcon(ICON_SAVE), tr("Edit"), this);
            connect(action_save, SIGNAL(triggered()), this, SLOT(editSave()));
        toolBar->addAction(action_save);
        action_remove = new QAction(QIcon(ICON_TRASH), tr("Remove"), this);
            connect(action_remove, SIGNAL(triggered()), this, SLOT(editRemove()));
        toolBar->addAction(action_remove);
        action_edit->setEnabled(user::can(USER_PERM_MEMBER_EDIT));
        action_save->setVisible(user::can(USER_PERM_MEMBER_EDIT));
        action_remove->setVisible(user::can(USER_PERM_MEMBER_EDIT) && user::can(USER_PERM_MEMBER_REMOVE));

    addToolBar(toolBar);
    lock();

    if(_create)
    {
        memberID=0;
        QSqlQuery query;
        QSqlQuery query2;
        QString uuidText = QUuid::createUuid().toString();
        query.prepare("INSERT INTO members (firstName, otherName) VALUES (?, ?)");
        query.addBindValue(tr("New Member - Unamed"));
        query.addBindValue(uuidText);
        if(query.exec())
        {
            query.prepare("SELECT id FROM members WHERE otherName=? ORDER BY id DESC");
            query.addBindValue(uuidText);
            if(query.exec())
            {
                if(query.next())
                {
                    memberID = query.record().value("id").toUInt();
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

    view_ID->setText(QString::number(memberID));
    updateView();
}

/*void window_member::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    QMessageBox::information(nullptr, "", "");
}*/

void window_member::openMember(QTableWidgetItem *item)
{
    window_member *child = new window_member(view_childs_table->item(item->row(), 0)->text().toUInt(), mainMdi, nullptr);
    mainMdi->addSubWindow(child);
    child->show();
}

void window_member::openEvent(QTableWidgetItem *item)
{
    window_event *child = new window_event(view_events_table->item(item->row(), 0)->text().toUInt(), mainMdi, nullptr);
    mainMdi->addSubWindow(child);
    child->show();
}

void window_member::updateView()
{
    user::updateDB();

    view_gender->clear();
    for(int i=0; i<user::gender_text.size(); i++)
    {
        view_gender->addItem(user::gender_text.at(i), user::gender_id.at(i));
    }

    view_memberType->clear();
    for(int i=0; i<user::type_text.size(); i++)
    {
        view_memberType->addItem(user::type_text.at(i), user::type_id.at(i));
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM members WHERE id = "+QString::number(memberID));
    //query.addBindValue((int)memberID);
    if(query.exec())
    {
        if(query.next())
        {
            view_regNum->setText(query.record().value("regNum").toString());
            int temp = user::gender_id.indexOf(query.record().value("gender").toInt());
            if(temp<1) { temp=0; }
            view_gender->setCurrentIndex(temp);
            temp = user::type_id.indexOf(query.record().value("type").toInt());
            if(temp<1) { temp=0; }
            view_memberType->setCurrentIndex(temp);
            view_firstName->setText(query.record().value("firstName").toString());
            view_otherName->setText(query.record().value("otherName").toString());
            view_lastName->setText(query.record().value("lastName").toString());
            view_birthDate->setDate(query.record().value("birthDate").toDate());
            view_birthLocation->setText(query.record().value("birthLocation").toString());
            view_createdAt->setDateTime(query.record().value("createdAt").toDateTime());
            view_editedAt->setDateTime(query.record().value("updatedAt").toDateTime());
            view_comment->setText(query.record().value("comment").toString());
            updateAge();

            spouseID=0;
            fatherID=0;
            motherID=0;

            QSqlQuery query2;
            query2.prepare("SELECT member1, member2 FROM members_relation WHERE type=1 AND ( member1 = ? OR member2 = ? )");
            query2.addBindValue(memberID);
            query2.addBindValue(memberID);
            if(query2.exec())
            {
                if(query2.next())
                {
                    if(query2.record().value("member1").toUInt()!=memberID)
                    {
                        spouseID=query2.record().value("member1").toUInt();
                    }
                    else
                    {
                        spouseID=query2.record().value("member2").toUInt();
                    }
                }
            }
            else
            {
                engine_db::dispERR(this, query);
            }

            query2.prepare("SELECT member1 FROM members_relation WHERE type=2 AND member2 = ?");
            query2.addBindValue(memberID);
            if(query2.exec())
            {
                if(query2.next())
                {
                    fatherID=query2.record().value("member1").toUInt();
                }
            }
            else
            {
                engine_db::dispERR(this, query);
            }

            query2.prepare("SELECT member1 FROM members_relation WHERE type=3 AND member2 = ?");
            query2.addBindValue(memberID);
            if(query2.exec())
            {
                if(query2.next())
                {
                    motherID=query2.record().value("member1").toUInt();
                }
            }
            else
            {
                engine_db::dispERR(this, query);
            }

            updateSpouse();
            updateFather();
            updateMother();
            updateChilds();
            updateEvents();
        }
    }
    else
    {
        engine_db::dispERR(this, query);
    }
}

void window_member::updateAge()
{
    view_Age->setText(QString::number(global::age(view_birthDate->date()))+" "+tr("years old"));
}

void window_member::updateSpouse()
{
    QSqlQuery query;
    query.prepare("SELECT regNum, firstName, lastName, birthDate FROM members WHERE id = ?");
    query.addBindValue(spouseID);
    if(query.exec())
    {
        if(query.next())
        {
            view_spouse_regNum->setText(query.record().value("regNum").toString());
            view_spouse_firstName->setText(query.record().value("firstName").toString());
            view_spouse_lastName->setText(query.record().value("lastName").toString());
            view_spouse_age->setText(QString::number(global::age(query.record().value("birthDate").toDate())));
        }
        else
        {
            view_spouse_regNum->setText("");
            view_spouse_firstName->setText("");
            view_spouse_lastName->setText("");
            view_spouse_age->setText("");
        }
    }
    else
    {
        engine_db::dispERR(this, query);
    }

    if(spouseID!=0) { button_spouse_open->setEnabled(true); }
    else            { button_spouse_open->setEnabled(false); }
}

void window_member::updateFather()
{
    QSqlQuery query;
    query.prepare("SELECT regNum, firstName, lastName, birthDate FROM members WHERE id = ?");
    query.addBindValue(fatherID);
    if(query.exec())
    {
        if(query.next())
        {
            view_father_regNum->setText(query.record().value("regNum").toString());
            view_father_firstName->setText(query.record().value("firstName").toString());
            view_father_lastName->setText(query.record().value("lastName").toString());
            view_father_age->setText(QString::number(global::age(query.record().value("birthDate").toDate())));
        }
        else
        {
            view_father_regNum->setText("");
            view_father_firstName->setText("");
            view_father_lastName->setText("");
            view_father_age->setText("");
        }
    }
    else
    {
        engine_db::dispERR(this, query);
    }

    if(fatherID!=0) { button_father_open->setEnabled(true); }
    else            { button_father_open->setEnabled(false); }
}

void window_member::updateMother()
{
    QSqlQuery query;
    query.prepare("SELECT regNum, firstName, lastName, birthDate FROM members WHERE id = ?");
    query.addBindValue(motherID);
    if(query.exec())
    {
        if(query.next())
        {
            view_mother_regNum->setText(query.record().value("regNum").toString());
            view_mother_firstName->setText(query.record().value("firstName").toString());
            view_mother_lastName->setText(query.record().value("lastName").toString());
            view_mother_age->setText(QString::number(global::age(query.record().value("birthDate").toDate())));
        }
        else
        {
            view_mother_regNum->setText("");
            view_mother_firstName->setText("");
            view_mother_lastName->setText("");
            view_mother_age->setText("");
        }
    }
    else
    {
        engine_db::dispERR(this, query);
    }

    if(motherID!=0) { button_mother_open->setEnabled(true); }
    else            { button_mother_open->setEnabled(false); }
}

void window_member::updateChilds()
{
    int count=0;
    view_childs_table->setSortingEnabled(false);
    view_childs_table->setRowCount(0);

    QSqlQuery query;
    query.prepare("SELECT member2, regNum, firstName, lastName, birthDate FROM members JOIN members_relation ON members.id = members_relation.member2 WHERE member1 = "+QString::number(memberID)+" AND ( members_relation.type = 2 OR members_relation.type = 3 )");
    //query.addBindValue(memberID);
    if(query.exec())    //exec query
    {
        while(query.next())    //Get user infos
        {
            view_childs_table->setRowCount(count+1);
            view_childs_table->setItem(count, 0, new QTableWidgetItem(query.record().value("member2").toString()));
            view_childs_table->setItem(count, 1, new QTableWidgetItem(query.record().value("regNum").toString()));
            view_childs_table->setItem(count, 2, new QTableWidgetItem(QString::number(global::age(query.record().value("birthDate").toDate()))));
            view_childs_table->setItem(count, 3, new QTableWidgetItem(query.record().value("firstName").toString()));
            view_childs_table->setItem(count, 4, new QTableWidgetItem(query.record().value("lastName").toString()));
            count++;
        }
    }
    else
    {
        engine_db::dispERR(nullptr, query);
    }

    view_childs_table->resizeColumnsToContents();
    view_childs_table->resizeRowsToContents();
    view_childs_table->sortByColumn(2, Qt::AscendingOrder);
    view_childs_table->setSortingEnabled(true);
}

void window_member::updateEvents()
{
    int count=0;
    view_events_table->setSortingEnabled(false);
    view_events_table->setRowCount(0);

    QSqlQuery query;
    query.prepare("SELECT event, start, end, name, comment FROM events JOIN events_members ON events.id = events_members.event WHERE member = "+QString::number(memberID));
    //query.addBindValue(memberID);
    if(query.exec())    //exec query
    {
        while(query.next())    //Get user infos
        {
            view_events_table->setRowCount(count+1);
            view_events_table->setItem(count, 0, new QTableWidgetItem(QString::number(query.record().value("event").toInt())));
            view_events_table->setItem(count, 1, new QTableWidgetItem(query.record().value("start").toDateTime().toString()));
            view_events_table->setItem(count, 2, new QTableWidgetItem(query.record().value("end").toDateTime().toString()));
            view_events_table->setItem(count, 3, new QTableWidgetItem(query.record().value("name").toString()));
            view_events_table->setItem(count, 4, new QTableWidgetItem(query.record().value("comment").toString()));
            count++;
        }
    }
    else
    {
        engine_db::dispERR(nullptr, query);
    }

    view_events_table->resizeColumnsToContents();
    view_events_table->resizeRowsToContents();
    view_events_table->sortByColumn(1, Qt::DescendingOrder);
    view_events_table->setSortingEnabled(true);
}

void window_member::lock()
{
    view_regNum->setReadOnly(true);
    view_gender->setEnabled(false);
    view_memberType->setEnabled(false);
    view_firstName->setReadOnly(true);
    view_otherName->setReadOnly(true);
    view_lastName->setReadOnly(true);
    view_birthDate->setReadOnly(true);
    view_birthLocation->setReadOnly(true);
    view_comment->setReadOnly(true);

    button_spouse_nobody->setEnabled(false);
    button_spouse_select->setEnabled(false);
    button_father_nobody->setEnabled(false);
    button_father_select->setEnabled(false);
    button_mother_nobody->setEnabled(false);
    button_mother_select->setEnabled(false);

    action_save->setEnabled(false);
    action_remove->setEnabled(false);
}

void window_member::unlock()
{
    view_regNum->setReadOnly(false);
    view_gender->setEnabled(true);
    view_memberType->setEnabled(true);
    view_firstName->setReadOnly(false);
    view_otherName->setReadOnly(false);
    view_lastName->setReadOnly(false);
    view_birthDate->setReadOnly(false);
    view_birthLocation->setReadOnly(false);
    view_comment->setReadOnly(false);

    button_spouse_nobody->setEnabled(true);
    button_spouse_select->setEnabled(true);
    button_father_nobody->setEnabled(true);
    button_father_select->setEnabled(true);
    button_mother_nobody->setEnabled(true);
    button_mother_select->setEnabled(true);

    action_save->setEnabled(true);
    action_remove->setEnabled(true);
}

void window_member::editTrig(bool isActive)
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

void window_member::editSave()
{
    QSqlQuery query;
    QSqlQuery query2;
    query.prepare("UPDATE members SET regNum=?, firstName=?, otherName=?, lastName=?, birthDate=?, birthLocation=?, gender=?, type=?, comment=?, updatedAt= CURRENT_TIMESTAMP WHERE id = ?");
    query.addBindValue(view_regNum->text());
    query.addBindValue(view_firstName->text());
    query.addBindValue(view_otherName->text());
    query.addBindValue(view_lastName->text());
    query.addBindValue(view_birthDate->date());
    query.addBindValue(view_birthLocation->text());
    query.addBindValue(view_gender->currentData().toInt());
    query.addBindValue(view_memberType->currentData().toInt());
    query.addBindValue(view_comment->toPlainText());
    query.addBindValue(memberID);
    if(!query.exec())
    {
        engine_db::dispERR(this, query);
    }

    query.prepare("DELETE FROM members_relation WHERE type=1 AND ( member1 = ? OR member2 = ? )");
    query.addBindValue(memberID);
    query.addBindValue(memberID);
    if(query.exec())
    {
        query2.prepare("INSERT INTO members_relation (member1, member2, type) VALUES (?, ?, 1)");
        query2.addBindValue(spouseID);
        query2.addBindValue(memberID);
        query2.addBindValue(query.record().value("id").toUInt());
        if(!query2.exec())
        {
            engine_db::dispERR(this, query2);
        }
    }
    else
    {
        engine_db::dispERR(this, query);
    }

    query.prepare("DELETE FROM members_relation WHERE type=2 AND member2 = ?");
    query.addBindValue(memberID);
    if(query.exec())
    {
        query2.prepare("INSERT INTO members_relation (member1, member2, type) VALUES (?, ?, 2)");
        query2.addBindValue(fatherID);
        query2.addBindValue(memberID);
        if(!query2.exec())
        {
            engine_db::dispERR(this, query2);
        }
    }
    else
    {
        engine_db::dispERR(this, query);
    }

    query.prepare("DELETE FROM members_relation WHERE type=3 AND member2 = ?");
    query.addBindValue(memberID);
    if(query.exec())
    {
        query2.prepare("INSERT INTO members_relation (member1, member2, type) VALUES (?, ?, 3)");
        query2.addBindValue(motherID);
        query2.addBindValue(memberID);
        if(!query2.exec())
        {
            engine_db::dispERR(this, query2);
        }
    }
    else
    {
        engine_db::dispERR(this, query);
    }

    action_edit->setChecked(false);
    lock();
    updateView();
}

void window_member::editRemove()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Remove a member ?"), tr("Warning !\nRemove a member is generaly not a good idea. Please consider to associate this member to a specific Type.\nSure to remove ?"), QMessageBox::Yes|QMessageBox::No);
    if(reply != QMessageBox::Yes) { return; }

    reply = QMessageBox::question(this, tr("Remove a member ?"), tr("Association with this member will be lost !\nThis action cannot be canceled !\nSure to remove ?"), QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
        QSqlQuery query;
        query.prepare("DELETE FROM members WHERE id = ?");
        query.addBindValue(memberID);
        if(!query.exec())    //exec query
        {
            engine_db::dispERR(nullptr, query);
        }

        this->close();
    }
}

void window_member::spouseRemove()
{
    spouseID=0;
    updateSpouse();
}

void window_member::spouseSelect()
{
    bwin_member_select = new QDialog(this, Qt::Tool);
    bwin_member_select->setWindowTitle(tr("Select Spouse"));
    bwin_member_select_module = new module_search_member();
    connect(bwin_member_select_module, SIGNAL(doubleCliked(uint64_t)), this, SLOT(spouseSelectReturn(uint64_t)));
    bwin_member_select_layout = new QGridLayout();
    bwin_member_select_layout->addWidget(bwin_member_select_module);
    bwin_member_select->setLayout(bwin_member_select_layout);
    bwin_member_select->exec();
}

void window_member::spouseSelectReturn(uint64_t memberID)
{
    bwin_member_select->close();
    spouseID=memberID;
    updateSpouse();
}

void window_member::spouseOpen()
{
    window_member *child = new window_member(spouseID, mainMdi, nullptr);
    mainMdi->addSubWindow(child);
    child->show();
}

void window_member::fatherRemove()
{
    fatherID=0;
    updateFather();
}

void window_member::fatherSelect()
{
    bwin_member_select = new QDialog(this, Qt::Tool);
    bwin_member_select->setWindowTitle(tr("Select Father"));
    bwin_member_select_module = new module_search_member();
    connect(bwin_member_select_module, SIGNAL(doubleCliked(uint64_t)), this, SLOT(fatherSelectReturn(uint64_t)));
    bwin_member_select_layout = new QGridLayout();
    bwin_member_select_layout->addWidget(bwin_member_select_module);
    bwin_member_select->setLayout(bwin_member_select_layout);
    bwin_member_select->exec();
}

void window_member::fatherSelectReturn(uint64_t memberID)
{
    bwin_member_select->close();
    fatherID=memberID;
    updateFather();
}

void window_member::fatherOpen()
{
    window_member *child = new window_member(fatherID, mainMdi, nullptr);
    mainMdi->addSubWindow(child);
    child->show();
}

void window_member::motherRemove()
{
    motherID=0;
    updateMother();
}

void window_member::motherSelect()
{
    bwin_member_select = new QDialog(this, Qt::Tool);
    bwin_member_select->setWindowTitle(tr("Select Mother"));
    bwin_member_select_module = new module_search_member();
    connect(bwin_member_select_module, SIGNAL(doubleCliked(uint64_t)), this, SLOT(motherSelectReturn(uint64_t)));
    bwin_member_select_layout = new QGridLayout();
    bwin_member_select_layout->addWidget(bwin_member_select_module);
    bwin_member_select->setLayout(bwin_member_select_layout);
    bwin_member_select->exec();
}

void window_member::motherSelectReturn(uint64_t memberID)
{
    bwin_member_select->close();
    motherID=memberID;
    updateMother();
}

void window_member::motherOpen()
{
    window_member *child = new window_member(motherID, mainMdi, nullptr);
    mainMdi->addSubWindow(child);
    child->show();
}
