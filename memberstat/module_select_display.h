#ifndef MODULE_DISPLAY_SELECT_H
#define MODULE_DISPLAY_SELECT_H

#include <QtWidgets>

class module_select_display : public QWidget
{
    Q_OBJECT
public:
    module_select_display(QStringList names, int _col, QWidget *_parent = nullptr);
    int count();
    bool getState(int at);
    void setState(int at, bool state);
    void setDisplayed(int at, bool displayed);

private:
    int col;
    QGridLayout *layout;
    QList<QCheckBox*> listWidgets;
    QList<bool> listDisplayed;

private slots:
    void emitSelChanged(int);

signals:
    void selectionChanged();
};

#endif // MODULE_DISPLAY_SELECT_H
