#include "Character.h"

CCharacter::CCharacter(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CGameObject(pGraphic_Device)
{
}

CCharacter::CCharacter(const CCharacter& rhs) :
    CGameObject(rhs)
{
}

HRESULT CCharacter::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCharacter::Initialize(void* pArg)
{
    return S_OK;
}

void CCharacter::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CCharacter::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CCharacter::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CCharacter::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;

    return S_OK;
}

_float3 CCharacter::LerpVec3(_float3& StartVec, _float3& EndVec, _float fDurTime, _float fTimeDelta)
{
    _float3 NewPostion = {};
    _float AccTime = m_LerpData[0].m_AccLerpTime;
    m_LerpData[0].m_AccLerpTime += fTimeDelta;
    m_LerpData[0].m_bLerpMove   = true;

    D3DXVec3Lerp(&NewPostion, &StartVec, &EndVec, Clamp<_float>(AccTime / fDurTime, 0.f, 1.f));
    if (AccTime >= fDurTime)
    {
        m_LerpData[0].m_AccLerpTime = 0.f;
        m_LerpData[0].m_bLerpMove   = false;
    }
   
    return NewPostion;
}

//_bool CCharacter::LerpTurn(_float3& TrunAxis, _float StartAngle, _float EndAngle, _float DurTime)
//{
//    return false;
//}

CGameObject* CCharacter::Clone(void* pArg)
{
    return nullptr;
}

void CCharacter::Free()
{
    __super::Free();
}
