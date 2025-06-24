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
        m_pParentTransformCom = UI_MoveDesc->pParentTransformCom;
        Safe_AddRef(m_pParentTransformCom);
        m_pTransformCom = UI_MoveDesc->pTransformCom;
        Safe_AddRef(m_pTransformCom);
        m_fSize = UI_MoveDesc->fSize;
        m_fPoint = UI_MoveDesc->fPoint;
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

    m_pTransformCom->SetScale(_float3(m_fSize.x / 400.f, m_fSize.y / 400.f, 0.f));
    _float3		vPosition = m_pParentTransformCom->GetWorldState(WORLDSTATE::POSITION);
    _float3		vLook = m_pParentTransformCom->GetWorldState(WORLDSTATE::UP);

    vPosition += *D3DXVec3Normalize(&vLook, &vLook) * (m_fPoint.y / 400.f);

    vLook = m_pParentTransformCom->GetWorldState(WORLDSTATE::RIGHT);

    vPosition += *D3DXVec3Normalize(&vLook, &vLook) * (m_fPoint.x / 400.f);

    m_pTransformCom->SetPosition(vPosition);
    m_Frame.pAnimTexture->Set_Texture(m_Frame.iStartFrame);
}

bool CPlayerAnim::IsEnd()
{
    return m_Frame.iStartFrame == m_Frame.iEndFrame;
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
}
