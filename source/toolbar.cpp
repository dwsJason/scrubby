//
// Toolbar - My Dear ImGUI Toolbar Window
//
#include "toolbar.h"

#include <SDL_image.h>
#include "log.h"

#include "sdl_helpers.h"


//------------------------------------------------------------------------------
Toolbar* Toolbar::GToolbar = nullptr;
//------------------------------------------------------------------------------


Toolbar::Toolbar()
	: m_GLImage(0)
	, m_currentMode(-1)
	, m_previousMode(-1)
{
	// So I think this should not be in a file, it should be linked in, maybe
	// an XPM image like the eye dropper?

	// Get Icons loaded into an OpenGL Texture
	SDL_Surface* pImage = IMG_Load(".\\data\\buttons.png");

	if (pImage)
	{
		m_GLImage = SDL_GL_LoadTexture(pImage, m_UV);
		SDL_FreeSurface(pImage);
	}
	else
	{
		LOG("ERR IMG_Load: %s\n", IMG_GetError());
		LOG("Toolbar Class missing buttons.png, unable to instantiate\n");
	}

	if (pImage)
	{
		// Go ahead and add the buttons we support to the toolbar

	}

	GToolbar = this;
}

Toolbar::~Toolbar()
{
	if (m_GLImage)
	{
		glDeleteTextures(1, &m_GLImage);
		m_GLImage = 0;
	}
}

//------------------------------------------------------------------------------
void Toolbar::SetCurrentMode(int toolBarMode)
{
	if (m_currentMode != toolBarMode)
	{
		m_previousMode = m_currentMode;
		m_currentMode = toolBarMode;
	}
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//
//  This works assuming that you have a window, that is catching these buttons
//  At least right now, this ought to be a toolbar window
//
void Toolbar::Render()
{
	// Don't bother, if we don't have the icons
	if (!m_GLImage) return;

static ImVec4 bg_color = ImVec4(0,0,0,0);
static ImVec4 tint_color = ImVec4(1,1,1,1);
static const	ImVec2 buttonSize = ImVec2(20*2,12*2);

static const char* helpStrings[] = 
{
	"App",
	"Connect to Target",
	"Load Executable",
	"Reload Executable",
	"Reset Target",
	"Refresh Rate"
};

static const int buttonXY[][2] =
{
	{0,18},  // app menu
	{0,17},  // connect, {2,17},  // disconnect
	{6,3},   // load executable
	{4,8},   // reload executable
	{0,19},  // reset
	{2,14},  // refresh rate

};

static int lastHovered = -1;

	// Current Button X Position
	float xPos = 4.0f;

	int wasHovered = lastHovered;
	lastHovered = -1;

	for (int idx = 0; idx < eToolBarMode_COUNT; ++idx)
	{
		ImGui::SameLine(xPos); xPos+=buttonSize.x;

		ImGui::PushID( idx );

		tint_color.w = wasHovered==idx ? 0.7f : 1.0f; // So it does something when you hover

		SetButtonImage(buttonXY[idx][0] + (idx==GetCurrentMode() ? 1 : 0)
					   ,buttonXY[idx][1]);

		if (ImGui::ImageButton((ImTextureID)(m_GLImage),
							   buttonSize, m_uv0, m_uv1, 0,
							   bg_color, tint_color))
		{
			SetCurrentMode( idx );
			ImGui::SetWindowFocus(m_focusWindow.c_str());
		}

		// I don't like this, but just make it work.
		if (eAppMenu == idx)
		{
			if (ImGui::BeginPopupContextItem(0,0))
			{
				if (ImGui::MenuItem("Debug Memory"))
				{
				}
				/*if*/ImGui::TextDisabled("Debug Code");
				/*if*/ImGui::TextDisabled("Console Log");
				/*if*/ImGui::TextDisabled("Manage Files");
				/*if*/ImGui::TextDisabled("Install Program");
				/*if*/ImGui::TextDisabled("Flash Firmware");

				ImGui::EndPopup();
			}
			else
			{
				// Unpop the button
				if (eAppMenu == m_currentMode)
				{
					SetCurrentMode(m_previousMode);
				}
			}
		}


		ImGui::PopID();

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
				ImGui::Text(helpStrings[ idx ]);
			ImGui::EndTooltip();

			lastHovered = idx;
		}
	}
}

//------------------------------------------------------------------------------
//
// Buttons on the sheet are broken up into rows, and columns
// set the uv0, and uv1 based on which button we want
//
// Buttons are 20 x 12 pixels $$JGA TODO, move this into constants
//
void Toolbar::SetButtonImage(int x, int y)
{
	m_uv0.x = x*20.0f/256.0f;
	m_uv1.x = m_uv0.x + 20.0f/256.0f;
	m_uv0.y = y*12.0f/256.0f;
	m_uv1.y = m_uv0.y + 12.0f/256.0f;
}

//------------------------------------------------------------------------------
void Toolbar::SetFocusWindow(const char* pFocusID)
{
	m_focusWindow = pFocusID;
}
//------------------------------------------------------------------------------
// Share our ImageButtons with the rest of the Application
// Right now, that might just be the timeline / scrubber
//
bool Toolbar::ImageButton(int col, int row, bool pressed)
{
static ImVec4 bg_color = ImVec4(0,0,0,0);
static ImVec4 tint_color = ImVec4(1,1,1,1);
static const	ImVec2 buttonSize = ImVec2(20*2,12*2);
static int lastHovered = -1;

	int wasHovered = lastHovered;
	bool bDoHover = ImGui::IsWindowHovered(ImGuiFocusedFlags_RootAndChildWindows);

	if (!bDoHover)
	{
		wasHovered = -1;
	}

	int id = (col * 100) + row; // I know this is going to cause some
								// render issues in windows that are not
								// focused

	ImGui::PushID( id );

	if (bDoHover && id == lastHovered)
	{
		lastHovered = -1;
	}

	tint_color.w = wasHovered==id ? 0.7f : 1.0f; // So it does something when you hover

	// Set the UV coordinates
	SetButtonImage(col + (pressed ? 1 : 0), row);


	bool result = ImGui::ImageButton((ImTextureID)(m_GLImage),
						   buttonSize, m_uv0, m_uv1, 0,
						   bg_color, tint_color);

	if (ImGui::IsItemHovered())
	{
		//ImGui::BeginTooltip();
		//	ImGui::Text(helpStrings[ idx ]);
		//ImGui::EndTooltip();

		lastHovered = id;
	}

	ImGui::PopID();

	return result;
}

//------------------------------------------------------------------------------

