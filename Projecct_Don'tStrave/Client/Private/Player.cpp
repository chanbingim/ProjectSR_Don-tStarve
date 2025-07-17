#include "Player.h"
#include "GameInstance.h"
#include "Monster.h"
#include "Enviornment_Object.h"
#include "XML_Manager.h"
#include "DamageEffectUI.h"
#include "Fire.h"
#include "Item.h"
#include "MonsterData_Manager.h"
#include "QuestManager.h"
#include "CharacterManager.h"
#include "SlotFrame.h"
#include "Slot.h"
#include "PortalObject.h"
#include "QuestFrameUI.h"
#include "Item_Manager.h"
#include "Inventory.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCharacter{ pGraphic_Device }
{
}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CCharacter{ Prototype },
	m_tGhostImageVec{ Prototype.m_tGhostImageVec },
	m_tWigfridAnimation{ Prototype.m_tWigfridAnimation },
	m_tWigfridImageVec{ Prototype.m_tWigfridImageVec },
	m_tWigfridGhostImageVec{ Prototype.m_tWigfridGhostImageVec },
	m_tItemAnimation{ Prototype.m_tItemAnimation },
	m_tMakeImageVec{ Prototype.m_tMakeImageVec },
	m_tMakeAnimation{ Prototype.m_tMakeAnimation }
{
	for (int i = 0; i < ENUM_CLASS(SWAPOBJECT::NONE); ++i) {

		m_tItemImageVec[i] = Prototype.m_tItemImageVec[i];
	}
}

HRESULT CPlayer::Initialize_Prototype()
{
	auto XML_Instance = CXML_Manager::GetInstance();

	XML_Instance->AddTexture("../Bin/Resources/Textures/Player/Wilson/wilson_idle.scml", L"../Bin/Resources/Textures/Player/Wilson/", &m_tImageVec);
	XML_Instance->AddTexture("../Bin/Resources/Textures/Player/Wilson/ghost_wilson.scml", L"../Bin/Resources/Textures/Player/Wilson/", &m_tGhostImageVec);
	XML_Instance->AddTexture("../Bin/Resources/Textures/Particles/Make/make_effect.scml", L"../Bin/Resources/Textures/Particles/Make/", &m_tMakeImageVec);

	XML_Instance->AddTexture("../Bin/Resources/Textures/Player/Item/axe.scml",
		L"../Bin/Resources/Textures/Player/Item/", &m_tItemImageVec[ENUM_CLASS(SWAPOBJECT::AXE)]);
	XML_Instance->AddTexture("../Bin/Resources/Textures/Player/Item/goldenaxe.scml",
		L"../Bin/Resources/Textures/Player/Item/", &m_tItemImageVec[ENUM_CLASS(SWAPOBJECT::GOLDAXE)]);
	XML_Instance->AddTexture("../Bin/Resources/Textures/Player/Item/pickaxe.scml",
		L"../Bin/Resources/Textures/Player/Item/", &m_tItemImageVec[ENUM_CLASS(SWAPOBJECT::PICKAXE)]);
	XML_Instance->AddTexture("../Bin/Resources/Textures/Player/Item/goldenpickaxe.scml",
		L"../Bin/Resources/Textures/Player/Item/", &m_tItemImageVec[ENUM_CLASS(SWAPOBJECT::GOLDPICKAXE)]);
	XML_Instance->AddTexture("../Bin/Resources/Textures/Player/Item/shovel.scml",
		L"../Bin/Resources/Textures/Player/Item/", &m_tItemImageVec[ENUM_CLASS(SWAPOBJECT::SHOVEL)]);
	XML_Instance->AddTexture("../Bin/Resources/Textures/Player/Item/goldenshovel.scml",
		L"../Bin/Resources/Textures/Player/Item/", &m_tItemImageVec[ENUM_CLASS(SWAPOBJECT::GOLDSHOVEL)]);
	XML_Instance->AddTexture("../Bin/Resources/Textures/Player/Item/spear.scml",
		L"../Bin/Resources/Textures/Player/Item/", &m_tItemImageVec[ENUM_CLASS(SWAPOBJECT::SPEAR)]);
	XML_Instance->AddTexture("../Bin/Resources/Textures/Player/Item/spear_lightning.scml",
		L"../Bin/Resources/Textures/Player/Item/", &m_tItemImageVec[ENUM_CLASS(SWAPOBJECT::LIGHTNINGSPEAR)]);
	XML_Instance->AddTexture("../Bin/Resources/Textures/Player/Item/torch.scml",
		L"../Bin/Resources/Textures/Player/Item/", &m_tItemImageVec[ENUM_CLASS(SWAPOBJECT::TORCH)]);

	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Item/axe.scml", &m_tItemAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Item/pickaxe.scml", &m_tItemAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Item/shovel.scml", &m_tItemAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Item/attack.scml", &m_tItemAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Item/spear.scml", &m_tItemAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Item/idles.scml", &m_tItemAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Item/basic.scml", &m_tItemAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Item/actions.scml", &m_tItemAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Item/itemactions.scml", &m_tItemAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Item/eat.scml", &m_tItemAnimation);

	XML_Instance->LoadScml("../Bin/Resources/Textures/Particles/Make/make_effect.scml", &m_tMakeAnimation);

	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Wilson/wilson_idle.scml", &m_tAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Wilson/wilson_atk.scml", &m_tAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Wilson/wilson_item.scml", &m_tAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Wilson/wilson_run.scml", &m_tAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Wilson/wilson_action.scml", &m_tAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Wilson/wilson_axe.scml", &m_tAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Wilson/wilson_pickaxe.scml", &m_tAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Wilson/wilson_eat.scml", &m_tAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Wilson/wilson_spear.scml", &m_tAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Wilson/ghost_wilson.scml", &m_tAnimation);

	XML_Instance->AddTexture("../Bin/Resources/Textures/Player/Wigfrid/wathgrithr_idle.scml", L"../Bin/Resources/Textures/Player/Wigfrid/", &m_tWigfridImageVec);
	XML_Instance->AddTexture("../Bin/Resources/Textures/Player/Wigfrid/ghost_wathgrithr.scml", L"../Bin/Resources/Textures/Player/Wigfrid/", &m_tWigfridGhostImageVec);

	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Wigfrid/wathgrithr_idle.scml", &m_tWigfridAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Wigfrid/wathgrithr_atk.scml", &m_tWigfridAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Wigfrid/wathgrithr_item.scml", &m_tWigfridAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Wigfrid/wathgrithr_run.scml", &m_tWigfridAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Wigfrid/wathgrithr_action.scml", &m_tWigfridAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Wigfrid/wathgrithr_axe.scml", &m_tWigfridAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Wigfrid/wathgrithr_pickaxe.scml", &m_tWigfridAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Wigfrid/wathgrithr_eat.scml", &m_tWigfridAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Wigfrid/wathgrithr_spear.scml", &m_tWigfridAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Wigfrid/ghost_wathgrithr.scml", &m_tWigfridAnimation);

	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	for (auto& ItemImageVec : m_tItemImageVec) {
		for (auto& folder : ItemImageVec) {
			for (auto& file : folder.tFilesVec) {
				if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
					TEXT("Com_" + file.szName), reinterpret_cast<CComponent**>(&file.pTexture))))
				{
					m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
						CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, file.szName.c_str()));
					__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
						TEXT("Com_" + file.szName), reinterpret_cast<CComponent**>(&file.pTexture));
				}
			}
		}
	}

	for (auto& folder : m_tMakeImageVec) {
		for (auto& file : folder.tFilesVec) {
			if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
				TEXT("Com_" + file.szName), reinterpret_cast<CComponent**>(&file.pTexture))))
			{
				m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
					CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, file.szName.c_str()));
				__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
					TEXT("Com_" + file.szName), reinterpret_cast<CComponent**>(&file.pTexture));
			}
		}
	}
	m_fAngle = 90;
	PLAYER_DESC data = *static_cast<PLAYER_DESC*>(pArg);
	m_pPlayer = new PLAYER_DATA;

	m_pPlayer->iId = data.iId;
	m_pPlayer->iMaxHp = data.iMaxHp;
	m_pPlayer->iMaxHunger = data.iMaxHunger;
	m_pPlayer->iMaxMental = data.iMaxMental;
	m_pPlayer->iHp = data.iMaxHp;
	m_pPlayer->iHunger = data.iMaxHunger;
	m_pPlayer->iMental = data.iMaxMental;
	m_pPlayer->tItem = SWAPOBJECT::NONE;
	m_pPlayer->fSpeed = 2.f;

	m_pPlayer->iTemp = 0;
	m_pPlayer->fAtkRatio = data.fAtk;
	m_pPlayer->fDefRatio = data.fDef;
	m_pPlayer->iAtk = 50;
	m_pPlayer->iDef = 0;
	m_pPlayer->iMaxHit = 10;
	m_pPlayer->iHit = 10;
	m_pPlayer->fPos = data.fPos;
	m_pPlayer->pWorkObject = nullptr;
	m_tSwapItem = m_pPlayer->tItem;
	SetAnimation(DIR::DIR_END, MOTION::BUCKED);

	switch (m_pPlayer->iId)
	{
	case 200:
		LoadImageFile();

		for (auto& folder : m_tGhostImageVec) {
			for (auto& file : folder.tFilesVec) {
				if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
					TEXT("Com_" + file.szName), reinterpret_cast<CComponent**>(&file.pTexture))))
				{
					m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
						CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, file.szName.c_str()));
					__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
						TEXT("Com_" + file.szName), reinterpret_cast<CComponent**>(&file.pTexture));
				}
			}
		}
		break;
	case 201:
		for (auto& folder : m_tWigfridImageVec) {
			for (auto& file : folder.tFilesVec) {
				if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
					TEXT("Com_" + file.szName), reinterpret_cast<CComponent**>(&file.pTexture))))
				{
					m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
						CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, file.szName.c_str()));
					__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
						TEXT("Com_" + file.szName), reinterpret_cast<CComponent**>(&file.pTexture));
				}
			}
		}

		for (auto& folder : m_tWigfridGhostImageVec) {
			for (auto& file : folder.tFilesVec) {
				if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
					TEXT("Com_" + file.szName), reinterpret_cast<CComponent**>(&file.pTexture))))
				{
					m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
						CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, file.szName.c_str()));
					__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
						TEXT("Com_" + file.szName), reinterpret_cast<CComponent**>(&file.pTexture));
				}
			}
		}
		break;
	}


	m_pTorchFire = dynamic_cast<CTorchFire*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_TorchFire"), pArg));



	m_pChar = m_pPlayer;

	m_pTransformCom->SetPosition(m_pPlayer->fPos);
	m_pCollision_Com->SetCollisionSize({ 0.2f, 0.f ,0.f });
	m_bLightningAttack = false;
	m_bControll = true;
	m_bCol = false;
	m_bEat = false;
	m_tDamage.Attacker = this;
	m_fFightTime = 30.f;
	m_iDarkTime = 0;

	m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& _Dir) { BeginHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& _Dir) { OverlapHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& _Dir) { EndHitActor(HitActor, _Dir); });
	CCharacterManager::GetInstance()->SetPlayer(this);
	return S_OK;
}

