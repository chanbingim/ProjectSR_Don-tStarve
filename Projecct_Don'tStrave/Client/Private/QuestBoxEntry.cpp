#include "QuestBoxEntry.h"

#include "GameInstance.h"
#include "EventButton.h"

#include "QuestManager.h"
#include "QuestData.h"
#include "Script.h"

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
	m_CancelBut->SetClickEvent([&]() { EntryCancelClickEvent(); });

	m_AcceptBut->ChangeButtonTex(0);
	m_CancelBut->ChangeButtonTex(2);

	Safe_AddRef(m_pParentTransform);
	UpdatePosition(0.9f);

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
			m_IsShowAcceptBut = false;
			m_CancelBut->Late_Update(fTimeDelta);
			if (CQuestManager::GetInstance()->CheckAndApplyCompensation(m_pQuestData, false))
			{
				m_AcceptBut->Late_Update(fTimeDelta);
				m_IsShowAcceptBut = true;
			}
		
		}
		break;
		}
	}
}

HRESULT CQuestBoxEntry::Render()
{
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

	D3DXCOLOR Black = { 0.f,0.f,0.f,1.f };
	m_pTexture_Com->Set_Texture(0);
	m_pVIBuffer_Com->Render();

	if (m_pQuestData)
	{
		m_pGameInstance->Render_Font(TEXT("Font_25"), m_pQuestData->QuestTitle.c_str(), &m_FontRect, Black, DT_LEFT);
		switch (m_QuestListType)
		{
		case 0:
		{
			m_AcceptBut->Render();
		}
		break;
		case 1:
		{
			if (m_IsShowAcceptBut)
				m_AcceptBut->Render();
			m_CancelBut->Render();
		}
		break;
		}
	}
	
	return S_OK;
}

void CQuestBoxEntry::SetQuestData(_uint QuestIndex, void* pArg)
{
	m_QuestListType = QuestIndex;
	if (nullptr == pArg)
		m_pQuestData = nullptr;
	else
	{
		m_pQuestData = static_cast<CQuestData*>(pArg);
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

void CQuestBoxEntry::EntryCancelClickEvent()
{
	switch (m_QuestListType)
	{
	case 1:
		ShowScriptData();
		break;
	}
}

HRESULT CQuestBoxEntry::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransform_Com)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer_Com)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_QuestEntryFrame"), TEXT("Com_Texture"), (CComponent**)&m_pTexture_Com)))
		return E_FAIL;

	return S_OK;
}

HRESULT CQuestBoxEntry::ADD_Buttons()
{
	CEventButton::BUTTON_DESC Desc;
	Desc.pParentTransform = m_pTransform_Com;
	Desc.fSizeX = 50.f;
	Desc.fSizeY = 50.f;

	Desc.fRelativeX = m_fSizeX * 0.6f - (Desc.fSizeX * 2) + 10;
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

void CQuestBoxEntry::ShowScriptData()
{
	CScript::SCRIPT_DESC Scirpt_Desc;
	Scirpt_Desc.DataScript.push_back(L"중퇴하고 싶지 않다면 내말을 잘 따르도록 해 !!");

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY),
		TEXT("Prototype_GameObject_ScriptUI"), EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_UserInterface"), &Scirpt_Desc)))
		return;
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
