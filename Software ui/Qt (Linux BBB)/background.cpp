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
    this->move(0, 0);
	
	QPalette pal = QPalette();
    pal.setColor(QPalette::Window, Qt::darkGray);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}