HRESULT CPlayer::Initialize_Late()
{
	__super::Initialize_Late();
	m_pTorchFire->Initialize_Late();
	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	if (m_pGameInstance->KeyDown(VK_F4))
	{
		CQuestManager::GetInstance()->ClearCheatFunc();
	}

	m_pTorchFire->Priority_Update(fTimeDelta);
	if (0 < m_pPlayer->iHp) {
		m_fHungTime += fTimeDelta / 2;
		m_fFightTime += fTimeDelta;
		if (1 <= m_fHungTime) {
			m_pPlayer->iHunger = max(m_pPlayer->iHunger - 1, 0);
			if (30 < *m_pTime) {
				m_pPlayer->iMental = max(m_pPlayer->iMental - 1, 0);
				if (4 <= m_pGameInstance->Get_NearLight()) {
					m_pPlayer->iMental = max(m_pPlayer->iMental - 1, 0);
					if (50 < *m_pTime) {
						m_iDarkTime += 1;
						if (5.f <= m_iDarkTime) {
							DamageBaseDesc damage;
							damage.Attacker = this;
							damage.Damage = 1000;
							Damage(&damage);
						}
					}
					else {
						m_iDarkTime = 0.f;
					}
				}
			}
			if (m_fFightTime < 15) {
				switch (m_pPlayer->iId)
				{
				case 200:
					m_pPlayer->iMental = max(m_pPlayer->iMental - 1, 0);
					break;
				case 201:
					m_pPlayer->iMental = min(m_pPlayer->iMental + 2, m_pPlayer->iMaxMental);
					break;
				}
			}
			if (30 == m_pPlayer->iHunger && MOTION::IDLE == m_tMotion) {
				SetAnimation(DIR::DIR_END, MOTION::HUNGRY);
			}
			m_fHungTime = 0;
		}
		if (m_pPlayer->iMaxMental / 2 <= m_pPlayer->iMental) {
			m_bCrawling = true;
		}
		else if (m_bCrawling && m_pPlayer->iMaxMental / 2 > m_pPlayer->iMental) {
			m_bCrawling = false;
			MONSTER_DESC data = CMonsterData_Manager::GetInstance()->Get_MonsterData(109);
			size_t max = (rand() % 3) + 1;
			data.fPos = m_pPlayer->fPos;
			data.fPos.x += (rand() % 5) - (rand() % 5);
			data.fPos.z += (rand() % 5) - (rand() % 5);
			m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), data.strPath.c_str(), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Monster"), &data);
		}
		if (m_pPlayer->iMaxMental / 3 <= m_pPlayer->iMental) {
			m_bTerrorbeak = true;
		}
		else if (m_bTerrorbeak && m_pPlayer->iMaxMental / 3 > m_pPlayer->iMental) {
			m_bTerrorbeak = false;
			MONSTER_DESC data = CMonsterData_Manager::GetInstance()->Get_MonsterData(110);
			size_t max = (rand() % 3) + 1;
			data.fPos = m_pPlayer->fPos;
			data.fPos.x += (rand() % 5) - (rand() % 5);
			data.fPos.z += (rand() % 5) - (rand() % 5);
			m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), data.strPath.c_str(), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Monster"), &data);
		}
		if (SWAPOBJECT::TORCH == m_tSwapItem) {
			m_pTorchFire->Update_TorchFire(true);
		}
		else {
			m_pTorchFire->Update_TorchFire(false);
		}
		m_tDamage.Attacker = this;
		m_tDamage.Damage = (_int)(m_pPlayer->iAtk * m_pPlayer->fAtkRatio);
		if (SWAPOBJECT::TORCH == m_tSwapItem) {
			m_tDamage.DamageType = ATTACK_TYPE::FIRE;
		}
		else if (SWAPOBJECT::LIGHTNINGSPEAR == m_tSwapItem) {
			m_tDamage.DamageType = ATTACK_TYPE::LIGHTNING;
		}
		else {
			m_tDamage.DamageType = ATTACK_TYPE::ATTACK;
		}
		if ((MOTION::EAT == m_tMotion || MOTION::FASTEAT == m_tMotion) && m_bEat && 450 <= m_fAniTime) {

			switch (m_tMotion)
			{
			case MOTION::EAT:
				m_pGameInstance->Manager_PlaySound(L"eat_2.wav", CHANNELID::PLAYER_SOUND, 0.8f);
				break;
			case MOTION::FASTEAT:
				m_pGameInstance->Manager_PlaySound(L"eat_1.wav", CHANNELID::PLAYER_SOUND, 0.8f);
				break;
			}
			m_bEat = false;
		}
		if (MOTION::DEATH1 != m_tMotion && MOTION::DEATH2 != m_tMotion && (0 >= m_pPlayer->iHp || 0 >= m_pPlayer->iHunger)) {
			Dead();
		}
	}
	else {
		if(m_pPlayer->pWorkObject && (!dynamic_cast<CEnviornment_Object*>(m_pPlayer->pWorkObject) || CEnviornment_Object::Enviornment_TYPE::RESERREECTION != dynamic_cast<CEnviornment_Object*>(m_pPlayer->pWorkObject)->GetEnviornMentType()))
			m_pPlayer->pWorkObject = nullptr;
	}
	m_bLightningAttack = false;
	switch (m_tMotion)
	{
	case MOTION::IDLE_TO_BUILD:
	case MOTION::BUILD:
		break;
	case MOTION::ATTACK:
	case MOTION::IDLE_TO_SPEAR:
	case MOTION::SPEAR:
	case MOTION::IDLE_TO_AXE:
	case MOTION::AXE:
	case MOTION::IDLE_TO_PICKAXE:
	case MOTION::PICKAXE:
	case MOTION::GHOST_DISSIPATE:
		m_pSlot = nullptr;
		m_tItem.iItemID = 0;
		break;
	default:
		m_pSlot = nullptr;
		m_tItem.iItemID = 0;
		m_bAttack = false;
		break;
	}
}

