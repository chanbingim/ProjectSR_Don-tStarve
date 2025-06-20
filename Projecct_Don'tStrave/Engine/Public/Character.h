#pragma once

#include "GameObject.h"

NS_BEGIN(Engine)

typedef struct LerpStat
{
	_bool				m_bLerpMove = {};
	_float				m_AccLerpTime = {};
}LERP_DATA;

class ENGINE_DLL CCharacter abstract : public CGameObject
{
private :
	CCharacter(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCharacter(const CCharacter& rhs);
	virtual ~CCharacter() = default;

public :
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Priority_Update(_float fTimeDelta);
	virtual void		Update(_float fTimeDelta);
	virtual void		Late_Update(_float fTimeDelta);
	virtual HRESULT		Render();

	_float3				LerpVec3(_float3& StartVec, _float3& EndVec, _float fDurTime, _float fTimeDelta);
	//_bool				LerpTurn(_float3& TrunAxis, _float StartAngle, _float EndAngle, _float DurTime);

protected :
	LERP_DATA			m_LerpData[2];


public :
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;

};
NS_END