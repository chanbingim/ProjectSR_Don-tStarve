#include "DropItemComponent.h"
#include "CMath.h"

CDropItemComponent::CDropItemComponent(LPDIRECT3DDEVICE9 pGraphic_Device) : 
	CComponent(pGraphic_Device)
{
}

CDropItemComponent::CDropItemComponent(const CDropItemComponent& rhs) :
	CComponent(rhs)
{
}

HRESULT CDropItemComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDropItemComponent::Initialize(void* pArg)
{
	m_vecItemID.reserve(5);
	m_DropItemVec.reserve(5);

	return S_OK;
}

HRESULT CDropItemComponent::ADD_ItemData(_uint iItemID, _float fProbability)
{
	auto iter = find_if(m_vecItemID.begin(), m_vecItemID.end(), [&](auto& pair)
				{
					return pair.first == iItemID ? true : false;
				});

	if (iter != m_vecItemID.end())
		return E_FAIL;

	m_vecItemID.emplace_back(iItemID, fProbability);
	return S_OK;
}

HRESULT CDropItemComponent::DropItem()
{
	ComputeDropItem();

	for (auto iter : m_DropItemVec)
	{
		//아이템 데이터를 통한 생성
	}

	return S_OK;
}

HRESULT CDropItemComponent::ComputeDropItem()
{
	//Math 쪽의 랜덤을 통해서 몇개 뿌릴지 선택하고
	//그다음 확률에 의해 선택
	//배지어 곡선을 통한 뿌리기 구현

	return S_OK;
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
}
