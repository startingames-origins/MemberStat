/*****************************************
 *         STARTINGAMES Software
 *
 * Project : MemberStat
 * File : module_search_event.cpp
 *
 ****************************************/

#include "module_search_event.h"

module_search_event::module_search_event(QWidget *_parent) : QWidget(_parent)
{
    QLabel *refLabel = new QLabel(tr("Reference"), this);
    QLabel *nameLabel = new QLabel(tr("Name"), this);
    QLabel *startLabel = new QLabel(tr("Start"), this);
    QLabel *endLabel = new QLabel(tr("End"), this);

    ref = new QLineEdit(this);
    name = new QLineEdit(this);
    desc = new QCheckBox(tr("Search also in Description"), this);
    start = new QDateTimeEdit(this);
        start->setCalendarPopup(true);
        start->setDateTime(start->minimumDateTime());
    end = new QDateTimeEdit(this);
        end->setCalendarPopup(true);
        end->setDateTime(end->maximumDateTime());

    result = new QLabel(tr("Ready"), this);
        result->setStyleSheet("font-weight: bold;");
    search = new QPushButton(QIcon(ICON_SEARCH), tr("Search"), this);
        connect(search, SIGNAL(released()), this, SLOT(searchExec()));
    table = new QTableWidget(0, 6, this);
        table->setAlternatingRowColors(true);
        table->setStyleSheet("alternate-background-color: rgb(210,240,250);");
        table->setColumnHidden(0, true);
        table->verticalHeader()->setHidden(true);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        QStringList horizontalHeaders;
        horizontalHeaders << "id" << tr("Ref.") << tr("Name") << tr("Start") << tr("End") << tr("Description");
        table->setHorizontalHeaderLabels(horizontalHeaders);
        table->resizeColumnsToContents();
        table->sortByColumn(1, Qt::AscendingOrder);
        table->verticalHeader()->setDefaultSectionSize(40);
        table->setSortingEnabled(true);
        //connect(table, SIGNAL(itemSelectionChanged()), this, SLOT(versionSelChange()));
        connect(table, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(openEvent(QTableWidgetItem*)));

    QHBoxLayout *topLayout = new QHBoxLayout();
        topLayout->addWidget(refLabel);
        topLayout->addWidget(ref);
        topLayout->addWidget(startLabel);
        topLayout->addWidget(start);
        topLayout->addWidget(endLabel);
        topLayout->addWidget(end);

    QGridLayout *mainLayout = new QGridLayout(this);
        mainLayout->addLayout(topLayout, 0, 0, 1, 3);
        mainLayout->addWidget(nameLabel, 1, 0);
        mainLayout->addWidget(name, 1, 1);
        mainLayout->addWidget(desc, 1, 2);
        mainLayout->addWidget(result, 2, 0, 1, 2);
        mainLayout->addWidget(search, 2, 2);
        mainLayout->addWidget(table, 3, 0, 1, 3);

    setLayout(mainLayout);
}

void module_search_event::openEvent(QTableWidgetItem *item)
{
    emit doubleCliked(table->item(item->row(), 0)->text().toUInt());
}

void module_search_event::searchExec()
{
    result->setText(tr("Searching..."));
    int count = 0;
    table->setRowCount(0);

    uint8_t flags=0;
    if(!ref->text().isEmpty()) { flags+=1; }
    if(!name->text().isEmpty()) { flags+=8; }
    if(desc->isChecked()) { flags+=16; }

    QString queryString = "SELECT * FROM events WHERE ( (start >= '"+start->dateTime().toString(Qt::ISODate)+"' AND start <= '"+end->dateTime().toString(Qt::ISODate)+"') OR (end >= '"+start->dateTime().toString(Qt::ISODate)+"' AND end <= '"+end->dateTime().toString(Qt::ISODate)+"'))";

    if(flags>0)
    {
        queryString+=" AND ";
        if(flags&1)
        {
            queryString+= "ref LIKE '%"+engine_db::echap(ref->text())+"%'";
            flags-=1;
            if(flags>0) { queryString+=" AND "; }
        }
        if(flags&(8+16)) { queryString+="( "; }
        if(flags&8)
        {
            queryString+= "name LIKE '%"+engine_db::echap(name->text())+"%'";
            if(flags&(16)) { queryString+=" OR "; }
        }
        if(flags&16)
        {
            queryString+= "`desc` LIKE '%"+engine_db::echap(name->text())+"%'";
        }
        if(flags&(8+16))
        {
            queryString+=" )";
            flags=flags&0b11100111;
        }
    }

    QSqlQuery query;
    query.prepare(queryString);
    if(query.exec())
    {
       while(query.next())
        {
            table->setRowCount(count+1);
            table->setItem(count, 0, new QTableWidgetItem(query.record().value("id").toString()));
            table->setItem(count, 1, new QTableWidgetItem(query.record().value("ref").toString()));
            table->setItem(count, 2, new QTableWidgetItem(query.record().value("name").toString()));
            table->setItem(count, 3, new QTableWidgetItem(query.record().value("start").toDateTime().toString()));
            table->setItem(count, 4, new QTableWidgetItem(query.record().value("end").toDateTime().toString()));
            table->setItem(count, 5, new QTableWidgetItem(query.record().value("desc").toString()));
            count++;
        }
    }
    else
    {
        engine_db::dispERR(this, query);
    }

    result->setText(tr("%n Result(s) Found", "", count));
    table->resizeColumnsToContents();
    table->sortByColumn(4, Qt::DescendingOrder);
}
