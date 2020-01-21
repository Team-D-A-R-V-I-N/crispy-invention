#include <iostream>
#include <stdio.h>
#include <string.h>

#include <thread>
#include <chrono>

#include <algorithm>
#include <cmath>

#include "RoboteqDevice.h"
#include "ErrorCodes.h"
#include "Constants.h"

using namespace std;

#define RQERROR(x) {int status = x; if(status != RQ_SUCCESS) {cout<<"Error in file: "<<__FILE__<<" at line: "<<__LINE__<<" with error code: "<<status<<endl;}}sleepms(10)

#define MAXPOWER 300
#define STEP 10

char input = 'w';
bool exclusive = false;

void keyboard()
{
    cout<<"Started input thread"<<endl;
    bool quit = false;
    while(!quit)
    {
      if(exclusive) continue;
      cout<<"Enter command: ";
      cin>>input;
      cout<<(char)10;
      if(input == 'q' || input == 'Q') quit = true;
      if(input == 'i' || input == 'i') exclusive = true;
      this_thread::sleep_for(chrono::milliseconds(10));
    }
}

int main(int argc, char *argv[])
{
    RoboteqDevice device;
    int result;
    // To store RQERROR response
    int res; 	

    if(device.Connect("/dev/ttyACM1")!=RQ_SUCCESS)
    {
        if(device.Connect("/dev/ttyACM0")!=RQ_SUCCESS)
	{
		cout<<"No device connected"<<endl;
	}
    }

    cout<<"Setting config"<<endl;
    RQERROR(device.SetConfig(_MMOD, 1, 0  ));
    RQERROR(device.SetConfig(_MMOD, 2, 0  ));
    RQERROR(device.SetConfig(_ALIM, 1, 600));
    RQERROR(device.SetConfig(_ALIM, 2, 600));

    cout<<"Lauching input thread"<<endl;
    thread io(keyboard);

    int power1 = 0;
    int power2 = 0;

    char previnput = 'w';

    bool quit = false;
    while(!quit)
    {
        switch(input)
        {
            case 'w':
            case 'W':
                if(previnput == 'w' || previnput == 's')
                {
                    if(power1 < MAXPOWER) power1 += STEP;
                    if(power2 < MAXPOWER) power2 += STEP;
                }
                else
                {
                    int temp = min(abs(power1), abs(power2));
                    power1 = temp;
                    power2 = temp;
                }
                break;
            case 'a':
            case 'A':
                if(power1 > 0) power1 -= STEP;
                if(power2 < MAXPOWER) power2 += STEP;
                break;
            case 's':
            case 'S':
                if(previnput == 'w' || previnput == 's')
                {
                    if(power1 > -MAXPOWER) power1 -= STEP;
                    if(power2 > -MAXPOWER) power2 -= STEP;
                }
                else
                {
                    int temp = -min(abs(power1), abs(power2));
                    power1 = temp;
                    power2 = temp;
                }
                break;
            case 'd':
            case 'D':
                if(power1 < MAXPOWER) power1 += STEP;
                if(power2 > 0) power2 -= STEP;
                break;
            case 'k':
            case 'K':
                power1 = 0;
                power2 = 0;
                break;
            case 'j':
            case 'J':
                if(power1 > -MAXPOWER) power1 -= STEP;
                if(power2 < MAXPOWER) power2 += STEP;
                break;
            case 'l':
            case 'L':
                if(power1 < MAXPOWER) power1 += STEP;
                if(power2 > -MAXPOWER) power2 -= STEP;
                break;
            case 'i':
            case 'I':
                cout<<"Enter Power(-1000,1000): ";
                cin>>power1>>power2;
                input = 'v';
                previnput = 'v';
                exclusive = false;
            case 'e':
            case 'E':
                RQERROR(device.SetCommand(_EX, 1));
                break;
            case 't':
            case 'T':
                RQERROR(device.SetCommand(_MG, 1));
                break;
            case 'q':
            case 'Q':
                cout<<"Quitting"<<endl;
                quit = true;
                break;
            default:
                cout<<"Breaking"<<endl;
                break;
        };      
	RQERROR(device.GetValue(_DIN, 3, result)); 
	cout<<"RES: "<<result;
	//result++;
	if(result == 1) {
		RQERROR(device.SetCommand(_EX, 1));
	}
	//else if(result == 1){
	//	RQERROR(device.SetCommand(_MG, 1));
	//}      
	RQERROR(device.SetCommand(_GO, 1, power1)); 
	RQERROR(device.SetCommand(_GO, 2, power2));
        previnput = input;
    }

    io.join();

    device.Disconnect();
	return 0;
}
