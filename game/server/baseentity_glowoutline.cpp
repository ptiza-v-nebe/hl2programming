#include <cbase.h>
#include <baseentity_glowoutline.h>


CBaseEntityGlowOutline::CBaseEntityGlowOutline(){

	m_bGlowEnabled.Set(false);

}

CBaseEntityGlowOutline::~CBaseEntityGlowOutline(){

	RemoveGlowEffect();

}


//-----------------------------------------------------------------------------
// Purpose: Add glow to entity
//-----------------------------------------------------------------------------
void CBaseEntityGlowOutline::AddGlowEffect(void)
{
	SetTransmitState(FL_EDICT_ALWAYS);
	m_bGlowEnabled.Set(true);
}

//-----------------------------------------------------------------------------
// Purpose: Remove glow on entity
//-----------------------------------------------------------------------------
void CBaseEntityGlowOutline::RemoveGlowEffect(void)
{
	m_bGlowEnabled.Set(false);
}

//-----------------------------------------------------------------------------
// Purpose: Is the entity glowing?
//-----------------------------------------------------------------------------
bool CBaseEntityGlowOutline::IsGlowEffectActive(void)
{
	return m_bGlowEnabled;
}



// Character killed (only fired once)
void CBaseEntityGlowOutline::Event_Killed(const CTakeDamageInfo &info)
{

	RemoveGlowEffect();

}

void CBaseEntityGlowOutline::UpdateOnRemove(void)
{

	RemoveGlowEffect();

}


IMPLEMENT_SERVERCLASS_ST(CBaseEntityGlowOutline, DT_BaseEntityGlowOutline)
	SendPropBool(SENDINFO(m_bGlowEnabled)),
END_SEND_TABLE()


BEGIN_DATADESC(CBaseEntityGlowOutline)

	DEFINE_FIELD(m_bGlowEnabled, FIELD_BOOLEAN),
	DEFINE_KEYFIELD(m_bGlowEnabled, FIELD_BOOLEAN, "glow_enable"),

END_DATADESC()