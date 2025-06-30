#include "Crafting_Button.h"
#include "GameInstance.h"

CCrafting_Button::CCrafting_Button(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CButton{pGraphic_Device}
{
}

CCrafting_Button::CCrafting_Button(const CCrafting_Button& Prototype)
    :CButton{Prototype}
{
}

HRESULT CCrafting_Button::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCrafting_Button::Initialize(void* pArg)
{
    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    __super::UpdatePosition();

    m_iTextureIndex = 0;

    return S_OK;
}

void CCrafting_Button::Priority_Update(_float fTimeDelta)
{
}

void CCrafting_Button::Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);

    __super::Update(fTimeDelta);

    HoverEevent();

}

void CCrafting_Button::Late_Update(_float fTimeDelta)
{
}

HRESULT CCrafting_Button::Render()
{
    m_pTexture_Com->Set_Texture(m_iTextureIndex);

    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    m_pVIBuffer_Com->Render();

    return S_OK;
}

void CCrafting_Button::HoverEevent()
{
    _float3 vPos = {};

    if (true == isMouseOver())
    {
        m_iTextureIndex = 1;
        ClickedEevent();
    }
    else
    {
        m_iTextureIndex = 0;
        m_isClicked = false;
    }
}

void CCrafting_Button::ClickedEevent()
{
    if (m_pGameInstance->KeyDown(VK_LBUTTON))
        m_isClicked = true;
    else
        m_isClicked = false;
    
}

HRESULT CCrafting_Button::ADD_Components()
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

    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_CraftBar_Button"),
        TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pTexture_Com))))
        return E_FAIL;

    return S_OK;
}

CCrafting_Button* CCrafting_Button::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCrafting_Button* pInstance = new CCrafting_Button(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Create : CCrafting_Button");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCrafting_Button::Clone(void* pArg)
{
    CGameObject* pInstance = new CCrafting_Button(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CCrafting_Button");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCrafting_Button::Free()
{
    __super::Free();

    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);
}
