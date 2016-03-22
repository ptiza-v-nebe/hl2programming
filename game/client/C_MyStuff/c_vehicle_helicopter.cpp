#include "cbase.h"
#include "c_vehicle_helicopter.h"
//#include "c_ai_basenpc.h"
#include "c_te_particlesystem.h"
#include "fx.h"
#include "fx_sparks.h"
#include "c_tracer.h"
#include "clientsideeffects.h"
#include "iefx.h"
#include "dlight.h"
#include "bone_setup.h"
#include "c_rope.h"
#include "fx_line.h"
#include "c_sprite.h"
#include "view.h"
#include "view_scene.h"
#include "materialsystem/imaterialvar.h"
#include "simple_keys.h"
#include "fx_envelope.h"
#include "iclientvehicle.h"
#include "engine/ivdebugoverlay.h"
#include "particles_localspace.h"
#include "dlight.h"
#include "iefx.h"
#include "movevars_shared.h"
#include "view.h"
#include "flashlighteffect.h"
#include "c_baseplayer.h"
#include "c_te_effect_dispatch.h"
#include "tier0/vprof.h"
#include "ClientEffectPrecacheSystem.h"
#include <bitbuf.h>
#include "fx_water.h"

#include "c_prop_vehicle.h"
#include "hud.h"


// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


#define HELICOPTER_MSG_MACHINEGUN				1

extern ConVar default_fov;

class CHeliAnimState;

//=============================

//Clientside helicopter

//=============================

class C_QUA_helicopter : public C_BaseAnimating, public IClientVehicle
{
	DECLARE_CLASS( C_QUA_helicopter, C_BaseAnimating );
public:
	DECLARE_CLIENTCLASS();
	DECLARE_INTERPOLATION();
	DECLARE_DATADESC();

	C_QUA_helicopter();
	~C_QUA_helicopter();
	void MakeTracer( const Vector &vecTracerSrc, const trace_t &tr, int iTracerType );
	virtual void ReceiveMessage( int classID, bf_read &msg );

	void Simulate();

	
public:

	//virtual const QAngle& GetRenderAngles() { return m_HeliAnimState.GetRenderAngles(); }
	//virtual const QAngle& GetRenderAngles() {return GetLocalAngles(); }

	float m_Time;
	QAngle testangle;/*

	virtual const QAngle& GetRenderAngles() {

		
		if (gpGlobals->curtime > m_Time){
			
			m_Time = gpGlobals->curtime + 0.5f;

			return testangle + QAngle(10,0,0);
		}
		return testangle;
	}*/

	virtual void GetVehicleFOV( float &flFOV ) { flFOV = 0.0f; }
	virtual void DrawHudElements();
	virtual bool IsPassengerUsingStandardWeapons( int nRole = VEHICLE_ROLE_DRIVER ) { return false; }
	virtual void UpdateViewAngles( C_BasePlayer *pLocalPlayer, CUserCmd *pCmd ) {}
	virtual C_BasePlayer* GetPassenger( int nRole );
	virtual int	GetPassengerRole(CBaseCombatCharacter *pPassenger);
	virtual void GetVehicleClipPlanes( float &flZNear, float &flZFar ) const;
	virtual int GetPrimaryAmmoType() const { return -1; }
	virtual int GetPrimaryAmmoCount() const { return m_iAmmoCount; }
	virtual int GetPrimaryAmmoClip() const  { return m_iCannonCount; }
	virtual bool PrimaryAmmoUsesClips() const { return false; }
	// IClientVehicle overrides.
	void GetVehicleViewPosition(int nRole, Vector *pOrigin, QAngle *pAngles, float *pFOV);
	void RestrictView( float *pYawBounds, float *pPitchBounds, float *pRollBounds, QAngle &vecViewAngles );
	virtual int		GetHealth() const { return m_iHealth; };
	virtual IClientVehicle*	GetClientVehicle() { return this; }
	virtual C_BaseEntity	*GetVehicleEnt() { return this; }
	virtual void SetupMove( C_BasePlayer *player, CUserCmd *ucmd, IMoveHelper *pHelper, CMoveData *move ) {}
	virtual void ProcessMovement( C_BasePlayer *pPlayer, CMoveData *pMoveData ) {}
	virtual void FinishMove( C_BasePlayer *player, CUserCmd *ucmd, CMoveData *move ) {}
	virtual bool IsPredicted() const { return false; }
	virtual void ItemPostFrame( C_BasePlayer *pPlayer ) {}
	int GetJoystickResponseCurve() const { return 0;  }

