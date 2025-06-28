#include "QuickSlot_Button.h"
#include "GameInstance.h"

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
}

void CQuickSlot_Button::Late_Update(_float fTimeDelta)
{
}

HRESULT CQuickSlot_Button::Render()
{
    m_pTexture_Com->Set_Texture(m_iTextureIndex);

    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    m_pVIBuffer_Com->Render();

    return S_OK;
}

void CQuickSlot_Button::HoverEevent()
{
    if (true == isMouseOver())
    {
        ClickedEevent();
        m_pTransform_Com->SetScale(_float3(m_fSizeX * 1.2f, m_fSizeY, 1.f));
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

    return S_OK;
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

    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);
}
