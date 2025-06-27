#include "HeadUpDisplay.h"

#include "GameInstance.h"
#include "Player.h"

CHeadUpDisplay::CHeadUpDisplay(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUserInterface{ pGraphic_Device }
{
}

CHeadUpDisplay::CHeadUpDisplay(const CHeadUpDisplay& Prototype)
    : CUserInterface{ Prototype }
{
}

HRESULT CHeadUpDisplay::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CHeadUpDisplay::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    /*m_pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Player")));

    if (nullptr == m_pPlayer)
        return E_FAIL;*/

    return S_OK;
}

void CHeadUpDisplay::Priority_Update(_float fTimeDelta)
{

}

void CHeadUpDisplay::Update(_float fTimeDelta)
{  

}

void CHeadUpDisplay::Late_Update(_float fTimeDelta)
{
}

HRESULT CHeadUpDisplay::Render()
{
    return S_OK;
}


void CHeadUpDisplay::Free()
{
    __super::Free();
}
