#pragma once

#include "AinimationObject.h"

NS_BEGIN(Engine)
class CCollision_Component;
NS_END

NS_BEGIN(Client)

class CSpriteEffect;

class CEnviornment_Object : public CAinimationObject
{
public:
	enum class Enviornment_STATE { IDLE, RECOVERY, DAMAGED, BROKEN, BROKEN_IDLE, END };
	enum class Enviornment_TYPE { GRASS, STONE, TREE, NPC, RESERREECTION, END };

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

	BASE_DESC				GetCurrentInfo();		
	Enviornment_STATE		GetState();				
	Enviornment_TYPE		GetEnviornMentType();	
	_float					GetAnimationFrame();
	const _wstring			GetMotionName();		

	void					HoverEevent();

protected :
	_wstring				m_FrontName = {};
	_wstring				m_TailName = {};

	BASE_DESC				m_EnviormentInfo = {};
	_float					m_MaxRecoverTime = {};
	_float					m_CurRecoverTime = {};

	Enviornment_STATE		m_EnviromentState = { };
	Enviornment_TYPE		m_EnviornmentType = {};

	//클라이언트에서 정의되어있는 아이템 출력 버퍼
	CCollision_Component*	m_pCollision_Com = { nullptr };
	CSpriteEffect*			m_pSpirteEffect = { nullptr };

public :
	static		CEnviornment_Object*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual		CGameObject*			Clone(void* pArg) override;
	virtual		void					Free();

};
NS_END