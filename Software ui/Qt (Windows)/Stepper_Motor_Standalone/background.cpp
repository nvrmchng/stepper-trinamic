#include "background.h"

Background::Background(QWidget *parent) : QWidget(parent)
{
	this->setupUi();
}

Background::~Background()
{
    ;
}

void Background::setupUi()
{
	this->resize(480,272);
    int pos_x = this->x();
    int pos_y = this->y();
    this->move(pos_x + 960 - (480/2), pos_y + 540 - (272/2));
	
	QPalette pal = QPalette();
    pal.setColor(QPalette::Window, Qt::darkGray);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}
