#include "QuickSlot_Button.h"

#include "GameInstance.h"
#include "Item_Manager.h"

#include "Inventory.h"
#include "Slot.h"

CQuickSlot_Button::CQuickSlot_Button(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CButton{pGraphic_Device}
{
}

CQuickSlot_Button::CQuickSlot_Button(const CQuickSlot_Button& Prototype)
    :CButton{Prototype}
{
}

HRESULT CQuickSlot_Button::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CQuickSlot_Button::Initialize(void* pArg)
{
    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    __super::UpdatePosition();

    m_iTextureIndex = 0;
    m_iPreID = 0;

    m_pInventory = dynamic_cast<CInventory*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_UserInterface"), 0));

    Safe_AddRef(m_pInventory);

    

    return S_OK;
}

void CQuickSlot_Button::Priority_Update(_float fTimeDelta)
{
}

void CQuickSlot_Button::Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);

    __super::Update(fTimeDelta);

    HoverEevent();

    if (m_iPreID != m_iTextureIndex)
    {
        ITEM_DATA Data = CItem_Manager::GetInstance()->Get_ItemData(m_iTextureIndex);
        Set_Data(Data);
    }

    if(0 != m_iTextureIndex)
    {
        ITEM_DATA Data = CItem_Manager::GetInstance()->Get_ItemData(m_iTextureIndex);
        Check(Data);
    }
    
}

void CQuickSlot_Button::Late_Update(_float fTimeDelta)
{
}

HRESULT CQuickSlot_Button::Render()
{
    m_pTexture_Com->Set_Texture(0);
   
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    m_pVIBuffer_Com->Render();

    m_pItemTexture_Com->Set_Texture(m_iTextureIndex);

    if (false == m_bActive)
    {
        m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

        m_pGraphic_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
        m_pGraphic_Device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT); // Stage0 결과
        m_pGraphic_Device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE | D3DTA_ALPHAREPLICATE);

        m_pGraphic_Device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        m_pGraphic_Device->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT); 
        m_pGraphic_Device->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
        

        m_pVIBuffer_Com->Render();

        m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

        m_pGraphic_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
        m_pGraphic_Device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    }
    else
        m_pVIBuffer_Com->Render();

    return S_OK;
}

void CQuickSlot_Button::HoverEevent()
{
    if (true == isMouseOver())
    {
        ClickedEevent();
        m_pTransform_Com->SetScale(_float3(m_fSizeX * 1.2f, m_fSizeY * 1.2f, 1.f));
    }
    else
        m_pTransform_Com->SetScale(_float3(m_fSizeX, m_fSizeY, 1.f));
}

void CQuickSlot_Button::ClickedEevent()
{
    if (m_pGameInstance->KeyDown(VK_LBUTTON))
        m_isClicked = true;
    else
        m_isClicked = false;
    
}

HRESULT CQuickSlot_Button::ADD_Components()
{
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"),
        reinterpret_cast<CComponent**>(&m_pVIBuffer_Com))))
        return E_FAIL;

    Engine::CTransform::TRANSFORM_DESC Transform_Desc = {5.f, D3DXToRadian(90.f)};

    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"),
        reinterpret_cast<CComponent**>(&m_pTransform_Com), &Transform_Desc)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_QuickSlot_Button"),
        TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pTexture_Com))))
        return E_FAIL;
    
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Item"),
        TEXT("Com_ItemTexture"),
        reinterpret_cast<CComponent**>(&m_pItemTexture_Com))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Scale"),
        TEXT("Com_BlendTexture"),
        reinterpret_cast<CComponent**>(&m_pBlend_Texture_Com))))
        return E_FAIL;

    return S_OK;
}

void CQuickSlot_Button::Set_Data(ITEM_DATA& Data)
{
    m_iNumNeeded = 0;

    m_iMaterials[0] = Data.iMaterialID_1;
    m_iNeededs[0] = Data.iNumMaterial_1;

    m_iMaterials[1] = Data.iMaterialID_2;
    m_iNeededs[1] = Data.iNumMaterial_2;

    m_iMaterials[2] = Data.iMaterialID_3;
    m_iNeededs[2] = Data.iNumMaterial_3;

    for (_uint i = 0; i < 3; ++i)
    {
        if (0 != m_iMaterials[i])
            ++m_iNumNeeded;
    }
}

void CQuickSlot_Button::Check(ITEM_DATA& Data)
{
    _uint iCheck = {};

    for (_uint i = 0; i < m_iNumNeeded; ++i)
    {
        m_itemCount[i] = m_pInventory->Check_ItemCount(m_iMaterials[i]);

        if (m_itemCount[i] >= m_iNeededs[i])
        {
            ++iCheck;
        }
    }

    if (iCheck == m_iNumNeeded)
    {
        // 이미지 활성화
        CSlot* pSlot = dynamic_cast<CSlot*>(m_pGameInstance->Chagne_Slot());
        if (true == m_isClicked && 0 == pSlot->Get_ItemID())
        {
            for (_uint i = 0; i < m_iNumNeeded; ++i)
            {
                m_pInventory->Use_Item(m_iMaterials[i], m_iNeededs[i]);
            }
            ITEM_DESC Desc = {};
            Desc.iItemID = Data.iItemID;
            Desc.eItemType = Data.eItemType;
            Desc.eSlot = Data.eSlot;
            Desc.fDurability = 100.f;
            Desc.iNumItem = 1;

            pSlot->Set_Info(Desc);
        }
        m_bActive = true;
    }
    else
    {
        m_bActive = false;
    }
    
}

CQuickSlot_Button* CQuickSlot_Button::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CQuickSlot_Button* pInstance = new CQuickSlot_Button(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Create : CQuickSlot_Button");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CQuickSlot_Button::Clone(void* pArg)
{
    CGameObject* pInstance = new CQuickSlot_Button(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CQuickSlot_Button");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CQuickSlot_Button::Free()
{
    __super::Free();

    Safe_Release(m_pItemTexture_Com);
    Safe_Release(m_pBlend_Texture_Com);
    Safe_Release(m_pInventory);
    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);
}
