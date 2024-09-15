#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QWidget>

class Background : public QWidget
{
    Q_OBJECT

public:
    explicit Background(QWidget *parent = nullptr);
    ~Background();
    void setupUi();
};
#endif // WIDGET_H
