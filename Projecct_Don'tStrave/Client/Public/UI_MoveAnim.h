#pragma once

#include "Client_Defines.h"
#include "State.h"

NS_BEGIN(Engine)
class CTransform;
NS_END

NS_BEGIN(Client)

typedef struct LerpStat
{
	_bool				m_bLerpMove = {};
	_float				m_AccLerpTime = {};
}LERP_DATAC;

class CUI_MoveAnim final : public CState
{
public :
	typedef struct UI_Move_Desc
	{
		FRAME_DESC			Frame;
		CTransform*			pTransformCom;
		_float				fDurTime;
		_float3				StartPoint = {};
		_float3				EndPoint = {};

	}UI_MOVE_DESC;

protected :
	CUI_MoveAnim() = default;
	virtual ~CUI_MoveAnim() = default;

public :
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);

	virtual HRESULT			Reset_StateData();

	//여기서 텍스쳐 컴포넌트의 정보를 가지고있어도 되는거같은데
	virtual void			Tick(_float fTimeDelta);
	virtual void			Render();

private : 
	CTransform*				m_pTransformCom = { nullptr };
	_float					m_fDurTime = {};
	_float					m_fAccTime = {};

	_float3					m_StartPoint = {};
	_float3					m_EndPoint = {};
	LERP_DATAC				m_LerpData;

public :
	static		CUI_MoveAnim*	Create(void* pArg);
	virtual		void			Free() override;

};
NS_END