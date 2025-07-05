#include "LightObejct.h"
#include "LightComponent.h"

CLightObejct::CLightObejct(LPDIRECT3DDEVICE9 pGraphic_Device) : 
    CGameObject(m_pGraphic_Device)
{
}

CLightObejct::CLightObejct(const CLightObejct& rhs) :
    CGameObject(rhs)
{
}

HRESULT CLightObejct::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CLightObejct::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CLightObejct::Initialize_Late()
{
    return S_OK;
}

void CLightObejct::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CLightObejct::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CLightObejct::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CLightObejct::Render()
{

    return S_OK;
}

CGameObject* CLightObejct::Clone(void* pArg)
{
    return nullptr;
}

void CLightObejct::Free()
{
    __super::Free();

    Safe_Release(m_pLight_Com);
}
