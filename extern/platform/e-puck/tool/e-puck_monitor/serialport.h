// THIS FILE IS INTENDED TO BE USED FOR THE EPFL BLIMP CONTEST
// ONLY. DO NOT DISTRIBUTE. DO NOT USE FOR OTHER PURPOSE.


#ifndef _SERIALPORT_H
#define _SERIALPORT_H

#include <windows.h>

#define SERIAL_BUFFERLEN 512


class SerialPort
{
public:

	SerialPort();
	virtual ~SerialPort();

	/* ---------------- CONNECTION ------------------ */

	virtual char Connect();
	virtual char Disconnect();

  /* ---------------- I/O ------------------ */

	virtual char SendBuffer(const char *buffer, const unsigned short length);
	virtual char ReceiveBuffer(char *buffer, const unsigned short length);

	/** Use this method to resync the CommLink when synchronization is lost. */
	virtual char Synchronize();

	/* ---------------- ACCESSORS ------------------ */

	bool GetConnected() { return mConnected; }

	void SetCommPort(AnsiString port) {mPort = port;}
	void SetBaudRate(unsigned int rate) {mBaudRate = rate;}


protected:
	AnsiString	mPort;
	unsigned int	mBaudRate;
	bool mConnected;

	char *mBuffer;

	char mParity, mStopBits, mByteSize;

	HANDLE mHandle;
	DCB	mDcb;
	COMMTIMEOUTS mTimeOuts;
};

#endif /* _SERIALPORT_H */
