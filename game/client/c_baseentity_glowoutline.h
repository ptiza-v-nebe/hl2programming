#ifndef C_BASEENTITYGLOW_HEADER
#define C_BASEENTITYGLOW_HEADER

#include <cbase.h>
#include "glow_outline_effect.h"


class C_BaseEntityGlowOutline : public C_BaseEntity{
public:
	DECLARE_CLASS(C_BaseEntityGlowOutline, C_BaseEntity);
	DECLARE_DATADESC();
	DECLARE_CLIENTCLASS();

	C_BaseEntityGlowOutline(void);

	~C_BaseEntityGlowOutline();

	void Clear();

	void OnPreDataChanged(DataUpdateType_t type);
	void OnDataChanged(DataUpdateType_t type);

public:
	CGlowObject			*GetGlowObject(void){ return m_pGlowEffect; }
	virtual void		GetGlowEffectColor(float *r, float *g, float *b);
	//	void				EnableGlowEffect( float r, float g, float b );

	void				SetClientSideGlowEnabled(bool bEnabled){ m_bClientSideGlowEnabled = bEnabled; UpdateGlowEffect(); }
	bool				IsClientSideGlowEnabled(void){ return m_bClientSideGlowEnabled; }

protected:
	virtual void		UpdateGlowEffect(void);
	virtual void		DestroyGlowEffect(void);


private:
	bool				m_bClientSideGlowEnabled;	// client-side only value used for spectator
	bool				m_bGlowEnabled;				// networked value
	bool				m_bOldGlowEnabled;
	CGlowObject			*m_pGlowEffect;

};

#endif