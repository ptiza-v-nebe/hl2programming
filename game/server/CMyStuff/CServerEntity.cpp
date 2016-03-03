#include <cbase.h>

class CServerEntity : public CBaseEntity
{
public:
	DECLARE_CLASS(CServerEntity, CBaseEntity);	// setup some macros
	DECLARE_DATADESC();
	DECLARE_SERVERCLASS();  // make this entity networkable

	int UpdateTransmitState()	// always send to all clients
	{
		return SetTransmitState(FL_EDICT_ALWAYS);
	}

	CServerEntity(){
	
	};

	void Spawn(void){

		SetThink(&CServerEntity::Think);
		SetNextThink(gpGlobals->curtime);

		m_nMyInteger = 18833;
		m_fMyFloat = 33.88f;

	}

	void Think(void){
	
			DevMsg("Sending from Server\n");
			SetNextThink(gpGlobals->curtime+1.0f);
		
	}

public:
	// public networked member variables:
	CNetworkVar(int, m_nMyInteger); // integer
	CNetworkVar(float, m_fMyFloat); // floating point
};

//Link a global entity name to this class (name used in Hammer etc.)
LINK_ENTITY_TO_CLASS(mynetworkentity, CServerEntity);

BEGIN_DATADESC(CServerEntity)
	DEFINE_FIELD(m_nMyInteger, FIELD_INTEGER),
	DEFINE_FIELD(m_fMyFloat, FIELD_FLOAT),
END_DATADESC()

// Server data table describing networked member variables (SendProps)
// DO NOT create this in the header! Put it in the main CPP file.
IMPLEMENT_SERVERCLASS_ST(CServerEntity, DT_ServerEntity)
	SendPropInt(SENDINFO(m_nMyInteger), 8, SPROP_UNSIGNED),
	SendPropFloat(SENDINFO(m_fMyFloat), 0, SPROP_NOSCALE),
END_SEND_TABLE()
