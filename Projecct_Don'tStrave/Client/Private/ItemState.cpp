#include "ItemState.h"
#include "GameInstance.h"

HRESULT CItemState::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CItemState::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

HRESULT CItemState::Reset_StateData()
{
    __super::Reset_StateData();

    return S_OK;
}

void CItemState::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CItemState::Render()
{
    m_Frame.pAnimTexture->Set_Texture(m_Frame.iStartFrame);
}

CItemState* CItemState::Create(void* pArg)
{
    CItemState* pInstance = new CItemState();

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create CItemSate");
        return nullptr;
    }

    return pInstance;
}

void CItemState::Free()
{
    __super::Free();
}
