#include "MiniMap_Button.h"
#include "GameInstance.h"

CMiniMap_Button::CMiniMap_Button(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CButton{ pGraphic_Device }
{
}

CMiniMap_Button::CMiniMap_Button(const CMiniMap_Button& Prototype)
    :CButton{ Prototype }
{
}

HRESULT CMiniMap_Button::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMiniMap_Button::Initialize(void* pArg)
{
    if (FAILED(ADD_Components()))
        return E_FAIL;

    Button_Desc Desc = {};
    Desc.fSizeX = 50.f;
    Desc.fSizeY = 50.f;
    Desc.fX = g_iWinSizeX - 100.f;
    Desc.fY = g_iWinSizeY - 50.f;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    __super::UpdatePosition();

    return S_OK;
}

void CMiniMap_Button::Priority_Update(_float fTimeDelta)
{
}

void CMiniMap_Button::Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);

    __super::Update(fTimeDelta);

    HoverEevent();

    ClickedEevent();
}

void CMiniMap_Button::Late_Update(_float fTimeDelta)
{
}

HRESULT CMiniMap_Button::Render()
{
    m_pTexture_Com->Set_Texture(0);

    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pVIBuffer_Com->Render();

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    return S_OK;
}

void CMiniMap_Button::ClickedEevent()
{
    if ((true == isMouseOver()) && (GetAsyncKeyState(VK_LBUTTON) & 0x8000))
        m_isClicked = true; // 미니맵 창 출력 활성화
    else
        m_isClicked = false;

}

void CMiniMap_Button::HoverEevent()
{
    if (true == isMouseOver())
        m_pTransform_Com->SetScale(_float3(m_fSizeX * 1.2f, m_fSizeY * 1.2f, 1.f));
    else
        m_pTransform_Com->SetScale(_float3(m_fSizeX, m_fSizeY, 1.f));
}

HRESULT CMiniMap_Button::ADD_Components()
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

    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_MiniMap_Button"),
        TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pTexture_Com))))
        return E_FAIL;

    return S_OK;
}

CMiniMap_Button* CMiniMap_Button::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CMiniMap_Button* pInstance = new CMiniMap_Button(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Create : CMiniMap_Button");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMiniMap_Button::Clone(void* pArg)
{
    CGameObject* pInstance = new CMiniMap_Button(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CCrafting_Button");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMiniMap_Button::Free()
{
    __super::Free();

    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);
}
