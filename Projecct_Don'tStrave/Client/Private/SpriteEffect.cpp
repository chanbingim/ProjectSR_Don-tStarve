#include "SpriteEffect.h"

CSpriteEffect::CSpriteEffect(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CGameObject(m_pGraphic_Device)
{
}

CSpriteEffect::CSpriteEffect(const CSpriteEffect& rhs) :
    CGameObject(rhs)
{
}

HRESULT CSpriteEffect::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSpriteEffect::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CSpriteEffect::Initialize_Late()
{
    return S_OK;
}

void CSpriteEffect::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CSpriteEffect::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CSpriteEffect::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CSpriteEffect::Render()
{
    return S_OK;
}

CSpriteEffect* CSpriteEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CSpriteEffect* pInstance = new CSpriteEffect(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : SPRITE EFFECT");
    }
    return pInstance;
}

CGameObject* CSpriteEffect::Clone(void* pArg)
{
    CSpriteEffect* pInstance = new CSpriteEffect(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : SPRITE EFFECT");
    }
    return pInstance;
}

void CSpriteEffect::Free()
{
    __super::Free();
}
