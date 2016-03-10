#include "cbase.h"
#include "c_EntityControlPanel.h"
#include "iinput.h"
//#include "../hl2/hl_in_main.cpp"
#include "input.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


//IMPLEMENT_CLIENTCLASS_DT(CParentPanel, DT_CPlayerPunc, CPlayerPunch)
//	RecvPropVector(RECVINFO(posTable)),
//END_RECV_TABLE()


ConVar cl_showmypanel("cl_showmypanel", "0", FCVAR_CLIENTDLL, "Sets the state of myPanel <state>");

//Create and Bind our Panel
void PanelExperimental_Create(vgui::VPANEL parent){
	g_pPanel = new CParentPanel(parent, "ParentPanel");

}

void PanelExperimental_Destroy(){
	g_pPanel->SetParent((vgui::Panel *)NULL);
	delete g_pPanel;
	
}

void PanelExperimental_Activate(){
	g_pPanel->Activate();
}


//Define Class
CParentPanel::CParentPanel(vgui::VPANEL parent, char const *panelName) : BaseClass(NULL, panelName){


	DevMsg("%s constructed\n", GetName());
	SetParent(parent);

	SetKeyBoardInputEnabled(true);
	SetMouseInputEnabled(true);

	SetProportional(false);
	SetTitleBarVisible(true);
	SetMinimizeButtonVisible(false);
	SetMaximizeButtonVisible(false);
	SetCloseButtonVisible(true);
	SetSizeable(false);
	SetMoveable(true);
	SetVisible(true);

	SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme"));
	LoadControlSettings("resource/UI/mypanel.res");
	vgui::ivgui()->AddTickSignal(GetVPanel(), 100);

	SetPos(ScreenWidth() / 2 - GetWide() / 2, ScreenHeight() / 2 - GetTall() / 2);

	m_pButton = new vgui::Button(this, "EXECPanelName", "EXEC", this, "exec");
	m_pButton->SetPos(100, 200);

	m_pSlider1 = new vgui::Slider(this, "SliderName1");
	m_pSlider1->SetPos(100, 50);
	m_pSlider1->SetRange(0, 360);

	m_pSlider2 = new vgui::Slider(this, "SliderName2");
	m_pSlider2->SetPos(100, 100);
	m_pSlider2->SetRange(0, 360);

	m_pSlider3 = new vgui::Slider(this, "SliderName3");
	m_pSlider3->SetPos(100, 150);
	m_pSlider3->SetRange(0, 360);

	m_pSlider1->AddActionSignalTarget(this);
	m_pSlider2->AddActionSignalTarget(this);
	m_pSlider3->AddActionSignalTarget(this);


	ent_angle.Init();


}




void CParentPanel::OnTick()
{
	BaseClass::OnTick();
	SetVisible(cl_showmypanel.GetBool());
}


void CParentPanel::OnCommand(const char* pcCommand)
{
	BaseClass::OnCommand(pcCommand);
	//DevMsg("actual command: %s\n", pcCommand);


	if (!Q_stricmp(pcCommand, "turnoff"))
		cl_showmypanel.SetValue(0);

	if (!Q_stricmp(pcCommand, "exec")){

		//engine->ClientCmd("test_command");

	}


}

//This is called if slider element dispatches PostAction()
void CParentPanel::OnMyMessage(KeyValues *data){


	if (!strcmpi(data->GetString("SliderName1"), "SliderName1"))
		ent_angle.x = data->GetInt("position");

	if (!strcmpi(data->GetString("SliderName2"), "SliderName2"))
		ent_angle.y = data->GetInt("position");

	if (!strcmpi(data->GetString("SliderName3"), "SliderName3"))
		ent_angle.z = data->GetInt("position");
		

	//DevMsg("x:%.2f,y:%.2f,z:%.2f\n", ent_angle.x, ent_angle.y, ent_angle.z);

	((CInput*)input)->SetSliderPos(ent_angle);

		
}


void CParentPanel::OnCursorMoved(int x, int y)
{
	//DevMsg("Cursor moved. New position: %d,%d\n", x, y);
}

CON_COMMAND(openmypanel, "Toggles testpanelfenix on or off")
{
	cl_showmypanel.SetValue(!cl_showmypanel.GetBool());
	PanelExperimental_Activate();

};



void CParentPanel::Paint(void)
{
	static float mtime;
	BaseClass::Paint();
	vgui::surface()->DrawSetColor(255, 0, 0, 255); //RGBA
	vgui::surface()->DrawFilledRect(30, 30, 60, 60); //x0,y0,x1,y1

}