void CPlayer::Update(_float fTimeDelta)
{
	m_pTorchFire->Update(fTimeDelta);
	if (m_pGameInstance->KeyDown('P'))
	{
		auto GameObject = m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Gameplay_Quest_UI"));
		if (GameObject)
		{
			auto Quest = dynamic_cast<CQuestFrameUI*>(GameObject);
			if (Quest)
			{
				Quest->ToggleObject();
			}
		}
	}
	switch (m_tMotion)
	{
	case CPlayer::IDLE:
	case CPlayer::IDLE_TO_RUN:
	case CPlayer::ITEM_IN:
	case CPlayer::ITEM_OUT:
	case CPlayer::RUN:
	case CPlayer::RUN_TO_IDLE:
	case CPlayer::IDLE_TO_BUILD:
	case CPlayer::BUILD:
	case CPlayer::BUILD_TO_IDLE:
	case CPlayer::HUNGRY:
	case CPlayer::EAT:
	case CPlayer::FASTEAT:
	case CPlayer::IDLE_TO_AXE:
	case CPlayer::AXE:
	case CPlayer::IDLE_TO_PICKAXE:
	case CPlayer::PICKAXE:
	case CPlayer::PICKAXE_TO_IDLE:
	case CPlayer::IDLE_TO_SHOVEL:
	case CPlayer::SHOVEL:
	case CPlayer::SHOVEL_TO_IDLE:
	case CPlayer::IDLE_TO_ATTACK:
	case CPlayer::ATTACK:
	case CPlayer::IDLE_TO_SPEAR:
	case CPlayer::SPEAR:
	case CPlayer::PICKUP:
	case CPlayer::GIVE:
	case CPlayer::DAMAGE:
		switch (m_tMoveDIr)
		{
		case MOVE_DIR::MOVE_DOWN:
			m_tDir = DIR::DOWN;
			break;
		case MOVE_DIR::MOVE_LEFT:
		case MOVE_DIR::MOVE_RIGHT:
			m_tDir = DIR::SIDE;
			break;
		case MOVE_DIR::MOVE_UP:
			m_tDir = DIR::UP;
			break;
		}
		SetAnimation(m_tDir, m_tMotion);
	}
	if (m_tSwapItem != m_pPlayer->tItem) {
		if (m_bControll) {
			if (m_tSwapItem != SWAPOBJECT::NONE) {
				SetAnimation(m_tDir, MOTION::ITEM_IN);
			}
			else {
				m_tSwapItem = m_pPlayer->tItem;
				SetAnimation(m_tDir, MOTION::ITEM_OUT);
			}
			m_bControll = false;
			m_pPlayer->pWorkObject = nullptr;
		}
		else if(MOTION::ITEM_IN != m_tMotion && MOTION::ITEM_OUT != m_tMotion){
			m_pPlayer->tItem = m_tSwapItem;
		}
	}
	if (m_bControll) {
		if (MOTION::BUCKED == m_tMotion) {
			m_fAniTime = 0;
			m_bControll = false;
		}
		if (m_pGameInstance->KeyPressed('W') || m_pGameInstance->KeyPressed('S') || m_pGameInstance->KeyPressed('D') || m_pGameInstance->KeyPressed('A'))
		{
			switch (m_tMotion)
			{
			case MOTION::GHOST_APPEAR:
			case MOTION::GHOST_IDLE:
			case MOTION::GHOST_DISSIPATE:
				break;
			case MOTION::IDLE_TO_RUN:
				if (m_iLength <= m_fAniTime) {
					SetAnimation(m_tDir, MOTION::RUN);
				}
			case MOTION::RUN:
				break;
			default:
				SetAnimation(m_tDir, MOTION::IDLE_TO_RUN);
				break;
			}
			_float3		vMove = { 0.f ,0.f ,0.f };
			if (GetKeyState('W') & 0x8000) {
				_float3		vLook = m_pTransformCom->GetWorldState(WORLDSTATE::LOOK);
				vMove += *D3DXVec3Normalize(&vLook, &vLook);
			}
			if (GetKeyState('S') & 0x8000)
			{
				_float3		vLook = m_pTransformCom->GetWorldState(WORLDSTATE::LOOK);

				vMove -= *D3DXVec3Normalize(&vLook, &vLook);
			}
			if (GetKeyState('A') & 0x8000)
			{
				_float3		vLook = m_pTransformCom->GetWorldState(WORLDSTATE::RIGHT);

				vMove -= *D3DXVec3Normalize(&vLook, &vLook);
			}
			if (GetKeyState('D') & 0x8000)
			{
				_float3		vLook = m_pTransformCom->GetWorldState(WORLDSTATE::RIGHT);

				vMove += *D3DXVec3Normalize(&vLook, &vLook);
			}
			m_pPlayer->pWorkObject = nullptr;
			m_pPlayer->fPos += *D3DXVec3Normalize(&vMove, &vMove) * m_pPlayer->fSpeed * fTimeDelta;
			m_pTransformCom->SetPosition(m_pPlayer->fPos);
		}
		else {
			if (nullptr != m_pPlayer->pWorkObject && !m_bCol) {


				_float3 transform = m_pPlayer->pWorkObject->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
				_float distance = D3DXVec3Length(&transform);
				if (0.4f > distance) {
					if (dynamic_cast<CItem*>(m_pPlayer->pWorkObject)) {
						SetAnimation(m_tDir, MOTION::PICKUP);
						m_pGameInstance->Manager_PlaySound(L"getitem.wav", CHANNELID::SOUND_ITEM, 0.5f);
						dynamic_cast<CItem*>(m_pPlayer->pWorkObject)->EnterInvenTory();
						m_pPlayer->pWorkObject = nullptr;
						return;
					}
				}
				_float3 move = m_pPlayer->pWorkObject->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pPlayer->fPos;
				m_pPlayer->fPos += *D3DXVec3Normalize(&move, &move) * m_pPlayer->fSpeed * fTimeDelta;
				m_pTransformCom->SetPosition(m_pPlayer->fPos);
				switch (m_tMotion)
				{
				case MOTION::GHOST_APPEAR:
				case MOTION::GHOST_IDLE:
				case MOTION::GHOST_DISSIPATE:
					break;
				case MOTION::IDLE_TO_RUN:
					if (m_iLength <= m_fAniTime) {
						SetAnimation(m_tDir, MOTION::RUN);
					}
				case MOTION::RUN:
					break;
				default:
					SetAnimation(m_tDir, MOTION::IDLE_TO_RUN);
					break;
				}
			}
			else {
				switch (m_tMotion)
				{
				case  MOTION::IDLE_TO_RUN:
				case  MOTION::RUN:
					if (m_iLength <= m_fAniTime)
					{
						SetAnimation(m_tDir, MOTION::RUN_TO_IDLE);
					}
					break;
				case MOTION::IDLE_TO_SPEAR:
					if (m_iLength <= m_fAniTime)
					{
						SetAnimation(m_tDir, MOTION::SPEAR);
					}
				case MOTION::DIAL:
				case MOTION::RUN_TO_IDLE:
				case MOTION::HUNGRY:
				case MOTION::AXE:
				case MOTION::PICKAXE_TO_IDLE:
				case MOTION::ATTACK:
				case MOTION::SPEAR:
				case MOTION::PICKUP:
					if (m_iLength <= m_fAniTime)
					{
						SetAnimation(m_tDir, MOTION::IDLE);
						m_bControll = true;
					}
					break;
				default:
					break;
				}
			}
		}
		if (m_pGameInstance->KeyDown(VK_SPACE))
		{
			list<CGameObject*> NearObjects;

			auto GroundObejcts = m_pGameInstance->GetAllObejctsToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("EnviornmenLayer"));


			if (GroundObejcts && !GroundObejcts->empty()) {
				for (auto& object : (*GroundObejcts)) {
					if (dynamic_cast<CEnviornment_Object*>(object)) {
						CEnviornment_Object* enviornment = dynamic_cast<CEnviornment_Object*>(object);
						if (MOTION::GHOST_APPEAR <= m_tMotion) {
							if (CEnviornment_Object::Enviornment_TYPE::RESERREECTION == enviornment->GetEnviornMentType() && CEnviornment_Object::Enviornment_STATE::IDLE == enviornment->GetState()) {
								_float3 transform = object->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
								_float distance = D3DXVec3Length(&transform);
								if (3.f > distance) {
									NearObjects.push_back(object);
								}
								continue;
							}
						}
						else {
							if (CEnviornment_Object::Enviornment_TYPE::GRASS == enviornment->GetEnviornMentType() && CEnviornment_Object::Enviornment_STATE::DAMAGED >= enviornment->GetState()) {
								_float3 transform = object->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
								_float distance = D3DXVec3Length(&transform);
								if (3.f > distance) {
									NearObjects.push_back(object);
								}
								continue;
							}
							switch (m_tSwapItem) {
							case SWAPOBJECT::AXE:
							case SWAPOBJECT::GOLDAXE:
								if (CEnviornment_Object::Enviornment_TYPE::TREE == enviornment->GetEnviornMentType() && CEnviornment_Object::Enviornment_STATE::DAMAGED >= enviornment->GetState()) {
									_float3 transform = object->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
									_float distance = D3DXVec3Length(&transform);
									if (3.f > distance) {
										NearObjects.push_back(object);
									}
								}
								break;
							case SWAPOBJECT::PICKAXE:
							case SWAPOBJECT::GOLDPICKAXE:
								if (CEnviornment_Object::Enviornment_TYPE::STONE == enviornment->GetEnviornMentType() && CEnviornment_Object::Enviornment_STATE::DAMAGED >= enviornment->GetState()) {
									_float3 transform = object->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
									_float distance = D3DXVec3Length(&transform);
									if (3.f > distance) {
										NearObjects.push_back(object);
									}
								}
								break;
								//case SWAPOBJECT::TORCH:
								//	if (CEnviornment_Object::Enviornment_TYPE::TREE == enviornment->GetEnviornMentType()) {
								//		_float3 transform = object->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
								//		_float distance = sqrtf(powf(transform.x, 2) + powf(transform.z, 2));
								//		if (3.f > distance) {
								//			NearObjects.push_back(object);
								//		}
								//	}
								//	break;
							case SWAPOBJECT::SHOVEL:
								if (CEnviornment_Object::Enviornment_TYPE::TREE == enviornment->GetEnviornMentType() && CEnviornment_Object::Enviornment_STATE::BROKEN <= enviornment->GetState()) {
									_float3 transform = object->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
									_float distance = D3DXVec3Length(&transform);
									if (3.f > distance) {
										NearObjects.push_back(object);
									}
								}
								break;
							}

							if (CEnviornment_Object::Enviornment_TYPE::NPC == enviornment->GetEnviornMentType() ) {
								_float3 transform = object->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
								_float distance = D3DXVec3Length(&transform);
								if (3.f > distance) {
									NearObjects.push_back(object);
								}
							}
						}
					}
				}
				GroundObejcts = m_pGameInstance->GetAllObejctsToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Item"));
				if (GroundObejcts && !GroundObejcts->empty()) {
					for (auto& object : (*GroundObejcts)) {
						_float3 transform = object->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
						_float distance = D3DXVec3Length(&transform);

						if (3.f > distance) {
							NearObjects.push_back(object);
						}
					}
				}
				NearObjects.sort([this](CGameObject* pSour, CGameObject* pDest)->_bool
					{
						_float3 transform = pSour->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - this->m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
						_float3 transform2 = pDest->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - this->m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
						_float distance = D3DXVec3Length(&transform);
						_float distance2 = D3DXVec3Length(&transform2);
						return distance < distance2;
					});

				if (!NearObjects.empty()) {
					CGameObject* object = NearObjects.front();
					if (object) {
						m_pPlayer->pWorkObject = object;
						//ȯ�������Ʈ�� �ٲٸ��̰Ź޾Ƽ� �۾��ϸ� �ɰŰ����ϱ� �̰� �޾Ƽ� ���� �÷���
					}
				}
			}
		}
		if (m_pGameInstance->KeyDown(VK_CONTROL))
		{
			list<CGameObject*> NearObjects;

			auto GroundObejcts = m_pGameInstance->GetAllObejctsToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Monster"));
			if (GroundObejcts && !GroundObejcts->empty()) {
				for (auto& object : (*GroundObejcts)) {
					_float3 transform = object->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
					_float distance = D3DXVec3Length(&transform);
					if (dynamic_cast<CMonster*>(object) && dynamic_cast<CMonster*>(object)->Get_Active() && dynamic_cast<CMonster*>(object)->Get_Monster()->iHostile)
						if (5.f > distance) {
							NearObjects.push_back(object);
						}
				}
			}
			NearObjects.sort([this](CGameObject* pSour, CGameObject* pDest)->_bool
				{
					_float3 transform = pSour->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - this->m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
					_float3 transform2 = pDest->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - this->m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
					_float distance = D3DXVec3Length(&transform);
					_float distance2 = D3DXVec3Length(&transform2);
					return distance < distance2;
				});
			if (!NearObjects.empty()) {
				CGameObject* object = NearObjects.front();
				if (object) {
					m_pPlayer->pWorkObject = object;
				}
			}
		}
		if (m_pGameInstance->KeyDown('F'))
		{
			m_pGameInstance->Manager_StopSound(CHANNELID::SOUND_BGM);
			auto ScreenEffect = static_cast<CDamageEffectUI*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Gameplay_Screen_Effect")));
			ScreenEffect->SetEnd();
			//m_tDamage.Damage = 50;
			//Damage(&m_tDamage);
		}
	}
	else {
		if (m_pTarget != m_pPlayer->pWorkObject) {
			m_pPlayer->pWorkObject = m_pTarget;
		}
		switch (m_tMotion)
		{
		case MOTION::IDLE:
			m_bControll = true;
			break;
		case MOTION::ITEM_IN:
			if (m_pPlayer->tItem != SWAPOBJECT::NONE) {
				if (200 <= m_fAniTime) {
					_float time = m_fAniTime;
					SetAnimation(m_tDir, MOTION::ITEM_OUT);
					m_fAniTime = time;
					m_tSwapItem = m_pPlayer->tItem;
				}
			}
			else if (m_iLength <= m_fAniTime)
			{
				SetAnimation(m_tDir, MOTION::IDLE);
				m_bControll = true;
				m_tSwapItem = m_pPlayer->tItem;
			}
			break;
		case MOTION::ITEM_OUT:
			if (m_iLength <= m_fAniTime)
			{
				SetAnimation(m_tDir, MOTION::IDLE);
			}
			break;
		case MOTION::BUCKED:
			if (m_pChar->iId == 201) {
				auto Slot = static_cast<CInventory*>(CGameInstance::GetInstance()->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_UserInterface")))->Find_Item(8);
				if (8 != Slot->Get_ItemID()) {
					auto ItemData = CItem_Manager::GetInstance()->Get_ItemData(8);
					ITEM_DESC Item_Desc;
					Item_Desc.iItemID = 8;
					Item_Desc.eSlot = ItemData.eSlot;
					Item_Desc.fDurability = 100.f;
					Item_Desc.eItemType = ItemData.eItemType;
					Item_Desc.iNumItem = 1;

					Slot->Set_Info(Item_Desc);
				}
			}
			if (m_iLength <= m_fAniTime)
			{
				SetAnimation(DIR::DIR_END, MOTION::BUCK_PST);
			}
			break;
		case MOTION::IDLE_TO_AXE:
			if (m_iLength <= m_fAniTime)
			{
				SetAnimation(m_tDir, MOTION::AXE);
			}
			break;
		case MOTION::IDLE_TO_BUILD:
			if (m_iLength <= m_fAniTime)
			{
				SetAnimation(m_tDir, MOTION::BUILD);
			}
			break;
		case MOTION::BUILD:
			if (m_iLength <= m_fAniTime && !m_bAttack)
			{
				SetAnimation(m_tDir, MOTION::BUILD_TO_IDLE);
				if (m_tItem.iItemID) {
					if (m_pSlot) {
						m_pSlot->Set_Info(m_tItem);
						m_pSlot = nullptr;
					}
					else {
						if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY), m_sItem.c_str(),
							EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Item"), &m_tItem)))
						{
							MSG_BOX("Failed to Add Item");
						}
					}
				}
				m_tItem.iItemID = 0;
				m_pGameInstance->Manager_PlaySound(L"makeitem.wav", CHANNELID::SOUND_ITEM, 0.5f);
			}
			break;
		case MOTION::PICKAXE:
			if (!m_pPlayer->pWorkObject && m_iLength <= m_fAniTime) {
				m_bControll = true;
				SetAnimation(m_tDir, MOTION::PICKAXE_TO_IDLE);
			}
			break;
		case MOTION::EAT:
		case MOTION::FASTEAT:
			if (m_iLength <= m_fAniTime) {
				m_pPlayer->iHp = min(m_pPlayer->iHp + m_iHealthChange, (_int)m_pPlayer->iMaxHp);
				m_pPlayer->iMental = min(m_pPlayer->iMental + m_iSanityChange, m_pPlayer->iMaxMental);
				m_pPlayer->iHunger = min(m_pPlayer->iHunger + m_iHungerChange, m_pPlayer->iMaxHunger);
				m_iHealthChange = 0;
				m_iSanityChange = 0;
				m_iHungerChange = 0;
				if (0 >= m_pPlayer->iHp) {
					Dead();
					return;
				}
				m_pPlayer->pWorkObject = nullptr;
				SetAnimation(m_tDir, MOTION::IDLE);
				m_bControll = true;
			}
			break;
		case MOTION::WAKEUP:
			if (m_iLength <= m_fAniTime) {
				m_bAttack = false;
				m_pPlayer->pWorkObject = nullptr;
				m_pPlayer->iHp = m_pPlayer->iMaxHp;
				m_pPlayer->iHunger = m_pPlayer->iMaxHunger;
				m_pPlayer->iMental = m_pPlayer->iMaxMental;
				SetAnimation(m_tDir, MOTION::IDLE);
				m_bControll = true;
			}
		case MOTION::IDLE_TO_ATTACK:
			if (m_iLength <= m_fAniTime) {
				_float3 oldPos = m_pPlayer->fPos;
				m_pPlayer->fPos += m_fLightning;
				m_pTransformCom->SetPosition(m_pPlayer->fPos);
				m_bLightningAttack = true;

				auto GroundObejcts = m_pGameInstance->GetAllObejctsToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Monster"));
				if (GroundObejcts && !GroundObejcts->empty()) {
					for (auto& object : (*GroundObejcts)) {
						_float3 transform = object->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
						_float distance = D3DXVec3Length(&transform);
						if (dynamic_cast<CMonster*>(object) && dynamic_cast<CMonster*>(object)->Get_Active()) {
							CMonster* monster = dynamic_cast<CMonster*>(object);
							_float3 pos = monster->Get_Monster()->fPos;
							_float4 rect = { min(oldPos.x, m_pPlayer->fPos.x), min(oldPos.z, m_pPlayer->fPos.z), max(oldPos.x, m_pPlayer->fPos.x), max(oldPos.z, m_pPlayer->fPos.z) };
							if (rect.x - 0.5f <= pos.x && rect.z + 0.5f >= pos.x && rect.y - 0.5f <= pos.z && rect.w + 0.5f >= pos.z) {
								DamageBaseDesc damage;
								damage.Attacker = this;
								damage.Damage = 100;
								monster->Damage(&damage);
							}
						}
					}
				}



			}
		case MOTION::IDLE_TO_SPEAR:
			if (m_iLength <= m_fAniTime) {
				SetAnimation(m_tDir, MOTION::SPEAR);
			}
			break;
		case MOTION::AXE:
			if (m_iLength <= m_fAniTime) {
				m_pEquipment_Slot->Update_Equipment();
				if (!m_pPlayer->pWorkObject) {
					m_bControll = true;
					SetAnimation(m_tDir, MOTION::IDLE);
				}
			}
			break;
		case MOTION::PICKAXE_TO_IDLE:
		case MOTION::SHOVEL_TO_IDLE:
		case MOTION::ATTACK:
		case MOTION::SPEAR:
			if (m_iLength <= m_fAniTime) {
				m_pEquipment_Slot->Update_Equipment();
			}
		case MOTION::BUCK_PST:
		case MOTION::DIAL:
		case MOTION::RUN_TO_IDLE:
		case MOTION::BUILD_TO_IDLE:
		case MOTION::PICKUP:
		case MOTION::GIVE:
		case MOTION::DAMAGE:
			if (m_iLength <= m_fAniTime) {
				SetAnimation(m_tDir, MOTION::IDLE);
				m_bControll = true;
			}
			break;
		case MOTION::DEATH1:
		case MOTION::DEATH2:
			if (m_iLength <= m_fAniTime) {
				m_pPlayer->bIsDead = true;
				m_tSwapItem = SWAPOBJECT::NONE;
				m_pPlayer->tItem = SWAPOBJECT::NONE;
				SetAnimation(DIR::DIR_END, MOTION::GHOST_APPEAR);
			}
			break;
		case MOTION::GHOST_APPEAR:
			if (m_iLength <= m_fAniTime) {
				SetAnimation(DIR::DIR_END, MOTION::GHOST_IDLE);
				m_bControll = true;
			}
			break;
		}
	}

	if (GetKeyState('R') & 0x8000)
	{
		SetAnimation(DIR::DIR_END, MOTION::DIAL);
		m_bControll = false;
	}
	SetAnimation(m_tDir, m_tMotion);

	if (m_pGameInstance->KeyPressed('E'))
	{
		m_pTransformCom->TurnRate(_float3(0.f, 1.f, 0.f), fTimeDelta);               
	}
	if (m_pGameInstance->KeyPressed('Q'))
	{
		m_pTransformCom->TurnRate(_float3(0.f, -1.f, 0.f), fTimeDelta);
	}
	if (m_pGameInstance->KeyPressed('Y'))
	{
		m_pPlayer->tItem = SWAPOBJECT::NONE;
		SetAnimation(m_tDir, m_tMotion);
	}
	if (GetKeyState('X') & 0x8000)
	{
		m_pPlayer->tItem = SWAPOBJECT::GOLDAXE;
		SetAnimation(m_tDir, m_tMotion);
	}
	if (GetKeyState('Z') & 0x8000)
	{
		m_pPlayer->tItem = SWAPOBJECT::GOLDPICKAXE;
		SetAnimation(m_tDir, m_tMotion);
	}
	if (m_pGameInstance->KeyPressed('C')) {
		//SetAnimation(m_tDir, MOTION::IDLE_TO_BUILD);
		//m_bControll = false;
		m_pPlayer->tItem = SWAPOBJECT::TORCH;
		SetAnimation(m_tDir, m_tMotion);
	}
	if (m_pGameInstance->KeyPressed('V')) {
		SetAnimation(m_tDir, MOTION::EAT);
		m_iHealthChange = 1000;
		m_iSanityChange = 1000;
		m_iHungerChange = 1000;
		m_bControll = false;
	}
	if (m_pGameInstance->KeyPressed('B')) {
		SetAnimation(m_tDir, MOTION::FASTEAT);
		m_iHealthChange = -10;
		m_iSanityChange = -60;
		m_iHungerChange = 100;
		m_bControll = false;
	}

}

