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
    if (UI_MoveDesc)
    {
        __super::Initialize(pArg);
        m_pTransformCom = UI_MoveDesc->pTransformCom;
        Safe_AddRef(m_pTransformCom);
        m_StartPoint = UI_MoveDesc->StartPoint;
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
    m_pTransformCom->SetPosition(m_pTransformCom->GetWorldState(WORLDSTATE::POSITION) + m_StartPoint);
    m_Frame.pAnimTexture->Set_Texture(m_Frame.iStartFrame);
}

void CPlayerAnim::Render_End()
{
    m_pTransformCom->SetPosition(m_pTransformCom->GetWorldState(WORLDSTATE::POSITION) - m_StartPoint);
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
    Safe_Release(m_pTransformCom);
}
