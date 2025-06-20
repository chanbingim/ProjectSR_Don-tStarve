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
	//�ʱ�ȭ
	virtual		HRESULT				Initalize_Prototype();
	virtual		HRESULT				Initialize(void* pArg);

	//���⼭ �ؽ��� ������Ʈ�� ������ �������־ �Ǵ°Ű�����
	virtual void					Tick(_float fTimeDelta);
	virtual void					Render();

	void							ChangeState(CState* pNewState);

private :
	//FSM�� ���ؼ� Texture�� �̵��ϵ� ���ϵ� �ϴ� �Ŵ� ���
	CState*							m_CurState = { nullptr };

public :
	static	CAnimController*		Create();
	virtual CComponent*				Clone(void* pArg) override;
	virtual void					Free() override;
};
NS_END