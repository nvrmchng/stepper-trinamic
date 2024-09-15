#ifndef CONTROLWINDOW_H
#define CONTROLWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QRadioButton>
#include <QPushButton>

class ControlWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ControlWindow();
    ~ControlWindow();
    void setupUi();
private:
    QLabel *label_current;
	QSlider *slider_current;
    QLabel *label_current_val;
    QLabel *label_speed;
    QSlider *slider_speed;
    QLabel *label_speed_val;
    QLabel *label_dir;
    QRadioButton *dir_ccw;
    QRadioButton *dir_cw;
    QLabel *label_start_stop;
    QPushButton *start_stop_btn;
private slots:
    void on_slider_current_moved();
    void on_slider_speed_moved();
    void on_start_stop_btn_clicked();
};

#endif // CONTROLWINDOW_H
