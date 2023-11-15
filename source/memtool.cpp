//
// Memory Editor Tool for Scrubby
//

#include "memtool.h"
#include "targets.h"
#include "log.h"

// $$JGA TODO - Figure out how to share this with other tools
// we want the actual "updates" between the target and this buffer
// to happen in a different thread.  When windows have overlapping views
// we want to be smart, and only transfer the data that we need.
//
char MemoryTool::MEMORY_BUFFER[ DATA_SIZE ];

void MemoryTool::Render()
{
	#if 0
	static size_t DataPreviewAddr = 0;
	static size_t DataEditingAddr = 0;

	// What is the window start address, and size
	if (DataPreviewAddr != mem_edit.DataPreviewAddr)
	{
		DataPreviewAddr = mem_edit.DataPreviewAddr;
		LOG("DataPreviewAddr = $%llx\n", DataPreviewAddr);
	}

	if (DataEditingAddr != mem_edit.DataEditingAddr)
	{
		DataEditingAddr = mem_edit.DataEditingAddr;
		LOG("DataEditingAddr = $%llx\n", DataEditingAddr);
	}
	#endif

	bool bIsDirty = false;



	bool bIsConnected = false;
	Target* pTarget = TargetManager::GetInstance()->GetCurrentTarget();

	if (nullptr != pTarget)
	{
		bIsConnected = pTarget->IsConnected();
	}


	if (bIsConnected &&
		((LowAddress != m_Editor.LowAddress)||
		 (HighAddress != m_Editor.HighAddress)))
	{
		LowAddress = m_Editor.LowAddress;
		HighAddress = m_Editor.HighAddress;

		LOG("%lx --> %lx\n", LowAddress, HighAddress);

		bIsDirty = true;
	}


	static int rate = 60;
	rate-=6;
	if (rate <= 0)
	{
		rate = 60;
		bIsDirty = true;
	}

	if (bIsDirty)
	{

		if (bIsConnected)
		{
			// pull data from the Jr
			std::vector<u8> payload(HighAddress-LowAddress+1);

			pTarget->Send(CMD_CPU_STOP);
			pTarget->Send(CMD_READ_MEM, (u32)LowAddress, &payload);
			pTarget->Send(CMD_CPU_RESUME);

			memcpy(&MEMORY_BUFFER[LowAddress], payload.data(), payload.size());
		}

	}

	m_Editor.DrawWindow(m_windowName.c_str(), MEMORY_BUFFER, DATA_SIZE);
	m_bOpen = m_Editor.Open;
}


