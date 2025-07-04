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

   /* if (FAILED(ADD_Components()))
        return E_FAIL;*/

    m_pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Player"), 0));

    Safe_AddRef(m_pPlayer);

    if (nullptr == m_pPlayer)
        return E_FAIL;

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
    m_pArrowTexture_Com->Set_Texture(0);

    return S_OK;
}


HRESULT CHeadUpDisplay::ADD_Components()
{
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Arrow"),
        TEXT("Com_ArrowTexture"),
        reinterpret_cast<CComponent**>(&m_pArrowTexture_Com))))
        return E_FAIL;

    return S_OK;
}

void CHeadUpDisplay::Free()
{
    __super::Free();

    Safe_Release(m_pPlayer);
    Safe_Release(m_pArrowTexture_Com);
}
