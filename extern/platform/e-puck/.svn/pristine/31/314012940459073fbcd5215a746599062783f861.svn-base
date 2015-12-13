// THIS FILE IS INTENDED TO BE USED FOR THE EPFL BLIMP CONTEST
// ONLY. DO NOT DISTRIBUTE. DO NOT USE FOR OTHER PURPOSE.
#include <vcl.h>
#pragma hdrstop
#include <windows.h>

#include "status.h"
#include "serialport.h"


SerialPort::SerialPort() {
	mBuffer = new char[SERIAL_BUFFERLEN];

	mPort = "COM1";
	mParity = NOPARITY;
	mByteSize = 8;

	mStopBits = ONESTOPBIT;
	mBaudRate = 115200;
  mConnected = false;
}

SerialPort::~SerialPort() {
	if(mConnected)
		Disconnect();
	delete [] mBuffer;
}

char SerialPort::Connect() {
	if(mConnected) return 0; // Already connected..

  mPort = "\\\\.\\" + mPort;

	mHandle = CreateFile(mPort.c_str(),
                        GENERIC_READ | GENERIC_WRITE,
                        0,    /* comm devices must be opened w/exclusive-access */
                        NULL, /* no security attrs */
                        OPEN_EXISTING, /* comm devices must use OPEN_EXISTING */
                        0,    /* not overlapped I/O */
                        NULL  /* hTemplate must be NULL for comm devices */
                        );
	if(mHandle == INVALID_HANDLE_VALUE)
		return GetLastError();
	// Now get the DCB properties of the port we just opened
	if(!GetCommState(mHandle,&mDcb)) {
		CloseHandle(mHandle);
		return 2;
	}

	// Fill mDcb with requested settings
	mDcb.BaudRate  =  mBaudRate;
	mDcb.ByteSize  =  mByteSize;
	mDcb.Parity    =  mParity;
	mDcb.StopBits  =  mStopBits;
	mDcb.fRtsControl = RTS_CONTROL_HANDSHAKE;

	// Set the port with requested settings.
	if(!SetCommState(mHandle,&mDcb)) {
		CloseHandle(mHandle);
		return 3;
	}

	// Set the intial size of the transmit and receive queues.
	// Receive buffer to 32k, and the transmit buffer to 9k (just a default).
	if(!SetupComm(mHandle, 1024*32, 1024*9)) {
		CloseHandle(mHandle);
		return 4;
	}

	// Original
  mTimeOuts.ReadIntervalTimeout         = 15;
  mTimeOuts.ReadTotalTimeoutMultiplier  = 1;
  mTimeOuts.ReadTotalTimeoutConstant    = 250;
  mTimeOuts.WriteTotalTimeoutMultiplier = 1;
  mTimeOuts.WriteTotalTimeoutConstant   = 250;


	if(!SetCommTimeouts(mHandle, &mTimeOuts)) {
		CloseHandle(mHandle);
		return 5;
	}

	/* Flush the content of the port. */
	//FlushFileBuffers(mHandle);
	char buffer;
	unsigned long read;
	do	{
		ReadFile(mHandle, &buffer, 1, &read, NULL);
	}
	while(read != 0);

	// Connected!
	mConnected = true;
	return 0;
}

char SerialPort::Disconnect() {
	if(!mConnected)       // if already closed, return
		return 0;

	if(!CloseHandle(mHandle)) // non-zero on success
		return 1;
	mHandle = 0x0;


	mConnected = false;
	return 0;
}

char SerialPort::SendBuffer(const char *buffer, const unsigned short length) {
	if(!mConnected) return 1;

	if((length==0) || (buffer==NULL))
		return 2;
	unsigned long bogus;
	if(!WriteFile(mHandle, buffer, length, &bogus, NULL))
        return 3;

	return 0;
}

char SerialPort::ReceiveBuffer(char *buffer, const unsigned short length) {
	if(!mConnected) return 1;
	unsigned long bytesRead = 0, totalRead = 0;
	char counter = 0;

	while(totalRead < length && counter < 50) {
		if(!ReadFile(mHandle, buffer + totalRead, length - totalRead, &bytesRead, NULL) || !bytesRead)
			counter++;
		totalRead += bytesRead;
	}

	if(totalRead < length) {
		StatusLine("size mismatch");
		return 2;
	}
	return 0;
}

char SerialPort::Synchronize() {
	unsigned long bytesRead = 1;
	unsigned short count = 0;
	char buff[1];

	// Drain input buffer
	while(bytesRead && count < 100) {
		ReadFile(mHandle, buff, 1, &bytesRead, NULL);
		if(!bytesRead) count++;
	}

	// Flush output buffer
	PurgeComm(mHandle, PURGE_TXCLEAR | PURGE_RXCLEAR);
	return 0;
}
