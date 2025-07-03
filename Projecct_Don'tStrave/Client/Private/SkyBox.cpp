#include "SkyBox.h"

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CGameObject(m_pGraphic_Device)
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
    return S_OK;
}

HRESULT CSkyBox::Initialize_Late()
{
    return S_OK;
}

void CSkyBox::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CSkyBox::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CSkyBox::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CSkyBox::Render()
{
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
}
