#include "QuestBoxEntry.h"

#include "GameInstance.h"
#include "EventButton.h"

#include "QuestManager.h"
#include "QuestData.h"

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
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(ADD_Buttons()))
		return E_FAIL;

	QuestBoxEntry_desc* Desc = static_cast<QuestBoxEntry_desc*>(pArg);
	m_fRelativePos = { Desc->fX, Desc->fY };
	m_pParentTransform = Desc->pParentTransform_Com;
	m_EntryIndex = Desc->EntryIndex;

	m_fX = 0;
	m_fY = 0;

	m_AcceptBut->SetClickEvent([&]() { EntryClickEvent(); });

	Safe_AddRef(m_pParentTransform);
	UpdatePosition();

	return S_OK;
}

void CQuestBoxEntry::Priority_Update(_float fTimeDelta)
{

}

void CQuestBoxEntry::Update(_float fTimeDelta)
{
	m_AcceptBut->Update(fTimeDelta);
	m_CancelBut->Update(fTimeDelta);
}

void CQuestBoxEntry::Late_Update(_float fTimeDelta)
{
	auto ParentPos = m_pParentTransform->GetWorldState(WORLDSTATE::POSITION);
	m_fX = ParentPos.x + m_fRelativePos.x;
	m_fY = ParentPos.y - m_fRelativePos.y;
	m_pTransform_Com->SetPosition({ m_fX, m_fY, ParentPos.z });

	m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);
	if (m_pQuestData)
	{
		size_t textSize = m_pQuestData->QuestTitle.length();
		m_FontRect = {
			   static_cast<long>(640.f + m_fX - (m_fSizeX * 0.5f) + 10),
			   static_cast<long>(360.f - m_fY - (m_fSizeY * 0.5f) + 10),
			   static_cast<long>(640.f + m_fX - (m_fSizeX * 0.5f) + (textSize * 20)),
			   static_cast<long>(360.f - m_fY - (m_fSizeY * 0.5f) + 70),
		};

		switch (m_QuestListType)
		{
		case 0:
		{
		
			m_AcceptBut->Late_Update(fTimeDelta);
		}
		break;
		case 1:
		{
			if (CQuestManager::GetInstance()->CheckAndApplyCompensation(m_pQuestData, false))
				m_AcceptBut->Late_Update(fTimeDelta);
			m_CancelBut->Late_Update(fTimeDelta);
		}
		break;
		}
	}
}

HRESULT CQuestBoxEntry::Render()
{
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());
	m_pTexture_Com->Set_Texture(0);
	m_pVIBuffer_Com->Render();

	D3DXCOLOR white = { 1.f,1.f,1.f,1.f };
	if (m_pQuestData)
		m_pGameInstance->Render_Font(TEXT("Font_25"), m_pQuestData->QuestTitle.c_str(), &m_FontRect, white, DT_LEFT);

	return S_OK;
}

void CQuestBoxEntry::SetQuestData(_uint QuestIndex, void* pArg)
{
	//버튼 호버는 나중에 만들거
	m_QuestListType = QuestIndex;
	if (nullptr == pArg)
		m_pQuestData = nullptr;
	else
	{
		m_pQuestData = static_cast<CQuestData*>(pArg);
		m_AcceptBut->ChangeButtonTex(QuestIndex * 2);
		m_CancelBut->ChangeButtonTex(QuestIndex * 2 + 1);
	}
}

void CQuestBoxEntry::EntryClickEvent()
{
	switch (m_QuestListType)
	{
	case  0 :
		AcceptQuest();
		break;

	case 1 :
		ClearQuest();
		break;
	}
}

HRESULT CQuestBoxEntry::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransform_Com)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer_Com)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Inventory"), TEXT("Com_Texture"), (CComponent**)&m_pTexture_Com)))
		return E_FAIL;

	return S_OK;
}

HRESULT CQuestBoxEntry::ADD_Buttons()
{
	CEventButton::BUTTON_DESC Desc;
	Desc.pParentTransform = m_pTransform_Com;
	Desc.fSizeX = 100.f;
	Desc.fSizeY = 40.f;

	Desc.fRelativeX = m_fSizeX * 0.6f - (Desc.fSizeX * 2) + 10;
	Desc.fRelativeY -= 10;
	m_AcceptBut = static_cast<CEventButton*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_EventButton"), &Desc));

	Desc.fRelativeX = m_fSizeX * 0.6f - Desc.fSizeX;
	m_CancelBut = static_cast<CEventButton*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_EventButton"), &Desc));

	return S_OK;
}

void CQuestBoxEntry::AcceptQuest()
{
	CQuestManager::GetInstance()->Accept_Quest(m_pQuestData->QuestID);
}

void CQuestBoxEntry::ClearQuest()
{
	CQuestManager::GetInstance()->Clear_Quest(m_pQuestData->QuestID);
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

	Safe_Release(m_AcceptBut);
	Safe_Release(m_CancelBut);

	Safe_Release(m_pParentTransform);
	Safe_Release(m_pTexture_Com);
	Safe_Release(m_pVIBuffer_Com);
	Safe_Release(m_pTransform_Com);
}
