#include "Slot.h"

#include "GameInstance.h"

CSlot::CSlot(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CMouseSlotUI{ pGraphic_Device }
{
}

CSlot::CSlot(const CSlot& Prototype)
    : CMouseSlotUI{ Prototype }
{
}

void CSlot::Merge_Item(CSlot* pSlot)
{
    m_iNumItem += pSlot->m_iNumItem;
    m_fDurability = (m_fDurability + pSlot->m_fDurability) * 0.5f;

    pSlot->Clear();
}

void CSlot::Clear()
{
    m_iItemID = 0;
    m_iNumItem = 0;
    m_fDurability = 0.f;
}

HRESULT CSlot::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSlot::Initialize(void* pArg)
{
    m_iItemID = 0;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;



    return S_OK;
}

void CSlot::Priority_Update(_float fTimeDelta)
{
}

void CSlot::Update(_float fTimeDelta)
{


    Update_Item(fTimeDelta);
}

void CSlot::Late_Update(_float fTimeDelta)
{

}

HRESULT CSlot::Render()
{
    if (0 != m_iItemID)
    {
        Render_Item();
    }

    return S_OK;
}

HRESULT CSlot::ADD_Components()
{
    // Texture Component Item
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Item"),
        TEXT("Com_Texture_Item"),
        reinterpret_cast<CComponent**>(&m_pTexture_Com))))
        return E_FAIL;

    // Texture Component ItemState
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_ItemState"),
        TEXT("Com_Texture_ItemState"),
        reinterpret_cast<CComponent**>(&m_pTexture_Com_ItemState))))
        return E_FAIL;

    // Texture Component ItemState
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Number"),
        TEXT("Com_Texture_Number"),
        reinterpret_cast<CComponent**>(&m_pTexture_Com_NumItem))))
        return E_FAIL;

    // VIBuffer_Rect Component
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"),
        reinterpret_cast<CComponent**>(&m_pVIBuffer_Com))))
        return E_FAIL;

    //Engine::CTransform::TRANSFORM_DESC Transform_Desc = { 5.f, D3DXToRadian(90.f) };


    return S_OK;
}

void CSlot::Key_Input()
{

}

void CSlot::Update_Item(_float fTimeDelta)
{
    switch (m_eItemType)
    {
    case Client::ITEM_TYPE::MERTARIAL:
        break;

    case Client::ITEM_TYPE::FOOD:
        m_fDurability -= 3.f * fTimeDelta;
        break;
    case Client::ITEM_TYPE::EQUIPMENT:
        break;
    default:
        break;
    }
}

void CSlot::Render_ItemState()
{
    if (0 != m_iItemID && m_eItemType == ITEM_TYPE::FOOD)
    {
        int iTextureIndex = {};

        if (1.f < m_fDurability)
            iTextureIndex = 50 - static_cast<_uint>(m_fDurability);

        m_pTexture_Com_ItemState->Set_Texture(iTextureIndex);

        m_pVIBuffer_Com->Render();
    }
}

void CSlot::Render_Item()
{
    m_pTexture_Com->Set_Texture(m_iItemID);

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pVIBuffer_Com->Render();

    m_pTexture_Com_NumItem->Set_Texture(m_iNumItem);

    

    m_pVIBuffer_Com->Render();

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CSlot* CSlot::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CSlot* pInstance = new CSlot(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CSlot");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSlot::Clone(void* pArg)
{
    CSlot* pInstance = new CSlot(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CSlot");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSlot::Free()
{
    __super::Free();

    Safe_Release(m_pTexture_Com_ItemState);
    Safe_Release(m_pTexture_Com_NumItem);
    Safe_Release(m_pVIBuffer_Com);
    Safe_Release(m_pTexture_Com);
}