	CHandle<C_BasePlayer>	m_hPlayer;
	CHandle<C_BasePlayer>	m_hPrevPlayer;

	bool					m_bEnterAnimOn;
	bool					m_bExitAnimOn;
	Vector					m_vecEyeExitEndpoint;
	int						m_iAmmoCount;
	int						m_iCannonCount;
	bool					m_bMagnetOn;

	Vector					m_vecOldShadowDir;

	ViewSmoothingData_t			m_ViewSmoothingData;

	Vector						m_vecGunCrosshair;
	CInterpolatedVar<Vector>	m_iv_vecGunCrosshair;

private:
	CHeliAnimState m_HeliAnimState;
};
IMPLEMENT_CLIENTCLASS_DT(C_QUA_helicopter, DT_QUA_helicopter, QUA_helicopter)
	RecvPropEHandle( RECVINFO(m_hPlayer) ),
	RecvPropBool( RECVINFO( m_bEnterAnimOn ) ),
	RecvPropBool( RECVINFO( m_bExitAnimOn ) ),
	RecvPropVector( RECVINFO( m_vecEyeExitEndpoint ) ),
	RecvPropVector( RECVINFO( m_vecGunCrosshair ) ),
	RecvPropInt		(RECVINFO(m_iHealth)),
	RecvPropInt		(RECVINFO(m_iAmmoCount)),
	RecvPropInt		(RECVINFO(m_iCannonCount)),
END_RECV_TABLE()

BEGIN_DATADESC( C_QUA_helicopter )
	DEFINE_EMBEDDED( m_ViewSmoothingData ),
END_DATADESC()

C_QUA_helicopter::C_QUA_helicopter() : m_HeliAnimState(this)
{
	memset( &m_ViewSmoothingData, 0, sizeof( m_ViewSmoothingData ) );
	m_ViewSmoothingData.pVehicle = this;
	m_Time = 0.0f;
	testangle.Init();
}

C_QUA_helicopter::~C_QUA_helicopter()
{

}
C_BasePlayer* C_QUA_helicopter::GetPassenger( int nRole )
{
	if (nRole == VEHICLE_ROLE_DRIVER)
		return m_hPlayer.Get();
	return NULL;
}

void C_QUA_helicopter::GetVehicleClipPlanes( float &flZNear, float &flZFar ) const
{
	// FIXME: Need something a better long-term, this fixes the buggy.
	flZNear = 6;
}

//int	C_QUA_helicopter::GetPassengerRole( C_BasePlayer *pEnt )
int C_QUA_helicopter::GetPassengerRole(CBaseCombatCharacter *pPassenger)
{
	if (m_hPlayer.Get() == pPassenger)
	{
		return VEHICLE_ROLE_DRIVER;
	}
	return -1;
}

void C_QUA_helicopter::DrawHudElements( )
{
	CHudTexture *pIcon;
	//int iIconX, iIconY;


		// draw crosshairs for vehicle gun
		pIcon = gHUD.GetIcon( "gunhair" );

		if ( pIcon != NULL )
		{
			float x, y;
			Vector screen;

			x = ScreenWidth()/2;
			y = ScreenHeight()/2;

		
			ScreenTransform( m_vecGunCrosshair, screen );
			x += 0.5 * screen[0] * ScreenWidth() + 0.5;
			y -= 0.5 * screen[1] * ScreenHeight() + 0.5;

			x -= pIcon->Width() / 2; 
			y -= pIcon->Height() / 2; 
			bool unable;
			unable=false;
			Color	clr = ( unable ) ? gHUD.m_clrCaution : gHUD.m_clrNormal;
			pIcon->DrawSelf( x, y, clr );
		}

	// Aqui pondremos el crosshair
}

