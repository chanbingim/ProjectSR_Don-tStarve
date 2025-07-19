#include "Cook_Button.h"

#include "GameInstance.h"

#include "Item_Manager.h"
#include "Inventory.h"

CCook_Button::CCook_Button(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CButton{ pGraphic_Device }
{
}

CCook_Button::CCook_Button(const CCook_Button& Prototype)
    :CButton{ Prototype }
{
}

HRESULT CCook_Button::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCook_Button::Initialize(void* pArg)
{

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    __super::UpdatePosition();

    return S_OK;
}

void CCook_Button::Priority_Update(_float fTimeDelta)
{

}

void CCook_Button::Update(_float fTimeDelta)
{


    __super::Update(fTimeDelta);

    HoverEevent();
}

void CCook_Button::Late_Update(_float fTimeDelta)
{
}

HRESULT CCook_Button::Render()
{

    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    m_pTexture_Com->Set_Texture(m_iTextureIndex);

    m_pVIBuffer_Com->Render();

    return S_OK;
}

void CCook_Button::HoverEevent()
{
    if (true == isMouseOver())
    {
        ClickedEevent();
        m_pTransform_Com->SetScale(_float3(m_fSizeX * 1.1f, m_fSizeY * 1.1f, 1.f));
    }
    else
        m_pTransform_Com->SetScale(_float3(m_fSizeX, m_fSizeY, 1.f));

}

void CCook_Button::ClickedEevent()
{
    if (m_pGameInstance->KeyDown(VK_LBUTTON))
        m_isClicked = true;
    else
        m_isClicked = false;

}

HRESULT CCook_Button::ADD_Components()
{
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"),
        reinterpret_cast<CComponent**>(&m_pVIBuffer_Com))))
        return E_FAIL;

    Engine::CTransform::TRANSFORM_DESC Transform_Desc = { 5.f, D3DXToRadian(90.f) };

    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"),
        reinterpret_cast<CComponent**>(&m_pTransform_Com), &Transform_Desc)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Create_Button"),
        TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pTexture_Com))))
        return E_FAIL;

    return S_OK;
}

CCook_Button* CCook_Button::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCook_Button* pInstance = new CCook_Button(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Create : CCook_Button");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCook_Button::Clone(void* pArg)
{
    CGameObject* pInstance = new CCook_Button(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CCook_Button");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCook_Button::Free()
{
    __super::Free();

    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);
}
