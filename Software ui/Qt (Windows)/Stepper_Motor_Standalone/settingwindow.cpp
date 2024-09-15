#include "settingwindow.h"
bool IsAnySettingChanged = false;

SettingWindow::SettingWindow()
{
    this->setupUi();
    connect(combox_cfg0, SIGNAL(currentIndexChanged(int)), this, SLOT(on_any_setting_changed()));
    connect(combox_cfg1_2, SIGNAL(currentIndexChanged(int)), this, SLOT(on_any_setting_changed()));
    connect(combox_cfg3, SIGNAL(currentIndexChanged(int)), this, SLOT(on_any_setting_changed()));
    connect(combox_cfg4, SIGNAL(currentIndexChanged(int)), this, SLOT(on_any_setting_changed()));
    connect(combox_cfg5, SIGNAL(currentIndexChanged(int)), this, SLOT(on_any_setting_changed()));
    connect(combox_cfg6, SIGNAL(currentIndexChanged(int)), this, SLOT(on_any_setting_changed()));
    connect(setButton, SIGNAL(clicked()), this, SLOT(on_set_button_clicked()));

    setButton->hide();
}

SettingWindow::~SettingWindow()
{
    delete label_cfg0;
    delete label_cfg1_2;
    delete label_cfg3;
    delete label_cfg4;
    delete label_cfg5;
    delete label_cfg6;
    delete combox_cfg0;
    delete combox_cfg1_2;
    delete combox_cfg3;
    delete combox_cfg4;
    delete combox_cfg5;
    delete combox_cfg6;
}

void SettingWindow::setupUi()
{
    this->setFixedSize(380, 252);
    int pos_x = this->x();
    int pos_y = this->y();
    this->move(pos_x + 960 - (480/2) + 90, pos_y + 540 - (272/2) + 10);

    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, QColor(0xFFA0A0A0));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    label_cfg0 = new QLabel("Chopper off time:");
    label_cfg0->setParent(this);
    QFont font;
    font.setPixelSize(12);
    font.setBold(true);
    label_cfg0->setFont(font);
    label_cfg0->setStyleSheet("QLabel { color : black; }");
    label_cfg0->move(5,8);

    combox_cfg0 = new QComboBox();
    combox_cfg0->setParent(this);
    combox_cfg0->addItem(QString("140 TCLK"));
    combox_cfg0->addItem(QString("236 TCLK"));
    combox_cfg0->addItem(QString("332 TCLK"));
    combox_cfg0->move(130,5);

    label_cfg1_2 = new QLabel("Microstep\nresolution:");
    label_cfg1_2->setParent(this);
    label_cfg1_2->setFont(font);
    label_cfg1_2->setStyleSheet("QLabel { color : black; }");
    label_cfg1_2->move(5,32);

    combox_cfg1_2 = new QComboBox();
    combox_cfg1_2->setParent(this);
    combox_cfg1_2->addItem(QString("1 Fullstep, No Interpolation, SpreadCycle"));
    combox_cfg1_2->addItem(QString("2 Halfstep, No Interpolation, SpreadCycle"));
    combox_cfg1_2->addItem(QString("2 Halfstep, Use Interpolation, SpreadCycle"));
    combox_cfg1_2->addItem(QString("4 Quarterstep, No Interpolation, SpreadCycle"));
    combox_cfg1_2->addItem(QString("16 microstep, No Interpolation, SpreadCycle"));
    combox_cfg1_2->addItem(QString("4 Quarterstep, Use Interpolation, SpreadCycle"));
    combox_cfg1_2->addItem(QString("16 microstep, Use Interpolation, SpreadCycle"));
    combox_cfg1_2->addItem(QString("4 Quarterstep, Use Interpolation, StealthChop"));
    combox_cfg1_2->addItem(QString("16 microstep, Use Interpolation, StealthChop"));
    combox_cfg1_2->move(82,37);

    label_cfg3 = new QLabel("Current setting:");
    label_cfg3->setParent(this);
    label_cfg3->setFont(font);
    label_cfg3->setStyleSheet("QLabel { color : black; }");
    label_cfg3->move(5,73);

    combox_cfg3 = new QComboBox();
    combox_cfg3->setParent(this);
    combox_cfg3->addItem(QString("External sense resistor"));
    combox_cfg3->addItem(QString("Internal sense resistor"));
    combox_cfg3->addItem(QString("External sense resistor, scaled by AIN"));
    combox_cfg3->move(115,70);

    label_cfg4 = new QLabel("Chopper hysteresis:");
    label_cfg4->setParent(this);
    label_cfg4->setFont(font);
    label_cfg4->setStyleSheet("QLabel { color : black; }");
    label_cfg4->move(5,105);

    combox_cfg4 = new QComboBox();
    combox_cfg4->setParent(this);
    combox_cfg4->addItem(QString("Hysteresis end 5"));
    combox_cfg4->addItem(QString("Hysteresis end 9"));
    combox_cfg4->addItem(QString("Hysteresis end 13"));
    combox_cfg4->move(150,102);

    label_cfg5 = new QLabel("Chopper blank time:");
    label_cfg5->setParent(this);
    label_cfg5->setFont(font);
    label_cfg5->setStyleSheet("QLabel { color : black; }");
    label_cfg5->move(5,137);

    combox_cfg5 = new QComboBox();
    combox_cfg5->setParent(this);
    combox_cfg5->addItem(QString("Blank time 16"));
    combox_cfg5->addItem(QString("Blank time 24"));
    combox_cfg5->addItem(QString("Blank time 36"));
    combox_cfg5->move(150,133);

    label_cfg6 = new QLabel("Driver enable:");
    label_cfg6->setParent(this);
    label_cfg6->setFont(font);
    label_cfg6->setStyleSheet("QLabel { color : black; }");
    label_cfg6->move(5,168);

    combox_cfg6 = new QComboBox();
    combox_cfg6->setParent(this);
    combox_cfg6->addItem(QString("Driver enabled"));
    combox_cfg6->addItem(QString("Driver disabled"));
    combox_cfg6->addItem(QString("Driver enabled, standstill power down"));
    combox_cfg6->move(110,164);

    QPixmap pixmap(":/resources/images/send_icon_30x30.png");
    QIcon buttonicon(pixmap);
    setButton = new QPushButton;
    setButton->setParent(this);
    setButton->setIcon(buttonicon);
    setButton->resize(85,25);
    setButton->move(280, 215);
    setButton->setText("Set Change");
    font.setPixelSize(9);
    setButton->setFont(font);
    setButton->setStyleSheet("QPushButton { background-color : rgb(132,222,2); }");
}

void SettingWindow::on_any_setting_changed()
{
    if(IsAnySettingChanged == true){
        return;
    }
    setButton->show();
    IsAnySettingChanged = true;
}

void SettingWindow::on_set_button_clicked()
{
    setButton->hide();
    IsAnySettingChanged = false;
}
