#include "SkyBox.h"

#include "GameInstance.h"

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CGameObject(pGraphic_Device)
{
}

CSkyBox::CSkyBox(const CSkyBox& rhs) :
    CGameObject(rhs)
{
}

HRESULT CSkyBox::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSkyBox::Initialize(void* pArg)
{
    if (FAILED(Add_Components()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkyBox::Initialize_Late()
{
    m_pCamera_TransfromCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Camera"), TEXT("Com_Transform")));

    if (nullptr == m_pCamera_TransfromCom)
        return E_FAIL;

    return S_OK;
}

void CSkyBox::Priority_Update(_float fTimeDelta)
{
   
}

void CSkyBox::Update(_float fTimeDelta)
{
    

    m_pGameInstance->Add_RenderGroup(RENDER::PRIORITY, this);
    
}

void CSkyBox::Late_Update(_float fTimeDelta)
{
    _float4x4			ViewMatrix = {};

    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
    D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

    m_pTransformCom->SetPosition(*reinterpret_cast<_float3*>(&ViewMatrix.m[3]));

    m_pGameInstance->Add_RenderGroup(RENDER::PRIORITY, this);
}

HRESULT CSkyBox::Render()
{
    



    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_World());

    m_pTextureCom->Set_Texture(5);

    m_pVIBufferCom->Render();



    return S_OK;
}

HRESULT CSkyBox::Add_Components()
{
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Cube"),
        TEXT("Com_VIBuffer"),
        reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    Engine::CTransform::TRANSFORM_DESC Transform_Desc = { 5.f, D3DXToRadian(90.f) };

    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"),
        reinterpret_cast<CComponent**>(&m_pTransformCom), &Transform_Desc)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Skybox"),
        TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;


    return S_OK;
}

CSkyBox* CSkyBox::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CSkyBox* pInstance = new CSkyBox(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : SKY BOX");
    }
    return pInstance;
}

CGameObject* CSkyBox::Clone(void* pArg)
{
    CSkyBox* pInstance = new CSkyBox(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : SKY BOX");
    }
    return pInstance;
}

void CSkyBox::Free()
{
    __super::Free();

    Safe_Release(m_pCamera_TransfromCom);

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);

}
