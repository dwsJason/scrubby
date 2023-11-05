//
// Memory Editor Tool for Scrubby
//

#include "memtool.h"
#include "targets.h"

// $$JGA TODO - Figure out how to share this with other tools
// we want the actual "updates" between the target and this buffer
// to happen in a different thread.  When windows have overlapping views
// we want to be smart, and only transfer the data that we need.
//
char MemoryTool::MEMORY_BUFFER[ DATA_SIZE ];

void MemoryTool::Render()
{
	//ImGui::Begin(m_windowName.c_str(),&m_bOpen, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse);
	//ImGui::End();

	// This ID is used, so that when multiple Windows have the same name
	// IMGUI can tell them apart
	//ImGui::PushID( m_id );
	m_Editor.DrawWindow(m_windowName.c_str(), MEMORY_BUFFER, DATA_SIZE);
	//ImGui::PopID();

	m_bOpen = m_Editor.Open;
}


