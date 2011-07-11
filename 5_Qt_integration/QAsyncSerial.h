/**
 * Author: Terraneo Federico
 * Distributed under the Boost Software License, Version 1.0.
 */

#ifndef QASYNCSERIAL_H
#define QASYNCSERIAL_H

#include <QObject>
#include <boost/shared_ptr.hpp>

class QAsyncSerialImpl;

/**
 * Asynchronous serial class that integrates well with Qt GUI applictions.
 */
class QAsyncSerial : public QObject
{
    Q_OBJECT

public:
    /**
     * Default constructor
     */
    QAsyncSerial();

    /**
     * Constructor. Opens a serial port
     * \param devname port name, like "/dev/ttyUSB0" or "COM4"
     * \param baudrate port baud rate, example 115200
     * Format is 8N1, flow control is disabled.
     */
    QAsyncSerial(QString devname, unsigned int baudrate);

    /**
     * Opens a serial port
     * \param devname port name, like "/dev/ttyUSB0" or "COM4"
     * \param baudrate port baud rate, example 115200
     * Format is 8N1, flow control is disabled.
     */
    void open(QString devname, unsigned int baudrate);

    /**
     * Closes a serial port.
     */
    void close();

    /**
     * \return true if the port is open
     */
    bool isOpen();

    /**
     * \return true if any error
     */
    bool errorStatus();

    /**
     * Write a string to the serial port
     */
    void write(QString data);

    /**
     * Destructor
     */
    ~QAsyncSerial();

signals:

    /**
     * Signal called when data is received from the serial port.
     * This signal is line based, data is grouped by line and a signal
     * is emitted for each line.
     * \param data the line of text just received.
     */
    void lineReceived(QString data);

private:
    /**
     * Called when data is received
     */
    void readCallback(const char *data, size_t size);

    boost::shared_ptr<QAsyncSerialImpl> pimpl; ///< Pimpl idiom
};

#endif // QASYNCSERIAL_H
