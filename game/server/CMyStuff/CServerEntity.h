#include <cbase.h>

class CServerEntity : public CBaseAnimating
{
public:
	DECLARE_CLASS(CServerEntity, CBaseAnimating);	// setup some macros
	DECLARE_DATADESC();
	DECLARE_SERVERCLASS();  // make this entity networkable

	int UpdateTransmitState()	// always send to all clients
	{
		return SetTransmitState(FL_EDICT_ALWAYS);
	}


	CServerEntity();


	void Precache(void);


	void Spawn(void);

	void ProcessSlider(int sliderpos);

	void UseFunc(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value);

	int ObjectCaps(void);

	void Think(void);

public:
	
	CNetworkVar(int, m_nMyInteger); // integer
	CNetworkVar(float, m_fMyFloat); // floating point

	int m_SliderPos;
};