void CPlayer::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pTorchFire->Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);
	if (m_pPlayer->pWorkObject && m_pPlayer->pWorkObject->isDead()) {
		m_pPlayer->pWorkObject = nullptr;
	}
	if (dynamic_cast<CEnviornment_Object*>(m_pPlayer->pWorkObject)) {
		switch (dynamic_cast<CEnviornment_Object*>(m_pPlayer->pWorkObject)->GetEnviornMentType())
		{
		case CEnviornment_Object::Enviornment_TYPE::TREE:
			if (SWAPOBJECT::AXE == m_tSwapItem || SWAPOBJECT::GOLDAXE == m_tSwapItem)
				break;
		case CEnviornment_Object::Enviornment_TYPE::STONE:
			if (SWAPOBJECT::PICKAXE == m_tSwapItem || SWAPOBJECT::GOLDPICKAXE == m_tSwapItem)
				break;
			m_pPlayer->pWorkObject = nullptr;
		}
	}
	m_pTarget = m_pPlayer->pWorkObject;
	SetDir();
}

void CPlayer::SetDir()
{
	if (m_pPlayer->pWorkObject) {
		_float3 fMoving = m_pPlayer->pWorkObject->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
		if (0.0001f < abs(fMoving.x) + abs(fMoving.z)) {
			m_fAngle = D3DXToDegree(acosf(fMoving.x / D3DXVec3Length(&fMoving)));
			if (0 < fMoving.z) {
				m_fAngle = 360.f - m_fAngle;
			}
		}
		D3DMATRIX view;
		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &view);
		_float3 look = view.m[2];
		look.z *= -1;
		look.y = 0;
		_float lookAngle = D3DXToDegree(acosf(look.x / D3DXVec3Length(&look)));
		lookAngle += 180;
		if (0 < look.z) {
			lookAngle = 360.f - lookAngle;
		}
		_float fAngle = fmodf(lookAngle - m_fAngle + 720.f, 360.f);
		if ((0.f <= fAngle && fAngle < 44.9f) || (fAngle < 360.f && fAngle >= 314.9f)) {
			m_tMoveDIr = MOVE_DIR::MOVE_UP;
		}
		else if ((fAngle < 134.9f && fAngle >= 44.9f)) {
			m_tMoveDIr = MOVE_DIR::MOVE_LEFT;
		}
		else if (fAngle < 224.9f && fAngle >= 134.9f) {
			m_tMoveDIr = MOVE_DIR::MOVE_DOWN;
		}
		else if (fAngle < 314.9f && fAngle >= 224.9f) {
			m_tMoveDIr = MOVE_DIR::MOVE_RIGHT;
		}
	}
	else {
		__super::SetDir();
	}
}

