#include "module_search_member.h"


module_search_member::module_search_member(QWidget *_parent) : QWidget(_parent)
{
    QLabel *regNumLabel = new QLabel(tr("Registration Number"), this);
    QLabel *genderLabel = new QLabel(tr("Gender"), this);
    QLabel *memberTypeLabel = new QLabel(tr("Member Type"), this);
    QLabel *firstNameLabel = new QLabel(tr("First Name"), this);
    QLabel *lastNameLabel = new QLabel(tr("Last Name"), this);
    QLabel *ageLabel = new QLabel(tr("< Age <"), this);

    regNum = new QLineEdit(this);
        connect(regNum, SIGNAL(returnPressed()), this, SLOT(searchExec()));
    gender = new member_gender_widget(this, true);
    memberType = new QComboBox(this);
    firstName = new QLineEdit(this);
        connect(firstName, SIGNAL(returnPressed()), this, SLOT(searchExec()));
    otherName = new QCheckBox(tr("Search also in Other Name"), this);
    lastName = new QLineEdit(this);
        connect(lastName, SIGNAL(returnPressed()), this, SLOT(searchExec()));
    autoLast = new QCheckBox(tr("Same as First Name"), this);
        connect(autoLast, SIGNAL(stateChanged(int)), this, SLOT(autoLastChanged(int)));
        autoLast->setChecked(true);
    ageMin = new QSpinBox(this);
        ageMin->setRange(0, 1000);
        ageMin->setValue(0);
    ageMax = new QSpinBox(this);
        ageMin->setRange(0, 1000);
        ageMin->setValue(0);

    result = new QLabel(tr("Ready"), this);
        result->setStyleSheet("font-weight: bold;");
    search = new QPushButton(QIcon(ICON_SEARCH), tr("Search"), this);
        connect(search, SIGNAL(released()), this, SLOT(searchExec()));
    table = new QTableWidget(0, 12, this);
        table->setAlternatingRowColors(true);
        table->setStyleSheet("alternate-background-color: rgb(210,240,250);");
        table->setColumnHidden(0, true);
        table->verticalHeader()->setHidden(true);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        QStringList horizontalHeaders;
        horizontalHeaders << "id" << tr("Reg Num") << tr("First Name") << tr("Other Name") << tr("Last Name") << tr("Age") << tr("Birth Date") << tr("Birth Location") << tr("Member Type") << tr("Gender") << tr("Child Count\n(Man, Woman)") << tr("Event Count");
        table->setHorizontalHeaderLabels(horizontalHeaders);
        table->resizeColumnsToContents();
        table->sortByColumn(1, Qt::AscendingOrder);
        table->verticalHeader()->setDefaultSectionSize(40);
        //connect(table, SIGNAL(itemSelectionChanged()), this, SLOT(versionSelChange()));
        connect(table, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(openMember(QTableWidgetItem*)));

    selectDisplay = new module_select_display(horizontalHeaders, 5, this);
    connect(selectDisplay, SIGNAL(selectionChanged()), this, SLOT(selDisplay()));
    selectDisplay->setState(0, true);
    selectDisplay->setDisplayed(1, true);   selectDisplay->setState(1, true);
    selectDisplay->setDisplayed(2, true);   selectDisplay->setState(2, true);
    selectDisplay->setDisplayed(3, true);   selectDisplay->setState(3, false);
    selectDisplay->setDisplayed(4, true);   selectDisplay->setState(4, true);
    selectDisplay->setDisplayed(5, true);   selectDisplay->setState(5, true);
    selectDisplay->setDisplayed(6, true);   selectDisplay->setState(6, false);
    selectDisplay->setDisplayed(7, true);   selectDisplay->setState(7, false);
    selectDisplay->setDisplayed(8, true);   selectDisplay->setState(8, true);
    selectDisplay->setDisplayed(9, true);   selectDisplay->setState(9, true);

    QGridLayout *spoilerLayout = new QGridLayout();
    spoilerLayout->addWidget(selectDisplay);
    spoilerLayout->setMargin(0);
    spoilerWidget *spoiler = new spoilerWidget(tr("Columns Filters"), 100, this);
    spoiler->setContentLayout(*spoilerLayout);

    connect(spoiler, &spoilerWidget::stateChanged, [this]() {
        /*if(subContainer->sizeHint().height()>subContainer->size().height())
        {
            subContainer->resize(subContainer->size().width(), subContainer->sizeHint().height()-100);
        }*/
    });

    QWidget *optionWidget = new QWidget(this);
        option_child_box = new QGroupBox(tr("Child count"), optionWidget);
            option_child_box->setCheckable(true);
            option_child_box->setChecked(false);
            QLabel *option_child_ageMax_label = new QLabel(tr("Max Age"), optionWidget);
            option_child_ageMax = new QSpinBox(optionWidget);
                option_child_ageMax->setRange(0, 1000);
                option_child_ageMax->setValue(0);
            QGridLayout *option_child_layout = new QGridLayout(this);
                option_child_layout->addWidget(option_child_ageMax_label, 0, 0);
                option_child_layout->addWidget(option_child_ageMax, 0, 1);
        option_child_box->setLayout(option_child_layout);
        option_event_box = new QGroupBox(tr("Event count"), optionWidget);
            option_event_box->setCheckable(true);
            option_event_box->setChecked(false);
            QLabel *option_event_label1 = new QLabel(tr("Between"), optionWidget);
            QLabel *option_event_label2 = new QLabel(tr("And"), optionWidget);
            option_event_dateMin = new QDateEdit(QDate(2000, 1, 1), optionWidget);
                option_event_dateMin->setCalendarPopup(true);
            option_event_dateMax = new QDateEdit(QDate::currentDate(), optionWidget);
                option_event_dateMax->setCalendarPopup(true);
            QGridLayout *option_event_layout = new QGridLayout(this);
                option_event_layout->addWidget(option_event_label1, 0, 0);
                option_event_layout->addWidget(option_event_dateMin, 0, 1);
                option_event_layout->addWidget(option_event_label2, 0, 2);
                option_event_layout->addWidget(option_event_dateMax, 0, 3);
        option_event_box->setLayout(option_event_layout);
        QGridLayout *optionLayout = new QGridLayout(this);
            optionLayout->addWidget(option_child_box);
            optionLayout->addWidget(option_event_box);
    optionWidget->setLayout(optionLayout);

    QGridLayout *spoilerOptionLayout = new QGridLayout();
    spoilerOptionLayout->addWidget(optionWidget);
    spoilerOptionLayout->setMargin(0);
    spoilerWidget *spoilerOption = new spoilerWidget(tr("Options"), 100, this);
    spoilerOption->setContentLayout(*spoilerOptionLayout);


    QHBoxLayout *topLayout = new QHBoxLayout();
        topLayout->addWidget(regNumLabel);
        topLayout->addWidget(regNum);
        topLayout->addWidget(genderLabel);
        topLayout->addWidget(gender);
        topLayout->addWidget(memberTypeLabel);
        topLayout->addWidget(memberType);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
        bottomLayout->addWidget(ageMin);
        bottomLayout->addWidget(ageLabel);
        bottomLayout->addWidget(ageMax);
            QSpacerItem *bottomSapcer = new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        bottomLayout->addItem(bottomSapcer);
        bottomLayout->addWidget(search);

    QGridLayout *mainLayout = new QGridLayout(this);
        mainLayout->addLayout(topLayout, 0, 0, 1, 3);
        mainLayout->addWidget(firstNameLabel, 1, 0);
        mainLayout->addWidget(firstName, 1, 1);
        mainLayout->addWidget(otherName, 1, 2);
        mainLayout->addWidget(lastNameLabel, 2, 0);
        mainLayout->addWidget(lastName, 2, 1);
        mainLayout->addWidget(autoLast, 2, 2);
        mainLayout->addLayout(bottomLayout, 3, 0, 1, 3);
        mainLayout->addWidget(spoiler, 4, 0, 1, 3);
        mainLayout->addWidget(spoilerOption, 5, 0, 1, 3);
        mainLayout->addWidget(result, 6, 0, 1, 3);
        mainLayout->addWidget(table, 7, 0, 1, 3);

    setLayout(mainLayout);

    user::updateDB();

    memberType->addItem("", -1);
    for(int i=0; i<user::type_text.size(); i++)
    {
        memberType->addItem(user::type_text.at(i), user::type_id.at(i));
    }
}

