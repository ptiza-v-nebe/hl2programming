#include <cbase.h>
#include <c_baseentity_glowoutline.h>

C_BaseEntityGlowOutline::C_BaseEntityGlowOutline( void ){

	m_pGlowEffect = NULL;
	m_bGlowEnabled = false;
	m_bOldGlowEnabled = false;
	m_bClientSideGlowEnabled = false;

}

C_BaseEntityGlowOutline::~C_BaseEntityGlowOutline(){

}

void C_BaseEntityGlowOutline::Clear(void){

	DestroyGlowEffect();

}

void C_BaseEntityGlowOutline::OnPreDataChanged(DataUpdateType_t type){

	m_bOldGlowEnabled = m_bGlowEnabled;

}

void C_BaseEntityGlowOutline::OnDataChanged(DataUpdateType_t type){

	if (m_bOldGlowEnabled != m_bGlowEnabled)
	{
		UpdateGlowEffect();
	}


}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_BaseEntityGlowOutline::GetGlowEffectColor(float *r, float *g, float *b)
{
	*r = 0.76f;
	*g = 0.76f;
	*b = 0.76f;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
/*
void C_BaseCombatCharacter::EnableGlowEffect( float r, float g, float b )
{
// destroy the existing effect
if ( m_pGlowEffect )
{
DestroyGlowEffect();
}

m_pGlowEffect = new CGlowObject( this, Vector( r, g, b ), 1.0, true );
}
*/

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_BaseEntityGlowOutline::UpdateGlowEffect(void)
{
	// destroy the existing effect
	if (m_pGlowEffect)
	{
		DestroyGlowEffect();
	}

	// create a new effect
	if (m_bGlowEnabled || m_bClientSideGlowEnabled)
	{
		float r, g, b;
		GetGlowEffectColor(&r, &g, &b);

		m_pGlowEffect = new CGlowObject(this, Vector(r, g, b), 1.0, true);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_BaseEntityGlowOutline::DestroyGlowEffect(void)
{
	if (m_pGlowEffect)
	{
		delete m_pGlowEffect;
		m_pGlowEffect = NULL;
	}
}

BEGIN_DATADESC(C_BaseEntityGlowOutline)

	DEFINE_FIELD(m_bGlowEnabled, FIELD_BOOLEAN),
	DEFINE_KEYFIELD(m_bGlowEnabled, FIELD_BOOLEAN, "glow_enable"),

END_DATADESC()


IMPLEMENT_CLIENTCLASS_DT(C_BaseEntityGlowOutline, DT_BaseEntityGlowOutline, CBaseEntityGlowOutline)

	RecvPropBool(RECVINFO(m_bGlowEnabled)),

END_RECV_TABLE()