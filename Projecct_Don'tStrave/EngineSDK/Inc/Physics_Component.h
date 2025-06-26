#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class CTransform;
class CColiision_Component;

class ENGINE_DLL CPhysics_Component final : public CComponent
{
public :
	typedef struct	Physics_Desc
	{
		_float		fGravityVal = {};

	}PHYSICS_DESC;

private :
	CPhysics_Component() = default;
	CPhysics_Component(const CPhysics_Component& rhs);
	virtual ~CPhysics_Component() = default;

public :
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);

	void				Update(_float fTimeDeleta);
	void				SetGravity(_bool flag);

private :
	//충돌체를 만들어서 여기에 바인딩 옵션이 켜져있다면 충돌체를 통해서
	//충돌된 녀석을 가져와서 충돌 처리
	CTransform*				m_pTransform_Com = { nullptr };

	_bool					m_bGravity = { false };
	PHYSICS_DESC			m_PhysicsData;

public :
	static	CPhysics_Component*		Create();
	virtual CComponent*				Clone(void* pArg) override;
	virtual void					Free() override;

};
NS_END