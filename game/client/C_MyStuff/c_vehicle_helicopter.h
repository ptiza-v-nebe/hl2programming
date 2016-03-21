//Turning helicopter with mouse

// Below this many degrees, slow down turning rate linearly
#define FADE_TURN_DEGREES_HELI	45.0f
// After this, need to start turning feet
#define MAX_TORSO_ANGLE_HELI		90.0f
// Below this amount, don't play a turning animation/perform IK
#define MIN_TURN_ANGLE_REQUIRING_TURN_ANIMATION_HELI		15.0f
extern ConVar mp_ik_heli;

class C_QUA_helicopter;

class CHeliAnimState
{
public:
	enum
	{
		TURN_NONE = 0,
		TURN_LEFT,
		TURN_RIGHT
	};

	CHeliAnimState(C_QUA_helicopter *outer);

	Activity			BodyYawTranslateActivity(Activity activity);

	void				Update();

	const QAngle&		GetRenderAngles();

	void				GetPoseParameters(CStudioHdr *pStudioHdr, float poseParameter[MAXSTUDIOPOSEPARAM]);

	C_QUA_helicopter		*GetOuter();

private:
	void				GetOuterAbsVelocity(Vector& vel);

	int					ConvergeAngles(float goal, float maxrate, float dt, float& current);

	void				EstimateYaw(void);
	void				ComputePoseParam_BodyYaw(void);
	void				ComputePoseParam_BodyPitch(CStudioHdr *pStudioHdr);
	//void				ComputePoseParam_BodyLookYaw(void);

	void				ComputePlaybackRate();

	C_QUA_helicopter		*m_pOuter;

	float				m_flGaitYaw;
	float				m_flStoredCycle;

	// The following variables are used for tweaking the yaw of the upper body when standing still and
	//  making sure that it smoothly blends in and out once the player starts moving
	// Direction feet were facing when we stopped moving
	float				m_flGoalFeetYaw;
	float				m_flCurrentFeetYaw;

	float				m_flCurrentTorsoYaw;

	// To check if they are rotating in place
	float				m_flLastYaw;
	// Time when we stopped moving
	float				m_flLastTurnTime;

	// One of the above enums
	int					m_nTurningInPlace;

	QAngle				m_angRender;

	float				m_flTurnCorrectionTime;
};