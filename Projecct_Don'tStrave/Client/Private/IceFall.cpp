#include "IceFall.h"
#include "GameInstance.h"
#include "XML_Manager.h"
#include "Deerclops.h"
#include "Terrain.h"
#include "Terrian_Manager.h"
#include "Camera.h"
#include "Collision_Component.h"
#include "Texture.h"

CIceFall::CIceFall(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CAinimationObject{ pGraphic_Device }
{
}

CIceFall::CIceFall(const CIceFall& Prototype)
	: CAinimationObject{ Prototype }
{
}

HRESULT CIceFall::Initialize_Prototype()
{
	auto XML_Instance = CXML_Manager::GetInstance();
    
	XML_Instance->AddTexture("../Bin/Resources/Textures/Particles/IceImpact/ice_impact.scml", L"../Bin/Resources/Textures/Particles/IceImpact/", &m_tImageVec);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Particles/IceImpact/ice_impact.scml", &m_tAnimation);

	return S_OK;
}

HRESULT CIceFall::Initialize(void* pArg)
{

    if (FAILED(Ready_Components()))
        return E_FAIL;
    LoadImageFile();
	CLandObject::LANDOBJECT_DESC Arg = {};
	GAMEOBJECT_DESC* TeerrainDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

	m_pTransformCom->SetPosition(TeerrainDesc->vPosition);
	m_pTransformCom->SetScale(TeerrainDesc->vScale);
	m_pTransformCom->SetRotation(TeerrainDesc->vRotation);

	auto Terrian = m_pTerrian_Manager->GetOnTerrian(TeerrainDesc->vPosition);

	if (Terrian) {
		Arg.pLandVIBuffer = Terrian->GetCurVIBuffer();
		Arg.pLandTransform = Terrian->GetTransfrom();
        if (FAILED(__super::Initialize(&Arg)))
            return E_FAIL;
		SetUp_OnTerrain(m_pTransformCom);
	}
    m_fYPos = 10;
    m_bBreak = false;


    m_pCollision_Com->SetCollisionSize({ 1.f,0.f,0.f });
    m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& _Dir) { BeginHitActor(HitActor, _Dir); });
    m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& _Dir) { OverlapHitActor(HitActor, _Dir); });
    m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& _Dir) { EndHitActor(HitActor, _Dir); });

	return S_OK;
}

void CIceFall::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CIceFall::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
    if (0 < m_fYPos)
        m_fYPos -= fTimeDelta * 5;
    else if (!m_bBreak) {
        m_bBreak = true;
        Death();
        m_fAniTime = 0.f;
        m_iLength = 100;
        m_pCamera->ShakeCamera(0.5f);
        m_pGameInstance->Manager_PlaySound(L"Deerclops_iceattack_small_3.wav", CHANNELID::SOUND_EFFECT, 1.f);
    }
    if (m_bBreak && m_iLength <= m_fAniTime) {
        m_isDead = true;
    }
}

void CIceFall::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

    if (!m_isDead && m_pCamera->IsInObject(m_pTransformCom->GetWorldState(WORLDSTATE::POSITION)))
    {
        m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
    }
}

void CIceFall::Reset_State()
{
}

HRESULT CIceFall::Ready_Components()
{
    /* Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    /* Com_VIBuffer */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_IceFall"),
        TEXT("Com_IceFall"), reinterpret_cast<CComponent**>(&m_pIceTex))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_IceShadow"),
        TEXT("Com_IceShadow"), reinterpret_cast<CComponent**>(&m_pShadowTex))))
        return E_FAIL;

    /* Com_Collision */
    CSphere_Collision_Component::Collision_Desc Col_Desc = {};
    Col_Desc.pOwner = this;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SphereCollision"),
        TEXT("Com_SphereCollision"), reinterpret_cast<CComponent**>(&m_pCollision_Com), &Col_Desc)))
        return E_FAIL;
}

HRESULT CIceFall::Render()
{
    if (m_bBreak) {
        CAinimationObject::Render();
        XMLRenderAnimation(L"ice_impact");
    }
    else {
        D3DXMATRIX matPivot, matScale, matRotZ, matTrans, matBillboard, matWorld;
        _float3 pos = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
        matBillboard = m_pCamera->GetTransfrom()->Get_World();
        memcpy((_float3*)&matBillboard.m[3], pos, sizeof(_float3));

        D3DXMatrixScaling(&matScale, 1.5f + m_fYPos * 0.1, 0.75f + m_fYPos * 0.05f, 1.f);
        matWorld = matScale * matBillboard;

        m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
        m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(200 - (int)(m_fYPos * 15), 255, 255, 255));
        m_pShadowTex->Set_Texture(0);
        m_pGraphic_Device->SetTransform(D3DTS_WORLD, &matWorld);
        m_pVIBufferCom->Render();
        m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));



        D3DXMatrixTranslation(&matPivot, -0.003043f, -0.07829f, 0.f);
        D3DXMatrixScaling(&matScale, 493.f / 400.f, 455.f / 400.f, 1.f);
        D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(180.f));
        D3DXMatrixTranslation(&matTrans, 0.f, m_fYPos, 0.f);

        matWorld = matPivot * matScale * matRotZ * matTrans * matBillboard;

        m_pIceTex->Set_Texture(0);
        m_pGraphic_Device->SetTransform(D3DTS_WORLD, &matWorld);
        m_pVIBufferCom->Render();
    }
	return S_OK;
}

void CIceFall::Damage(void* pArg)
{
}

void CIceFall::Death()
{
    __super::Death();
}

void CIceFall::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CIceFall::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
    if (m_bBreak && m_fAniTime == 0 && !dynamic_cast<CDeerclops*>(HitActor)) {
        DamageBaseDesc desc;
        desc.Attacker = nullptr;
        desc.Damage = 30;
        desc.DamageType = ATTACK_TYPE::ICE;
        HitActor->Damage(&desc);
    }
}

void CIceFall::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

CIceFall* CIceFall::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CIceFall* pInstance = new CIceFall(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CIceFall::Clone(void* pArg)
{
	CIceFall* pInstance = new CIceFall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CIceFall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIceFall::Free()
{
	__super::Free();
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pShadowTex);
    Safe_Release(m_pIceTex);
    Safe_Release(m_pCollision_Com);
}
