#include "BossHpBar.h"
#include "GameInstance.h"
#include "MonsterData_Manager.h"

CBossHpBar::CBossHpBar(LPDIRECT3DDEVICE9 pGraphic_Device):
    CUserInterface(pGraphic_Device)
{
}

CBossHpBar::CBossHpBar(const CBossHpBar& rhs):
    CUserInterface(rhs)
{
}

HRESULT CBossHpBar::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBossHpBar::Initialize(void* pArg)
{
    CUserInterface::UIOBJECT_DESC Desc = {};

    Desc.fX = g_iWinSizeX * 0.5f;
    Desc.fY = 80.f;
    Desc.fSizeX = 100.f;
    Desc.fSizeY = 100.f;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    __super::UpdatePosition();


    D3DVIEWPORT9		ViewportDesc{};
    m_pGraphic_Device->GetViewport(&ViewportDesc);


    m_pBossData = static_cast<MONSTER_DATA*>(pArg);

    m_strName = CMonsterData_Manager::GetInstance()->Get_MonsterData(m_pBossData->iId).strName;
    m_FontRect = {
        _long(m_fX - m_fSizeX * 0.5f),
        _long(m_fY - 20),
        _long(m_fX + m_fSizeX * 0.5f),
        _long(m_fY + 10)
    };

    return S_OK;
}

void CBossHpBar::Priority_Update(_float fTimeDelta)
{
}

void CBossHpBar::Update(_float fTimeDelta)
{
    if (0 >= m_pBossData->iHp)
        m_isDead = true;
}

void CBossHpBar::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);
}

HRESULT CBossHpBar::Render()
{
    __super::Render();

    D3DXMATRIX matPivot, matScale, matHpTrans, matTrans, matBillboard, matWorld;

    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matBillboard);
    matBillboard._41 = matBillboard._42 = matBillboard._43 = 0.0f;
    D3DXMatrixTranspose(&matBillboard, &matBillboard);


    D3DXMatrixScaling(&matScale, 1082.f / 120.f, 188.f / 120.f, 1.f);
    D3DXMatrixTranslation(&matTrans, m_pBossData->fPos.x, m_pBossData->fPos.y, 0.f);

    matWorld = matScale * m_pTransform_Com->Get_World();

    m_pHpBarTex_Com->Set_Texture(0);
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &matWorld);
    m_pVIBuffer_Com->Render();



    D3DXMatrixTranslation(&matPivot, 0.5f - 10 / 1082.f, 0.f, 0.f);
    D3DXMatrixScaling(&matScale, (_float)m_pBossData->iHp / m_pBossData->iMaxHp * 1082.f / 120.f, 188.f / 120.f, 1.f);
    D3DXMatrixTranslation(&matHpTrans, -(1082.f / 240.f) + ((10 / 120.f)), 0.f, 0.f);


    D3DXMatrixTranslation(&matTrans, m_pBossData->fPos.x, m_pBossData->fPos.y, m_pBossData->fPos.z);

    matWorld = matPivot * matScale * matHpTrans * m_pTransform_Com->Get_World();

    m_pHpTex_Com->Set_Texture(0);
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &matWorld);
    m_pVIBuffer_Com->Render();



    D3DXCOLOR Black = { 0.f,0.f,0.f,1.f };


    m_pGameInstance->Render_Font(TEXT("Font_25"), m_strName.c_str(), &m_FontRect, Black, DT_CENTER);





    return S_OK;
}

HRESULT CBossHpBar::Ready_Components()
{
    /* Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.0f) };
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransform_Com), &TransformDesc)))
        return E_FAIL;

    /* Com_VIBuffer */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBuffer_Com))))
        return E_FAIL;

    /* Com_VIBuffer */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_BossHp"),
        TEXT("Com_Hp"), reinterpret_cast<CComponent**>(&m_pHpTex_Com))))
        return E_FAIL;

    /* Com_VIBuffer */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_BossHpBar"),
        TEXT("Com_HpBar"), reinterpret_cast<CComponent**>(&m_pHpBarTex_Com))))
        return E_FAIL;

    return S_OK;
}

CBossHpBar* CBossHpBar::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBossHpBar* pInstance = new CBossHpBar(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Created : CBossHpBar");
    }
    return pInstance;
}

CGameObject* CBossHpBar::Clone(void* pArg)
{
	CBossHpBar* pInstance = new CBossHpBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBossHpBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBossHpBar::Free()
{
    __super::Free();
    Safe_Release(m_pTransform_Com);
	Safe_Release(m_pVIBuffer_Com);
	Safe_Release(m_pHpBarTex_Com);
	Safe_Release(m_pHpTex_Com);
}
