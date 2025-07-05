#include "Player.h"
#include "GameInstance.h"
#include "Monster.h"
#include "Enviornment_Object.h"

#include "XML_Manager.h"

#include "Item.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCharacter{ pGraphic_Device }
{

}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CCharacter{ Prototype },
	m_tGhostImageVec{Prototype.m_tGhostImageVec},
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
	//XML_Instance->AddTexture("../Bin/Resources/Textures/Objects/Evergreen/evergreen_new.scml", L"../Bin/Resources/Textures/Objects/Evergreen/", &m_tImageVec);
	//XML_Instance->LoadScml("../Bin/Resources/Textures/Objects/Evergreen/evergreen_new.scml", &m_tAnimation);

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


	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Item/axe.scml", &m_tItemAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Item/pickaxe.scml", &m_tItemAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Item/shovel.scml", &m_tItemAnimation);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Item/attack.scml", &m_tItemAnimation);
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
	XML_Instance->LoadScml("../Bin/Resources/Textures/Player/Wilson/ghost_wilson.scml", &m_tAnimation);
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

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

	PLAYER_DESC data = *static_cast<PLAYER_DESC*>(pArg);
	m_pPlayer = new PLAYER_DATA;
	
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
	m_pPlayer->tItem = SWAPOBJECT::SPEAR;
	SetAnimation(DIR::DIR_END, MOTION::BUCKED);

	m_pChar = m_pPlayer;

	m_pTransformCom->SetPosition({ 0.f, 0.f, 0.f });

	m_pCollision_Com->SetCollisionSize({ 0.2f, 0.f ,0.f });

	m_bControll = true;
	m_bIsGhost = false;
	m_bCol = false;

	m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& _Dir) { BeginHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& _Dir) { OverlapHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& _Dir) { EndHitActor(HitActor, _Dir); });
	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
	m_fHungTime += fTimeDelta * 10;
	if (1 <= m_fHungTime) {
		m_pPlayer->iHunger--;
		if (m_pPlayer->iHunger == 30) {
			SetAnimation(DIR::DIR_END, MOTION::HUNGRY);
		}
		m_fHungTime = 0;
	}
	if (0 >= m_pPlayer->iHunger) {
		Dead();
	} 
	switch (m_tMotion)
	{
	case MOTION::ATTACK:
	case MOTION::IDLE_TO_AXE:
	case MOTION::AXE:
		break;
	default:
		m_bAttack = false;
		break;
	}
}

