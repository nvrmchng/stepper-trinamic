QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialbus

QMAKE_CC += g++-11

TARGET = Stepper_Motor_Standalone
    target.files = Stepper_Motor_Standalone
    target.path = /home/debian/qt-projects/Stepper_Motor_Standalone/bin

INSTALLS += target

SOURCES += \
    controlwindow.cpp \
    main.cpp \
    background.cpp \
    serialbus.cpp \
    settingwindow.cpp \
    sidebar.cpp

HEADERS += \
    background.h \
    controlwindow.h \
    settingwindow.h \
    sidebar.h

RESOURCES += \
    myresources.qrc
