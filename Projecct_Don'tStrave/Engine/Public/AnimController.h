#pragma once

#include "Component.h"

NS_BEGIN(Engine)
class CState;

class ENGINE_DLL CAnimController final : public CComponent
{
private :
	CAnimController();
	CAnimController(const CAnimController& rhs);
	virtual ~CAnimController() = default;

public :
	//초기화
	virtual		HRESULT				Initalize_Prototype();
	virtual		HRESULT				Initialize(void* pArg);

	//여기서 텍스쳐 컴포넌트의 정보를 가지고있어도 되는거같은데
	virtual void					Tick(_float fTimeDelta);
	virtual void					Render();

	void							ChangeState(CState* pNewState);

private :
	//FSM을 통해서 Texture을 이동하든 뭘하든 하는 거는 어떨까
	CState*							m_CurState = { nullptr };

public :
	static	CAnimController*		Create();
	virtual CComponent*				Clone(void* pArg) override;
	virtual void					Free() override;
};
NS_END