HRESULT CPlayer::Render()
{

	if (MOTION::BUILD == m_tMotion && DIR::UP == m_tDir) {
		RenderAnimation(m_sAnim, m_tMakeAnimation, m_tMakeImageVec);
	}
	if (DIR::DOWN == m_tDir) {
		if (m_pPlayer->iId == 200) {
			RenderAnimation(m_sAnim, m_tAnimation, MOTION::GHOST_APPEAR <= m_tMotion ? m_tGhostImageVec : m_tImageVec);
		}
		else {
			RenderAnimation(m_sAnim, m_tWigfridAnimation, MOTION::GHOST_APPEAR <= m_tMotion ? m_tWigfridGhostImageVec : m_tWigfridImageVec);
		}
		if (SWAPOBJECT::NONE != m_tSwapItem) {
			if (SWAPOBJECT::TORCH != m_tSwapItem) {
				RenderAnimation(m_sAnim, m_tItemAnimation, m_tItemImageVec[ENUM_CLASS(m_tSwapItem)]);
			}
			else {
				D3DMATRIX mat = GetTorchAnimation(m_sAnim, m_tItemAnimation, m_tItemImageVec[ENUM_CLASS(m_tSwapItem)]);
				m_pGraphic_Device->SetTransform(D3DTS_WORLD, &mat);
				m_pTorchFire->Render(mat);
			}
		}
	}
	else {
		if (SWAPOBJECT::NONE != m_tSwapItem) {
			if (SWAPOBJECT::TORCH != m_tSwapItem) {
				RenderAnimation(m_sAnim, m_tItemAnimation, m_tItemImageVec[ENUM_CLASS(m_tSwapItem)]);
			}
			else {
				D3DMATRIX mat = GetTorchAnimation(m_sAnim, m_tItemAnimation, m_tItemImageVec[ENUM_CLASS(m_tSwapItem)]);
				m_pGraphic_Device->SetTransform(D3DTS_WORLD, &mat);
				m_pTorchFire->Render(mat);
			}
		}
		if (m_pPlayer->iId == 200) {
			RenderAnimation(m_sAnim, m_tAnimation, MOTION::GHOST_APPEAR <= m_tMotion ? m_tGhostImageVec : m_tImageVec);
		}
		else {
			RenderAnimation(m_sAnim, m_tWigfridAnimation, MOTION::GHOST_APPEAR <= m_tMotion ? m_tWigfridGhostImageVec : m_tWigfridImageVec);
		}
	}
	if (MOTION::BUILD == m_tMotion && DIR::UP != m_tDir) {
		RenderAnimation(m_sAnim, m_tMakeAnimation, m_tMakeImageVec);
	}
	return S_OK;
}

