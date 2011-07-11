
#include <iostream>
#include "SimpleSerial.h"

using namespace std;
using namespace boost;

int main(int argc, char* argv[])
{
    try {

        SimpleSerial serial("/dev/ttyUSB0",115200);

        serial.writeString("Hello world\n");

        cout<<"Received : "<<serial.readLine()<<" : end"<<endl;

    } catch(boost::system::system_error& e)
    {
        cout<<"Error: "<<e.what()<<endl;
        return 1;
    }
}

