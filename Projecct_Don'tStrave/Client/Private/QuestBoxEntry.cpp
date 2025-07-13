#include "QuestBoxEntry.h"

#include "GameInstance.h"
#include "EventButton.h"

CQuestBoxEntry::CQuestBoxEntry(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CUserInterface(pGraphic_Device)
{
}

CQuestBoxEntry::CQuestBoxEntry(const CQuestBoxEntry& rhs) :
	CUserInterface(rhs)
{
}

HRESULT CQuestBoxEntry::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CQuestBoxEntry::Initialize(void* pArg)
{
	//엔트리 박스의 사이즈는 안에서 결정해 주자
	if (FAILED(__super::Initialize(&pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	return S_OK;
}

void CQuestBoxEntry::Priority_Update(_float fTimeDelta)
{
	m_AcceptBut->Priority_Update(fTimeDelta);
	m_CancelBut->Priority_Update(fTimeDelta);
}

void CQuestBoxEntry::Update(_float fTimeDelta)
{
	m_AcceptBut->Update(fTimeDelta);
	m_CancelBut->Update(fTimeDelta);
}

void CQuestBoxEntry::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);
	//m_pQuestIcon_Com->Render();

	m_AcceptBut->Late_Update(fTimeDelta);
	m_CancelBut->Late_Update(fTimeDelta);
}

HRESULT CQuestBoxEntry::Render()
{
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());
	m_pTexture_Com->Set_Texture(0);
	m_pVIBuffer_Com->Render();

	m_AcceptBut->Render();
	m_CancelBut->Render();

	return S_OK;
}

HRESULT CQuestBoxEntry::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransform_Com)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer_Com)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Quest"), TEXT("Com_Texture"), (CComponent**)&m_pTexture_Com)))
		return E_FAIL;

	return S_OK;
}

CQuestBoxEntry* CQuestBoxEntry::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CQuestBoxEntry* pInstance = new CQuestBoxEntry(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CLONE FAILED : BOX ENTRY");
	}

	return pInstance;
}

CGameObject* CQuestBoxEntry::Clone(void* pArg)
{
	CQuestBoxEntry* pInstance = new CQuestBoxEntry(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("CLONE FAILED : BOX ENTRY");
	}

	return pInstance;
}

void CQuestBoxEntry::Free()
{
	__super::Free();

	Safe_Release(m_pTexture_Com);
	Safe_Release(m_pVIBuffer_Com);
	Safe_Release(m_pTransform_Com);
}