void CPlayer::Damage(void* pArg)
{
	if (MOTION::DAMAGE != m_tMotion && MOTION::DEATH1 != m_tMotion && MOTION::DEATH2 != m_tMotion) {
		if (0 < m_pPlayer->iHp) {
			__super::Damage(pArg);
			auto ScreenEffect = static_cast<CDamageEffectUI*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Gameplay_Screen_Effect")));
			ScreenEffect->ActiveEffect(m_pPlayer->iHp);
		}
	}
}

void CPlayer::Hit()
{
	if (200 == m_pPlayer->iId) {
		m_pGameInstance->Manager_PlaySound(L"WilsonVoice_hurt_1.wav", CHANNELID::PLAYER_SOUND, 1.f);
	}
	else if (201 == m_pPlayer->iId) {
		m_pGameInstance->Manager_PlaySound(L"wathgrithr_hurt.wav", CHANNELID::PLAYER_SOUND, 0.5f);
	}
	m_bControll = false;
	m_fFightTime = 0.f;
	SetAnimation(m_tDir, MOTION::DAMAGE);
}

void CPlayer::Attack()
{
	m_bAttack = true;
	m_bControll = false;
	if (SWAPOBJECT::SPEAR == m_tSwapItem || SWAPOBJECT::LIGHTNINGSPEAR == m_tSwapItem)
	{
		SetAnimation(m_tDir, MOTION::IDLE_TO_SPEAR);
	}
	else {
		SetAnimation(m_tDir, MOTION::ATTACK);
	}
	m_fFightTime = 0.f;
}

void CPlayer::Death()
{
	if (200 == m_pPlayer->iId) {
		m_pGameInstance->Manager_PlaySound(L"wilson_death.wav", CHANNELID::PLAYER_SOUND, 1.f);
	}
	else if (201 == m_pPlayer->iId) {
		m_pGameInstance->Manager_PlaySound(L"wathgrithr_death.wav", CHANNELID::PLAYER_SOUND, 0.5f);
	}
	m_pPlayer->iHp = 0;
	m_pPlayer->iMental = 0;
	m_pPlayer->iHunger = 0;
	m_pPlayer->iHp = 0;
	m_bControll = false;
	m_tDir = DIR::DOWN;
	m_pPlayer->pWorkObject = nullptr;
	SetAnimation(DIR::DIR_END, MOTION::DEATH2);
}

