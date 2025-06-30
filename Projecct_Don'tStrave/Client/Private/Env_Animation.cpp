#include "Env_Animation.h"

#include "Texture.h"

HRESULT CEnv_Animation::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEnv_Animation::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

HRESULT CEnv_Animation::Reset_StateData()
{
    __super::Reset_StateData();

    return S_OK;
}

void CEnv_Animation::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CEnv_Animation::Render()
{
    m_Frame.pAnimTexture->Set_Texture(m_Frame.iStartFrame);
}

CEnv_Animation* CEnv_Animation::Create(void* pArg)
{
    CEnv_Animation* pInstance = new CEnv_Animation();

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : ENVIORNMENT ANIMATION");
    }

    return pInstance;
}

void CEnv_Animation::Free()
{
    __super::Free();
}
