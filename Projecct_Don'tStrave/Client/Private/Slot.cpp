#include "Slot.h"

#include "GameInstance.h"
#include "Item.h"

CSlot::CSlot(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CMouseSlotUI{ pGraphic_Device }
{
}

CSlot::CSlot(const CSlot& Prototype)
    : CMouseSlotUI{ Prototype }
{
}

void CSlot::Set_Info(ITEM_DESC& Item_Desc)
{
    memcpy(&m_Item_Desc, &Item_Desc, sizeof(ITEM_DESC));
}

ITEM_DESC& CSlot::Get_Info()
{
    return m_Item_Desc;
}

void CSlot::Merge_Item(CSlot* pSlot)
{
    ITEM_DESC Desc = pSlot->Get_Info();

    m_Item_Desc.iNumItem += Desc.iNumItem;
    m_Item_Desc.fDurability = min(Desc.fDurability , m_Item_Desc.fDurability);

    pSlot->Clear();
}

void CSlot::Merge_Item(ITEM_DESC& Item_Desc)
{
    m_Item_Desc.iNumItem += Item_Desc.iNumItem;
    m_Item_Desc.fDurability = (Item_Desc.fDurability + Item_Desc.fDurability) * 0.5f;
}

void CSlot::Clear()
{
    ZeroMemory(&m_Item_Desc, sizeof(ITEM_DESC));
}

HRESULT CSlot::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSlot::Initialize(void* pArg)
{
    ZeroMemory(&m_Item_Desc, sizeof(ITEM_DESC));

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

HRESULT CSlot::Render(RECT& rcText)
{
    if (0 != m_Item_Desc.iItemID)
    {
        Render_Item(rcText);
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
    switch (m_Item_Desc.eItemType)
    {
    case Client::ITEM_TYPE::MERTARIAL:
        break;

    case Client::ITEM_TYPE::FOOD:
        m_Item_Desc.fDurability -= 3.f * fTimeDelta;
        break;
    case Client::ITEM_TYPE::EQUIPMENT:
        break;
    default:
        break;
    }
}

void CSlot::Render_ItemState()
{
    if (m_Item_Desc.eItemType == ITEM_TYPE::FOOD)
    {
        int iTextureIndex = {};

        if (1.f < m_Item_Desc.fDurability)
            iTextureIndex = static_cast<_uint>((100.f - m_Item_Desc.fDurability) * 0.5f);

        m_pTexture_Com_ItemState->Set_Texture(iTextureIndex);

        m_pVIBuffer_Com->Render();
    }
}

void CSlot::Render_Item(RECT& rcText)
{
    m_pTexture_Com->Set_Texture(m_Item_Desc.iItemID);

    m_pVIBuffer_Com->Render();

    //m_pTexture_Com_NumItem->Set_Texture(m_Item_Desc.iNumItem);

    //m_pVIBuffer_Com->Render();

    //RECT Rect = { LONG(m_fX - m_fSizeX), LONG(m_fY - m_fSizeY - 20.f),LONG(m_fX + m_fSizeX), LONG(m_fY + m_fSizeY - 20.f) };
    
    D3DXCOLOR Black = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
    m_pGameInstance->Render_Font(TEXT("Font_20"), TEXT("1"), &rcText, Black);
    
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