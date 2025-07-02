#include "BookMark_Button.h"
#include "GameInstance.h"

CBookMark_Button::CBookMark_Button(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CButton{ pGraphic_Device }
{
}

CBookMark_Button::CBookMark_Button(const CBookMark_Button& Prototype)
    :CButton{ Prototype }
{
}

HRESULT CBookMark_Button::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBookMark_Button::Initialize(void* pArg)
{
    m_isSelected = false;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    __super::UpdatePosition();

    return S_OK;
}

void CBookMark_Button::Priority_Update(_float fTimeDelta)
{

}

void CBookMark_Button::Update(_float fTimeDelta)
{

    __super::Update(fTimeDelta);

    HoverEevent();
}

void CBookMark_Button::Late_Update(_float fTimeDelta)
{
}

HRESULT CBookMark_Button::Render()
{
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    m_pTexture_Com->Set_Texture(m_isSelected);

    m_pVIBuffer_Com->Render();

    return S_OK;
}

void CBookMark_Button::HoverEevent()
{
    if (true == isMouseOver())
    {
        ClickedEevent();
    }

}

void CBookMark_Button::ClickedEevent()
{
    if (m_pGameInstance->KeyDown(VK_LBUTTON))
    {
        m_isClicked = true;
    }
    else
        m_isClicked = false;


}

HRESULT CBookMark_Button::ADD_Components()
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

    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Bookmark_Button"),
        TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pTexture_Com))))
        return E_FAIL;

    return S_OK;
}

CBookMark_Button* CBookMark_Button::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBookMark_Button* pInstance = new CBookMark_Button(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Create : CItem_Button");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBookMark_Button::Clone(void* pArg)
{
    CGameObject* pInstance = new CBookMark_Button(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CItem_Button");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBookMark_Button::Free()
{
    __super::Free();

    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);
}
