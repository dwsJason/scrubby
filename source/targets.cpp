//
// Target Manager Window for Scrubby
//

#include "targets.h"
#include "toolbar.h"

#include "foenix.h"

// Target Class ---------------------------------------------------------------

void Target::Render()
{



}


// TargetManager --------------------------------------------------------------

void TargetManager::Render()
{
	// Some Buttons
	Toolbar* toolBar = Toolbar::GToolbar;

	// Start of the Bar
	float xPos = 8.0f;
	ImGui::SameLine(xPos);

	// Add Target -------------------------------------------------------------
	if (toolBar->ImageButton(0,4))
	{
	}

	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
			ImGui::Text("Add Target");
		ImGui::EndTooltip();
	}



	// Default Target ---------------------------------------------------------
	ImGui::SameLine(xPos+=40.0f);

	if (toolBar->ImageButton(4,7))  //(6,6))
	{
	}

	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
			ImGui::Text("Set Default Target");
		ImGui::EndTooltip();
	}


	// Delete Target ----------------------------------------------------------
	ImGui::SameLine(xPos+=40.0f);

	if (toolBar->ImageButton(4,0))
	{
	}

	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
			ImGui::Text("Remove Target");
		ImGui::EndTooltip();
	}


	// Render the Targets

	for (int target_no = 0; target_no < m_targets.size(); ++target_no)
	{
		m_targets[target_no]->Render();
	}

}

//-----------------------------------------------------------------------------
