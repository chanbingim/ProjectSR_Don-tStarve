#pragma once
#include "DropItemEnviornment.h"

NS_BEGIN(Client)

class CClock;

class CPigKing final : public CDropItemEnviornment
{
public :
	enum pigKingState { HAPPY, COINTOSS, E_SLEEP, UNIMPREESED, F_SLEEP, L_SLEEP, IDLE };

protected:
	CPigKing(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPigKing(const CPigKing& rhs);
	virtual ~CPigKing() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual void			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual	void			Reset_State();
	virtual HRESULT			Render() override;

private :
	_int					m_TradeCnt = { 3 };
	_float3					m_PickingPos = {};
	pigKingState			m_PigState = { pigKingState::IDLE };
	
	CClock*					m_pClock = { nullptr };
	_bool					m_bIsQuestStart = { false };

private :
	HRESULT					ADD_Components();
	void					Change_Motion(pigKingState ChangeState);
	void					ChangeTimeState();
	void					ChangeMeetToGold();

	void					ShowFirstClickMotion();
	
public:
	static		CPigKing*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual		CGameObject*		Clone(void* pArg) override;
	virtual		void				Free();
};
NS_END