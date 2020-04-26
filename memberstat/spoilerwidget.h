#ifndef SPOILERWIDGET_H
#define SPOILERWIDGET_H

#include <QFrame>
#include <QGridLayout>
#include <QParallelAnimationGroup>
#include <QScrollArea>
#include <QToolButton>
#include <QWidget>
#include <QPropertyAnimation>

class spoilerWidget : public QWidget {
    Q_OBJECT
public:
    QGridLayout mainLayout;
    QToolButton toggleButton;
    QFrame headerLine;
    QParallelAnimationGroup toggleAnimation;
    QScrollArea contentArea;
    int animationDuration{300};
public:
    explicit spoilerWidget(const QString & title = "", const int animationDuration = 300, QWidget *parent = nullptr);
    void setContentLayout(QLayout & contentLayout);

private slots:
    void stateChangedRequest(QAbstractAnimation::State newState, QAbstractAnimation::State);

signals:
    void stateChanged();
};

#endif // SPOILERWIDGET_H
