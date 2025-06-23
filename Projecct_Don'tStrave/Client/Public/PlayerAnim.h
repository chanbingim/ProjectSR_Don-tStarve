#pragma once
#include "Client_Defines.h"
#include "State.h"

NS_BEGIN(Engine)
class CTransform;
NS_END

NS_BEGIN(Client)
class CPlayerAnim final : public CState
{
public:
	typedef struct Player_Desc
	{
		FRAME_DESC			Frame;
		CTransform* pTransformCom;
		_float3				StartPoint = {};

	}PLAYER_DESC;

protected:
	CPlayerAnim() = default;
	virtual ~CPlayerAnim() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);

	virtual HRESULT			Reset_StateData();

	//여기서 텍스쳐 컴포넌트의 정보를 가지고있어도 되는거같은데
	virtual void			Tick(_float fTimeDelta);
	virtual void			Render();
	void					Render_End();

private:
	CTransform* m_pTransformCom = { nullptr };
	_float3					m_StartPoint = {};

public:
	static		CPlayerAnim* Create(void* pArg);
	virtual		void			Free() override;

};
NS_END