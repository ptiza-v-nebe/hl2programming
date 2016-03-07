#include <cbase.h>
#include "baseentity_glowoutline.h"

// Name of our entity's model
#define	ENTITY_MODEL	"models/gibs/airboat_broken_engine.mdl"


class CServerEntity : public CBaseAnimating
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

	void Precache(void)
	{
		PrecacheModel(ENTITY_MODEL);

		BaseClass::Precache();
	}

	void Spawn(void){


		Precache();

		SetModel(ENTITY_MODEL);
		SetSolid(SOLID_BBOX);
		UTIL_SetSize(this, -Vector(20, 20, 20), Vector(20, 20, 20));

		//SetThink(&CServerEntity::Think);
		//SetNextThink(gpGlobals->curtime);
		
	}

	void MakeGlow(inputdata_t &inputData){

		this->AddGlowEffect();
	}

	void Think(void){
		
			
			//SetNextThink(gpGlobals->curtime+1.0f);
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
	DEFINE_INPUTFUNC(FIELD_VOID, "glow", MakeGlow),
END_DATADESC()

// Server data table describing networked member variables (SendProps)
// DO NOT create this in the header! Put it in the main CPP file.
IMPLEMENT_SERVERCLASS_ST(CServerEntity, DT_ServerEntity)
	SendPropInt(SENDINFO(m_nMyInteger), 16, SPROP_UNSIGNED),
	SendPropFloat(SENDINFO(m_fMyFloat), 0, SPROP_NOSCALE),
END_SEND_TABLE()


void TestMethod(const CCommand& args)
{
	DevMsg("how much args passed?%d\n", args.ArgC());
	DevMsg("what args passed? %s\n", args.GetCommandString());
	//DevMsg("run\n");
}
static ConCommand my_test("my_test", TestMethod, "test ", FCVAR_CHEAT);


CON_COMMAND(testentity, "Creates an instance of the sdk model entity in front of the player.")
{
	Vector vecForward;
	CBasePlayer *pPlayer = UTIL_GetCommandClient();
	if (!pPlayer)
	{
		Warning("Could not determine calling player!\n");
		return;
	}

	AngleVectors(pPlayer->EyeAngles(), &vecForward);
	CBaseEntity *pEnt = CreateEntityByName("mynetworkentity");
	if (pEnt)
	{
		Vector vecOrigin = pPlayer->GetAbsOrigin() + vecForward * 256 + Vector(0, 0, 64);
		QAngle vecAngles(0, pPlayer->GetAbsAngles().y - 90, 0);
		pEnt->SetAbsOrigin(vecOrigin);
		pEnt->SetAbsAngles(vecAngles);
		DispatchSpawn(pEnt);
	}
}