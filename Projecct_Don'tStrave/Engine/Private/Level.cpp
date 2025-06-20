#include "Level.h"
#include "GameInstance.h"

CLevel::CLevel(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iLevelID)
    : m_pGraphic_Device { pGraphic_Device }
    , m_pGameInstance { CGameInstance::GetInstance() }
    , m_iID { iLevelID }
{
    Safe_AddRef(m_pGameInstance);
    Safe_AddRef(m_pGraphic_Device);
}

HRESULT CLevel::Initialize()
{
    return S_OK;
}

void CLevel::Update(_float fTimeDelta)
{
}

HRESULT CLevel::Render()
{
    return S_OK;
}

void CLevel::Free()
{
    __super::Free();

    Safe_Release(m_pGameInstance);
    Safe_Release(m_pGraphic_Device);
}
