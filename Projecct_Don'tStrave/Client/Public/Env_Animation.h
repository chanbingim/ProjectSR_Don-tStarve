#pragma once
#include "Client_Defines.h"
#include "State.h"

NS_BEGIN(Client)
class CEnv_Animation : public CState
{
public :
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);

	virtual HRESULT			Reset_StateData();

	//여기서 텍스쳐 컴포넌트의 정보를 가지고있어도 되는거같은데
	virtual void			Tick(_float fTimeDelta);
	virtual void			Render();

public :
	static		CEnv_Animation* Create(void* pArg);
	virtual		void			Free() override;
};
NS_END