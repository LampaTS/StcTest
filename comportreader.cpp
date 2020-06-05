#include "comportreader.h"

comPortReader::comPortReader()
{
    connect(&m_port, &QSerialPort::readyRead, this, &comPortReader::readData);

    // таймер для обновлния списка портов
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkAvaliblePorts()));
    timer->start(10000); // каждые 10 секунд

    checkAvaliblePorts();
}

void comPortReader::startListening(QString path)
{
    m_port.close(); // закрыли старый порт

    // изменили имя порта и начали его слушать
    m_port.setPortName(path);
    if (!m_port.open(QIODevice::ReadOnly))
        emit sendMessage("Error opening port: " + path);
}

void comPortReader::readData()
{
    // очистили буфер и прочитали сообщение
    m_data.clear();
    m_data = m_port.readAll();
    QDataStream stream(&m_data, QIODevice::ReadOnly);

    emit sendMessage("Received message");

    // проверяем префикс
    quint8 prefix;
    stream >> prefix;
    if (prefix != 64) // проверка на 0x40
    {
        emit sendMessage("Wrong prefix, skipping this message");
        return;
    }

    // считываем 4 числа типа float
    // важно: если сообщение слишком короткое, то будут выведены только те числа, которые удалось прочитать
    // например при длине сообщения 11 байт будут считаны: префикс (1 байт) и 2 первых числа по 4 байта (sizeof(float))
    float numbers[MSG_AMOUNT];
    for (int i = 0; i < MSG_AMOUNT; i++)
    {
        if (stream.readRawData(reinterpret_cast<char*>(&numbers[i]), sizeof(float)) < sizeof(float))
        {
            emit sendMessage("Message is too short, number can't be read properly");
            return;
        }
        emit sendMessage(QString::number(numbers[i]));
    }

    // прочитали префикс и 4 числа, но в буфере что-то осталось
    if (!stream.atEnd())
        emit sendMessage("Message is too long: all data is read but some bytes are left");
}

void comPortReader::checkAvaliblePorts()
{
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    emit portListChanged(ports);
}
