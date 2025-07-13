#include "DropItemComponent.h"

#include "GameInstance.h"
#include "Item_Manager.h"
#include "CMath.h"

CDropItemComponent::CDropItemComponent(LPDIRECT3DDEVICE9 pGraphic_Device) : 
	CComponent(pGraphic_Device),
	m_pItem_Manager(CItem_Manager::GetInstance())
{
	Safe_AddRef(m_pItem_Manager);
}

CDropItemComponent::CDropItemComponent(const CDropItemComponent& rhs) :
	CComponent(rhs),
	m_pItem_Manager(CItem_Manager::GetInstance())
{
	Safe_AddRef(m_pItem_Manager);
}

HRESULT CDropItemComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDropItemComponent::Initialize(void* pArg)
{
	m_vecItemID.reserve(5);

	return S_OK;
}

HRESULT CDropItemComponent::ADD_ItemData(_uint iItemID, _uint  iMaxCnt)
{
	auto iter = find_if(m_vecItemID.begin(), m_vecItemID.end(), [&](auto& pair)
				{
					return pair.first == iItemID ? true : false;
				});

	if (iter != m_vecItemID.end())
		return E_FAIL;

	m_vecItemID.emplace_back(iItemID, iMaxCnt);
	return S_OK;
}

HRESULT CDropItemComponent::DropItem(_uint iLevelID, const _wstring& PrototypeTag, _uint iLayerID, const _wstring& LayerTag, _float3& Point)
{
	if (nullptr == m_pItem_Manager)
		return E_FAIL;

	for (auto iter : m_vecItemID)
	{
		Point += m_pItem_Manager->Get_Position(static_cast<_uint>(m_pGameInstance->Random(0, 7)));
		auto Data = CItem_Manager::GetInstance()->Get_ItemData(iter.first);
		ITEM_DESC ItemDesc = {};
		ItemDesc.fDurability = 100.f;
		ItemDesc.eSlot = Data.eSlot;
		ItemDesc.iItemID = Data.iItemID;
		ItemDesc.iNumItem = rand() % iter.second + 1;
		ItemDesc.eItemType = Data.eItemType;
		ItemDesc.vPosition = Point;
		ItemDesc.iItemEffect = m_iCreateEffect;
		m_pGameInstance->Add_GameObject_ToLayer(iLevelID, PrototypeTag, iLayerID, LayerTag, &ItemDesc);
	}

	return S_OK;
}

void CDropItemComponent::SetCreateEffect(_uint EffectType)
{
	m_iCreateEffect = EffectType;
}

CDropItemComponent* CDropItemComponent::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDropItemComponent* pInstance = new CDropItemComponent(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CREATE FAIL : DROP ITEM COMPONENT");
	}
	return pInstance;
}

CComponent* CDropItemComponent::Clone(void* pArg)
{
	CDropItemComponent* pInstance = new CDropItemComponent(*this);
	if(FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("CLONE FAIL : DROP ITEM COMPONENT");
	}
	return pInstance;
}

void CDropItemComponent::Free()
{
	__super::Free();

	Safe_Release(m_pItem_Manager);
}
