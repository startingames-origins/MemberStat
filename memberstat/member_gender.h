#ifndef MEMBER_GENDER_H
#define MEMBER_GENDER_H

#include <QtWidgets>

class member_gender : public QComboBox
{
    Q_OBJECT
public:
    member_gender();
    static bool init();
    static QString fromID(int id);
    static int toID(QString text);
    static int count();

private:
    static QStringList genderList;
};

class member_gender_widget : public QComboBox
{
    Q_OBJECT
public:
    member_gender_widget(QWidget *_parent = nullptr, bool _searchMode = false);
    int getID();
    void setID(int id);

private:
    bool searchMode;
};

#endif // MEMBER_GENDER_H
