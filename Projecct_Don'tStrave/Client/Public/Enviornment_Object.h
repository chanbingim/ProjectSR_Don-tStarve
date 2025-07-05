#pragma once

#include "AinimationObject.h"

NS_BEGIN(Engine)
class CCollision_Component;
NS_END

NS_BEGIN(Client)
class CEnviornment_Object : public CAinimationObject
{
public:
	enum class Enviornment_STATE { IDLE, RECOVERY, DAMAGED, BROKEN, BROKEN_IDLE, END };

protected :
	CEnviornment_Object(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEnviornment_Object(const CEnviornment_Object& rhs);
	virtual ~CEnviornment_Object() = default;

public :
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual void			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual	void			Reset_State();
	virtual HRESULT			Render() override;
	
	virtual void			Damage(void* pArg) override;
	virtual void			Death() override;

	_wstring				GetEnviornmnetName();
	_uint					GetEnviormentID();

	BASE_DESC				GetCurrentInfo() { return m_EnviormentInfo; }
	Enviornment_STATE		GetState() { return m_EnviromentState; }

protected :
	_wstring				m_FrontName = {};
	const WCHAR*			m_TailName = {};

	BASE_DESC				m_EnviormentInfo = {};
	_float					m_MaxRecoverTime = {};
	_float					m_CurRecoverTime = {};

	_uint					m_EnviornmentID = {};

	Enviornment_STATE		m_EnviromentState = { };

	//클라이언트에서 정의되어있는 아이템 출력 버퍼
	CCollision_Component*	m_pCollision_Com = { nullptr };

public :
	static		CEnviornment_Object*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual		CGameObject*			Clone(void* pArg) override;
	virtual		void					Free();

};
NS_END