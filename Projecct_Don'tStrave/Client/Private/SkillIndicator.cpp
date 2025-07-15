#include "SkillIndicator.h"

#include "GameInstance.h"
#include "Terrian_Manager.h"
#include "EffectPoolManager.h"

#include "Mouse.h"
#include "Terrain.h"
#include "Player.h"
#include "AinimationObject.h"
#include "SpriteEffect.h"

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
	m_fAngle = 0.f;
	m_fTimeAcc = 0.f;

	if (FAILED(ADD_Components()))
		return E_FAIL;
	m_pTerrains = CTerrian_Manager::GetInstance()->GetTerrains();


	m_pTransformCom->SetRotAxis(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	m_pTransformCom->SetScale(_float3(1.5f, 1.5f, 1.5f));

	m_pLightCom->SetDiffuseColor(D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	m_pLightCom->SetAmbientColor(D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	m_pLightCom->SetPosition(m_pTransformCom->GetWorldState(WORLDSTATE::POSITION));
	m_pLightCom->SetMaxRange(3.3f);

	m_pLightCom->SetAttenuation(0.f, 0.3f, 0.8f);

	m_pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Player")));
	return S_OK;
}

void CSkillIndicator::Priority_Update(_float fTimeDelta)
{
}

void CSkillIndicator::Update(_float fTimeDelta)
{
	if (201 == m_pPlayer->Get_Player()->iId && SWAPOBJECT::LIGHTNINGSPEAR == m_pPlayer->Get_Player()->tItem) {
		if (m_pGameInstance->KeyPressed(VK_RBUTTON))
		{
			_float3 vPickingPos = {};

			auto Player_Pos = m_pPlayer->Get_Player()->fPos;

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
			m_vDir = vPickingPos - Player_Pos;

			D3DXVec3Normalize(&m_vDir, &m_vDir);

			// 위치 
			_float3 vPosition = Player_Pos + m_vDir * (m_Charge - 0.3f);

			m_pTransformCom->SetPosition(vPosition);

			// 회전
			WorldMat = m_pTransformCom->Get_World();
			_float3 vLook = { 0.f, 1.f, 0.f };
			_float3 vUp = {};

			_float3 vPlayerLook = m_pPlayer->GetTransfrom()->GetWorldState(WORLDSTATE::LOOK);

			m_fAngle = D3DXVec3Dot(&vPlayerLook, &m_vDir);
			
			
			if(g_iWinSizeX * 0.5f < m_pGameInstance->GetMousePosition(0).x)
			{
				m_fAngle = 90.f * m_fAngle + 180.f;
			}
			else
				m_fAngle *= -90.f ;
			
			D3DXVec3Cross(&vUp, &vLook, &m_vDir);

			if (m_Charge <= 1.f)
				m_Charge += fTimeDelta * 0.5f;

			_float3 vRight = m_vDir * m_Charge;

			memcpy(&WorldMat.m[0], &vRight, sizeof(_float3));
			memcpy(&WorldMat.m[1], &vUp, sizeof(_float3));
			memcpy(&WorldMat.m[2], &vLook, sizeof(_float3));
			memcpy(&WorldMat.m[3], &vPosition, sizeof(_float3));


			m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);
		}
		else if (m_pGameInstance->KeyUp(VK_RBUTTON))
		{
			if (0.75f <= m_Charge) {
				m_bIsEffectActive = true;
				m_fTimeAcc = 1.f;
				
				m_vEffectPos = m_pPlayer->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION);

				_float3 vPickingPos = {};
				
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

				_float3 vDir = vPickingPos - m_pPlayer->Get_Player()->fPos;
				D3DXVec3Normalize(&vDir, &vDir);
				m_pPlayer->LightningAttack(vDir, m_Charge);
			}
			m_Charge = 0.5f;
		}
	}

	if (0.f < m_fTimeAcc)
	{
		Update_Effect(fTimeDelta);
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
	
	CLightComponent::LIGHT_DESC Light_Desc = {};
	_D3DLIGHT9 Light = {};
	Light.Type = D3DLIGHT_POINT;

	Light.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Light.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	Light_Desc.LightData = Light;
	Light_Desc.pOwner = this;
	Light_Desc.PlayerPoint = m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Player"))->GetTransfrom();

	// Light Component
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Light"),
		TEXT("Com_Light"),
		reinterpret_cast<CComponent**>(&m_pLightCom), &Light_Desc)))
		return E_FAIL;

	return S_OK;
}

void CSkillIndicator::Update_Effect(_float fTimeDelta)
{
	D3DXCOLOR Color = { 1.f,1.f, 1.f, 1.f };

	if (true == m_bIsEffectActive && m_fTimeAcc < 0.8f)
	{
		// up 이랑 dir 외적 나온걸 축으로 회전
		CSpriteEffect::SPRITE_EFFECT Desc;

		Desc.AnimName = L"fx_side";
		auto Effect = CEffectPoolManager::GetInstance()->Add_ActiveEffect(3, (CAinimationObject**)&m_pSpirteEffect, &Desc);
		m_pSpirteEffect->GetTransfrom()->SetPosition(m_vEffectPos);
		m_pSpirteEffect->Set_Angle(m_fAngle);

		m_bIsEffectActive = false;
	}

	if(false == m_bIsEffectActive)
	{
		Color *= m_fTimeAcc * 2.f;

		m_pLightCom->SetAmbientColor(Color);
		m_pLightCom->SetPosition(m_vEffectPos + m_vDir * 0.5f);

		m_pLightCom->SetLight(true);
	}

	m_fTimeAcc -= fTimeDelta;

	if(m_fTimeAcc <= 0.f)
	{
		m_pLightCom->SetLight(false);
		m_fTimeAcc = 0.f;
	}
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

	Safe_Release(m_pLightCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
}
