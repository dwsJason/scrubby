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

						// it's the first Target, just set to to be the default target right now
						if (1 == m_targets.size())
						{
							m_CurrentTarget = m_targets[0];
						}
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
		//SetDefaultTarget( m_targets[ m_RadioTargetNumber ] );
		m_CurrentTarget = m_targets[ m_RadioTargetNumber ];
	}

	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
			ImGui::Text("Set Default Target");
		ImGui::EndTooltip();
	}

	// Rename Target ----------------------------------------------------------
	ImGui::SameLine(xPos+=40.0f);

	if (toolBar->ImageButton(0,0))
	{
		// I envision injecting a text edit widget during the RadioButton
		// render loop, for this specific object, to edit it's m_UserName
		// (which will then automatically be displayed)
	}

	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
			ImGui::Text("Set Target Name");
		ImGui::EndTooltip();
	}



	// Delete Target ----------------------------------------------------------
	ImGui::SameLine(xPos+=40.0f);

	if (toolBar->ImageButton(4,0))
	{
		// Disconnect the target if it's connected, then delete the target
		// probably the target delete code could do the disconnect
		// to make this as simple as deleting the target

		// need to set the m_CurrentTarget to nullptr, if we are deleting
		// the current default target

	}

	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
			ImGui::Text("Remove Target");
		ImGui::EndTooltip();
	}


	// Render the Targets
	static ImVec4 regularColor = ImVec4(0.6f,0.6f,0.6f,1.0f);
	static ImVec4 defaultColor = ImVec4(1.0f,1.0f,0.15f,1.0f);

	for (int target_no = 0; target_no < m_targets.size(); ++target_no)
	{
		std::string description = m_targets[target_no]->GetDisplayName();

		//char temp[1024];
		//sprintf(temp, "%s##%d", description.c_str(), target_no);
		//ImGui::RadioButton(temp, &m_RadioTargetNumber, target_no);
		ImGui::PushID(target_no);

		const ImVec4& color = m_CurrentTarget == m_targets[ target_no ] ? defaultColor : regularColor;

		ImGui::PushStyleColor(ImGuiCol_Text, color);

		ImGui::RadioButton(description.c_str(), &m_RadioTargetNumber, target_no);

		ImGui::PopStyleColor();

		ImGui::PopID();

	}

}

//-----------------------------------------------------------------------------
