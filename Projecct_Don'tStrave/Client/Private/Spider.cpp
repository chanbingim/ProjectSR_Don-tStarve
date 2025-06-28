#include "Spider.h"
#include "GameInstance.h"

CSpider::CSpider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{

}

CSpider::CSpider(const CSpider& Prototype)
	: CMonster{ Prototype }
{
}

HRESULT CSpider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSpider::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->SetPosition(_float3(rand() % 20, 0.f, rand() % 20));
	m_pAnimTransformCom->SetPosition(m_pTransformCom->GetWorldState(WORLDSTATE::POSITION));


	CPlayerAnim::PLAYER_DESC AnimDesc;
	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.pVIBufferCom = m_pVIBufferCom;
	AnimDesc.Frame.iStartFrame = 0;

	for (int j = 0; j < DIR::DIR_END; ++j) {
		for (int k = 0; k < MOTION::MOTION_END; ++k) {
			if (m_pTextureCom[j][k]) {
				AnimDesc.Frame.pAnimTexture = m_pTextureCom[j][k];
				m_pSpiderAnim[j][k] = CPlayerAnim::Create(&AnimDesc);
			}
		}
	}
	m_tDir = DIR::DIR_END;
	m_tMotion = MOTION::IDLE;
	SetAnimation(m_tDir, m_tMotion);
	m_iMaxHp = 100;
	m_iHp = m_iMaxHp;
	m_iTemp = 0;
	m_iAtk = 30;
	m_iDef = 0;
	m_iMaxHit = 10;
	m_iHit = m_iMaxHit;
	m_bMove = false;

	return S_OK;
}

void CSpider::Priority_Update(_float fTimeDelta)
{
}

void CSpider::Update(_float fTimeDelta)
{
	_float3 move = m_pPlayer->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
	if ((abs(move.x) + abs(move.z)) / 2.f < 2) {
		if (m_tMotion != MOTION::RUN && m_tMotion != MOTION::IDLE_TO_RUN) {
			switch (m_tMotion)
			{
			case MOTION::TAUNT:
				if (m_pSpiderAnim[m_tDir][m_tMotion]->IsEnd()) {
					m_tMotion = MOTION::IDLE_TO_RUN;
				}
				break;
			default:
				m_tDir = DIR::DIR_END;
				m_tMotion = MOTION::TAUNT;
				break;
			}
			SetAnimation(m_tDir, m_tMotion);
		}
		else {
			if (m_tMotion == MOTION::IDLE_TO_RUN && m_pSpiderAnim[m_tDir][m_tMotion]->IsEnd())
			{
				m_tMotion = MOTION::RUN;
				SetAnimation(m_tDir, m_tMotion);
			}
			D3DXVec3Normalize(&move, &move);
			_float3		vPosition = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
			vPosition += move * fTimeDelta;
			m_pTransformCom->SetPosition(vPosition);
		}
	}
	else{
		switch (m_tMotion)
		{
		case MOTION::RUN:
			if (m_pSpiderAnim[m_tDir][m_tMotion]->IsEnd()) {
				m_tMotion = MOTION::RUN_TO_IDLE;
				SetAnimation(m_tDir, m_tMotion);
			}
			break;
		case MOTION::RUN_TO_IDLE:
			if (m_pSpiderAnim[m_tDir][m_tMotion]->IsEnd()) {
				m_tDir = DIR::DIR_END;
				m_tMotion = MOTION::IDLE;
				SetAnimation(m_tDir, m_tMotion);
			}
			break;
		default:
			break;
		}
	}
}

void CSpider::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pAnimController->Tick(fTimeDelta);
	m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

HRESULT CSpider::Render()
{
	if (FAILED(Begin_RenderState()))
		return E_FAIL;
	m_pAnimController->Render();
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pAnimTransformCom->Get_World());
	m_pVIBufferCom->Render();

	if (FAILED(End_RenderState()))
		return E_FAIL;

	return S_OK;
}

void CSpider::Damage()
{
	m_tMotion = MOTION::DAMAGE;
	SetAnimation(m_tDir, m_tMotion);
}

void CSpider::Attack()
{
	m_tMotion = MOTION::ATTACK;
	SetAnimation(m_tDir, m_tMotion);
}

void CSpider::Death()
{
	m_tMotion = MOTION::DEATH;
	SetAnimation(DIR::DIR_END, m_tMotion);
}

HRESULT CSpider::SetAnimation(DIR dir, MOTION motion)
{
	AddAnimation(dir, motion);
	DIR _dir = dir;
	if (DIR::DIR_END == dir)
		_dir = DIR::DOWN;
	m_pAnimController->ChangeState(m_pSpiderAnim[_dir][motion]);
	return S_OK;
}