void CPlayer::Dead()
{

	if (200 == m_pPlayer->iId) {
		m_pGameInstance->Manager_PlaySound(L"wilson_death.wav", CHANNELID::PLAYER_SOUND, 1.f);
	}
	else if (201 == m_pPlayer->iId) {
		m_pGameInstance->Manager_PlaySound(L"wathgrithr_death.wav", CHANNELID::PLAYER_SOUND, 0.5f);
	}
	m_pPlayer->iHp = 0;
	m_pPlayer->iMental = 0;
	m_pPlayer->iHunger = 0;
	m_bControll = false;
	m_tDir = DIR::DOWN;
	m_pPlayer->pWorkObject = nullptr;
	SetAnimation(DIR::DIR_END, MOTION::DEATH1);
}

void CPlayer::ChargeAttack()
{
}

PLAYER_DATA* CPlayer::Get_Player()
{
	return m_pPlayer;
}

void CPlayer::SetItem(SWAPOBJECT tItem)
{
}

_bool CPlayer::Eat(void* pArg)
{
	if (m_bControll) {
		ITEM_DATA* food = static_cast<ITEM_DATA*>(pArg);
		if (FOOD::MEAT == food->eFoodtype) {
			SetAnimation(m_tDir, MOTION::EAT);
			m_bEat = true;
		}
		else {
			if (201 == m_pPlayer->iId) {
				m_pGameInstance->Manager_PlaySound(L"wathgrithr_dial.wav", CHANNELID::PLAYER_SOUND, 0.5f);
				SetAnimation(DIR::DIR_END, MOTION::DIAL);
				return false;
			}
			else {
				SetAnimation(m_tDir, MOTION::FASTEAT);
				m_bEat = true;
			}
		}
		m_iHealthChange = food->iHealthChange;
		m_iSanityChange = food->iSanityChange;
		m_iHungerChange = food->iHungerChange;
		m_bControll = false;
		return true;
	}
	return false;
}

void CPlayer::LightningAttack(_float3 fAttack, _float fPower)
{
	m_fMoving = m_pPlayer->fPos - fAttack;
	m_pPlayer->pWorkObject = nullptr;
	m_fLightning = fAttack * fPower * 2;
	m_bControll = false;
	m_pTransformCom->SetPosition(m_pPlayer->fPos);
	SetAnimation(m_tDir, MOTION::IDLE_TO_ATTACK);
	m_fFightTime = 0.f;
}

void CPlayer::MakeItem(_wstring prototype, ITEM_DESC itemDesc)
{
	m_sItem = prototype;
	m_tItem = itemDesc;
	m_fMoving += m_pPlayer->fPos - m_tItem.vPosition;
	SetAnimation(m_tDir, MOTION::IDLE_TO_BUILD);
	m_bControll = false;
}

void CPlayer::MakeMaterialItem(CSlot* slot, ITEM_DESC itemDesc)
{
	m_pSlot = slot;
	m_tItem = itemDesc;
	SetAnimation(m_tDir, MOTION::IDLE_TO_BUILD);
	m_bControll = false;
}

HRESULT CPlayer::Set_EquipmentSlot(CSlotFrame* pSlotFrame)
{
	if (nullptr == pSlotFrame)
		return E_FAIL;

	m_pEquipment_Slot = pSlotFrame;

	Safe_AddRef(m_pEquipment_Slot);

	return S_OK;
}

HRESULT CPlayer::SetAnimation(DIR dir, MOTION motion)
{
	if (motion != m_tMotion) {
		m_fAniTime = 0;
	}
	switch (motion)
	{
	case MOTION::BUCKED:
		m_sAnim = L"bucked";
		break;
	case MOTION::BUCK_PST:
		m_sAnim = L"buck_pst";
		break;
	case MOTION::IDLE:
		m_sAnim = L"idle_loop";
		break;
	case MOTION::IDLE_TO_RUN:
		m_sAnim = L"run_pre";
		break;
	case MOTION::ITEM_IN:
		m_sAnim = L"item_in";
		break;
	case MOTION::ITEM_OUT:
		m_sAnim = L"item_out";
		break;
	case MOTION::RUN:
		m_sAnim = L"run_loop";
		break;
	case MOTION::RUN_TO_IDLE:
		m_sAnim = L"run_pst";
		break;
	case MOTION::DIAL:
		m_sAnim = L"dial_loop";
		break;
	case MOTION::IDLE_TO_BUILD:
		m_sAnim = L"build_pre";
		break;
	case MOTION::BUILD:
		m_sAnim = L"build_loop";
		break;
	case MOTION::BUILD_TO_IDLE:
		m_sAnim = L"build_pst";
		break;
	case MOTION::IDLE_TO_AXE:
		m_sAnim = L"chop_pre";
		break;
	case MOTION::HUNGRY:
		m_sAnim = L"hungry";
		break;
	case MOTION::EAT:
		m_sAnim = L"eat";
		break;
	case MOTION::FASTEAT:
		m_sAnim = L"quick_eat";
		break;
	case MOTION::AXE:
		m_sAnim = L"chop_loop";
		break;
	case MOTION::IDLE_TO_PICKAXE:
		m_sAnim = L"pickaxe_pre";
		break;
	case MOTION::PICKAXE:
		m_sAnim = L"pickaxe_loop";
		break;
	case MOTION::PICKAXE_TO_IDLE:
		m_sAnim = L"pickaxe_pst";
		break;
	case MOTION::IDLE_TO_SHOVEL:
		m_sAnim = L"shovel_pre";
		break;
	case MOTION::SHOVEL:
		m_sAnim = L"shovel_loop";
		break;
	case MOTION::SHOVEL_TO_IDLE:
		m_sAnim = L"shovel_pst";
		break;
	case MOTION::IDLE_TO_ATTACK:
		m_sAnim = L"atk_pre";
		break;
	case MOTION::ATTACK:
		m_sAnim = L"atk";
		break;
	case MOTION::IDLE_TO_SPEAR:
		m_sAnim = L"spearjab_pre";
		break;
	case MOTION::SPEAR:
		m_sAnim = L"spearjab";
		break;
	case MOTION::PICKUP:
		m_sAnim = L"pickup";
		break;
	case MOTION::GIVE:
		m_sAnim = L"give";
		break;
	case MOTION::DAMAGE:
		m_sAnim = L"hit";
		break;
	case MOTION::DEATH1:
		m_sAnim = L"death";
		break;
	case MOTION::DEATH2:
		m_sAnim = L"death2";
		break;
	case MOTION::WAKEUP:
		m_sAnim = L"wakeup";
		break;
	case MOTION::GHOST_APPEAR:
		m_sAnim = L"ghost_appear";
		break;
	case MOTION::GHOST_IDLE:
		m_sAnim = L"ghost_idle";
		break;
	case MOTION::GHOST_DISSIPATE:
		m_sAnim = L"ghost_dissipate";
		break;
	}
	m_tMotion = motion;
	D3DMATRIX view;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &view);
	_float3 look = view.m[2];
	switch (motion)
	{
	case MOTION::BUCKED:
	case MOTION::BUCK_PST:
	case MOTION::DIAL:
	case MOTION::HUNGRY:
	case MOTION::EAT:
	case MOTION::FASTEAT:
	case MOTION::DEATH1:
	case MOTION::DEATH2:
	case MOTION::WAKEUP:
	case MOTION::GHOST_APPEAR:
	case MOTION::GHOST_IDLE:
	case MOTION::GHOST_DISSIPATE:
		look.z *= -1;
		look.y = 0;
		m_fAngle = D3DXToDegree(acosf(look.x / D3DXVec3Length(&look)));
		m_tDir = DIR::DOWN;
		break;
	default:
		switch (dir)
		{
		case DIR::DOWN:
			m_sAnim += L"_down";
			break;
		case DIR::SIDE:
			m_sAnim += L"_side";
			break;
		case DIR::UP:
			m_sAnim += L"_up";
			break;
		}
		m_tDir = dir;
	}

	switch (motion)
	{
	case MOTION::ITEM_IN:
	case MOTION::ITEM_OUT:
	case MOTION::BUCKED:
	case MOTION::BUCK_PST:
	case MOTION::BUILD:
	case MOTION::IDLE_TO_AXE:
	case MOTION::AXE:
	case MOTION::IDLE_TO_PICKAXE:
	case MOTION::PICKAXE:
	case MOTION::PICKAXE_TO_IDLE:
	case MOTION::IDLE_TO_SHOVEL:
	case MOTION::SHOVEL:
	case MOTION::SHOVEL_TO_IDLE:
	case MOTION::IDLE_TO_ATTACK:
	case MOTION::IDLE_TO_SPEAR:
	case MOTION::SPEAR:
	case MOTION::GHOST_APPEAR:
	case MOTION::GHOST_IDLE:
	case MOTION::GHOST_DISSIPATE:
		break;
	default:
		if (SWAPOBJECT::NONE != m_tSwapItem) {
			m_sAnim += L"_item";
		}
		break;
	}
	return S_OK;
}