void C_QUA_helicopter::ReceiveMessage( int classID, bf_read &msg )
{
	if ( classID != GetClientClass()->m_ClassID )
	{
		// message is for subclass
		BaseClass::ReceiveMessage( classID, msg );
		return;
	}

	int messageType = msg.ReadByte();
	switch( messageType )
	{
	case HELICOPTER_MSG_MACHINEGUN:
		{
			// Necesario en MP, porque si no no se ven las balas.
			Vector muz,dir,spr;
			msg.ReadBitVec3Coord( muz );
			msg.ReadBitVec3Coord( dir );
			msg.ReadBitVec3Coord( spr );
			int ammo = msg.ReadByte();
			//Warning("x: %f,x: %f,x: %f,ammo: %i",muz.x,dir.x,spr.x,ammo);
			FireBulletsInfo_t info;
			info.m_iShots = 1;
			Vector vectorspr;
			QAngle no;
			this->GetAttachment(LookupAttachment("Muzzle"),vectorspr,no);
			info.m_vecSrc = vectorspr;
			info.m_vecDirShooting = dir;
			info.m_vecSpread = spr;
			info.m_pAttacker =	(C_BaseEntity *) m_hPlayer;
			info.m_flDistance = MAX_TRACE_LENGTH;
			info.m_iAmmoType =  ammo;
			info.m_iTracerFreq = 1;
			FireBullets(info);
			CEffectData data;
			//data.m_nEntIndex = entindex();
			data.m_nAttachmentIndex = entindex();
			data.m_nAttachmentIndex = LookupAttachment( "muzzle" );
			//data.m_flScale = 1.0f;
			//DispatchEffect( "QUA_APCMuzzleFlash", data );
		/*	CEffectData data;
			data.m_nAttachmentIndex = LookupAttachment( "MiniGun" );
			data.m_nEntIndex = entindex();
			DispatchEffect("QUA_StriderMuzzleFlash",data );*/

		}
	}
}


//void C_QUA_helicopter::GetVehicleViewPosition( int nRole, Vector *pAbsOrigin, QAngle *pAbsAngles )

void C_QUA_helicopter::GetVehicleViewPosition(int nRole, Vector *pOrigin, QAngle *pAngles, float *pFOV)
{
	//VehicleViewSmoothingHLC( m_hPlayer, pAbsOrigin, pAbsAngles, m_bEnterAnimOn, m_bExitAnimOn, &m_vecEyeExitEndpoint, &m_ViewSmoothingData, NULL );
	SharedVehicleViewSmoothing(m_hPlayer, pOrigin, pAngles, m_bEnterAnimOn, m_bExitAnimOn, m_vecEyeExitEndpoint, &m_ViewSmoothingData, NULL);

}
void C_QUA_helicopter::RestrictView( float *pYawBounds, float *pPitchBounds,
										   float *pRollBounds, QAngle &vecViewAngles )
{
	int eyeAttachmentIndex = LookupAttachment( "Bomb" );
	
	Vector vehicleEyeOrigin;
	QAngle vehicleEyeAngles;
	QAngle no;
	GetAttachmentLocal( eyeAttachmentIndex, vehicleEyeOrigin, no );
	Vector up,forward,right;
	this->GetVectors(&forward,&right,&up);
	vehicleEyeOrigin+=(forward*218)+(up*-10);
	vehicleEyeAngles=this->GetAbsAngles();
	// Limit the yaw.
	if ( pYawBounds )
	{
		float flAngleDiff = AngleDiff( vecViewAngles.y, vehicleEyeAngles.y );
		flAngleDiff = clamp( flAngleDiff, pYawBounds[0], pYawBounds[1] );
		vecViewAngles.y = vehicleEyeAngles.y + flAngleDiff;
	}

	// Limit the pitch.
	/*if ( pPitchBounds )
	{
		float flAngleDiff = AngleDiff( vecViewAngles.x, vehicleEyeAngles.x );
		flAngleDiff = clamp( flAngleDiff, pPitchBounds[0], pPitchBounds[1] );
		vecViewAngles.x = vehicleEyeAngles.x + flAngleDiff;
	}*/

	// Limit the roll.
	if ( pRollBounds )
	{
		float flAngleDiff = AngleDiff( vecViewAngles.z, vehicleEyeAngles.z );
		flAngleDiff = clamp( flAngleDiff, pRollBounds[0], pRollBounds[1] );
		vecViewAngles.z = vehicleEyeAngles.z + flAngleDiff;
	}
}

