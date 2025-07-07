#include "LodingSpinner.h"

#include "GameInstance.h"

CLodingSpinner::CLodingSpinner(LPDIRECT3DDEVICE9 pGraphic_Device) : 
    CUserInterface(pGraphic_Device)
{
}

HRESULT CLodingSpinner::Initialize(void* pArg)
{
    if (FAILED(ADD_Components()))
        return E_FAIL;


    if (nullptr == pArg)
    {
        m_vSpinnerCenter = { 640.f, 250.f };
        m_fSpinnerDis = 20.f;
        m_fSpinnerSpeed = 7.f;
    }
    else
    {
        SPINNER_DESC* Spinner_Data = static_cast<SPINNER_DESC*>(pArg);
        m_vSpinnerCenter = Spinner_Data->Center;
        m_fSpinnerDis = Spinner_Data->fSpinnerDis;
        m_fSpinnerSpeed = Spinner_Data->fSpinnerSpeed;
    }

    m_fSizeX = 100.f;
    m_fSizeY = 100.f;

    m_fX = 570;
    m_fY = 270;
    m_fAngle = 0.f;
    return S_OK;
}

void CLodingSpinner::Priority_Update(_float fTimeDelta)
{
    
}

void CLodingSpinner::Update(_float fTimeDelta)
{
    m_fAngle += fTimeDelta * m_fSpinnerSpeed;

    if(!m_IsLoadingFinish)
        m_pTransform_Com->SetRotAxis({0.f ,0.f ,1.f}, m_fAngle );
    else
    {
        //로딩완료되면 문구 표시
    }
    UpdatePosition();
}

void CLodingSpinner::Late_Update(_float fTimeDelta)
{
}

HRESULT CLodingSpinner::Render()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 300);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());
    m_pTexture_Com->Set_Texture(0);
    m_pVIBuffer_Com->Render();

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
    return S_OK;
}

HRESULT CLodingSpinner::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), (CComponent**)&m_pTransform_Com)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer_Com)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Loading_Circle_Texture"),
        TEXT("Com_Texture"), (CComponent**)&m_pTexture_Com)))
        return E_FAIL;

    return S_OK;
}

CLodingSpinner* CLodingSpinner::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg)
{
    CLodingSpinner* pInstance = new CLodingSpinner(pGraphic_Device);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : LOADING SPIINER");
    }

    return pInstance;
}

void CLodingSpinner::Free()
{
    __super::Free();

    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pVIBuffer_Com);
}
