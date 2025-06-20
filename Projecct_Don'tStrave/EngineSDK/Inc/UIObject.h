
#include "GameObject.h"

NS_BEGIN(Engine)

class ENGINE_DLL CUIObject abstract : public CGameObject
{
public:
	typedef struct tagUIObject 
	{
		_float		fX{}, fY{}, fSizeX{}, fSizeY{};
	}UIOBJECT_DESC;

protected:
	CUIObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUIObject(const CUIObject& Prototype);
	virtual ~CUIObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	_float4x4				m_ViewMatrix = {};
	_float4x4				m_ProjMatrix = {};
	_float					m_fX{}, m_fY{}, m_fSizeX{}, m_fSizeY{};
	_float					m_fWinSizeX{}, m_fWinSizeY{};
	_float4x4				m_OldViewMatrix{}, m_OldProjMatrix{};

protected:
	void Update_Transform(class CTransform* pTransform);
	HRESULT Begin();
	HRESULT End();

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END