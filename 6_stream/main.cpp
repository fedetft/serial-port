//A serial class that appears as an iostream

#include <iostream>
#include "serialstream.h"

using namespace std;
using namespace boost::posix_time;

int main(int argc, char* argv[])
{
    SerialOptions options;
    options.setDevice("/dev/ttyUSB0");
    options.setBaudrate(115200);
    options.setTimeout(seconds(3));
    //options.setFlowControl(SerialOptions::software);
    //options.setParity(SerialOptions::even);
    //options.setCsize(7);
    SerialStream serial(options);
    serial.exceptions(ios::badbit | ios::failbit); //Important!
    serial<<"Hello world"<<endl;
    string s;
    //serial>>s;
    getline(serial,s);
    cout<<s<<endl;
    return 0;
}
