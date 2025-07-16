#include "LeafEffect.h"

#include "GameInstance.h"

#include "Camera.h"
#include "Player.h"

CLeafEffect::CLeafEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject{pGraphic_Device}
{
}

CLeafEffect::CLeafEffect(const CLeafEffect& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CLeafEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLeafEffect::Initialize(void* pArg)
{
	m_eCurState = CLeafEffect::IDLE;
	m_ePreState = CLeafEffect::IDLE;
	m_fTimeAcc = 0.f;

	m_fAngel = m_pGameInstance->Random(-360.f, 360.f);
	if (FAILED(ADD_Components()))
		return E_FAIL;

	LEAF_DESC* pDesc = static_cast<LEAF_DESC*>(pArg);

	m_iIndex = pDesc->iIndex;
	m_vMovePos = pDesc->vMovePos;
	m_pTransformCom->SetPosition(pDesc->vPosition);

	m_fReachTime = 1.f + m_iIndex * 0.07f;


	m_pCamera = dynamic_cast<CCamera*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Camera")));

	if (nullptr == m_pCamera)
		return E_FAIL;

	_float fScale = m_pGameInstance->Random(0.2f, 0.3f);

	m_pTransformCom->SetScale(_float3(fScale, fScale * 1.8f, 1.f));
	_float3 vLook = m_pTransformCom->GetWorldState(WORLDSTATE::LOOK);
	m_pTransformCom->SetRotAxis(vLook, D3DXToRadian(m_fAngel));

	m_pCollisionCom->SetCollisionSize({ 1.f, 1.f, 1.f });
	m_pCollisionCom->BindEnterFunction([&](CGameObject* HitActor, _float3& _Dir) { BeginHitActor(HitActor, _Dir); });

	return S_OK;
}

void CLeafEffect::Priority_Update(_float fTimeDelta)
{

}

void CLeafEffect::Update(_float fTimeDelta)
{

	
	LPDIRECT3DBASETEXTURE9 pTex = { nullptr };
	Excute_Billboard(m_pCamera->GetInvViewMat(), pTex);
	

	m_fTimeAcc += fTimeDelta;

	_float3 vPos = {};
	switch (m_eCurState)
	{
	case Client::CLeafEffect::IDLE:
		if (m_fTimeAcc > m_fReachTime)
		{
			m_eCurState = CLeafEffect::ATTACK;
			m_fTimeAcc = 0.f;
		}

		break;

	case Client::CLeafEffect::ATTACK:
		if (m_fTimeAcc > 0.5f)
		{
			m_eCurState = CLeafEffect::LAND;
			m_fTimeAcc = 0;
			break;
		}

		vPos = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
		vPos += (m_fDistance_Target) * fTimeDelta* 2.f * m_vAttackDir;
		m_pTransformCom->SetPosition(vPos);
		break;

	case Client::CLeafEffect::LAND:
		if (m_fTimeAcc > 1.f)
		{
			m_isDead = true;
			break;
		}


	default:
		break;
	}

	Change_State();

	m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);
}

void CLeafEffect::Late_Update(_float fTimeDelta)
{
	
}

HRESULT CLeafEffect::Render()
{
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CLeafEffect::ADD_Components()
{
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"),
		reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	Engine::CTransform::TRANSFORM_DESC Transform_Desc = { 5.f, D3DXToRadian(90.f) };

	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"),
		reinterpret_cast<CComponent**>(&m_pTransformCom), &Transform_Desc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_LeafEffect"),
		TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	CCollision_Component::COL_DESC Col_Desc = {};
	Col_Desc.pOwner = this;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_BoxCollision"),
		TEXT("Com_BoxCollision"), reinterpret_cast<CComponent**>(&m_pCollisionCom), &Col_Desc)))
		return E_FAIL;

	return S_OK;
}

void CLeafEffect::Change_State()
{
	if(m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case Client::CLeafEffect::IDLE:
			break;

		case Client::CLeafEffect::ATTACK:
			Ready_Attack();
			if(0 == m_iIndex % 3)
			{
				m_pGameInstance->Manager_PlaySound(L"leif_leaf.wav", CHANNELID::SOUND_EFFECT, 1.f);
			}
			break;

		case Client::CLeafEffect::LAND:
			m_pCollisionCom->SetCollisionSize(_float3(0.f, 0.f, 0.f));
			break;

		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CLeafEffect::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
	if (nullptr != dynamic_cast<CPlayer*>(HitActor))
	{
		m_eCurState = STATE::IDLE;
		DAMAGE_DATA_BASE DamageBase = {};
		DamageBase.DamageType = ATTACK_TYPE::ATTACK;
		DamageBase.Damage = 3;
		dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Player")))->Damage(&DamageBase);
		m_isDead = true;
	}
}

void CLeafEffect::Ready_Attack()
{
	_float3 fTargetPos = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
		EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform")))->GetWorldState(WORLDSTATE::POSITION);

	fTargetPos += _float3(m_pGameInstance->Random(0.f, 0.2f), 0.f, m_pGameInstance->Random(0.f, 0.2f));

	m_vAttackDir = fTargetPos - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);

	m_fDistance_Target = D3DXVec3Length(&m_vAttackDir);
	D3DXVec3Normalize(&m_vAttackDir, &m_vAttackDir);
	_float3 vLook = m_pTransformCom->GetWorldState(WORLDSTATE::LOOK);
	_float3 vUp = { 0.f, 1.f, 0.f };

	_float3 vCross = {};

	D3DXVec3Cross(&vCross, &m_vAttackDir, &vUp);

	D3DXVec3Normalize(&vCross, &vCross);

	

	if(0.5 < D3DXVec3Dot(&vLook, &vCross))
		m_pTransformCom->SetRotAxis(vLook, D3DXToRadian(45.f));
	else
		m_pTransformCom->SetRotAxis(vLook, D3DXToRadian(0.f));
	// 타겟쪽으로 회전
}

CLeafEffect* CLeafEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLeafEffect* pInstance = new CLeafEffect(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CREATE FAIL : CLeafEffect");
	}
	return pInstance;
}

CGameObject* CLeafEffect::Clone(void* pArg)
{
	CGameObject* pInstance = new CLeafEffect(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Clone FAIL : CLeafEffect");
	}
	return pInstance;
}

void CLeafEffect::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pCollisionCom);
}
