#include "Character.h"
#include "GameInstance.h"

CCharacter::CCharacter(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CLandObject{ pGraphic_Device }
{
}

CCharacter::CCharacter(const CCharacter& Prototype)
    : CLandObject{ Prototype }
{
}

HRESULT CCharacter::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCharacter::Initialize(void* pArg)
{
    CLandObject::LANDOBJECT_DESC			Desc{};
    Desc.pLandTransform = static_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BackGround"), TEXT("Com_Transform")));
    Desc.pLandVIBuffer = static_cast<CVIBuffer*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BackGround"), TEXT("Com_VIBuffer")));

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    return S_OK;
}

void CCharacter::Priority_Update(_float fTimeDelta)
{
}

void CCharacter::Update(_float fTimeDelta)
{
}

void CCharacter::Late_Update(_float fTimeDelta)
{
    SetUp_OnTerrain(m_pTransformCom, 0.f);
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

void CCharacter::Free()
{
    __super::Free();
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pAnimController);
    Safe_Release(m_pAnimTransformCom);
}
