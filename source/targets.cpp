//
// Target Manager Window for Scrubby
//

#include "targets.h"
#include "toolbar.h"

#include "foenix.h"

// Target Class ---------------------------------------------------------------

std::string Target::GetDisplayName()
{
	std::string result;

	if (m_UserName.empty())
	{
		result = m_PortName + " - " + m_PortDescription;

		if (3 == result.length())
		{
			result = "< unknown >";
		}
	}
	else
	{
		result = m_UserName + " on " + m_PortName;
	}

	return result;

}

// TargetManager --------------------------------------------------------------

TargetManager::TargetManager()
	: m_RadioTargetNumber(0)
	, m_CurrentTarget(nullptr)
{
}

// singleton thing
TargetManager* TargetManager::GetInstance()
{
	static TargetManager* TM = nullptr;

	// If there's no Target Manager
	if (nullptr == TM)
	{
		// Create One
		TM = new TargetManager();
	}

	return TM;
}


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
		m_targets.push_back(new Target());
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
		std::string description = m_targets[target_no]->GetDisplayName();

		char temp[1024];

		//std::string id = std::format("{}#{}", description, target_no);

		sprintf(temp, "%s##%d", description.c_str(), target_no);

		ImGui::RadioButton(temp, &m_RadioTargetNumber, target_no);

	}

}

//-----------------------------------------------------------------------------
