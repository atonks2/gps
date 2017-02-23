/*
MIT License

Copyright (c) 2017 Alan Tonks

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef SERAIL_H
#define SERIAL_H

#include <termios.h>
#include <iostream>
#include <string.h>

class Serial
{
public:
    // Default constructor assumes canonical input,
    // PORT = /dev/ttyUSB0 and BAUDRATE = 4800
    Serial();

    // speed_t is termios version of an unsigned int
    // Supported baudrates:
    //   2400, 4800, 9600, 19200, 38400, 57600,
    //   115200, 230400
    // This constructor also assumes canonical input
    Serial(speed_t baud, std::string port);

    // Same as previous constructor, except the
    // calling code specifies input mode with
    // 'bool canon'
    Serial(speed_t baud, std::string port, bool canon);

    // Sets baudrate in termios struct, but doesn't 
    // apply the change. Must call applyNewConfig() 
    // before any change is applied to the port.
    int setBaud(speed_t baud);

    // Calls tcsetattr() to apply changes to the
    // termios struct.
    int applyNewConfig();

    // Method for canonical read
    // Returns # of bytes read.
    int serialRead();

    // Method for non-canonical read
    // Returns # of bytes read.
    int serialRead(int bytes);

    int flush();
    // Writes to serial port
    int serialWrite(std::string str);

    // Returns current baudrate. Values in termios.h
    // are hexidecimal constants. Ex:
    // #define B4800 0x0000000c
    // Because of this, the return valud of this
    // function if baudrate = 4800 is 12.
    speed_t getBaud();

    // Returns entire termios struct
    termios getConfig();

    std::string getData();

    ~Serial();
private:
    // Opens port for read/write, sets termios
    // flags, then applys the configuration.
    void init();

    // Helper function needs access to private variable isOpen
    int setupRead();

    // old_config stores the configuration to
    // be restored by destructor. new_config is
    // the active port.
    struct termios oldConfig, terminalConfiguration;

    int dev_fd, bytesReceived;

	// Requested baudrate is stored here after ensuring its a valid speed.
	speed_t BAUDRATE;

	// Requested Port stored here after ensuring it exists.
    std::string PORT, serialData;
	
    // isOpen refers to the port.
    bool isOpen, isCanonical;
};

#endif // SERIAL_H
