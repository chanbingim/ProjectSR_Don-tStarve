#include "Character.h"
#include "GameInstance.h"

CCharacter::CCharacter(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CAlphaObject{ pGraphic_Device }
{
}

CCharacter::CCharacter(const CCharacter& Prototype)
    : CAlphaObject{ Prototype }
{
}

HRESULT CCharacter::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCharacter::Initialize(void* pArg)
{
    CLandObject::LANDOBJECT_DESC			Desc{};
    Desc.pLandTransform = static_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::TUTORIAL), TEXT("BackGroundLayer"), TEXT("Com_Transform")));
    if (Desc.pLandTransform) {
        Desc.pLandVIBuffer = static_cast<CVIBuffer*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::TUTORIAL), TEXT("BackGroundLayer"), TEXT("Com_VIBuffer")));
    }
    else {
        Desc.pLandTransform = static_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BackGround"), TEXT("Com_Transform")));
        Desc.pLandVIBuffer = static_cast<CVIBuffer*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BackGround"), TEXT("Com_VIBuffer")));
    }
    m_pCharacterInstance = CCharacter_Manager::GetInstance();
    m_pCharacterInstance->AddRef();
    m_pCharacterInstance->Add_Object(this);
    m_fAngle = 90;
    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    return S_OK;
}

void CCharacter::Priority_Update(_float fTimeDelta)
{
    m_fMoving = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
}

void CCharacter::Update(_float fTimeDelta)
{
}

void CCharacter::Late_Update(_float fTimeDelta)
{
    SetUp_OnTerrain(m_pTransformCom, 0.f);
    Compute_CamDistance(m_pTransformCom->GetWorldState(WORLDSTATE::POSITION));
}

HRESULT CCharacter::Render()
{
    return S_OK;
 }

void CCharacter::Get_Damage(_uint iAtk)
{
    if (0 <= m_iHp) {
        m_iHp -= max(0, iAtk - m_iDef);
        if (0 >= m_iHp) {
            Death();
        }
        else {
            m_iHit -= max(0, iAtk - m_iDef);
            if (0 >= m_iHit) {
                Damage();
                m_iHit = m_iMaxHit;
            }
        }
    }
}

void CCharacter::SetDir()
{
    m_fMoving = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION) - m_fMoving;
    if (0.01f < abs(m_fMoving.x) + abs(m_fMoving.z)) {
        m_fAngle = D3DXToDegree(acosf(m_fMoving.x / sqrtf(powf(m_fMoving.x, 2) + powf(m_fMoving.z, 2))));
        if (0 < m_fMoving.z) {
            m_fAngle = 360.f - m_fAngle;
        }
    }
    _float3 look = m_pTransformCom->GetWorldState(WORLDSTATE::LOOK);
    _float lookAngle = D3DXToDegree(acosf(look.x / sqrtf(powf(look.x, 2) + powf(look.z, 2))));
    if (0 < look.z) {
        lookAngle = 360.f - lookAngle;
    }
    _float fAngle = lookAngle - m_fAngle;
    if (0 > fAngle) {
        fAngle += 360;
    }
    if ((0.f <= fAngle && fAngle < 45.f) || (fAngle < 360.f && fAngle >= 315.f)) {
        m_tMoveDIr = MOVE_DIR::MOVE_UP;
    }else if ((fAngle < 135.f && fAngle >= 45.f)) {
        m_tMoveDIr = MOVE_DIR::MOVE_LEFT;
    }
    else if (fAngle < 225.f && fAngle >= 135.f) {
        m_tMoveDIr = MOVE_DIR::MOVE_DOWN;
    }
    else if (fAngle < 315.f && fAngle >= 225.f) {
        m_tMoveDIr = MOVE_DIR::MOVE_RIGHT;
    }
}

void CCharacter::Free()
{
    m_pCharacterInstance->Remove_Object(this);
    m_pCharacterInstance->DestroyInstance();
    __super::Free();
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pAnimController);
    Safe_Release(m_pAnimTransformCom);
}
