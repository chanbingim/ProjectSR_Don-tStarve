#include "Camera_Button.h"
#include "GameInstance.h"

CCamera_Button::CCamera_Button(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CButton{ pGraphic_Device }
{
}

CCamera_Button::CCamera_Button(const CCamera_Button& Prototype)
    :CButton{ Prototype }
{
}

HRESULT CCamera_Button::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCamera_Button::Initialize(void* pArg)
{
    CAMERABUTTON_DESC* pDesc = static_cast<CAMERABUTTON_DESC*>(pArg);

    m_iTextureIndex = pDesc->iTextIndex;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    
    __super::UpdatePosition();

    return S_OK;
}

void CCamera_Button::Priority_Update(_float fTimeDelta)
{
}

void CCamera_Button::Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);

    HoverEevent();

    ClickedEevent();
}

void CCamera_Button::Late_Update(_float fTimeDelta)
{
}

HRESULT CCamera_Button::Render()
{
     m_pTexture_Com->Set_Texture(m_iTextureIndex);

    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pVIBuffer_Com->Render();

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    return S_OK;
}

void CCamera_Button::HoverEevent()
{
    if (true == isMouseOver())
        m_pTransform_Com->SetScale(_float3(m_fSizeX * 1.2f, m_fSizeY * 1.2f, 1.f));
    else
        m_pTransform_Com->SetScale(_float3(m_fSizeX, m_fSizeY, 1.f));
}

void CCamera_Button::ClickedEevent()
{
    if ((true == isMouseOver()) && (GetAsyncKeyState(VK_LBUTTON) & 0x8000))
        m_isClicked = true;
    else
        m_isClicked = false;

}

HRESULT CCamera_Button::ADD_Components()
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

    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Camera_Button"),
        TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pTexture_Com))))
        return E_FAIL;

    return S_OK;
}

CCamera_Button* CCamera_Button::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCamera_Button* pInstance = new CCamera_Button(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Create : CCamera_Button");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCamera_Button::Clone(void* pArg)
{
    CGameObject* pInstance = new CCamera_Button(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CCamera_Button");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCamera_Button::Free()
{
    __super::Free();

    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);
}
