
#include <vgui/IVGui.h>
#include <vgui_controls/Frame.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/Slider.h>


#include <vgui_controls/ImagePanel.h>
#include <vgui_controls/Label.h>
#include <vgui_controls\Panel.h>
#include <vgui\ISurface.h> 
#include "usermessages.h" //For user messages at keypad
#include "hud_macros.h" //Needed for networking messages
#include "hudelement.h" //Needed for networking messages
#include "iclientmode.h" //Needed for getting the viewport of the player
#include "engine/IEngineSound.h" //For sounds



//#include "baseplayer_shared.h" 
//#include "memdbgon.h" 



void PanelExperimental_Create(vgui::VPANEL parent);
void PanelExperimental_Destroy();
void PanelExperimental_Activate();




class CParentPanel : public vgui::Frame{

	DECLARE_CLASS_SIMPLE(CParentPanel, vgui::Frame);
	MESSAGE_FUNC_PARAMS(OnMyMessage, "SliderMoved", data);

	//DECLARE_CLIENTCLASS();


	CParentPanel();
	CParentPanel(vgui::VPANEL parent, char const *panelName);
	~CParentPanel(){};

public:
	Vector	posTable;


protected:

	virtual void OnTick();
	virtual void OnCommand(const char* pcCommand);
	void OnCursorMoved(int x, int y);
	void Paint();

private:
	vgui::Button *m_pButton;

	vgui::Slider *m_pSlider1;
	vgui::Slider *m_pSlider2;
	vgui::Slider *m_pSlider3;

	QAngle ent_angle;




};


static CParentPanel *g_pPanel;

