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

	if (!m_UserName.empty())
	{
		result = m_UserName;
	}
	else if (!m_PortDescription.empty())
	{
		result = m_PortDescription;
	}
	else if (!m_PortName.empty())
	{
		result = m_PortName;
	}
	else
	{
		result = "< unknown >";
	}

	return result;
}

// TargetManager --------------------------------------------------------------

TargetManager::TargetManager()
	: m_RadioTargetNumber(0)
	, m_CurrentTarget(nullptr)
	, m_PortsCacheDirty(true)
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

	static int buttonNumber = -1;

	// Add Target -------------------------------------------------------------
	if (toolBar->ImageButton(0,4))
	{
		buttonNumber = 0;
	}

	if (0 == buttonNumber)
	{
		if (m_PortsCacheDirty)
		{
			m_PortsCacheDirty = false;

			m_DebugPort.ScanPorts(m_PortName, m_PortDesc);
		}

		if (m_PortName.empty())
		{
			if (ImGui::BeginPopupContextItem(0, 0))
			{
				ImGui::MenuItem("No valid ports detected");
				ImGui::EndPopup();
			}
		}
		else
		{
			if (ImGui::BeginPopupContextItem(0, 0))
			{
				for (int option_idx = 0; option_idx < m_PortName.size(); ++option_idx)
				{
					ImGui::PushID( option_idx );

					if (ImGui::MenuItem(m_PortDesc[option_idx].c_str()))
					{
						m_targets.push_back(new Target(m_PortName[option_idx], m_PortDesc[option_idx]));
					}

					ImGui::PopID();

				}

				ImGui::EndPopup();
			}
			else
			{
				buttonNumber = -1;
			}
		}

	}
	else
	{
		m_PortsCacheDirty = true;
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
