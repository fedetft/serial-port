/*
 * Author: Terraneo Federico
 * Distributed under the Boost Software License, Version 1.0.
 */

#include <string>
#include <stdexcept>
#include <boost/system/error_code.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/categories.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>

#ifndef SERIALSTREAM_H
#define	SERIALSTREAM_H

/**
 * Thrown if timeout occurs
 */
class TimeoutException: public std::ios_base::failure
{
public:
    TimeoutException(const std::string& arg): failure(arg) {}
};

/**
 * This class contains all the options for a serial port.
 */
class SerialOptions
{
    typedef boost::posix_time::time_duration time_duration;
    typedef boost::posix_time::seconds seconds;
    
public:
    ///Possible flow controls.
    enum FlowControl { noflow, software, hardware };

    ///Possible parities.
    enum Parity { noparity, odd, even };

    ///Possible stop bits.
    enum StopBits { one, onepointfive, two };

    /**
     * Default constructor.
     */
    SerialOptions() : device(), baudrate(9600), timeout(seconds(0)),
            parity(noparity), csize(8), flow(noflow), stop(one) {}

    /**
     * Constructor.
     * \param device device name (/dev/ttyS0, /dev/ttyUSB0, COM1, ...)
     * \param baudrate baudrate, like 9600, 115200 ...
     * \param timeout timeout when reading, use zero to disable
     * \param parity parity
     * \param csize character size (5,6,7 or 8)
     * \param flow flow control
     * \param stop stop bits
     * 
     */
    SerialOptions(const std::string& device, unsigned int baudrate,
            time_duration timeout=seconds(0), Parity parity=noparity,
            unsigned char csize=8, FlowControl flow=noflow, StopBits stop=one) :
            device(device), baudrate(baudrate), timeout(timeout),
            parity(parity), csize(csize), flow(flow), stop(stop) {}

    /**
     * Setter and getter for device name
     */
    void setDevice(const std::string& device) { this->device=device; }
    std::string getDevice() const { return this->device; }

    /**
     * Setter and getter for baudrate
     */
    void setBaudrate(unsigned int baudrate) { this->baudrate=baudrate; }
    unsigned int getBaudrate() const { return this->baudrate; }

    /**
     * Setter and getter for timeout
     */
    void setTimeout(time_duration timeout) { this->timeout=timeout; }
    time_duration getTimeout() const { return this->timeout; }

    /**
     * Setter and getter for parity
     */
    void setParity(Parity parity) { this->parity=parity; }
    Parity getParity() const { return this->parity; }

    /**
     * Setter and getter character size
     */
    void setCsize(unsigned char csize) { this->csize=csize; }
    unsigned char getCsize() const { return this->csize; }

    /**
     * Setter and getter flow control
     */
    void setFlowControl(FlowControl flow) { this->flow=flow; }
    FlowControl getFlowControl() const { return this->flow; }

    /**
     * Setter and getter for stop bits
     */
    void setStopBits(StopBits stop) { this->stop=stop; }
    StopBits getStopBits() const { return this->stop; }

private:
    std::string device;
    unsigned int baudrate;
    time_duration timeout;
    Parity parity;
    unsigned char csize;
    FlowControl flow;
    StopBits stop;
};

//Forward declaration
class SerialDeviceImpl;

/**
 * \internal
 * Implementation detail of a serial device.
 * User code should use SerialStream
 */
class SerialDevice
{
public:
    typedef char char_type;
    typedef boost::iostreams::bidirectional_device_tag category;

    /**
     * \internal
     * Constructor.
     * \throws ios_base::failure if there are errors with the serial port.
     * \param options serial port options
     */
    explicit SerialDevice(const SerialOptions& options);

    /**
     * \internal
     * Read from serial port.
     * \throws TimeoutException on timeout, or ios_base::failure if there are
     * errors with the serial port.
     * Note: TimeoutException derives from ios_base::failure so catching that
     * allows to catch any exception.
     * Use the clear() member function to go on reading after an exception was
     * thrown.
     * \param s where to store read characters
     * \param n max number of characters to read
     * \return number of character read
     */
    std::streamsize read(char *s, std::streamsize n);

    /**
     * \internal
     * Write to serial port.
     * \throws ios_base::failure if there are errors with the serial port.
     * Use the clear() member function to go on reading after an exception was
     * thrown.
     * \param s
     * \param n
     * \return
     */
    std::streamsize write(const char *s, std::streamsize n);

private:
    /**
     * Callack called either when the read timeout is expired or canceled.
     * If called because timeout expired, sets result to resultTimeoutExpired
     */
    void timeoutExpired(const boost::system::error_code& error);

    /**
     * Callback called either if a read complete or read error occurs
     * If called because of read complete, sets result to resultSuccess
     * If called because read error, sets result to resultError
     */
    void readCompleted(const boost::system::error_code& error,
            const size_t bytesTransferred);

    boost::shared_ptr<SerialDeviceImpl> pImpl; //Implementation
};

/**
 * SerialStream, an iostream-compatible serial port class.
 * Note: due to a limitation about error reporting with boost::iostreams,
 * this class *always* throws exceptions on error (timeout, failure, etc..)
 * so after creating an instance of this class you should alway enable
 * exceptions with the exceptions() member function:
 * \code SerialStream serial; serial.exceptions(ios::failbit | ios::badbit);
 * \endcode
 * If you don't, functions like getline() will swallow the exceptions, while
 * operator >> will throw, leading to unconsistent behaviour.
 */
typedef boost::iostreams::stream<SerialDevice> SerialStream;

#endif //SERIALSTREAM_H
