#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QCloseEvent>
#include <QPushButton>
#include "controlwindow.h"
#include "settingwindow.h"

class Sidebar : public QWidget
{
	Q_OBJECT
public:
	explicit Sidebar();
	~Sidebar();
	void setupUi();
    ControlWindow *controlwindow;
    QPushButton *exitButton;
private:
	QLabel *label1;
    QLabel *label2;
    SettingWindow *settingwindow;
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent *event);
signals:
    void sidebarclosed();
};

#endif
