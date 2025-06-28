#include "PlayerAnim.h"
#include "GameInstance.h"
#include "CMath.h"

HRESULT CPlayerAnim::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPlayerAnim::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    PLAYER_DESC* UI_MoveDesc = static_cast<PLAYER_DESC*>(pArg);
    m_iMax = UI_MoveDesc->Frame.pAnimTexture->Get_Frame();
    UI_MoveDesc->Frame.iEndFrame = m_iMax - 1;
    UI_MoveDesc->Frame.fTimeRate = m_iMax / 30.f;
    if (UI_MoveDesc)
    {
        __super::Initialize(pArg);
        m_pParentTransformCom = UI_MoveDesc->pParentTransformCom;
        Safe_AddRef(m_pParentTransformCom);
        m_pTransformCom = UI_MoveDesc->pTransformCom;
        Safe_AddRef(m_pTransformCom);
        m_pVIBufferCom = UI_MoveDesc->pVIBufferCom;
        Safe_AddRef(m_pVIBufferCom);
        m_fSize = UI_MoveDesc->Frame.pAnimTexture->Get_Size();
    }
    return S_OK;
}

HRESULT CPlayerAnim::Reset_StateData()
{
    if (FAILED(__super::Reset_StateData()))
        return E_FAIL;

    return S_OK;
}

void CPlayerAnim::Tick(_float fTimeDelta)
{
       __super::Tick(fTimeDelta);
}

void CPlayerAnim::Render()
{
    m_pTransformCom->SetPosition(m_pParentTransformCom->GetWorldState(WORLDSTATE::POSITION));
    m_Frame.pAnimTexture->Set_Texture(0);
    m_pVIBufferCom->SetUV(m_fSize.z, m_fSize.x, m_fSize.y, m_Frame.iStartFrame, m_iMax);

    m_pTransformCom->SetScale(_float3(m_fSize.x / m_fSize.z / 200.f, m_fSize.y / ((m_iMax) / (_uint)m_fSize.z + ((m_iMax) % (_uint)m_fSize.z ? 1 : 0)) / 200.f, 0.f));
}

bool CPlayerAnim::IsEnd()
{
    return m_Frame.iStartFrame == m_Frame.iEndFrame;
}

bool CPlayerAnim::IsAttack(_uint iAttackFrame)
{
    return m_Frame.iStartFrame == iAttackFrame;
}

CPlayerAnim* CPlayerAnim::Create(void* pArg)
{
    CPlayerAnim* pInstance = new CPlayerAnim();
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREAT FAIL : PLAYER ANIMATION");
    }

    return pInstance;
}

void CPlayerAnim::Free()
{
    __super::Free();
    Safe_Release(m_pParentTransformCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
}
