#include "sidebar.h"

void Sidebar::InitSerialBus()
{
	QString errorString;
	const QList<QCanBusDeviceInfo> devices = QCanBus::instance()->availableDevices(QStringLiteral("socketcan"), &errorString);
	if(!errorString.isEmpty()){
		qDebug() << errorString;
	}
    can_serial_bus = QCanBus::instance()->createDevice(QStringLiteral("socketcan"), QStringLiteral("can1"), &errorString);
    if(can_serial_bus == nullptr){
		qDebug() << errorString;
        cout << "Failed to Create Device" << endl;
	} else {
        if(can_serial_bus->state() == QCanBusDevice::UnconnectedState){
            if(can_serial_bus->connectDevice() == true){
                if(can_serial_bus->state() == QCanBusDevice::ConnectedState){
                    cout << "Successfully Connected" << endl;
                } else {
                    cout << "Connected but Wrong State" << endl;
                }
            } else {
                cout << "Failed to Connect Device" << endl;
            }
        } else {    cout << "Created but Wrong State" << endl;  }
	}
}

void Sidebar::close_serialbus()
{
    if(can_serial_bus->state() == QCanBusDevice::ConnectedState){
        can_serial_bus->disconnectDevice();
    }
    cout << "Closing serial bus" << endl;
}

void Sidebar::write_control_update(QByteArray can_tx_data)
{
    if(can_serial_bus->state() == QCanBusDevice::ConnectedState)
    {
        // writeFrame only accept const QCanBusFrame, so we do it like this
        if(can_serial_bus->writeFrame(QCanBusFrame(0x1C0, can_tx_data)) == true)
        {
            cout << "write frame success" << endl;
        }
    }
}

void Sidebar::write_setting_update(QByteArray can_tx_data)
{
    if(can_serial_bus->state() == QCanBusDevice::ConnectedState)
    {
        // writeFrame only accept const QCanBusFrame, so we do it like this
        if(can_serial_bus->writeFrame(QCanBusFrame(0x1C1, can_tx_data)) == true)
        {
            cout << "write frame success" << endl;
        }
    }
}
