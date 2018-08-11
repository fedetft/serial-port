
#include <iostream>
#include <thread>
#include "AsyncSerial.h"

using namespace std;
using namespace std::placeholders;

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
	serial.setCallback(bind(&Foo::received,foo,_1,_2));
	serial.writeString("Hello world\n");
	this_thread::sleep_for(chrono::seconds(5));
	serial.clearCallback();
}