void C_QUA_helicopter::MakeTracer( const Vector &vecTracerSrc, const trace_t &tr, int iTracerType )
{
	float flTracerDist;
	Vector vecDir;
	Vector vecEndPos;

	vecDir = tr.endpos - vecTracerSrc;
	
	flTracerDist = VectorNormalize( vecDir );

	UTIL_Tracer( vecTracerSrc, tr.endpos, 0, TRACER_DONT_USE_ATTACHMENT, 5000, true, "StriderTracer" );
}

void C_QUA_helicopter::Simulate(){

	//m_HeliAnimState.Update();
	//DevMsg("Simulate() heli\n");
}

//=============================

//Turning helicopter with mouse

//=============================

CHeliAnimState::CHeliAnimState(C_QUA_helicopter *outer)
	: m_pOuter(outer)
{
	m_flGaitYaw = 0.0f;
	m_flGoalFeetYaw = 0.0f;
	m_flCurrentFeetYaw = 0.0f;
	m_flCurrentTorsoYaw = 0.0f;
	m_flLastYaw = 0.0f;
	m_flLastTurnTime = 0.0f;
	m_flTurnCorrectionTime = 0.0f;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHeliAnimState::Update()
{
	m_angRender = GetOuter()->GetLocalAngles();
	m_angRender[PITCH] = m_angRender[ROLL] = 0.0f;

//	ComputePoseParam_BodyYaw();
//	ComputePoseParam_BodyPitch(GetOuter()->GetModelPtr());
//	ComputePoseParam_BodyLookYaw(); //Body Yaw

//	ComputePlaybackRate();
	/*
#ifdef CLIENT_DLL
	GetOuter()->UpdateLookAt();
#endif*/

}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHeliAnimState::ComputePlaybackRate()
{
	// Determine ideal playback rate
	Vector vel;
	GetOuterAbsVelocity(vel);

	float speed = vel.Length2D();

	bool isMoving = (speed > 0.5f) ? true : false;

	float maxspeed = GetOuter()->GetSequenceGroundSpeed(GetOuter()->GetSequence());

	if (isMoving && (maxspeed > 0.0f))
	{
		float flFactor = 1.0f;

		// Note this gets set back to 1.0 if sequence changes due to ResetSequenceInfo below
		GetOuter()->SetPlaybackRate((speed * flFactor) / maxspeed);

		// BUG BUG:
		// This stuff really should be m_flPlaybackRate = speed / m_flGroundSpeed
	}
	else
	{
		GetOuter()->SetPlaybackRate(1.0f);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : CBasePlayer
//-----------------------------------------------------------------------------
C_QUA_helicopter *CHeliAnimState::GetOuter()
{
	return m_pOuter;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : dt - 
//-----------------------------------------------------------------------------
void CHeliAnimState::EstimateYaw(void)
{
	float dt = gpGlobals->frametime;

	if (!dt)
	{
		return;
	}

	Vector est_velocity;
	QAngle	angles;

	GetOuterAbsVelocity(est_velocity);

	angles = GetOuter()->GetLocalAngles();

	if (est_velocity[1] == 0 && est_velocity[0] == 0)
	{
		float flYawDiff = angles[YAW] - m_flGaitYaw;
		flYawDiff = flYawDiff - (int)(flYawDiff / 360) * 360;
		if (flYawDiff > 180)
			flYawDiff -= 360;
		if (flYawDiff < -180)
			flYawDiff += 360;

		if (dt < 0.25)
			flYawDiff *= dt * 4;
		else
			flYawDiff *= dt;

		m_flGaitYaw += flYawDiff;
		m_flGaitYaw = m_flGaitYaw - (int)(m_flGaitYaw / 360) * 360;
	}
	else
	{
		m_flGaitYaw = (atan2(est_velocity[1], est_velocity[0]) * 180 / M_PI);

		if (m_flGaitYaw > 180)
			m_flGaitYaw = 180;
		else if (m_flGaitYaw < -180)
			m_flGaitYaw = -180;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Override for backpeddling
// Input  : dt - 
//-----------------------------------------------------------------------------
void CHeliAnimState::ComputePoseParam_BodyYaw(void)
{
	int iYaw = GetOuter()->LookupPoseParameter("move_yaw");
	if (iYaw < 0)
		return;

	// view direction relative to movement
	float flYaw;

	EstimateYaw();

	QAngle	angles = GetOuter()->GetLocalAngles();
	float ang = angles[YAW];
	if (ang > 180.0f)
	{
		ang -= 360.0f;
	}
	else if (ang < -180.0f)
	{
		ang += 360.0f;
	}

	// calc side to side turning
	flYaw = ang - m_flGaitYaw;
	// Invert for mapping into 8way blend
	flYaw = -flYaw;
	flYaw = flYaw - (int)(flYaw / 360) * 360;

	if (flYaw < -180)
	{
		flYaw = flYaw + 360;
	}
	else if (flYaw > 180)
	{
		flYaw = flYaw - 360;
	}

	GetOuter()->SetPoseParameter(iYaw, flYaw);
	/*static float m_Time;
	static int i;
	if (gpGlobals->curtime > m_Time){
	GetOuter()->SetPoseParameter(iYaw, i*10);
	m_Time = gpGlobals->curtime + 0.5f;
	i++;
	}*/

#ifndef CLIENT_DLL
	//Adrian: Make the model's angle match the legs so the hitboxes match on both sides.
	GetOuter()->SetLocalAngles(QAngle(GetOuter()->GetAnimEyeAngles().x, m_flCurrentFeetYaw, 0));
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHeliAnimState::ComputePoseParam_BodyPitch(CStudioHdr *pStudioHdr)
{
	// Get pitch from v_angle
	float flPitch = GetOuter()->GetLocalAngles()[PITCH];

	if (flPitch > 180.0f)
	{
		flPitch -= 360.0f;
	}
	flPitch = clamp(flPitch, -90, 90);

	QAngle absangles = GetOuter()->GetAbsAngles();
	absangles.x = 0.0f;
	m_angRender = absangles;
	m_angRender[PITCH] = m_angRender[ROLL] = 0.0f;

	// See if we have a blender for pitch
	GetOuter()->SetPoseParameter(pStudioHdr, "aim_pitch", flPitch);
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : goal - 
//			maxrate - 
//			dt - 
//			current - 
// Output : int
//-----------------------------------------------------------------------------
int CHeliAnimState::ConvergeAngles(float goal, float maxrate, float dt, float& current)
{
	int direction = TURN_NONE;

	float anglediff = goal - current;
	float anglediffabs = fabs(anglediff);

	anglediff = AngleNormalize(anglediff);

	float scale = 1.0f;
	if (anglediffabs <= FADE_TURN_DEGREES_HELI)
	{
		scale = anglediffabs / FADE_TURN_DEGREES_HELI;
		// Always do at least a bit of the turn ( 1% )
		scale = clamp(scale, 0.01f, 1.0f);
	}

	float maxmove = maxrate * dt * scale;

	if (fabs(anglediff) < maxmove)
	{
		current = goal;
	}
	else
	{
		if (anglediff > 0)
		{
			current += maxmove;
			direction = TURN_LEFT;
		}
		else
		{
			current -= maxmove;
			direction = TURN_RIGHT;
		}
	}

	current = AngleNormalize(current);

	return direction;
}




//-----------------------------------------------------------------------------
// Purpose: 
// Input  : activity - 
// Output : Activity
//-----------------------------------------------------------------------------
Activity CHeliAnimState::BodyYawTranslateActivity(Activity activity)
{
	// Not even standing still, sigh
	if (activity != ACT_IDLE)
		return activity;

	// Not turning
	switch (m_nTurningInPlace)
	{
	default:
	case TURN_NONE:
		return activity;

		//	case TURN_RIGHT:
		//	return ACT_TURNRIGHT45;
		//	case TURN_LEFT:
		//	return ACT_TURNLEFT45;

	case TURN_RIGHT:
	case TURN_LEFT:
		return mp_ik_heli.GetBool() ? ACT_TURN : activity;
	}

	Assert(0);

	return activity;
}

const QAngle& CHeliAnimState::GetRenderAngles()
{
	return m_angRender;
}


void CHeliAnimState::GetOuterAbsVelocity(Vector& vel)
{
#if defined( CLIENT_DLL )
	GetOuter()->EstimateAbsVelocity(vel);
#else
	vel = GetOuter()->GetAbsVelocity();
#endif
}
