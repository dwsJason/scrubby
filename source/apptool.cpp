// ApplicationTool Class Header
//
// Base Class for Application tool windows in the Scrubby Application
// So we can have a list of "Tool Windows" that are open, render/interact with
// them
//
//
#include "apptool.h"

// Unique ID's for IMGUI

/*static*/ int ApplicationTool::m_unique_id = 0;


ApplicationTool::ApplicationTool(std::string windowName)
{
	m_bOpen = true;
    m_id = m_unique_id++;

	char temp[256];

	sprintf(temp, "%d", m_id);

	// Make IMGUI Happy, differentiate between Windows with the same name
	m_windowName = windowName+"##"+temp;

}

