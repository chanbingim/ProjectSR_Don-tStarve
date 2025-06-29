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
		//������ �����͸� ���� ����
	}

	return S_OK;
}

HRESULT CDropItemComponent::ComputeDropItem()
{
	//Math ���� ������ ���ؼ� � �Ѹ��� �����ϰ�
	//�״��� Ȯ���� ���� ����
	//������ ��� ���� �Ѹ��� ����

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
