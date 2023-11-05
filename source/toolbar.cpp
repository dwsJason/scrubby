//
// Toolbar - My Dear ImGUI Toolbar Window
//
#include "toolbar.h"

#include <SDL_image.h>
#include "log.h"

#include "sdl_helpers.h"

// Target Manager
#include "targets.h"


//------------------------------------------------------------------------------
Toolbar* Toolbar::GToolbar = nullptr;
//------------------------------------------------------------------------------


Toolbar::Toolbar()
	: m_GLImage(0)
	, m_currentMode(-1)
	, m_previousMode(-1)
	, m_pollingRate(0)
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

static char str_connect[]    = "Connect to Target";
static char str_disconnect[] = "Disconnect from Target";

static char* helpStrings[] = 
{
	"App",
	str_connect,
	"Load Executable",
	"Reload Executable",
	"Reset Target",
	"Refresh Rate"
};

static int buttonXY[][2] =
{
	{0,18},  // app menu
	{0,17},  // connect, {2,17},  // disconnect
	{6,3},   // load executable
	{4,8},   // reload executable
	{0,19},  // reset
	{2,14},  // refresh rate

};

static const char* refreshStrings[] =
{
	"Lazy",
	"2500ms",
	"1000ms",
	" 500ms",
	" 250ms",
	" 100ms",
	"16.6ms",
	"14.3ms"
};

static const float refreshRates[] =
{
	0.0f,
	2.5f,
	1.0f,
	0.5f,
	0.25f,
	0.1f,
	1000.0f/60.0f,
	1000.0f/70.0f
};

// Special Sauce for the connect/disconnect button
//-----------------------------------------------------------------------------
	bool bIsConnected = false;
	Target* pTarget = TargetManager::GetInstance()->GetCurrentTarget();

	if (nullptr != pTarget)
	{
		bIsConnected = pTarget->IsConnected();
	}

	if (bIsConnected)
	{
		helpStrings[eConnect] = str_disconnect; // Hover text
		buttonXY[eConnect][0] = 2;				// Icon we show people
		buttonXY[eConnect][1] = 17;
	}
	else
	{
		helpStrings[ eConnect ] = str_connect;  // Hover text
		buttonXY[eConnect][0] = 0;				// Icon we show people
		buttonXY[eConnect][1] = 17;
	}
//-----------------------------------------------------------------------------


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

		// IMGUI is pretty dumb, but this also makes it pretty easy
		if (eRefreshRate == idx)
		{

			if (ImGui::BeginPopupContextItem(0,0))
			{
				for (int popidx = 0; popidx < 8;++popidx)
				{
					if (ImGui::MenuItem(refreshStrings[popidx]))
					{
						m_pollingRate = popidx;
					}
				}

				ImGui::EndPopup();
			}
			else
			{
				// Unpop the button
				if (eRefreshRate == m_currentMode)
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

	// Refresh Rate
	ImGui::SameLine(0,10);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.36f, 0.416f, 0.76f, 1.00f));
	ImGui::Text(refreshStrings[m_pollingRate]);
	ImGui::PopStyleColor();

	// Current Target
	ImGui::SameLine(0,10);
	// Red for no connection
	static ImVec4 redColor   = ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
	static ImVec4 greenColor = ImVec4(0.2f, 1.0f, 0.2f, 1.0f);

	ImGui::PushStyleColor(ImGuiCol_Text, bIsConnected ? greenColor : redColor);

	if (nullptr != pTarget)
	{
		ImGui::Text(pTarget->GetDisplayName().c_str());
	}
	else
	{
		ImGui::Text("<No Target>");
	}

	ImGui::PopStyleColor();


	if (eConnect == GetCurrentMode())
	{
		// Attempt Connect or disconnect
		if (bIsConnected)
		{
			// Let's disconnect
			if (nullptr != pTarget)
			{
				pTarget->Disconnect();
			}
		}
		else
		{
			if (nullptr != pTarget)
			{
				pTarget->Connect();
			}
			else
			{
				LOG("NO DEFAULT TARGET, cannot connect.\n");
				LOG("Please ADD a TARGET.\n");
			}

		}

		SetCurrentMode(m_previousMode);
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

