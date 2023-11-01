//
// Foenix Debug Port Class
//

#include "foenix.h"

#include "log.h"


//-----------------------------------------------------------------------------

FoenixDebugPort::FoenixDebugPort()
	: BaudRate(6000000)
	, pSCC(nullptr)
{

}

FoenixDebugPort::~FoenixDebugPort()
{

}

//-----------------------------------------------------------------------------

void FoenixDebugPort::OpenSerialPort(const char* portName)
{
	LOG("Open Serial Port %s\n", portName);

	PortName = portName;

	sp_get_port_by_name(portName, &pSCC);

	if (pSCC)
	{
		if (SP_OK == sp_open(pSCC, SP_MODE_READ_WRITE))
		{
			sp_set_baudrate(pSCC, BaudRate);
			sp_set_bits(pSCC, 8);
			sp_set_parity(pSCC, SP_PARITY_NONE);
			sp_set_stopbits(pSCC, 1);
			sp_set_flowcontrol(pSCC, SP_FLOWCONTROL_RTSCTS);
			//sp_set_flowcontrol(pSCC, SP_FLOWCONTROL_NONE);


			Send(CMD_ENTER_DEBUG);


		}
	}
}

u16 FoenixDebugPort::Send(ECommand Command)
{
	u16 status = 0;
	u8 lrc = 0;

	u8 header[7];

	header[0] = REQUEST_SYNC_BYTE;
	header[1] = (u8) Command;
	header[2] = 0;  // big endian target address
	header[3] = 0;  // big endian target address
	header[4] = 0;  // big endian target address
	header[5] = 0;  // big endian payload size
	header[6] = 0;  // big endian payload size

	for (u8 data : header)
	{
		lrc = lrc ^ data;
	}

	return status;
}

