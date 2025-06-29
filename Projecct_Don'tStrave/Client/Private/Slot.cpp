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

    __super::UpdatePosition();
    _uint iCountSize = { 20 };
    _float3 vPos = m_pTransform_Com->GetWorldState(WORLDSTATE::POSITION);
    vPos.y -= 10.f;
    m_Positions[0] = _float3(vPos.x - iCountSize * 0.5f, vPos.y, 0.f);
    m_Positions[1] = _float3(vPos.x + iCountSize * 0.5f, vPos.y, 0.f);
    m_Positions[2] = _float3(vPos.x - iCountSize, vPos.y, 0.f);
    m_Positions[3] = _float3(vPos.x, vPos.y, 0.f);
    m_Positions[4] = _float3(vPos.x + iCountSize, vPos.y, 0.f);
    m_Positions[5] = _float3(vPos.x - iCountSize * 1.5f, vPos.y, 0.f);
    m_Positions[6] = _float3(vPos.x + iCountSize * 1.5f, vPos.y, 0.f);

    m_TextureIndexes.reserve(4);
    
    for (_uint i = 0; i < 4; ++i)
    {
        m_TextureIndexes.push_back(0);
    }

    return S_OK;
}

void CSlot::Priority_Update(_float fTimeDelta)
{
}

void CSlot::Update(_float fTimeDelta)
{
    Update_Item(fTimeDelta);
    //Update_Count();
}

void CSlot::Late_Update(_float fTimeDelta)
{

}

HRESULT CSlot::Render(CTransform* pTransform)
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 80);

    if (0 != m_Item_Desc.iItemID)
    {
        Render_Item(pTransform);
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

    Engine::CTransform::TRANSFORM_DESC Desc = { 5.f, D3DXToRadian(90.f) };

    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"),
        reinterpret_cast<CComponent**>(&m_pTransform_Com), &Desc)))
        return E_FAIL;


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
        m_Item_Desc.fDurability -= 1.f * fTimeDelta;
        break;
    default:
        break;
    }
}

