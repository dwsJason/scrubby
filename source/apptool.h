// ApplicationTool Class Header
//
// Base Class for Application tool windows in the Scrubby Application
// So we can have a list of "Tool Windows" that are open, render/interact with
// them
//
//
#ifndef APP_TOOL_H_
#define APP_TOOL_H_

#include "imgui.h"

#include <string>

class ApplicationTool
{
public:
	ApplicationTool::ApplicationTool(std::string windowName);

	virtual bool IsClosed()
	{
		return !m_bOpen;
	}

	virtual void Render()
	{
		// Override this method, to render your own stuff
		// just be sure to refer to the m_bOpen, so that the window can be
		// closed / deleted
		ImGui::Begin(m_windowName.c_str(),&m_bOpen, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse);


		ImGui::End();
	}

protected:

	std::string m_windowName;
	bool m_bOpen;
	int m_id;

static int m_unique_id;

};

#endif // APP_TOOL_H_