void CPlayer::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CPlayer::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
	__super::OverlapHitActor(HitActor, _Dir);
	if (HitActor == m_pPlayer->pWorkObject && MOTION::DAMAGE != m_tMotion && MOTION::DEATH1 != m_tMotion && MOTION::DEATH2 != m_tMotion) {
		m_bCol = true;
		if (dynamic_cast<CMonster*>(HitActor)) {
			if (SWAPOBJECT::SPEAR == m_tSwapItem || SWAPOBJECT::LIGHTNINGSPEAR == m_tSwapItem) {
				if (!m_bAttack && m_tMotion != MOTION::IDLE_TO_SPEAR && m_tMotion != MOTION::SPEAR) {
					Attack();
				}
				if (m_bAttack && m_tMotion == MOTION::SPEAR && 150 <= (int)m_fAniTime) {
					dynamic_cast<CMonster*>(HitActor)->Damage(&m_tDamage);
					m_bAttack = false;
					m_bControll = true;
					if (0 >= dynamic_cast<CMonster*>(HitActor)->Get_Monster()->iHp) {
						m_pPlayer->pWorkObject = nullptr;
						m_pTarget = nullptr;
					}
				}
			}
			else {
				if (!m_bAttack && m_tMotion != MOTION::ATTACK) {
					Attack();
				}
				if (m_bAttack && m_tMotion == MOTION::ATTACK && (SWAPOBJECT::NONE != m_tSwapItem ? 330 : 200) <= (int)m_fAniTime) {
					dynamic_cast<CMonster*>(HitActor)->Damage(&m_tDamage);
					m_bAttack = false;
					m_bControll = true;
					if (0 >= dynamic_cast<CMonster*>(HitActor)->Get_Monster()->iHp) {
						m_pPlayer->pWorkObject = nullptr;
						m_pTarget = nullptr;
					}
				}
			}
		}
		if (dynamic_cast<CEnviornment_Object*>(HitActor)) {
			CEnviornment_Object* enviornment = dynamic_cast<CEnviornment_Object*>(HitActor);
			switch (enviornment->GetEnviornMentType())
			{
			case CEnviornment_Object::Enviornment_TYPE::GRASS:
				if (MOTION::BUILD != m_tMotion) {
					SetAnimation(m_tDir, MOTION::IDLE_TO_BUILD);
					m_bControll = false;
					m_bAttack = true;
				}
				else {
					if (m_bAttack && m_iLength <= m_fAniTime) {
						m_bAttack = false;
						HitActor->Damage(nullptr);
						SetAnimation(m_tDir, MOTION::BUILD_TO_IDLE);
						m_pPlayer->pWorkObject = nullptr;
						m_pTarget = nullptr;
					}
				}
				break;
			case CEnviornment_Object::Enviornment_TYPE::STONE:
				if (MOTION::PICKAXE != m_tMotion) {
					SetAnimation(m_tDir, MOTION::PICKAXE);
					m_bControll = false;
					m_bAttack = true;
				}
				else {
					if (m_iLength <= m_fAniTime) {
						m_bControll = true;
						SetAnimation(m_tDir, MOTION::PICKAXE_TO_IDLE);
					}
					if (m_bAttack && 270 <= (int)m_fAniTime) {
						m_bAttack = false;
						HitActor->Damage(nullptr);
						m_pGameInstance->Manager_PlaySound(L"hit_ground.wav", CHANNELID::PLAYER_SOUND, 0.8f);
						if (CEnviornment_Object::Enviornment_STATE::BROKEN <= dynamic_cast<CEnviornment_Object*>(HitActor)->GetState() || HitActor->isDead()) {
							m_pPlayer->pWorkObject = nullptr;
							m_pTarget = nullptr;
						}
					}
				}
				break;
			case CEnviornment_Object::Enviornment_TYPE::TREE:
				if (MOTION::AXE != m_tMotion) {
					SetAnimation(m_tDir, MOTION::IDLE_TO_AXE);
					m_bControll = false;
					m_bAttack = true;
				}
				else {
					if (m_iLength <= m_fAniTime) {
						if (CEnviornment_Object::Enviornment_STATE::BROKEN <= dynamic_cast<CEnviornment_Object*>(HitActor)->GetState() || HitActor->isDead()) {
							m_pPlayer->pWorkObject = nullptr;
							m_pTarget = nullptr;
						}
						m_bControll = true;
						SetAnimation(m_tDir, MOTION::IDLE);
					}
					if (m_bAttack && 160 <= (int)m_fAniTime) {
						m_bAttack = false;
						m_tDamage.Attacker = this;
						m_tDamage.Direaction.x = (MOVE_DIR::MOVE_DOWN == m_tMoveDIr || MOVE_DIR::MOVE_LEFT == m_tMoveDIr) ? -1.f : 1.f;
						HitActor->Damage(&m_tDamage);
						m_pGameInstance->Manager_PlaySound(L"chop_tree_2.wav", CHANNELID::PLAYER_SOUND, 1.f);
					}
				}
				break;
			case CEnviornment_Object::Enviornment_TYPE::RESERREECTION:
				if (MOTION::GHOST_IDLE == m_tMotion) {
					SetAnimation(m_tDir, MOTION::GHOST_DISSIPATE);
					HitActor->Damage(&m_tDamage);
					m_bControll = false;
					m_bAttack = true;
				}
				else {
					if (m_bAttack && m_iLength <= m_fAniTime) {
						m_pPlayer->bIsDead = false;
						SetAnimation(m_tDir, MOTION::WAKEUP);
					}
				}
				break;
			case CEnviornment_Object::Enviornment_TYPE::NPC:
				if (MOTION::BUILD != m_tMotion) {
					SetAnimation(m_tDir, MOTION::IDLE_TO_BUILD);
					m_bControll = false;
					m_bAttack = true;
				}
				else {
					if (m_bAttack && m_iLength <= m_fAniTime) {
						m_bAttack = false;
						HitActor->Damage(nullptr);
						SetAnimation(m_tDir, MOTION::BUILD_TO_IDLE);
						m_pPlayer->pWorkObject = nullptr;
						m_pTarget = nullptr;
					}
				}
				break;
			}
		}
	}
}

void CPlayer::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer* pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Delete(m_pPlayer);
	Safe_Release(m_pTorchFire);
	Safe_Release(m_pEquipment_Slot);

	for (auto& Folderiter : m_tWigfridImageVec)
	{
		for (auto& Fileiter : Folderiter.tFilesVec)
			Safe_Release(Fileiter.pTexture);
	}
	for (auto& Folderiter : m_tWigfridGhostImageVec)
	{
		for (auto& Fileiter : Folderiter.tFilesVec)
			Safe_Release(Fileiter.pTexture);
	}
	for (auto& Folderiter : m_tGhostImageVec)
	{
		for (auto& Fileiter : Folderiter.tFilesVec)
			Safe_Release(Fileiter.pTexture);
	}
	for (auto& ItemImageVec : m_tItemImageVec) {
		for (auto& folder : ItemImageVec) {
			for (auto& file : folder.tFilesVec) {
				Safe_Release(file.pTexture);
			}
		}
	}
	for (auto& Folderiter : m_tMakeImageVec)
	{
		for (auto& Fileiter : Folderiter.tFilesVec)
			Safe_Release(Fileiter.pTexture);
	}
}
