/*
 * Author: Terraneo Federico
 * Distributed under the Boost Software License, Version 1.0.
 *
 * v1.01:  Fixed a bug regarding reading after a timeout.
 *
 * v1.00: First release.
 */

#include "serialstream.h"

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using namespace std;
using namespace boost;
using namespace boost::asio;

/**
 * Possible outcome of a read. Set by callbacks, read from main code
 */
enum ReadResult
{
    resultInProgress,
    resultSuccess,
    resultError,
    resultTimeout
};

//
// class SerialDeviceImpl
//

/**
 * Contains all data of the SerialDevice class
 */
class SerialDeviceImpl : private boost::noncopyable
{
public:
    /**
     * Construct a SerialDeviceImpl from a SerialOptions class
     * \param options serial port options
     */
    SerialDeviceImpl(const SerialOptions& options);
    
    io_service io; ///< Io service object
    serial_port port; ///< Serial port object
    deadline_timer timer; ///< Timer for timeout
    posix_time::time_duration timeout; ///< Read/write timeout
    enum ReadResult result;  ///< Used by read with timeout
    streamsize bytesTransferred; ///< Used by async read callback
    char *readBuffer; ///< Used to hold read data
    streamsize readBufferSize; ///< Size of read data buffer
};

SerialDeviceImpl::SerialDeviceImpl(const SerialOptions& options)
        : io(), port(io), timer(io), timeout(options.getTimeout()),
        result(resultError), bytesTransferred(0), readBuffer(0),
        readBufferSize(0)
{
    try {
        //For this code to work, there should always be a timeout, so the
        //request for no timeout is translated into a very long timeout
        if(timeout==posix_time::seconds(0)) timeout=posix_time::hours(100000);

        port.open(options.getDevice());//Port must be open before setting option

        port.set_option(serial_port_base::baud_rate(options.getBaudrate()));

        switch(options.getParity())
        {
            case SerialOptions::odd:
                port.set_option(serial_port_base::parity(
                        serial_port_base::parity::odd));
                break;
            case SerialOptions::even:
                port.set_option(serial_port_base::parity(
                        serial_port_base::parity::even));
                break;
            default:
                port.set_option(serial_port_base::parity(
                        serial_port_base::parity::none));
                break;
        }

        port.set_option(serial_port_base::character_size(options.getCsize()));

        switch(options.getFlowControl())
        {
            case SerialOptions::hardware:
                port.set_option(serial_port_base::flow_control(
                        serial_port_base::flow_control::hardware));
                break;
            case SerialOptions::software:
                port.set_option(serial_port_base::flow_control(
                        serial_port_base::flow_control::software));
                break;
            default:
                port.set_option(serial_port_base::flow_control(
                        serial_port_base::flow_control::none));
                break;
        }

        switch(options.getStopBits())
        {
            case SerialOptions::onepointfive:
                port.set_option(serial_port_base::stop_bits(
                        serial_port_base::stop_bits::onepointfive));
                break;
            case SerialOptions::two:
                port.set_option(serial_port_base::stop_bits(
                        serial_port_base::stop_bits::two));
                break;
            default:
                port.set_option(serial_port_base::stop_bits(
                        serial_port_base::stop_bits::one));
                break;
        }
    } catch(std::exception& e)
    {
        throw ios::failure(e.what());
    }
}

//
// class SerialDevice
//

SerialDevice::SerialDevice(const SerialOptions& options)
                : pImpl(new SerialDeviceImpl(options)) {}

streamsize SerialDevice::read(char *s, streamsize n)
{
    pImpl->result=resultInProgress;
    pImpl->bytesTransferred=0;
    pImpl->readBuffer=s;
    pImpl->readBufferSize=n;

    pImpl->timer.expires_from_now(pImpl->timeout);
    pImpl->timer.async_wait(boost::bind(&SerialDevice::timeoutExpired,this,
            boost::asio::placeholders::error));
    
    pImpl->port.async_read_some(buffer(s,n),boost::bind(&SerialDevice::readCompleted,
            this,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));

    for(;;)
    {
        pImpl->io.run_one();
        switch(pImpl->result)
        {
            case resultSuccess:
                pImpl->timer.cancel();
                return pImpl->bytesTransferred;
            case resultTimeout:
                pImpl->port.cancel();
                throw(TimeoutException("Timeout expired"));
            case resultError:
                pImpl->port.cancel();
                pImpl->timer.cancel();
                throw(ios_base::failure("Error while reading"));
            default:
            //if resultInProgress remain in the loop
                break;
        }
    }
}

streamsize SerialDevice::write(const char *s, streamsize n)
{
    try {
        asio::write(pImpl->port,asio::buffer(s,n));
    } catch(std::exception& e)
    {
        throw(ios_base::failure(e.what()));
    }
    return n;
}

void SerialDevice::timeoutExpired(const boost::system::error_code& error)
{
    if(!error && pImpl->result==resultInProgress) pImpl->result=resultTimeout;
}

void SerialDevice::readCompleted(const boost::system::error_code& error,
        const size_t bytesTransferred)
{
    if(!error)
    {
        pImpl->result=resultSuccess;
        pImpl->bytesTransferred=bytesTransferred;
        return;
    }

    //In case a asynchronous operation is cancelled due to a timeout,
    //each OS seems to have its way to react.
    #ifdef _WIN32
    if(error.value()==995) return; //Windows spits out error 995
    #elif defined(__APPLE__)
    if(error.value()==45)
    {
        //Bug on OS X, it might be necessary to repeat the setup
        //http://osdir.com/ml/lib.boost.asio.user/2008-08/msg00004.html
        pImpl->port.async_read_some(
                asio::buffer(pImpl->readBuffer,pImpl->readBufferSize),
                boost::bind(&SerialDevice::readCompleted,this,boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
        return;
    }
    #else //Linux
    if(error.value()==125) return; //Linux outputs error 125
    #endif

    pImpl->result=resultError;
}
