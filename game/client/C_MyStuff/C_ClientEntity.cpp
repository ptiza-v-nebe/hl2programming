#include <cbase.h>

class C_ClientEntity : public C_BaseAnimating
{
public:
	DECLARE_CLASS(C_ClientEntity, C_BaseAnimating); // generic entity class macro
	DECLARE_DATADESC();
	DECLARE_CLIENTCLASS(); // this is a client representation of a server class 

	C_ClientEntity(){
	
	};

	void Spawn(void){

		//m_nMyInteger = 18833;
		//m_fMyFloat = 33.88f;


		SetThink(&C_ClientEntity::ClientThink);
		SetNextClientThink(gpGlobals->curtime);

		

	}

	void ClientThink(void){

		//DevMsg("Sending from Client: %d, %f, sizeof(int)=%d\n",m_nMyInteger,m_fMyFloat,sizeof(int));
		SetNextClientThink(gpGlobals->curtime + 1.0f);

	}

public:
	// networked variables as defined in server class
	int	m_nMyInteger;
	float	m_fMyFloat;
};

//Link a global entity name to this class (name used in Hammer etc.)
LINK_ENTITY_TO_CLASS(mynetworkentity, C_ClientEntity);

BEGIN_DATADESC(C_ClientEntity)
	DEFINE_FIELD(m_nMyInteger, FIELD_INTEGER),
	DEFINE_FIELD(m_fMyFloat, FIELD_FLOAT),
END_DATADESC()

// Link data table DT_MyEntity to client class and map variables (RecvProps)
// DO NOT create this in the header! Put it in the main CPP file.
IMPLEMENT_CLIENTCLASS_DT(C_ClientEntity, DT_ServerEntity, CServerEntity)
	RecvPropInt(RECVINFO(m_nMyInteger)),
	RecvPropFloat(RECVINFO(m_fMyFloat)),
END_RECV_TABLE()