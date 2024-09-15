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
    delete setButton;
}

void SettingWindow::setupUi()
{
    this->setFixedSize(380, 252);
    this->move(90, 10);

    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, QColor(0xFFA0A0A0));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    label_cfg0 = new QLabel("Chopper off time:");
    label_cfg0->setParent(this);
    QFont font_label;
    font_label.setPixelSize(12);
    font_label.setBold(true);
    label_cfg0->setFont(font_label);
    label_cfg0->setStyleSheet("QLabel { color : black; }");
    label_cfg0->move(5,8);

    combox_cfg0 = new QComboBox();
    combox_cfg0->setParent(this);
    QFont font_combox;
    font_combox.setPixelSize(11);
    combox_cfg0->setFont(font_combox);
    combox_cfg0->addItem(QString("140 TCLK"));
    combox_cfg0->addItem(QString("236 TCLK"));
    combox_cfg0->addItem(QString("332 TCLK"));
    combox_cfg0->move(130,5);

    label_cfg1_2 = new QLabel("Microstep\nresolution:");
    label_cfg1_2->setParent(this);
    label_cfg1_2->setFont(font_label);
    label_cfg1_2->setStyleSheet("QLabel { color : black; }");
    label_cfg1_2->move(5,32);

    combox_cfg1_2 = new QComboBox();
    combox_cfg1_2->setParent(this);
    combox_cfg1_2->setFont(font_combox);
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
    label_cfg3->setFont(font_label);
    label_cfg3->setStyleSheet("QLabel { color : black; }");
    label_cfg3->move(5,73);

    combox_cfg3 = new QComboBox();
    combox_cfg3->setParent(this);
    combox_cfg3->setFont(font_combox);
    combox_cfg3->addItem(QString("External sense resistor"));
    combox_cfg3->addItem(QString("Internal sense resistor"));
    combox_cfg3->addItem(QString("External sense resistor, scaled by AIN"));
    combox_cfg3->move(115,70);

    label_cfg4 = new QLabel("Chopper hysteresis:");
    label_cfg4->setParent(this);
    label_cfg4->setFont(font_label);
    label_cfg4->setStyleSheet("QLabel { color : black; }");
    label_cfg4->move(5,105);

    combox_cfg4 = new QComboBox();
    combox_cfg4->setParent(this);
    combox_cfg4->setFont(font_combox);
    combox_cfg4->addItem(QString("Hysteresis end 5"));
    combox_cfg4->addItem(QString("Hysteresis end 9"));
    combox_cfg4->addItem(QString("Hysteresis end 13"));
    combox_cfg4->move(150,102);

    label_cfg5 = new QLabel("Chopper blank time:");
    label_cfg5->setParent(this);
    label_cfg5->setFont(font_label);
    label_cfg5->setStyleSheet("QLabel { color : black; }");
    label_cfg5->move(5,137);

    combox_cfg5 = new QComboBox();
    combox_cfg5->setParent(this);
    combox_cfg5->setFont(font_combox);
    combox_cfg5->addItem(QString("Blank time 16"));
    combox_cfg5->addItem(QString("Blank time 24"));
    combox_cfg5->addItem(QString("Blank time 36"));
    combox_cfg5->move(150,133);

    label_cfg6 = new QLabel("Driver enable:");
    label_cfg6->setParent(this);
    label_cfg6->setFont(font_label);
    label_cfg6->setStyleSheet("QLabel { color : black; }");
    label_cfg6->move(5,168);

    combox_cfg6 = new QComboBox();
    combox_cfg6->setParent(this);
    combox_cfg6->setFont(font_combox);
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
    QFont font_button;
    font_button.setPixelSize(9);
    setButton->setFont(font_button);
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
    char standalone_parameter[6] = {0,0,0,0,0,0};
    // fill up
    standalone_parameter[0] = combox_cfg0->currentIndex();

    switch(combox_cfg1_2->currentIndex()){  // this one's the only one which has random pattern
    case 0: standalone_parameter[1] = 0x01; break;
    case 1: standalone_parameter[1] = 0x02; break;
    case 2: standalone_parameter[1] = 0x22; break;
    case 3: standalone_parameter[1] = 0x04; break;
    case 4: standalone_parameter[1] = 0x10; break;
    case 5: standalone_parameter[1] = 0x24; break;
    case 6: standalone_parameter[1] = 0x30; break;
    case 7: standalone_parameter[1] = 0x64; break;
    case 8: standalone_parameter[1] = 0x70; break;
    default: break;
    }

    standalone_parameter[2] = combox_cfg3->currentIndex();
    standalone_parameter[3] = combox_cfg4->currentIndex();
    standalone_parameter[4] = combox_cfg5->currentIndex();
    standalone_parameter[5] = combox_cfg6->currentIndex();

    emit latest_setting_parameter(QByteArray(standalone_parameter, 6));
    setButton->hide();
    IsAnySettingChanged = false;
}
