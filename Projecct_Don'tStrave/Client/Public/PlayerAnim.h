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
		CTransform*			pParentTransformCom;
		CTransform*			pTransformCom;
		_float2				fSize = {};
		_float3				fPoint = {};

	}PLAYER_DESC;

protected:
	CPlayerAnim() = default;
	virtual ~CPlayerAnim() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);

	virtual HRESULT			Reset_StateData();

	//���⼭ �ؽ��� ������Ʈ�� ������ �������־ �Ǵ°Ű�����
	virtual void			Tick(_float fTimeDelta);
	virtual void			Render();
	bool					IsEnd();

private:
	CTransform*				m_pParentTransformCom = { nullptr };
	CTransform*				m_pTransformCom = { nullptr };
	_float2					m_fSize = {};
	_float3					m_fPoint = {};

public:
	static		CPlayerAnim* Create(void* pArg);
	virtual		void			Free() override;

};
NS_END