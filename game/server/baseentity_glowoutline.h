#include <cbase.h>

//----------------------------------------
//Purpose: Glowing outline
//----------------------------------------

class CBaseEntityGlowOutline : public CBaseEntity{

	DECLARE_CLASS(CBaseEntityGlowOutline, CBaseEntity)
	DECLARE_DATADESC();
	DECLARE_SERVERCLASS();  // make this entity networkable

	int UpdateTransmitState()	// always send to all clients
	{
		return SetTransmitState(FL_EDICT_ALWAYS);
	}

	CBaseEntityGlowOutline();
	~CBaseEntityGlowOutline();

	void Event_Killed(const CTakeDamageInfo &info);
	void UpdateOnRemove(void);

	

protected:
	CNetworkVar(bool, m_bGlowEnabled);

public:

	void				AddGlowEffect(void);
	void				RemoveGlowEffect(void);
	bool				IsGlowEffectActive(void);

};