void CSlot::Update_Count()
{
    m_iDigit = 0;
    m_TextureIndexes.assign(4, 0);

    if(SLOT::NORMAL == m_Item_Desc.eSlot)
    {
        if (10 <= m_Item_Desc.iNumItem)
        {
            m_TextureIndexes[0] = m_Item_Desc.iNumItem / 10;
            ++m_iDigit;

            m_TextureIndexes[1] = m_Item_Desc.iNumItem % 10;
            ++m_iDigit;
        }
        else
        {
            m_TextureIndexes[0] = m_Item_Desc.iNumItem;
            ++m_iDigit;
        }

        
    }
    else
    {
        m_TextureIndexes[0] = 11;
        ++m_iDigit;

        if (100.f <= m_Item_Desc.fDurability)
        {
            m_TextureIndexes[1] = 1;
            ++m_iDigit;

            m_TextureIndexes[2] = static_cast<_uint>(m_Item_Desc.fDurability / 10.f - 10);
            ++m_iDigit;
           
            m_TextureIndexes[3] = static_cast<_uint>(static_cast<_uint>(m_Item_Desc.fDurability) % 10);
            ++m_iDigit;
        }
        else if (10.f <= m_Item_Desc.fDurability)
        {
            m_TextureIndexes[1] = static_cast<_uint>(m_Item_Desc.fDurability / 10.f);
            ++m_iDigit;

            m_TextureIndexes[2] = static_cast<_uint>(static_cast<_uint>(m_Item_Desc.fDurability) % 10);
            ++m_iDigit;
        }
        else
        {
            m_TextureIndexes[1] = static_cast<_uint>(m_Item_Desc.fDurability);
            ++m_iDigit;
        }

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

void CSlot::Render_Item(CTransform* pTransform)
{
    

    m_pTexture_Com->Set_Texture(m_Item_Desc.iItemID);

    m_pVIBuffer_Com->Render();

    _float3 vPos = pTransform->GetWorldState(WORLDSTATE::POSITION);

    pTransform->SetPosition(_float3(vPos.x, vPos.y + 10.f, 0.f));
    pTransform->SetScale(_float3(20.f, 20.f, 0.f));
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &pTransform->Get_World());


    if (SLOT::NORMAL == m_Item_Desc.eSlot)
    {
        if (1 == m_iDigit)
        {

            m_pTexture_Com_NumItem->Set_Texture(m_TextureIndexes[0]);
            m_pVIBuffer_Com->Render();

        }
        else
        {
            pTransform->SetPosition(_float3(vPos.x - 8.f, vPos.y + 9.f, 0.f));
            m_pGraphic_Device->SetTransform(D3DTS_WORLD, &pTransform->Get_World());
            m_pTexture_Com_NumItem->Set_Texture(m_TextureIndexes[0]);
            m_pVIBuffer_Com->Render();

            pTransform->SetPosition(_float3(vPos.x + 8.f, vPos.y + 9.f, 0.f));
            m_pGraphic_Device->SetTransform(D3DTS_WORLD, &pTransform->Get_World());
            m_pTexture_Com_NumItem->Set_Texture(m_TextureIndexes[1]);
            m_pVIBuffer_Com->Render();
        }

    }
    else
    {
        if (2 == m_iDigit)
        {
            pTransform->SetPosition(_float3(vPos.x - 7.f, vPos.y + 9.f, 0.f));
            m_pGraphic_Device->SetTransform(D3DTS_WORLD, &pTransform->Get_World());
            m_pTexture_Com_NumItem->Set_Texture(m_TextureIndexes[1]);
            m_pVIBuffer_Com->Render();

            pTransform->SetPosition(_float3(vPos.x + 7.f, vPos.y + 9.f, 0.f));
            m_pGraphic_Device->SetTransform(D3DTS_WORLD, &pTransform->Get_World());
            m_pTexture_Com_NumItem->Set_Texture(m_TextureIndexes[0]);
            m_pVIBuffer_Com->Render();

        }
        else if (3 == m_iDigit)
        {
            pTransform->SetPosition(_float3(vPos.x - 14.f, vPos.y + 9.f, 0.f));
            m_pGraphic_Device->SetTransform(D3DTS_WORLD, &pTransform->Get_World());
            m_pTexture_Com_NumItem->Set_Texture(m_TextureIndexes[1]);
            m_pVIBuffer_Com->Render();

            pTransform->SetPosition(_float3(vPos.x, vPos.y + 9.f, 0.f));
            m_pGraphic_Device->SetTransform(D3DTS_WORLD, &pTransform->Get_World());
            m_pTexture_Com_NumItem->Set_Texture(m_TextureIndexes[2]);
            m_pVIBuffer_Com->Render();

            pTransform->SetPosition(_float3(vPos.x + 14.f, vPos.y + 9.f, 0.f));
            m_pGraphic_Device->SetTransform(D3DTS_WORLD, &pTransform->Get_World());
            m_pTexture_Com_NumItem->Set_Texture(m_TextureIndexes[0]);
            m_pVIBuffer_Com->Render();


        }
        else if (4 == m_iDigit)
        {
            pTransform->SetScale(_float3(18.f, 18.f, 0.f));
            pTransform->SetPosition(_float3(vPos.x - 18.f, vPos.y + 9.f, 0.f));
            m_pGraphic_Device->SetTransform(D3DTS_WORLD, &pTransform->Get_World());
            m_pTexture_Com_NumItem->Set_Texture(m_TextureIndexes[1]);
            m_pVIBuffer_Com->Render();

            pTransform->SetPosition(_float3(vPos.x - 6.f, vPos.y + 9.f, 0.f));
            m_pGraphic_Device->SetTransform(D3DTS_WORLD, &pTransform->Get_World());
            m_pTexture_Com_NumItem->Set_Texture(m_TextureIndexes[2]);
            m_pVIBuffer_Com->Render();

            pTransform->SetPosition(_float3(vPos.x + 6.f, vPos.y + 9.f, 0.f));
            m_pGraphic_Device->SetTransform(D3DTS_WORLD, &pTransform->Get_World());
            m_pTexture_Com_NumItem->Set_Texture(m_TextureIndexes[3]);
            m_pVIBuffer_Com->Render();

            pTransform->SetPosition(_float3(vPos.x + 18.f, vPos.y + 9.f, 0.f));
            m_pGraphic_Device->SetTransform(D3DTS_WORLD, &pTransform->Get_World());
            m_pTexture_Com_NumItem->Set_Texture(m_TextureIndexes[0]);
            m_pVIBuffer_Com->Render();
        }
    }

    pTransform->SetPosition(vPos);

    
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
    Safe_Release(m_pTransform_Com);
}