#include "SkillIndicator.h"

#include "GameInstance.h"
#include "Terrian_Manager.h"

#include "Mouse.h"
#include "Terrain.h"

CSkillIndicator::CSkillIndicator(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject{ pGraphic_Device }
{
}

CSkillIndicator::CSkillIndicator(const CSkillIndicator& Prototype)
	:CGameObject{ Prototype }
{
}

HRESULT CSkillIndicator::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkillIndicator::Initialize(void* pArg)
{
	m_Charge = 0.5f;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	m_pPlayerTranformCom = static_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform"), 0));

	Safe_AddRef(m_pPlayerTranformCom);

	m_pTerrains = CTerrian_Manager::GetInstance()->GetTerrains();


	m_pTransformCom->SetRotAxis(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	m_pTransformCom->SetScale(_float3(1.5f, 1.5f, 1.5f));

	return S_OK;
}

void CSkillIndicator::Priority_Update(_float fTimeDelta)
{
}

void CSkillIndicator::Update(_float fTimeDelta)
{
	if(m_pGameInstance->KeyPressed(VK_RBUTTON))
	{
		_float3 vPickingPos = {};

		auto Player_Pos = m_pPlayerTranformCom->GetWorldState(WORLDSTATE::POSITION);

		for (auto pTerrain : *m_pTerrains)
		{
			if (nullptr == pTerrain)
				continue;

			auto VIbuffer = pTerrain->GetCurVIBuffer();
			auto Transform = pTerrain->GetTransfrom();

			if (VIbuffer->Picking(Transform, &vPickingPos))
			{
				break;
			}
		}

		// 마우스에서 플레이어 위치 뺀 벡터
		_float3 vDir = vPickingPos - Player_Pos;

		D3DXVec3Normalize(&vDir, &vDir);

		// 위치 
		_float3 vPosition = Player_Pos + vDir * (m_Charge - 0.3f);

		m_pTransformCom->SetPosition(vPosition);

		// 회전
		WorldMat = m_pTransformCom->Get_World();
		_float3 vLook = { 0.f, 1.f, 0.f };
		_float3 vUp = {};

		D3DXVec3Cross(&vUp, &vLook, &vDir);

		if (m_Charge <= 1.f)
			m_Charge += fTimeDelta * 0.5f;

		_float3 vRight = vDir * m_Charge;

		memcpy(&WorldMat.m[0], &vRight, sizeof(_float3));
		memcpy(&WorldMat.m[1], &vUp, sizeof(_float3));
		memcpy(&WorldMat.m[2], &vLook, sizeof(_float3));
		memcpy(&WorldMat.m[3], &vPosition, sizeof(_float3));


		m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);
	}
	else if (m_pGameInstance->KeyUp(VK_RBUTTON))
	{
		m_Charge = 0.5f;
	}
}

void CSkillIndicator::Late_Update(_float fTimeDelta)
{
}

HRESULT CSkillIndicator::Render()
{
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &WorldMat);

	m_pTextureCom->Set_Texture(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CSkillIndicator::ADD_Components()
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

	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_AttackPreview"),
		TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}


CSkillIndicator* CSkillIndicator::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSkillIndicator* pInstance = new CSkillIndicator(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CSkillIndicator");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CSkillIndicator::Clone(void* pArg)
{
	CGameObject* pInstance = new CSkillIndicator(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone CSkillIndicator");
		return nullptr;
	}

	return pInstance;
}

void CSkillIndicator::Free()
{
	__super::Free();

	Safe_Release(m_pPlayerTranformCom);

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
}
