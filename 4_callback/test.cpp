
#include <iostream>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "AsyncSerial.h"

using namespace std;

class Foo
{
public:
	void received(const char *data, unsigned int len)
	{
		string s(data,len);
		cout<<"Callback called! \""<<s<<"\""<<endl;
	}
};

int main()
{
	Foo foo;
	CallbackAsyncSerial serial("/dev/ttyUSB0",115200);
	//Bind the received() member function of the foo instance,
	//_1 and _2 are parameter forwarding placeholders
	serial.setCallback(boost::bind(&Foo::received,foo,_1,_2));
	serial.writeString("Hello world\n");
	boost::this_thread::sleep(boost::posix_time::seconds(5));
	serial.clearCallback();
}
