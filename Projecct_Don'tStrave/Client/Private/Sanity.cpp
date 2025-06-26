#include "Sanity.h"

#include "GameInstance.h"

CSanity::CSanity(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CHeadUpDisplay{ pGraphic_Device }
{
}

CSanity::CSanity(const CSanity& Prototype)
    : CHeadUpDisplay{ Prototype }
{
}

HRESULT CSanity::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSanity::Initialize(void* pArg)
{
    m_iTextureIndex = 0;
    m_fTimeAcc = 0.f;

    CUserInterface::UIOBJECT_DESC Desc = {};

    Desc.fX = g_iWinSizeX - 80.f;
    Desc.fY = g_iWinSizeY * 0.35f;
    Desc.fSizeX = 70.f;
    Desc.fSizeY = 70.f;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    __super::UpdatePosition();

    return S_OK;
}

void CSanity::Priority_Update(_float fTimeDelta)
{

}

void CSanity::Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);

    m_fTimeAcc += fTimeDelta;

    if (1.f <= m_fTimeAcc)
    {
        m_fTimeAcc = 0.f;
        ++m_iTextureIndex;
    }

    if (m_iTextureIndex > 50)
        m_iTextureIndex = 0;

}

void CSanity::Late_Update(_float fTimeDelta)
{
}

HRESULT CSanity::Render()
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

HRESULT CSanity::ADD_Components()
{
    // Texture Component
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Sanity"),
        TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pTexture_Com))))
        return E_FAIL;

    // VIBuffer_Rect Component
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"),
        reinterpret_cast<CComponent**>(&m_pVIBuffer_Com))))
        return E_FAIL;

    Engine::CTransform::TRANSFORM_DESC Transform_Desc = { 5.f, D3DXToRadian(90.f) };

    // Transform Component
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"),
        reinterpret_cast<CComponent**>(&m_pTransform_Com), &Transform_Desc)))
        return E_FAIL;


    return S_OK;
}

CSanity* CSanity::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CSanity* pInstance = new CSanity(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CSanity");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSanity::Clone(void* pArg)
{
    CSanity* pInstance = new CSanity(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CSanity");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSanity::Free()
{
    __super::Free();

    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);
}
