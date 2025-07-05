#include "DropItemEnviornment.h"

#include "GameInstance.h"
#include "DropItemComponent.h"

CDropItemEnviornment::CDropItemEnviornment(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CEnviornment_Object(pGraphic_Device)
{
}

CDropItemEnviornment::CDropItemEnviornment(const CDropItemEnviornment& rhs) :
    CEnviornment_Object(rhs)
{
}

HRESULT CDropItemEnviornment::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CDropItemEnviornment::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CDropItemEnviornment::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CDropItemEnviornment::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CDropItemEnviornment::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CDropItemEnviornment::Render()
{
    __super::Render();

    return S_OK;
}

HRESULT CDropItemEnviornment::CreateDropItem(const _float3& Pos)
{
    if (m_pDropItem_Com)
    {
        m_pDropItem_Com->DropItem(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Material_Item"),
                                  EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Item"), Pos);
        return S_OK;
    }

    return E_FAIL;
}

CGameObject* CDropItemEnviornment::Clone(void* pArg)
{
    return nullptr;
}

void CDropItemEnviornment::Free()
{
    __super::Free();

    Safe_Release(m_pDropItem_Com);
}