void module_search_member::selDisplay()
{
    for(int i=0; i<table->columnCount(); i++)
    {
        table->setColumnHidden(i, !selectDisplay->getState(i));
    }
}

void module_search_member::autoLastChanged(int newValue)
{
    if(newValue)    { lastName->setDisabled(true); }
    else            { lastName->setDisabled(false); }
}

void module_search_member::openMember(QTableWidgetItem *item)
{
    emit doubleCliked(table->item(item->row(), 0)->text().toUInt());
}

void module_search_member::searchExec()
{
    selectDisplay->setState(10, option_child_box->isChecked());
    selectDisplay->setState(11, option_event_box->isChecked());

    result->setText(tr("Searching..."));
    int count = 0;
    table->setSortingEnabled(false);
    table->setRowCount(0);

    QString queryString = "";
    QSqlQuery query;

    if(engine_db::local)
    {
        queryString = "SELECT \
                                *, \
                                ( \
                                    SELECT \
                                        COUNT(EQ.ID) \
                                    FROM \
                                        MEMBERS_RELATION ER, \
                                        MEMBERS EQ \
                                    WHERE \
                                        T.ID = ER.MEMBER1 \
                                    AND \
                                        EQ.ID = ER.MEMBER2 \
                                    AND \
                                        EQ.GENDER = 1 \
                                    AND \
                                        (ER.TYPE = 2 OR ER.TYPE = 3) \
                                    AND \
                                        (:childBirthDate IS NULL OR date(EQ.BIRTHDATE) > date(:childBirthDate)) \
                                    GROUP BY \
                                        ER.MEMBER1 \
                                ) AS NBBOY, \
                                ( \
                                    SELECT \
                                        COUNT(EQ.ID) \
                                    FROM \
                                        MEMBERS_RELATION ER, \
                                        MEMBERS EQ  \
                                    WHERE  \
                                        T.ID = ER.MEMBER1  \
                                    AND  \
                                        EQ.ID = ER.MEMBER2 \
                                    AND \
                                        EQ.GENDER = 2 \
                                    AND \
                                        (ER.TYPE = 2 OR ER.TYPE = 3) \
                                    AND \
                                        (:childBirthDate IS NULL OR date(EQ.BIRTHDATE) > date(:childBirthDate)) \
                                    GROUP BY \
                                        ER.MEMBER1 \
                                ) AS NBGIRL, \
                                ( \
                                    SELECT \
                                        COUNT(EQ.ID) \
                                    FROM \
                                        MEMBERS_RELATION ER, \
                                        MEMBERS EQ \
                                    WHERE \
                                        T.ID = ER.member1 \
                                    AND \
                                        EQ.ID = ER.member2 \
                                    AND \
                                        EQ.GENDER = 0 \
                                    AND \
                                        (ER.TYPE = 2 OR ER.TYPE = 3) \
                                    AND \
                                        (:childBirthDate IS NULL OR date(EQ.BIRTHDATE) > date(:childBirthDate)) \
                                    GROUP BY \
                                        ER.MEMBER1 \
                                ) AS NBOTHER, \
                                ( \
                                    SELECT \
                                        COUNT(EV.ID) \
                                    FROM \
                                        EVENTS_MEMBERS ER, \
                                        EVENTS EV \
                                    WHERE \
                                        T.ID = ER.MEMBER \
                                    AND \
                                        EV.id = ER.EVENT \
                                    AND \
                                        (:dateEventMin IS NULL OR date(EV.END) >= date(:dateEventMin)) \
                                    AND \
                                        (:dateEventMax IS NULL OR date(EV.START) <= date(:dateEventMax)) \
                                    GROUP BY \
                                        ER.MEMBER \
                                ) AS NBEVENT \
                                FROM \
                                    MEMBERS T \
                                WHERE \
                                    (:regNum IS NULL OR T.REGNUM LIKE '%:regNum%') \
                                AND \
                                    (:gender IS NULL OR T.GENDER = :gender) \
                                AND \
                                    (:type IS NULL OR T.TYPE = :type) \
                                AND \
                                  (  (:firstname IS NULL AND :othername IS NULL AND :lastname IS NULL) \
                                OR \
                                    T.FIRSTNAME LIKE :firstname \
                                OR \
                                    T.OTHERNAME LIKE :othername \
                                OR \
                                    T.LASTNAME LIKE :lastname ) \
                                AND \
                                    (:birthDateMax IS NULL OR date(T.BIRTHDATE) > date(:birthDateMax)) \
                                AND \
                                    (:birthDateMin IS NULL OR date(T.BIRTHDATE) < date(:birthDateMin))";
    }
    else
    {
        queryString = "SELECT \
                        CAST(id AS CHAR) as id, regNum, firstName, otherName, lastName, birthDate, birthLocation, CAST(gender AS CHAR) AS gender, CAST(type AS CHAR) AS type, createdAt, updatedAt, comment, \
                        ( \
                            SELECT \
                                CAST(COUNT(EQ.ID) AS CHAR) \
                            FROM \
                                MEMBERS_RELATION ER, \
                                MEMBERS EQ \
                            WHERE \
                                T.ID = ER.MEMBER1 \
                            AND \
                                EQ.ID = ER.MEMBER2 \
                            AND \
                                EQ.GENDER = 1 \
                            AND \
                                (ER.TYPE = 2 OR ER.TYPE = 3) \
                            AND \
                                (:childBirthDate IS NULL OR EQ.BIRTHDATE > :childBirthDate) \
                            GROUP BY \
                                ER.MEMBER1 \
                        ) AS NBBOY, \
                        ( \
                            SELECT \
                                CAST(COUNT(EQ.ID) AS CHAR) \
                            FROM \
                                MEMBERS_RELATION ER, \
                                MEMBERS EQ  \
                            WHERE  \
                                T.ID = ER.MEMBER1  \
                            AND  \
                                EQ.ID = ER.MEMBER2 \
                            AND \
                                EQ.GENDER = 2 \
                            AND \
                                (ER.TYPE = 2 OR ER.TYPE = 3) \
                            AND \
                                (:childBirthDate IS NULL OR EQ.BIRTHDATE > :childBirthDate) \
                            GROUP BY \
                                ER.MEMBER1 \
                        ) AS NBGIRL, \
                        ( \
                            SELECT \
                                CAST(COUNT(EQ.ID) AS CHAR) \
                            FROM \
                                MEMBERS_RELATION ER, \
                                MEMBERS EQ \
                            WHERE \
                                T.ID = ER.member1 \
                            AND \
                                EQ.ID = ER.member2 \
                            AND \
                                EQ.GENDER = 0 \
                            AND \
                                (ER.TYPE = 2 OR ER.TYPE = 3) \
                            AND \
                                (:childBirthDate IS NULL OR EQ.BIRTHDATE > :childBirthDate) \
                            GROUP BY \
                                ER.MEMBER1 \
                        ) AS NBOTHER, \
                        ( \
                            SELECT \
                                CAST(COUNT(EV.ID) AS CHAR) \
                            FROM \
                                EVENTS_MEMBERS ER, \
                                EVENTS EV \
                            WHERE \
                                T.ID = ER.MEMBER \
                            AND \
                                EV.id = ER.EVENT \
                            AND \
                                (:dateEventMin IS NULL OR EV.END >= :dateEventMin) \
                            AND \
                                (:dateEventMax IS NULL OR EV.START <= :dateEventMax) \
                            GROUP BY \
                                ER.MEMBER \
                        ) AS NBEVENT \
                        FROM \
                            MEMBERS T \
                        WHERE \
                            (:regNum IS NULL OR T.REGNUM LIKE '%:regNum%') \
                        AND \
                            (:gender IS NULL OR T.GENDER = :gender) \
                        AND \
                            (:type IS NULL OR T.TYPE = :type) \
                        AND \
                          (  (:firstname IS NULL AND :othername IS NULL AND :lastname IS NULL) \
                        OR \
                            T.FIRSTNAME LIKE :firstname \
                        OR \
                            T.OTHERNAME LIKE :othername \
                        OR \
                            T.LASTNAME LIKE :lastname ) \
                        AND \
                            (:birthDateMax IS NULL OR T.BIRTHDATE > :birthDateMax) \
                        AND \
                            (:birthDateMin IS NULL OR T.BIRTHDATE < :birthDateMin)";
    }


    query.prepare(queryString);

    //Bind
    if(!regNum->text().isEmpty())
    {
        query.bindValue(":regNum", regNum->text());
    }

    if(gender->getID() > -1)
    {
        query.bindValue(":gender", gender->getID());
    }

    if(memberType->currentData().toInt() != -1)
    {
        query.bindValue(":type", memberType->currentData().toInt());
    }

    if(!firstName->text().isEmpty())
    {
        query.bindValue(":firstname", "%" + firstName->text() + "%");
    }

    if(otherName->isChecked())
    {
        query.bindValue(":othername", "%" + firstName->text() + "%");
    }

    if(!lastName->text().isEmpty() || (autoLast->isChecked() && !firstName->text().isEmpty()))
    {
        if(autoLast->isChecked())
        {
            query.bindValue(":lastname", "%" + firstName->text() + "%");
        }
        else
        {
            query.bindValue(":lastname", "%" + lastName->text() + "%");
        }
    }

    if(ageMin->value() > 0)
    {
        query.bindValue(":birthDateMin", global::birthDate(ageMin->value()).toString(Qt::ISODate));
    }

    if(ageMax->value() > 0)
    {
        query.bindValue(":birthDateMax", global::birthDate(ageMax->value()).toString(Qt::ISODate));
    }

    if(selectDisplay->getState(11))
    {
        query.bindValue(":dateEventMin", option_event_dateMin->date().toString(Qt::ISODate));
        query.bindValue(":dateEventMax", option_event_dateMax->date().toString(Qt::ISODate));
    }

    if(selectDisplay->getState(10))
    {
        if(option_child_ageMax->value() > 0)
        {
            query.bindValue(":childBirthDate", global::birthDate(option_child_ageMax->value()).toString(Qt::ISODate));
        }
    }

    user::updateDB();


    if(query.exec())
    {
       while(query.next())
        {
            table->setRowCount(count+1);
            table->setItem(count, 0, new QTableWidgetItem(query.record().value("id").toString()));
            table->setItem(count, 1, new QTableWidgetItem(query.record().value("regNum").toString()));
            table->setItem(count, 2, new QTableWidgetItem(query.record().value("firstName").toString()));
            table->setItem(count, 3, new QTableWidgetItem(query.record().value("otherName").toString()));
            table->setItem(count, 4, new QTableWidgetItem(query.record().value("lastName").toString()));
            table->setItem(count, 5, new QTableWidgetItem(QString::number(global::age(query.record().value("birthDate").toDate()))));
            table->setItem(count, 6, new QTableWidgetItem(query.record().value("birthDate").toDate().toString()));
            table->setItem(count, 7, new QTableWidgetItem(query.record().value("birthLocation").toString()));
            table->setItem(count, 8, new QTableWidgetItem(query.record().value("type").toString()));
            int temp = user::type_id.indexOf(query.record().value("type").toInt());
            if(temp<1) { temp=0; }
            table->setItem(count, 8, new QTableWidgetItem(user::type_text.at(temp)));
            table->setItem(count, 9, new QTableWidgetItem(member_gender::fromID(query.record().value("gender").toInt())));
            int nbboy = query.record().value("NBBOY").toInt();
            int nbgirl = query.record().value("NBGIRL").toInt();
            int nbother = query.record().value("NBOTHER").toInt();
            table->setItem(count, 10, new QTableWidgetItem(QString::number(nbboy+nbgirl+nbother)+" ("+QString::number(nbboy)+","+QString::number(nbgirl)+")"));
            table->setItem(count, 11, new QTableWidgetItem(QString::number(query.record().value("NBEVENT").toInt())));
            count++;
        }
    }
    else
    {
        engine_db::dispERR(this, query);
    }

    result->setText(tr("%n Result(s) Found", "", count));
    table->resizeColumnsToContents();
    table->sortByColumn(2, Qt::AscendingOrder);
    table->setSortingEnabled(true);
}
