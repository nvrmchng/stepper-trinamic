#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QCloseEvent>
#include <QPushButton>
#include <QCanBus>
#include <QCanBusDevice>
#include <QCanBusDeviceInfo>
#include <iostream>
#include "controlwindow.h"
#include "settingwindow.h"

using namespace std;

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
    QCanBusDevice *can_serial_bus;
    void InitSerialBus();
private slots:
    void close_serialbus();
    void write_control_update(QByteArray can_tx_data);
    void write_setting_update(QByteArray can_tx_data);
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent *event);
};

#endif
