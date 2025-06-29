#pragma once
#include "Client_Defines.h"
#include "State.h"

NS_BEGIN(Engine)
class CTransform;
NS_END

NS_BEGIN(Client)
class CSwapObject final : public CState
{
public:
	typedef struct SwapObject_Desc
	{
		FRAME_DESC			Frame;
		CTransform* pParentTransformCom;
		CTransform* pTransformCom;
		_float3				StartPoint = {};

	}SWAPOBEJCT_DESC;

protected:
	CSwapObject() = default;
	virtual ~CSwapObject() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);

	virtual HRESULT			Reset_StateData();

	//여기서 텍스쳐 컴포넌트의 정보를 가지고있어도 되는거같은데
	virtual void			Tick(_float fTimeDelta);
	virtual void			Render();

private:
	CTransform* m_pParentTransformCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };

	_float3					m_StartPoint = {};

public:
	static		CSwapObject* Create(void* pArg);
	virtual		void			Free() override;

};
NS_END