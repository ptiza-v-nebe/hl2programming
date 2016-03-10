
#include <cbase.h>
#include "CServerEntity.h"



// Name of our entity's model
#define	ENTITY_MODEL	"models/combine_scanner.mdl"

	CServerEntity::CServerEntity(){
		entAngle.Init();
		bufAngle.Init();
	
	};


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
			

		    pPlayerWhoHitUseOnMe =(CBasePlayer*)pActivator;

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

		entAngle = pPlayerWhoHitUseOnMe->GetSliderPos();


		if (bufAngle.x != entAngle.x){
		
			//X Axis
			if ((entAngle.x - bufAngle.x) > 0)
				SetAbsAngles(QAngle((GetAbsAngles().x + (entAngle.x - bufAngle.x)), GetAbsAngles().y, GetAbsAngles().z));
			else
				SetAbsAngles(QAngle((GetAbsAngles().x - (bufAngle.x - entAngle.x) ), GetAbsAngles().y, GetAbsAngles().z));

			bufAngle.x = entAngle.x;
		}

		if (bufAngle.y != entAngle.y){
			//Y Axis
			if ((entAngle.y - bufAngle.y) > 0)
				SetAbsAngles(QAngle(GetAbsAngles().x, (GetAbsAngles().y + (entAngle.y - bufAngle.y)), GetAbsAngles().z));
			else
				SetAbsAngles(QAngle(GetAbsAngles().x, (GetAbsAngles().y - (bufAngle.y - entAngle.y)), GetAbsAngles().z));

			bufAngle.y = entAngle.y;
		}

		if (bufAngle.z != entAngle.z){
			//Z Axis
			if ((entAngle.z - bufAngle.z) > 0)
				SetAbsAngles(QAngle(GetAbsAngles().x, GetAbsAngles().y, (GetAbsAngles().z + (entAngle.z - bufAngle.z))));
			else
				SetAbsAngles(QAngle(GetAbsAngles().x, GetAbsAngles().y, (GetAbsAngles().z - (bufAngle.z - entAngle.z))));

			bufAngle.z = entAngle.z;
		}


		

			
		SetNextThink(gpGlobals->curtime + 0.1f);
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

		variant_t emptyVariant;
		pEnt->AcceptInput("Use", pPlayer, pPlayer,emptyVariant, USE_SET);
		
	}
}
