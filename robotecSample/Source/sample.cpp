#include <iostream>
#include <stdio.h>
#include <string.h>
#include <thread>

#include "RoboteqDevice.h"
#include "ErrorCodes.h"
#include "Constants.h"

using namespace std;

#define RQERROR(x) {int status = x; if(status!=RQ_SUCCESS) {cout<<"Error in: "<<__FILE__<<" at "<<__LINE__<<" with Error Code: "<<status<<'\n'; quit = true;}}sleepms(10)

bool quit = false;

int initmotor(RoboteqDevice &device, int channel)
{
	// Set Amp limit
	RQERROR(device.SetConfig(_ALIM,  channel, 600       ));
	// Set Encoder Usage
	RQERROR(device.SetConfig(_EMOD,  channel, 2         ));
	// Set Encoder Pulse Per Revolution
	RQERROR(device.SetConfig(_EPPR,  channel, -18000    ));
	// Set Command Linearity
	RQERROR(device.SetConfig(_CLIN,  channel, 0         ));
	// Set MOLEX to encoder
	RQERROR(device.SetConfig(_MLX,   channel, 1         ));
	// Set MMOD to open-loop
	RQERROR(device.SetConfig(_MMOD,  channel, 0         ));
	// Set Default Velocity
	RQERROR(device.SetConfig(_MVEL,  channel, 170       ));
	// Set Max RPM
	RQERROR(device.SetConfig(_MXRPM, channel, 170       ));
	// Set Motor Acceleration
	RQERROR(device.SetConfig(_MAC,   channel, 1000      ));
	// Set Motor Deceleration
	RQERROR(device.SetConfig(_MDEC,  channel, 1000      ));
	// Set Closed Loop Error Detection to disabled 
	RQERROR(device.SetConfig(_CLERD, channel, 0         ));
	// Set PID Prop Gain
	RQERROR(device.SetConfig(_KP,    channel, 10*1000000));
	// Set PID Integral Gain
	RQERROR(device.SetConfig(_KI,    channel, 1*1000000 ));
	return RQ_SUCCESS;
}

void reportspeed(RoboteqDevice &device)
{
	int counter;
	int speed;
	int channel = 0;
	while(!quit)
	{
		RQERROR(device.GetValue(_ABCNTR , channel + 1, counter));
		RQERROR(device.GetValue(_ABSPEED, channel + 1, speed  ));
		cout<<"Motor("<<channel<<") counter: "<<counter<<" speed: "<<speed<<endl;
		(channel == 0)? channel = 1 : channel = 0;
	}
}

int main(int argc, char *argv[])
{
	string port = "/dev/ttyACM1";

	RoboteqDevice device;
	int a;
	a = device.Connect(port);

	if(a != RQ_SUCCESS)
	{
		port = "/dev/ttyACM0";
		a = device.Connect(port);
		if(a != RQ_SUCCESS)
		{
			cout<<"Cant connect to any port"<<endl;
			return 1;
		}
	}

	cout<<"Start init"<<endl;
	RQERROR(initmotor(device, 1));
	RQERROR(initmotor(device, 2));
	cout<<"Done Initialization"<<endl;

	RQERROR(device.SetConfig(_PMOD, 1, 0));
	RQERROR(device.SetConfig(_PMOD, 2, 0));
	RQERROR(device.SetConfig(_PMOD, 3, 0));
	RQERROR(device.SetConfig(_PMOD, 4, 0));
	RQERROR(device.SetConfig(_PMOD, 5, 0));
	RQERROR(device.SetConfig(_PMOD, 6, 0));
	RQERROR(device.SetConfig(_PINA, 1, 0));
	RQERROR(device.SetConfig(_PINA, 2, 0));
	RQERROR(device.SetConfig(_PINA, 3, 0));
	RQERROR(device.SetConfig(_PINA, 4, 0));
	RQERROR(device.SetConfig(_PINA, 5, 0));
	RQERROR(device.SetConfig(_PINA, 6, 0));

	cout<<"Done setting config"<<endl;

	thread debug(reportspeed, ref(device));

	cout<<"Sending go command"<<endl;

	while(!quit)
	{
		RQERROR(device.SetCommand(_GO, 1, 100));
		RQERROR(device.SetCommand(_GO, 2, 100));
	}

	debug.join();

	device.Disconnect();
	return 0;
}
