#pragma once
#include "Client_Defines.h"
#include "AinimationObject.h"

NS_BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer;
class CCollision_Component;
NS_END

NS_BEGIN(Client)

class CItem abstract : public CAinimationObject
{
protected:
    CItem(LPDIRECT3DDEVICE9 pGraphic_Device);
    CItem(const CItem& Prototype);
    virtual ~CItem() = default;

public:
    ITEM_DESC& Get_Info() { return m_Item_Desc; }

public:
    virtual HRESULT  Initialize_Prototype();
    virtual HRESULT  Initialize(void* pArg);
    virtual void     Priority_Update(_float fTimeDelta);
    virtual void     Update(_float fTimeDelta);
    virtual void     Late_Update(_float fTimeDelta);
    virtual HRESULT     Render();

    void                HoverEvent();
    void                ClickedEvent();
    void                EnterInvenTory();

protected:
    ITEM_DESC    m_Item_Desc = {};

    _wstring            m_FrontName = {};
    const WCHAR* m_TailName = {};

    _bool        m_bIsplayAnim = { false };
    _bool        m_bHovered = {};

    CTransform* m_pPlayerTransform_Com = { nullptr };

    CTexture* m_pTexture_Com = { nullptr };
    CVIBuffer* m_pVIBuffer_Com = { nullptr };

    CCollision_Component* m_pCollision_Com = { nullptr };

    class CMouse* m_pMouse = { nullptr };

protected:
    HRESULT ADD_Components();
    void    Update_Item(_float fTimeDelta);
    _bool    isInRange(_float fRange = 0.8f);

    void    DropItemEffect(_float FallSpeed);


public:
    virtual CGameObject* Clone(void* pArg) = 0;
    virtual void Free() override;
};

NS_END