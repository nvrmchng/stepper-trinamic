#include "controlwindow.h"

ControlWindow::ControlWindow()
{
    this->setupUi();
    connect(slider_current, SIGNAL(valueChanged(int)), this, SLOT(on_slider_current_moved()));
    connect(slider_speed, SIGNAL(valueChanged(int)), this, SLOT(on_slider_speed_moved()));
    connect(start_stop_btn, SIGNAL(clicked()), this, SLOT(on_start_stop_btn_clicked()));
}

ControlWindow::~ControlWindow()
{
    delete label_current;
    delete slider_current;
    delete label_current_val;
    delete label_speed;
    delete slider_speed;
    delete label_speed_val;
    delete label_dir;
}

void ControlWindow::setupUi()
{
    this->setFixedSize(380, 252);
    int pos_x = this->x();
    int pos_y = this->y();
    this->move(pos_x + 960 - (480/2) + 90, pos_y + 540 - (272/2) + 10);

    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, QColor(0xFFA0A0A0));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    label_current = new QLabel("Current\n Scale:");
    label_current->setParent(this);
    QFont font;
    font.setPixelSize(12);
    font.setBold(true);
    label_current->setFont(font);
    label_current->setStyleSheet("QLabel { color : black; }");
    label_current->move(10,12);

    slider_current = new QSlider(Qt::Horizontal);
    slider_current->setParent(this);
    slider_current->setMinimum(0);
    slider_current->setMaximum(100);
    slider_current->setValue(25);
    slider_current->setTickInterval(1);
    slider_current->resize(240,18);
    slider_current->move(75,18);
    slider_current->setStyleSheet("QSlider::handle:horizontal { background-color: #ff0000; }");

    label_current_val = new QLabel(QString::number(slider_current->value()) + QString("%"));
    label_current_val->setParent(this);
    font.setPixelSize(12);
    font.setBold(true);
    label_current_val->setFont(font);
    label_current_val->setStyleSheet("QLabel { color : red; }");
    label_current_val->resize(40,18);
    label_current_val->move(330,18);

    label_speed = new QLabel("Speed:");
    label_speed->setParent(this);
    font.setPixelSize(12);
    font.setBold(true);
    label_speed->setFont(font);
    label_speed->setStyleSheet("QLabel { color : black; }");
    label_speed->move(14,75);

    slider_speed = new QSlider(Qt::Horizontal);
    slider_speed->setParent(this);
    slider_speed->setMinimum(1);
    slider_speed->setMaximum(50);
    slider_speed->setValue(1);
    slider_speed->setTickInterval(1);
    slider_speed->resize(240,18);
    slider_speed->move(75,75);
    slider_speed->setStyleSheet("QSlider::handle:horizontal { background-color: #0000ff; }");

    label_speed_val = new QLabel(QString::number(slider_speed->value()) + QString(" rpm"));
    label_speed_val->setParent(this);
    font.setPixelSize(12);
    font.setBold(true);
    label_speed_val->setFont(font);
    label_speed_val->setStyleSheet("QLabel { color : blue; }");
    label_speed_val->resize(45,18);
    label_speed_val->move(325,75);

    label_dir = new QLabel("Direction:");
    label_dir->setParent(this);
    font.setPixelSize(12);
    font.setBold(true);
    label_dir->setFont(font);
    label_dir->setStyleSheet("QLabel { color : black; }");
    label_dir->move(5,130);

    dir_cw = new QRadioButton();
    dir_cw->setParent(this);
    dir_cw->setText("Clockwise");
    dir_cw->move(75,128);

    dir_ccw = new QRadioButton();
    dir_ccw->setParent(this);
    dir_ccw->setChecked(true);
    dir_ccw->setText("CounterClockwise");
    dir_ccw->move(200,128);

    label_start_stop = new QLabel("Start/Stop:");
    label_start_stop->setParent(this);
    font.setPixelSize(12);
    font.setBold(true);
    label_start_stop->setFont(font);
    label_start_stop->setStyleSheet("QLabel { color : black; }");
    label_start_stop->move(5,185);

    QPixmap pixmap_start(":/resources/images/play_icon_25x25.png");
    QIcon icon_start(pixmap_start);
    start_stop_btn = new QPushButton;
    start_stop_btn->setParent(this);
    start_stop_btn->setIcon(icon_start);
    start_stop_btn->resize(85,30);
    start_stop_btn->move(145, 180);
    start_stop_btn->setText("START ");
    font.setPixelSize(12);
    font.setBold(true);
    start_stop_btn->setFont(font);
    start_stop_btn->setStyleSheet("QPushButton { background-color : rgb(204,204,255); text-align:center; }");
}

void ControlWindow::on_slider_current_moved()
{
    label_current_val->setText(QString::number(slider_current->value()) + QString("%"));
}

void ControlWindow::on_slider_speed_moved()
{
    label_speed_val->setText(QString::number(slider_speed->value()) + QString(" rpm"));
}

void ControlWindow::on_start_stop_btn_clicked()
{
    QPixmap pixmap_start(":/resources/images/play_icon_25x25.png");
    QIcon icon_start(pixmap_start);
    QPixmap pixmap_stop(":/resources/images/stop_icon_25x25.png");
    QIcon icon_stop(pixmap_stop);

    if(start_stop_btn->text() == "START "){
        start_stop_btn->setIcon(icon_stop);
        start_stop_btn->setText("STOP ");
    } else {
        start_stop_btn->setIcon(icon_start);
        start_stop_btn->setText("START ");
    }
}
