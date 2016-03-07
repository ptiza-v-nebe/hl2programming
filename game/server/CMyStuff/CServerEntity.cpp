
#include <cbase.h>
#include "CServerEntity.h"



// Name of our entity's model
#define	ENTITY_MODEL	"models/combine_scanner.mdl"

	CServerEntity::CServerEntity(){};


	void CServerEntity::Precache(void){
		PrecacheModel(ENTITY_MODEL);
		BaseClass::Precache();
	}


	void CServerEntity::Spawn(void){
		Precache();

		SetModel(ENTITY_MODEL);
		SetSolid(SOLID_BBOX);
		UTIL_SetSize(this, -Vector(20, 20, 20), Vector(20, 20, 20));

		//SetMoveType(MOVETYPE_VPHYSICS);
		this->VPhysicsInitNormal(SOLID_VPHYSICS,0,false);

		//If you hit use on entity this method runes
		SetUse(&CServerEntity::UseFunc);

		//SetThink(&CServerEntity::Think);
		//SetNextThink(gpGlobals->curtime);
	}

	void CServerEntity::ProcessSlider(int sliderpos){
		
		m_SliderPos = sliderpos;
		
	}

	void CServerEntity::UseFunc(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value){
			

			static bool activated = false;

			activated = !activated;

			DevMsg("UseFunc activated on this entity, state of activated: %s\n", activated ? "true" : "false");
			
			if (activated){
				this->AddGlowEffect();
				// Start thinking
				SetThink(&CServerEntity::Think);
				SetNextThink(gpGlobals->curtime + 1.0f);

				return;
			}

			//unglow
			this->RemoveGlowEffect();
			// stop thinking
			SetThink(NULL);
	
			
			
	}

	int CServerEntity::ObjectCaps(void){
		return FCAP_IMPULSE_USE;
	}

	void CServerEntity::Think(void){
		
		//DevMsg("Now entity thinking... \n");
		DevMsg("CServerEntity::Think() m_SliderPos --> %d\n", m_SliderPos);
			
			SetNextThink(gpGlobals->curtime+1.0f);
	}


//Link a global entity name to this class (name used in Hammer etc.)
LINK_ENTITY_TO_CLASS(mynetworkentity, CServerEntity);

BEGIN_DATADESC(CServerEntity)
	DEFINE_FIELD(m_nMyInteger, FIELD_INTEGER),
	DEFINE_FIELD(m_fMyFloat, FIELD_FLOAT),
//	DEFINE_INPUTFUNC(FIELD_VOID, "glow", MakeGlow),
	DEFINE_USEFUNC(UseFunc),
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
	CBaseEntityGlowOutline *pEnt = (CBaseEntityGlowOutline*)CreateEntityByName("mynetworkentity");
	if (pEnt)
	{
		Vector vecOrigin = pPlayer->GetAbsOrigin() + vecForward * 256 + Vector(0, 0, 64);
		QAngle vecAngles(0, pPlayer->GetAbsAngles().y - 90, 0);
		pEnt->SetAbsOrigin(vecOrigin);
		pEnt->SetAbsAngles(vecAngles);
		DispatchSpawn(pEnt);
		
	}
}
