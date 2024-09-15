#include "sidebar.h"

int WhichSelected = 1;

Sidebar::Sidebar()
{
	this->setupUi();
    controlwindow = new ControlWindow();
    settingwindow = new SettingWindow();
}

Sidebar::~Sidebar()
{
	delete label1;
    delete label2;
    delete exitButton;
    delete controlwindow;
    delete settingwindow;
}

void Sidebar::setupUi()
{
	this->setFixedSize(80, 272);
	int pos_x = this->x();
	int pos_y = this->y();
    this->move(pos_x + 960 - (480/2), pos_y + 540 - (272/2));
	
	QPalette pal = QPalette();
    pal.setColor(QPalette::Window, QColor(0xFF99CCFF));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
	
    label1 = new QLabel("Stepper\n Motor");
	label1->setParent(this);
	QFont font;
    font.setPixelSize(9);
	font.setBold(true);
    label1->setFont(font);
	label1->setStyleSheet("QLabel { color : black; }");
    label1->move(20,50);

    label2 = new QLabel("Standalone\n Operation");
    label2->setParent(this);
    label2->setFont(font);
    label2->setStyleSheet("QLabel { color : black; }");
    label2->move(12,130);

    QPixmap pixmap(":/resources/images/power_button.png");
    QIcon buttonicon(pixmap);
    exitButton = new QPushButton;
    exitButton->setParent(this);
    exitButton->setIcon(buttonicon);
    exitButton->resize(55,25);
    exitButton->move(12, 242);
    exitButton->setText("EXIT");
    font.setPixelSize(8);
    font.setBold(true);
    exitButton->setFont(font);
    exitButton->setStyleSheet("QPushButton { background-color : rgb(192,192,192); }");
}

void Sidebar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
	
    QImage img1 = QImage(QString(":/resources/images/electric_motor.png"), nullptr);
    painter.drawImage(QPoint(22,20),img1);

    QImage img2 = QImage(QString(":/resources/images/standalone_setting.png"), nullptr);
    painter.drawImage(QPoint(20,92),img2);

    if(WhichSelected == 1){
        painter.setPen(Qt::SolidLine);
        painter.drawRect(10,10,60,65);
        painter.fillRect(10,10,60,65,QBrush(QColor(0x00,0xFF,0x00,0x4F),Qt::SolidPattern));
    } else {
        if(WhichSelected == 2){
            painter.setPen(Qt::SolidLine);
            painter.drawRect(10,90,60,65);
            painter.fillRect(10,90,60,65,QBrush(QColor(0x00,0xFF,0x00,0x4F),Qt::SolidPattern));
        }
    }
}

void Sidebar::mousePressEvent(QMouseEvent *event)
{
    event->accept();
    qDebug() << "mouse pressed" << event->pos().x() << "&" << event->pos().y() << endl;

    int pos_y = event->pos().y();
    switch(WhichSelected){
    case 0: if(pos_y < 60)
            {   WhichSelected = 1;
                controlwindow->show();
            }
            if(pos_y > 90)
            {   WhichSelected = 2;
                settingwindow->show();
            }
            break;
    case 1: if(pos_y > 90)
            {   WhichSelected = 2;
                controlwindow->close();
                settingwindow->show();
            }
            break;
    case 2: if(pos_y < 60)
            {   WhichSelected = 1;
                controlwindow->show();
                settingwindow->close();
            }
            break;
    default:    break;
    }

    this->update();
}

void Sidebar::closeEvent(QCloseEvent *event)
{
    event->accept();
    if(WhichSelected == 1){
        controlwindow->close();
    }
    if(WhichSelected == 2){
        settingwindow->close();
    }
}
