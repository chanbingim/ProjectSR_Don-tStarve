#include "LodingInterface.h"

#include "GameInstance.h"
#include "LodingSpinner.h"

CLodingInterface::CLodingInterface(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CUserInterface(pGraphic_Device)
{
}

CLodingInterface::CLodingInterface(const CLodingInterface& rhs) :
    CUserInterface(rhs)
{
}

HRESULT CLodingInterface::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CLodingInterface::Initialize(void* pArg)
{
    if (FAILED(ADD_Components()))
        return E_FAIL;

    m_iTextureIndex = rand() % m_pTexture_Com->StoredTextureCount();

    m_fSizeX = 1280.f;
    m_fSizeY = 720.f;

    m_fX = 0;
    m_fY = 0;
    return S_OK;
}

void CLodingInterface::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CLodingInterface::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
    m_pSinner->Update(fTimeDelta);
}

void CLodingInterface::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    UpdatePosition();
    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);
}

HRESULT CLodingInterface::Render()
{
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());
    m_pTexture_Com->Set_Texture(m_iTextureIndex);
    m_pVIBuffer_Com->Render();

    if(!m_bFinishedLoad)
        m_pSinner->Render();
    else
    {
        //폰트 출력 또는 데이터 출력
    }
    return S_OK;
}

void CLodingInterface::Finished_Loading(_bool IsFinish)
{
    m_bFinishedLoad = IsFinish;
}

HRESULT CLodingInterface::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"), 
        TEXT("Com_Transform"), (CComponent**)&m_pTransform_Com)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer_Com)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Loading_Texture"),
        TEXT("Com_Texture"), (CComponent**)&m_pTexture_Com)))
        return E_FAIL;

    CLodingSpinner::SPINNER_DESC SpinnerData = {};
    m_pSinner = CLodingSpinner::Create(m_pGraphic_Device, nullptr);

    return S_OK;
}

CLodingInterface* CLodingInterface::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CLodingInterface* pInstance = new CLodingInterface(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : LOADING INTERFACE");
    }

    return pInstance;
}

CUserInterface* CLodingInterface::Clone(void* pArg)
{
    CLodingInterface* pInstance = new CLodingInterface(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : LOADING INTERFACE");
    }

    return pInstance;
}

void CLodingInterface::Free()
{
    __super::Free();

    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pVIBuffer_Com);

    Safe_Release(m_pSinner);
}
