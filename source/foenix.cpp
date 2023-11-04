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

		LOG("Found Port %s\n", portName);

		if (SP_OK == sp_open(pSCC, SP_MODE_READ_WRITE))
		{

			LOG("Open Serial Port %s - SUCCESS\n", portName);

			sp_set_baudrate(pSCC, BaudRate);
			sp_set_bits(pSCC, 8);
			sp_set_parity(pSCC, SP_PARITY_NONE);
			sp_set_stopbits(pSCC, 1);
			//sp_set_flowcontrol(pSCC, SP_FLOWCONTROL_RTSCTS);
			sp_set_flowcontrol(pSCC, SP_FLOWCONTROL_NONE);


//			Send(CMD_ENTER_DEBUG);
			u16 response = Send(CMD_REVISION);
//			Send(CMD_EXIT_DEBUG);

			LOG("Revision = $%04X\n", response);

		}
		else
		{
			LOG("FAIL to Open %s\n", portName);
		}

	}
}

u16 FoenixDebugPort::Send(ECommand Command, u32 TargetAddress, std::vector<u8>* pPayLoad)
{
	u16 status = 0;  // default status to zero
	u8 lrc = 0; 	 // xor crc
	u32 payload_size = 0;
	u8* pPayLoadData = nullptr;

	if (pPayLoad)
	{
		payload_size = (u32)pPayLoad->size();
		pPayLoadData = (u8*)pPayLoad->data();

		// it's important that this is less than 1K
	}

	u8 header[7];

	header[0] = REQUEST_SYNC_BYTE;
	header[1] = (u8) Command;
	header[2] = (TargetAddress>>16)&0xFF;  // big endian target address
	header[3] = (TargetAddress>>8 )&0xFF;  // big endian target address
	header[4] = (TargetAddress>>0 )&0xFF;  // big endian target address
	header[5] = (payload_size>>8)&0xFF;  // big endian payload size
	header[6] = (payload_size>>0)&0xFF;  // big endian payload size

	for (u8 data : header)
	{
		lrc = lrc ^ data;
	}

	for (u32 index = 0; index < payload_size; ++index)
	{
		lrc = lrc ^ pPayLoadData[ index ];
	}

	int num_bytes = sp_blocking_write(pSCC, &header[0], 7, 50);

	if ((CMD_WRITE_MEM == Command) && payload_size)
	{
		num_bytes += sp_blocking_write(pSCC, pPayLoadData, payload_size, 50);
	}

	num_bytes += sp_blocking_write(pSCC, &lrc, 1, 50);

	//-----------------------------------------------------------------------

	sp_blocking_read(pSCC, &header, 3, 50);

	if ((CMD_READ_MEM == Command) && payload_size)
	{
		sp_blocking_read(pSCC, pPayLoadData, payload_size, 50);
	}

	// lrc
	sp_blocking_read(pSCC, &header[3], 1, 50);

	status = header[1]*256 + header[2];

	//LOG("%02x\n", header[0]);

	return status;
}