HRESULT CSpider::AddAnimation(DIR dir, MOTION motion)
{
	if (DIR::DIR_END == dir) {
		m_tDir = DIR::DOWN;
	}
	if (!m_pSpiderAnim[m_tDir][m_tMotion]) {
		wstring str = L"../Bin/Resources/Textures/Monster/Spider";
		switch (motion)
		{
		case MOTION::IDLE:
			str += L"/idle";
			break;
		case MOTION::IDLE_TO_RUN:
			str += L"/run_pre";
			break;
		case MOTION::RUN:
			str += L"/run_loop";
			break;
		case MOTION::RUN_TO_IDLE:
			str += L"/run_pst";
			break;
		case MOTION::ATTACK:
			str += L"/atk";
			break;
		case MOTION::IDLE_TO_SLEEP:
			str += L"/build_pre";
			break;
		case MOTION::SLEEP:
			str += L"/build_pre";
			break;
		case MOTION::SLEEP_TO_IDLE:
			str += L"/build_pre";
			break;
		case MOTION::IDLE_TO_EAT:
			str += L"/eat_pre";
			break;
		case MOTION::EAT:
			str += L"/eat";
			break;
		case MOTION::EAT_TO_IDLE:
			str += L"/eat_pst";
			break;
		case MOTION::DAMAGE:
			str += L"/damage";
			break;
		case MOTION::IDLE_TO_COWER:
			str += L"/cower_pre";
			break;
		case MOTION::COWER:
			str += L"/cower_loop";
			break;
		case MOTION::COWER_TO_IDLE:
			str += L"/cower_pst";
			break;
		case MOTION::TAUNT:
			str += L"/taunt";
			break;
		case MOTION::DEATH:
			str += L"/death";
			break;
		}
		switch (dir)
		{
		case DIR::DOWN:
			str += L"_down";
			break;
		case DIR::SIDE:
			str += L"_side";
			break;
		case DIR::UP:
			str += L"_up";
			break;
		}
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + str),
			TEXT("Com_" + str), reinterpret_cast<CComponent**>(&m_pTextureCom[m_tDir][motion]))))
		{
			m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + str),
				CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, str.c_str()));
			__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + str),
				TEXT("Com_" + str), reinterpret_cast<CComponent**>(&m_pTextureCom[m_tDir][motion]));

		}

		CPlayerAnim::PLAYER_DESC AnimDesc;
		AnimDesc.pParentTransformCom = m_pTransformCom;
		AnimDesc.pTransformCom = m_pAnimTransformCom;
		AnimDesc.pVIBufferCom = m_pVIBufferCom;
		AnimDesc.Frame.iStartFrame = 0;
		AnimDesc.Frame.bIsLoop = true;

		/* Com_Texture */
		AnimDesc.Frame.pAnimTexture = m_pTextureCom[m_tDir][motion];
		m_pSpiderAnim[m_tDir][motion] = CPlayerAnim::Create(&AnimDesc);
	}

	return S_OK;
}

HRESULT CSpider::Ready_Components()
{
	/* Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Anim_Transform"), reinterpret_cast<CComponent**>(&m_pAnimTransformCom), &TransformDesc)))
		return E_FAIL;


	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_AnimController"),
		TEXT("Com_AnimController"), (CComponent**)&m_pAnimController)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpider::Begin_RenderState()
{
	/* 렌더링할 때 알파값을 기준으로 섞어준다.*/

	/*
	float4		vSourColor, vDestColor;
	vSourColor.rgb * vSourColor.a + vDestColor.rgb * (1.f - vSourColor.a);
	*/

	//
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//

	/* 알파 테스트 : 픽셀의 알파를 비교해서 그린다 안그린다를 설정. */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);



	return S_OK;
}

HRESULT CSpider::End_RenderState()
{
	// m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pVIBufferCom->SetUV(1, 1, 1, 0, 1);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

CSpider* CSpider::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSpider* pInstance = new CSpider(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpider::Clone(void* pArg)
{
	CSpider* pInstance = new CSpider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSpider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpider::Free()
{
	__super::Free();
	for (int j = 0; j < DIR::DIR_END; ++j) {
		for (int k = 0; k < MOTION::MOTION_END; ++k) {
			if (m_pTextureCom[j][k]) {
				Safe_Release(m_pTextureCom[j][k]);
			}
			if (m_pSpiderAnim[j][k]) {
				Safe_Release(m_pSpiderAnim[j][k]);
			}
		}
	}
}
