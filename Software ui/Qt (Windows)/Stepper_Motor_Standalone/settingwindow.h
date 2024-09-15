#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

class SettingWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SettingWindow();
    ~SettingWindow();
    void setupUi();
    QPushButton *setButton;
private:
    QLabel *label_cfg0;
    QComboBox *combox_cfg0;
    QLabel *label_cfg1_2;
    QComboBox *combox_cfg1_2;
    QLabel *label_cfg3;
    QComboBox *combox_cfg3;
    QLabel *label_cfg4;
    QComboBox *combox_cfg4;
    QLabel *label_cfg5;
    QComboBox *combox_cfg5;
    QLabel *label_cfg6;
    QComboBox *combox_cfg6;
private slots:
    void on_any_setting_changed();
    void on_set_button_clicked();
};

#endif // SETTINGWINDOW_H
