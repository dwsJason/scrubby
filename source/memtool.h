//
// Memory Editor Tool for Scrubby
//
#ifndef MEMTOOL_H_
#define MEMTOOL_H_

#include "apptool.h"
#include "memory_editor.h"

class MemoryTool : public ApplicationTool
{
public:
	MemoryTool::MemoryTool()
		: ApplicationTool("Memory Editor")
		, LowAddress(0)
		, HighAddress(0)
	{}

	virtual void Render() override;

private:

	size_t LowAddress;
	size_t HighAddress;

	MemoryEditor m_Editor;

#define DATA_SIZE (1024 * 1024)
	//$$JGA - by design, all the memory windows should share the same RAM
	//$$JGA - all Tools should share the same RAM, but work this out after
	//$$JGA - we have a second tool, like a disassembler
static char MEMORY_BUFFER[ DATA_SIZE ];

};

#endif // MEMTOOL_H_

