#include "GamePlay_Button.h"

#include "GameInstance.h"
#include "PlayerData_Manager.h"

CGamePlay_Button::CGamePlay_Button(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CButton{ pGraphic_Device }
{
}

CGamePlay_Button::CGamePlay_Button(const CGamePlay_Button& Prototype)
    :CButton{ Prototype }
{
}

HRESULT CGamePlay_Button::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CGamePlay_Button::Initialize(void* pArg)
{


    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    __super::UpdatePosition();



    return S_OK;
}

void CGamePlay_Button::Priority_Update(_float fTimeDelta)
{

}

void CGamePlay_Button::Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);

    __super::Update(fTimeDelta);

    HoverEevent();

}

void CGamePlay_Button::Late_Update(_float fTimeDelta)
{
}

HRESULT CGamePlay_Button::Render()
{
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    m_pTexture_Com->Set_Texture(0);

    m_pVIBuffer_Com->Render();

    return S_OK;
}

void CGamePlay_Button::HoverEevent()
{
    if (true == isMouseOver())
    {
        ClickedEevent();
        m_pTransform_Com->SetScale(_float3(m_fSizeX * 1.2f, m_fSizeY * 1.2f, 1.f));
    }
    else
        m_pTransform_Com->SetScale(_float3(m_fSizeX, m_fSizeY, 1.f));

}

void CGamePlay_Button::ClickedEevent()
{
    if (m_pGameInstance->KeyDown(VK_LBUTTON))
    {
        m_isClicked = true;
       
    }
    else
    {
        m_isClicked = false;
        
    }

}

HRESULT CGamePlay_Button::ADD_Components()
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

    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::SELECT), TEXT("Prototype_Component_Texture_GamePlay_Button"),
        TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pTexture_Com))))
        return E_FAIL;

    return S_OK;
}

CGamePlay_Button* CGamePlay_Button::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CGamePlay_Button* pInstance = new CGamePlay_Button(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Create : CGamePlay_Button");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CGamePlay_Button::Clone(void* pArg)
{
    CGameObject* pInstance = new CGamePlay_Button(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CGamePlay_Button");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CGamePlay_Button::Free()
{
    __super::Free();

    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);
}