void CPlayer::Update(_float fTimeDelta)
{
	if (m_bControll) {
		if (MOTION::BUCKED == m_tMotion) {
			m_fAniTime = 0.f;
			m_bControll = false;
		}
		switch (m_tMotion)
		{
		case CPlayer::IDLE:
		case CPlayer::IDLE_TO_RUN:
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
		case CPlayer::ATTACK:
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
		if (m_pGameInstance->KeyPressed('W') || m_pGameInstance->KeyPressed('S') || m_pGameInstance->KeyPressed('D') || m_pGameInstance->KeyPressed('A'))
		{
			switch (m_tMotion)
			{
			case MOTION::GHOST_IDLE:
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
			_float3		vMove = {0.f ,0.f ,0.f};
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
				_float3 move = m_pPlayer->pWorkObject->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pPlayer->fPos;
				m_pPlayer->fPos += *D3DXVec3Normalize(&move, &move) * m_pPlayer->fSpeed * fTimeDelta;
				m_pTransformCom->SetPosition(m_pPlayer->fPos);
				switch (m_tMotion)
				{
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
				case MOTION::RUN_TO_IDLE:
				case MOTION::HUNGRY:
				case MOTION::AXE:
				case MOTION::ATTACK:
					if (m_iLength <= m_fAniTime)
					{
						SetAnimation(m_tDir, MOTION::IDLE);
						m_bControll = false;
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

			auto GroundObejcts = m_pGameInstance->GetAllObejctsToLayer(ENUM_CLASS(LEVEL::TUTORIAL), TEXT("EnviornmenLayer"));


			if (!GroundObejcts->empty()) {
				for (auto& object : (*GroundObejcts)) {
					if (dynamic_cast<CEnviornment_Object*>(object)) {
						CEnviornment_Object* enviornment = dynamic_cast<CEnviornment_Object*>(object);
						if (MOTION::GHOST_APPEAR <= m_tMotion) {
							if (6 == enviornment->GetEnviormentID() && CEnviornment_Object::Enviornment_STATE::DAMAGED >= enviornment->GetState()) {
								_float3 transform = object->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
								_float distance = sqrtf(pow(transform.x, 2) + pow(transform.z, 2));
								if (3.f > distance) {
									NearObjects.push_back(object);
								}
								continue;
							}
						}
						else {
							if (2 == enviornment->GetEnviormentID() && CEnviornment_Object::Enviornment_STATE::DAMAGED >= enviornment->GetState()) {
								_float3 transform = object->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
								_float distance = sqrtf(pow(transform.x, 2) + pow(transform.z, 2));
								if (3.f > distance) {
									NearObjects.push_back(object);
								}
								continue;
							}
							switch (m_pPlayer->tItem) {
							case SWAPOBJECT::AXE:
							case SWAPOBJECT::GOLDAXE:
								if (4 == enviornment->GetEnviormentID() && CEnviornment_Object::Enviornment_STATE::DAMAGED >= enviornment->GetState()) {
									_float3 transform = object->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
									_float distance = sqrtf(pow(transform.x, 2) + pow(transform.z, 2));
									if (3.f > distance) {
										NearObjects.push_back(object);
									}
								}
								break;
							case SWAPOBJECT::PICKAXE:
							case SWAPOBJECT::GOLDPICKAXE:
								if (3 == enviornment->GetEnviormentID() || 5 == enviornment->GetEnviormentID() && CEnviornment_Object::Enviornment_STATE::DAMAGED >= enviornment->GetState()) {
									_float3 transform = object->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
									_float distance = sqrtf(pow(transform.x, 2) + pow(transform.z, 2));
									if (3.f > distance) {
										NearObjects.push_back(object);
									}
								}
								break;
							case SWAPOBJECT::TORCH:
								if (4 == enviornment->GetEnviormentID() || 2 == enviornment->GetEnviormentID()) {
									_float3 transform = object->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
									_float distance = sqrtf(pow(transform.x, 2) + pow(transform.z, 2));
									if (3.f > distance) {
										NearObjects.push_back(object);
									}
								}
								break;
							case SWAPOBJECT::SHOVEL:
								if ((4 == enviornment->GetEnviormentID() || 2 == enviornment->GetEnviormentID()) && CEnviornment_Object::Enviornment_STATE::BROKEN <= enviornment->GetState()) {
									_float3 transform = object->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
									_float distance = sqrtf(pow(transform.x, 2) + pow(transform.z, 2));
									if (3.f > distance) {
										NearObjects.push_back(object);
									}
								}
								break;
							}
						}
					}
				}
				NearObjects.sort([](CGameObject* pSour, CGameObject* pDest)->_bool
					{
						_float3 transform = pSour->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - pSour->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION);
						_float3 transform2 = pDest->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - pDest->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION);
						_float distance = sqrtf(pow(transform.x, 2) + pow(transform.z, 2));
						_float distance2 = sqrtf(pow(transform2.x, 2) + pow(transform2.z, 2));
						return distance > distance2;
					});

<<<<<<< HEAD
			if (!NearObjects.empty()) {
				CGameObject* object = NearObjects.front();
				if (object) {
					m_pPlayer->pWorkObject = object;
=======
				if (!NearObjects.empty()) {
					CGameObject* object = NearObjects.front();
					if (object) {
						m_pPlayer->pWorkObject = object;
						//환경오브젝트로 바꾸면이거받아서 작업하면 될거같으니까 이거 받아서 해줘 올려줌
					}
>>>>>>> origin/0705_kjh
				}
			}
		}
		if (m_pGameInstance->KeyDown(VK_CONTROL))
		{
			list<CGameObject*> NearObjects;

			auto GroundObejcts = m_pGameInstance->GetAllObejctsToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Monster"));
			if (!GroundObejcts->empty()) {
				for (auto& object : (*GroundObejcts)) {
					_float3 transform = object->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
					_float distance = sqrtf(pow(transform.x, 2) + pow(transform.z, 2));
					if (5.f > distance) {
						NearObjects.push_back(object);
					}
				}
			}
			NearObjects.sort([](CGameObject* pSour, CGameObject* pDest)->_bool
				{
					_float3 transform = pSour->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - pSour->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION);
					_float3 transform2 = pDest->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - pDest->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION);
					_float distance = sqrtf(pow(transform.x, 2) + pow(transform.z, 2));
					_float distance2 = sqrtf(pow(transform2.x, 2) + pow(transform2.z, 2));
					return distance > distance2;
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
			Get_Damage(50);
		}
	}
	else {
		switch (m_tMotion)
		{
		case MOTION::IDLE:
			m_bControll = true;
			break;
		case MOTION::BUCKED:
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
			if (m_iLength <= m_fAniTime)
			{
				SetAnimation(m_tDir, MOTION::BUILD_TO_IDLE);
			}
			break;

		case MOTION::EAT:
		case MOTION::FASTEAT:
			if (m_iLength <= m_fAniTime) {
				m_pPlayer->iHp = min(m_pPlayer->iHp + m_iHealthChange, m_pPlayer->iMaxHp);
				m_pPlayer->iMental = min(m_pPlayer->iMental + m_iSanityChange, m_pPlayer->iMaxMental);
				m_pPlayer->iHunger = min(m_pPlayer->iHunger + m_iHungerChange, m_pPlayer->iMaxHunger);
				m_iHealthChange = 0;
				m_iSanityChange = 0;
				m_iHungerChange = 0;
				if (0 >= m_pPlayer->iHp) {
					Dead();
					return;
				}
			}
		case MOTION::BUCK_PST:
		case MOTION::DIAL:
		case MOTION::RUN_TO_IDLE:
		case MOTION::BUILD_TO_IDLE:
		case MOTION::PICKAXE_TO_IDLE:
		case MOTION::SHOVEL_TO_IDLE:
		case MOTION::ATTACK:
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
	if (GetKeyState('P') & 0x8000)
	{
		m_isDead = true;
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
		SetAnimation(m_tDir, MOTION::IDLE_TO_BUILD);
		m_bControll = false;
	}
	if (m_pGameInstance->KeyPressed('V')) {
		SetAnimation(m_tDir, MOTION::EAT);
		m_iHealthChange = 10;
		m_iSanityChange = 10;
		m_iHungerChange = 10;
		m_bControll = false;
	}
	if (m_pGameInstance->KeyPressed('B')) {
		SetAnimation(m_tDir, MOTION::FASTEAT);
		m_iHealthChange = -10;
		m_iSanityChange = -10;
		m_iHungerChange = 30;
		m_bControll = false;
	}
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	SetDir();
	m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
	if (m_pPlayer->pWorkObject && m_pPlayer->pWorkObject->isDead()) {
		m_pPlayer->pWorkObject = nullptr;
	}
	m_bCol = false;
}

HRESULT CPlayer::Render()
{
	if (FAILED(Begin_RenderState()))
		return E_FAIL;



	if (MOTION::BUILD == m_tMotion && DIR::UP == m_tDir) {
		RenderAnimation(m_sAnim, m_tMakeAnimation, m_tMakeImageVec);
	}
	if (DIR::DOWN == m_tDir) {
		RenderAnimation(m_sAnim, m_tAnimation, MOTION::GHOST_APPEAR <= m_tMotion ? m_tGhostImageVec : m_tImageVec);
		if (SWAPOBJECT::NONE != m_pPlayer->tItem) {
			RenderAnimation(m_sAnim, m_tItemAnimation, m_tItemImageVec[ENUM_CLASS(m_pPlayer->tItem)]);
		}
	}
	else {
		if (SWAPOBJECT::NONE != m_pPlayer->tItem) {
			RenderAnimation(m_sAnim, m_tItemAnimation, m_tItemImageVec[ENUM_CLASS(m_pPlayer->tItem)]);
		}
		RenderAnimation(m_sAnim, m_tAnimation, MOTION::GHOST_APPEAR <= m_tMotion ? m_tGhostImageVec : m_tImageVec);
	}
	if (MOTION::BUILD == m_tMotion && DIR::UP != m_tDir) {
		RenderAnimation(m_sAnim, m_tMakeAnimation, m_tMakeImageVec);
	}
	if (FAILED(End_RenderState()))
		return E_FAIL;
	return S_OK;
}

void CPlayer::Damage(void* pArg)
{
	m_bControll = false;
	SetAnimation(m_tDir, MOTION::DAMAGE);
}

void CPlayer::Attack()
{
	m_bAttack = true;
	m_bControll = false;
	SetAnimation(m_tDir, MOTION::ATTACK);
}

void CPlayer::Death()
{
	m_bControll = false;
	m_bIsGhost = true;
	m_tDir = DIR::DOWN;
	SetAnimation(DIR::DIR_END, MOTION::DEATH2);
}

void CPlayer::Dead()
{
	m_bControll = false;
	m_bIsGhost = true;
	m_tDir = DIR::DOWN;
	SetAnimation(DIR::DIR_END, MOTION::DEATH1);
}

void CPlayer::Get_Damage(_uint iAtk)
{
	__super::Get_Damage(max(0, iAtk - (m_pPlayer->iDef * m_pPlayer->fDefRatio)));
}

PLAYER_DATA* CPlayer::Get_Player()
{
	return m_pPlayer;
}

void CPlayer::SetItem(SWAPOBJECT tItem)
{
}

void CPlayer::Eat(void* pArg)
{
	ITEM_DATA* food = static_cast<ITEM_DATA*>(pArg);
	if (FOOD::MEAT == food->eFoodtype) {
		SetAnimation(m_tDir, MOTION::EAT);
	}
	else {
		SetAnimation(m_tDir, MOTION::FASTEAT);
	}
	m_iHealthChange = food->iHealthChange;
	m_iSanityChange = food->iSanityChange;
	m_iHungerChange = food->iHungerChange;
}

HRESULT CPlayer::SetAnimation(DIR dir, MOTION motion)
{
	if (motion != m_tMotion) {
		m_fAniTime = 0.f;
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
	case MOTION::RUN:
		m_sAnim = L"run_loop";
		break;
	case MOTION::RUN_TO_IDLE:
		m_sAnim = L"run_pst";
		break;
	case MOTION::DIAL:
		m_sAnim = L"dial";
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
	case MOTION::ATTACK:
		m_sAnim = L"atk";
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
	case MOTION::GHOST_APPEAR:
	case MOTION::GHOST_IDLE:
	case MOTION::GHOST_DISSIPATE:
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
	case MOTION::GHOST_APPEAR:
	case MOTION::GHOST_IDLE:
	case MOTION::GHOST_DISSIPATE:
		break;
	default:
		if (SWAPOBJECT::NONE != m_pPlayer->tItem) {
			m_sAnim += L"_item";
		}
		break;
	}
	return S_OK;
}

HRESULT CPlayer::Begin_RenderState()
{
	/* 알파 테스트 : 픽셀의 알파를 비교해서 그린다 안그린다를 설정. */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);



	return S_OK;
}

HRESULT CPlayer::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CPlayer::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CPlayer::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
	if (HitActor == m_pPlayer->pWorkObject) {
		m_bCol = true;
		if (dynamic_cast<CMonster*>(HitActor)) {
			if (!m_bAttack && m_tMotion != MOTION::ATTACK) {
				Attack();
			}
			if (m_bAttack && m_tMotion == MOTION::ATTACK && (SWAPOBJECT::NONE != m_pPlayer->tItem ? 330 : 200) <= (int)m_fAniTime) {
				dynamic_cast<CMonster*>(HitActor)->Get_Damage(m_pPlayer->iAtk * m_pPlayer->fAtkRatio);
				m_bAttack = false;
				if (0 >= dynamic_cast<CMonster*>(HitActor)->Get_Monster()->iHp) {
					m_pPlayer->pWorkObject = nullptr;
				}
			}
		}
		if (dynamic_cast<CEnviornment_Object*>(HitActor)) {

			CEnviornment_Object* enviornment = dynamic_cast<CEnviornment_Object*>(HitActor);
			switch (enviornment->GetEnviormentID())
			{
			case 2:
				break;
			case 3:
			case 5:
				if (MOTION::PICKAXE != m_tMotion) {
					SetAnimation(m_tDir, MOTION::PICKAXE);
					m_bControll = false;
					m_bAttack = true;
				}
				else {
					if (m_iLength <= m_fAniTime) {
						if (CEnviornment_Object::Enviornment_STATE::BROKEN <= dynamic_cast<CEnviornment_Object*>(HitActor)->GetState()) {
							m_pPlayer->pWorkObject = nullptr;
						}
						m_bControll = true;
						SetAnimation(m_tDir, MOTION::PICKAXE_TO_IDLE);
					}
					if (m_bAttack && 160 <= (int)m_fAniTime) {
						m_bAttack = false;
						HitActor->Damage(nullptr);
					}
				}
				break;
			case 4:
				if (MOTION::AXE != m_tMotion) {
					SetAnimation(m_tDir, MOTION::IDLE_TO_AXE);
					m_bControll = false;
					m_bAttack = true;
				}
				else {
					if (m_iLength <= m_fAniTime) {
						if (CEnviornment_Object::Enviornment_STATE::BROKEN <= dynamic_cast<CEnviornment_Object*>(HitActor)->GetState()) {
							m_pPlayer->pWorkObject = nullptr;
						}
						m_bControll = true;
						SetAnimation(m_tDir, MOTION::IDLE);
					}
					if (m_bAttack && 160 <= (int)m_fAniTime) {
						m_bAttack = false;
						HitActor->Damage(nullptr);
					}
				}
				break;
			//case 6:
			//	if (MOTION::GHOST_IDLE == m_tMotion) {
			//		SetAnimation(m_tDir, MOTION::GHOST_DISSIPATE);
			//		m_bControll = false;
			//		HitActor->Damage(&m_pPlayer);
			//	}
			//	else {
			//		if (m_iLength <= m_fAniTime) {
			//			if (CEnviornment_Object::Enviornment_STATE::BROKEN <= dynamic_cast<CEnviornment_Object*>(HitActor)->GetState()) {
			//				m_pPlayer->pWorkObject = nullptr;
			//			}
			//			m_bControll = true;
			//			SetAnimation(m_tDir, MOTION::IDLE);
			//		}
			//		if (m_bAttack && 160 <= (int)m_fAniTime) {
			//			m_bAttack = false;
			//			HitActor->Damage(nullptr);
			//		}
			//	}
			//	break;
			}
		}
		if (dynamic_cast<CItem*>(HitActor)) {
			SetAnimation(m_tDir, MOTION::PICKUP);
			m_pPlayer->pWorkObject = nullptr;
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

	Safe_Release(m_pCollision_Com);
	Safe_Delete(m_pPlayer);


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
