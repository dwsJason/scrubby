//
// Foenix Debug Port Class
//

#ifndef FOENIX_H_
#define FOENIX_H_

#include "libserialport.h"
#include "bctypes.h"
#include <string>


//DELAY_ERASE_SECTOR = 1          # Number of seconds to wait after issueing an ERASE_SECTOR command
//DELAY_PROGRAM_SECTOR = 2        # Number of seconds to wait after issueing an PROGRAM_SECTOR command

//BOOT_SRC_RAM = 0x00             # For F256jr Rev A -- boot from RAM 
//BOOT_SRC_FLASH = 0x01           # For F256jr Rev A -- boot from Flash 

//PGX_CPU_65816 = 0x01
//PGX_CPU_680X0 = 0x02
//PGX_CPU_65C02 = 0x03

//PGX_OFF_SIG_START = 0
//PGX_OFF_SIG_END = 3
//PGX_OFF_VERSION = 3
//PGX_OFF_ADDR_START = 4
//PGX_OFF_ADDR_END = 8
//PGX_OFF_DATA = 8

#define	REQUEST_SYNC_BYTE 0x55
#define	RESPONSE_SYNC_BYTE 0xAA

enum ECommand
{
	CMD_READ_MEM = 0x00,
	CMD_WRITE_MEM = 0x01,
	CMD_PROGRAM_FLASH = 0x10,
	CMD_ERASE_FLASH = 0x11,
	CMD_ERASE_SECTOR = 0x12,
	CMD_CPU_STOP   = 0x20,  // halt CPU using the RDY Line
	CMD_CPU_RESUME = 0x21,  // Resume CPU execution

	CMD_PROGRAM_SECTOR = 0x13,
	CMD_ENTER_DEBUG = 0x80,
	CMD_EXIT_DEBUG = 0x81,
	CMD_BOOT_RAM = 0x90,
	CMD_BOOT_FLASH = 0x91,
	CMD_REVISION = 0xFE
};


class FoenixDebugPort
{
public:
	FoenixDebugPort();
	~FoenixDebugPort();
	//-------------------------------------------------------------------------
	void OpenSerialPort(const char* portName);

	u16 Send(ECommand Command);


private:
	std::string PortName;

	int BaudRate;  // how fast is the port

	struct sp_port* pSCC;  // libserialport port interface

};

#endif // FOENIX_H_

