#include "background.h"
#include "sidebar.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	
    Background *bground = new Background(NULL);
    Sidebar *sidebar = new Sidebar();

    QObject::connect(sidebar->exitButton, SIGNAL(clicked()), sidebar, SLOT(close()));
    QObject::connect(sidebar->exitButton, SIGNAL(clicked()), bground, SLOT(close()));
    QObject::connect(sidebar->exitButton, SIGNAL(clicked()), sidebar, SLOT(close_serialbus()));
	
    bground->show();
    sidebar->show();
    sidebar->controlwindow->show();
	
	const int resultCode = a.exec();
    return resultCode;
}
