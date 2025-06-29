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

	//���⼭ �ؽ��� ������Ʈ�� ������ �������־ �Ǵ°Ű�����
	virtual void			Tick(_float fTimeDelta);
	virtual void			Render();

public :
	static		CEnv_Animation* Create(void* pArg);
	virtual		void			Free() override;
};
NS_END