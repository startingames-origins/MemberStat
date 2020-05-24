#include "member_gender.h"

QStringList member_gender::genderList;

member_gender::member_gender() {}

bool member_gender::init()
{
    if(genderList.count()>0) { return false; }
    genderList.append(tr("-"));
    genderList.append(tr("Man"));
    genderList.append(tr("Woman"));
    return true;
}

QString member_gender::fromID(int id)
{
    if(id>(genderList.count()-1) || id<0) { id=0; }
    return genderList.at(id);
}

int member_gender::toID(QString text)
{
    int id = genderList.indexOf(text);
    if(id<0) { id=0; }
    return id;
}

int member_gender::count()
{
    return genderList.count();
}

member_gender_widget::member_gender_widget(QWidget *_parent, bool _searchMode) : QComboBox(_parent)
{
    searchMode = _searchMode;
    if(searchMode)
    {
        addItem("", -1);
    }
    for (int i=0; i<member_gender::count(); i++)
    {
        addItem(member_gender::fromID(i), i);
    }
}

int member_gender_widget::getID()
{
    return currentData().toInt();
}

void member_gender_widget::setID(int id)
{
    if(searchMode)
    {
        setCurrentIndex(id+1);
    }
    else
    {
        setCurrentIndex(id);
    }
}
