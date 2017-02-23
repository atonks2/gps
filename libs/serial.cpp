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

#include "serial.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int checkPort(std::string port)
{
    struct stat stat_buf;
    if (stat(port.c_str(), &stat_buf) == 0) return 0;
    else return -1;
}

Serial::Serial():Serial(4800, "/dev/ttyUSB0") { }  // Delegating constructor
Serial::Serial(speed_t baud, std::string port):Serial(baud,port,true){ }  // Delegating constructor
Serial::Serial(speed_t baud, std::string port, bool canon)  // Target constructor
{
	if (setBaud(baud) != 0) setBaud(4800);
	if (checkPort(port) == 0) PORT = port;
	else PORT = "/dev/ttyUSB0";
	isCanonical = canon;
	isOpen = false;
    init();
}

// Open and configure the port
void Serial::init()
{

    dev_fd = open(PORT.c_str(), O_RDWR | O_NOCTTY);
    if (dev_fd < 0) {
        perror("Failed to open device: "); 
        exit(-1); 
    }
    else isOpen = true;

    tcgetattr(dev_fd, &oldConfig);    
    memset(&terminalConfiguration, 0, sizeof(terminalConfiguration));  // Clear junk from location of terminalConfiguration to start with clean slate
    tcgetattr(dev_fd, &terminalConfiguration);

    // TERMIOS CONFIGURATION

    // BAUDRATE: Integer multiple of 2400
    // CRTSCTS: Hardware flow control
    // CS8: 8N1
    // CLOCAL: No modem control. (local device)
	// HUPCL: Generates modem disconnect when port is closed
    // CREAD: Receive chars
    terminalConfiguration.c_cflag &= (BAUDRATE | CS8 | CLOCAL | HUPCL | CREAD);

    // IGNPAR: Ignore parity errors
    terminalConfiguration.c_iflag |= IGNPAR;

    // 0 for raw output
    terminalConfiguration.c_oflag = 0;

    // Setting input mode
    if (isCanonical == true) {
        terminalConfiguration.c_lflag |= (ICANON | ECHO | ECHOE);  // Canonical input
    }
    else {
        //Configure non-canonical mode
        terminalConfiguration.c_lflag &= ~(ICANON | ECHO | ECHOE);  // Disable canonical mode and echo
        terminalConfiguration.c_cc[VMIN] = 1;  // Minimum number of chars to read before returning
        terminalConfiguration.c_cc[VTIME] = 0;  // Timeout in deciseconds. 0 to disregard timing between bytes
    }
	tcflush(dev_fd, TCIOFLUSH);
    applyNewConfig();
}

int Serial::setBaud(speed_t baud)
{
    int status = -1;

    switch (baud) {
    case 2400:
        status = cfsetspeed(&terminalConfiguration, B2400);
        BAUDRATE = B2400;
        break;
    case 4800:
        status = cfsetspeed(&terminalConfiguration, B4800);
        BAUDRATE = B4800;
        break;
    case 9600:
        status = cfsetspeed(&terminalConfiguration, B9600);
        BAUDRATE = B9600;
        break;
    case 19200:
        status = cfsetspeed(&terminalConfiguration, B19200);
        BAUDRATE = B19200;
        break;
    case 38400:
        status = cfsetspeed(&terminalConfiguration, B38400);
        BAUDRATE = B38400;
        break;
    case 57600:
        status = cfsetspeed(&terminalConfiguration, B57600);
        BAUDRATE = B57600;
        break;
    case 115200:
        status = cfsetspeed(&terminalConfiguration, B115200);
        BAUDRATE = B115200;
        break;
    case 230400:
        status = cfsetspeed(&terminalConfiguration, B230400);
        BAUDRATE = B230400;
        break;
    default:
        std::cout << "Invalid baudrate requested.\n";
        return -1;
    }
    if (status < 0) {
        perror("In function setBaud() failed to set requested baudrate: ");
        return -1;
    }
    else {
        return status;
    }
}

int Serial::applyNewConfig()
{
    tcdrain(dev_fd);
	if (tcsetattr(dev_fd, TCSANOW, &terminalConfiguration) < 0) {
        perror("Could not apply configuration");
        return -1;
    }
	else return 0;
}

speed_t Serial::getBaud()
{
    return cfgetispeed(&terminalConfiguration);
}

termios Serial::getConfig()
{
	return terminalConfiguration;
}

// Helper function checks if port is open,
// then flushes the serial line.
int Serial::setupRead()
{
	if (!isOpen) return -1;
	if (tcflush(dev_fd, TCIOFLUSH < 0)) {
			perror("Could not flush line: ");
			return -1;
	}
    else return 0;
}

int Serial::serialRead()
{
	return serialRead(255);  // Request 255 bytes. Will return when \n is received.
}

int Serial::serialRead(int bytes)
{
	if (setupRead() < 0) return -1;
    int buf_size = bytes;
	char buf[buf_size];
	bytesReceived = read(dev_fd, buf, bytes);
	if (bytesReceived < 0) perror("Read failed: ");
	else buf[bytesReceived] = '\0';  // Null terminated
	serialData.assign(buf);  // Store as std::string
	return bytesReceived;
}

int Serial::flush()
{
    return tcflush(dev_fd, TCIOFLUSH);
}

int Serial::serialWrite(std::string str)
{
    if (!isOpen) return -1;
    int write_status =  write(dev_fd, str.c_str(), str.length());
    if (write_status < 0) {
        perror("Failed to write to port: ");
        return -1;
    }
    else return write_status;
}

std::string Serial::getData()
{
	return serialData;
}

Serial::~Serial()
{
	tcsetattr(dev_fd, TCSANOW, &oldConfig);  // Leave port how we found it
	close(dev_fd);
}
