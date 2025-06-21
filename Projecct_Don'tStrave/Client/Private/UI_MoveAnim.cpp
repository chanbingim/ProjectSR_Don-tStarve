#include "UI_MoveAnim.h"
#include "GameInstance.h"
#include "CMath.h"

HRESULT CUI_MoveAnim::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_MoveAnim::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    UI_MOVE_DESC* UI_MoveDesc = static_cast<UI_MOVE_DESC*>(pArg);
    if (UI_MoveDesc)
    {
        __super::Initialize(pArg);
        m_pTransformCom = UI_MoveDesc->pTransformCom;
        Safe_AddRef(m_pTransformCom);

        m_fDurTime = UI_MoveDesc->fDurTime;
        m_StartPoint = UI_MoveDesc->StartPoint;
        m_EndPoint = UI_MoveDesc->EndPoint;
        m_LerpData.m_bLerpMove = true;
    }

    return S_OK;
}

HRESULT CUI_MoveAnim::Reset_StateData()
{
    if (FAILED(__super::Reset_StateData()))
        return E_FAIL;

    return S_OK;
}

void CUI_MoveAnim::Tick(_float fTimeDelta)
{
    _float3 NewVec3 = CMath::LerpVec3(m_StartPoint, m_EndPoint, m_fAccTime / m_fDurTime);
    m_pTransformCom->SetPosition(NewVec3);

    if (!m_LerpData.m_bLerpMove)
    {
        __super::Tick(fTimeDelta);
    }
    else
    {
        m_fAccTime += fTimeDelta;
        if (NewVec3 == m_EndPoint)
            m_LerpData.m_bLerpMove = false;
    }
}

void CUI_MoveAnim::Render()
{
    m_Frame.pAnimTexture->Set_Texture(m_Frame.iStartFrame);
}

CUI_MoveAnim* CUI_MoveAnim::Create(void* pArg)
{
    CUI_MoveAnim* pInstance = new CUI_MoveAnim();
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREAT FAIL : UI MOVE ANIMATION");
    }

    return pInstance;
}

void CUI_MoveAnim::Free()
{
    __super::Free();
    Safe_Release(m_pTransformCom);
}
