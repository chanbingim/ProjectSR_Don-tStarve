#include "DamageEffectUI.h"

#include "GameInstance.h"
#include "Player.h"
#include "Level_GamePlay.h"

CDamageEffectUI::CDamageEffectUI(LPDIRECT3DDEVICE9 pGraphic_Device) : CUserInterface(pGraphic_Device)
{
}

HRESULT CDamageEffectUI::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CDamageEffectUI::Initialize(void* pArg)
{
    if (FAILED(ADD_Compoenets()))
        return E_FAIL;

    m_fSizeX = g_iWinSizeX;
    m_fSizeY = g_iWinSizeY;

    m_fX = 0;
    m_fY = 0;

    m_pPlayer = static_cast<CPlayer*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Player")));

    m_Alpha = 0;
    m_fUVTime = 0.f;
    m_fPower = 0.f;
    m_fEnd = 1.f;
    m_fEnding = 0.f;
    m_fEndTime = 0.f;
    Setting_Shader(L"Particle.fx");

    return S_OK;
}

void CDamageEffectUI::Priority_Update(_float fTimeDelta)
{
    m_fUVTime += fTimeDelta * 0.2f;
}

void CDamageEffectUI::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
    if (m_pPlayer->Get_Player()->iMental <= m_pPlayer->Get_Player()->iMaxMental * 0.5f && 0 < m_pPlayer->Get_Char()->iHp) {
        m_fPower = max(0.04f, m_Alpha * 0.5f);
    }
    else {
        m_fPower = 0;
    }
    if (m_bEnd) {
        if (0 <= m_fEnd) {
            m_fEnd -= fTimeDelta;
        }
        else if (1 >= m_fEnding) {
            if (m_bEndBGM) {
                m_pPlayer->Get_Player()->iHp = m_pPlayer->Get_Player()->iMaxHp;
                m_pPlayer->Get_Player()->iMental = m_pPlayer->Get_Player()->iMaxMental;
                m_pPlayer->Get_Player()->iHunger = m_pPlayer->Get_Player()->iMaxHunger;
                m_bEndBGM = false;
                m_pGameInstance->Manager_PlayBGM(L"Ending.mp3", 1.0f);
            }
            m_fEnd = 0;
            m_fEnding += fTimeDelta * 0.5f;
        }
        m_fEndTime += fTimeDelta;
        if (15 <= m_fEndTime) {
            auto GamePlay = dynamic_cast<CLevel_GamePlay*>(m_pGameInstance->CurrentLevel());
            if (GamePlay)
            {
                GamePlay->ChangeLevel(LEVEL::LOGO);
                m_pGameInstance->ChangeGameState(GAMESTATE::GAMEPLAY);
            }
        }
    }
    if (m_bActive)
    {
        if (m_bInverse)
        {
            m_Alpha -= 0.01f;
            if (0.0f >= m_Alpha)
                ResetData();
        }
        else
        {
            m_Alpha += 0.05f;
            if (0.4f <= m_Alpha)
            {
                m_Alpha = 0.4f;
                m_bInverse = true;
            }
        }
    }
}

void CDamageEffectUI::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
    UpdatePosition();
    m_pGameInstance->Add_RenderGroup(RENDER::BLENDUI, this);
}

HRESULT CDamageEffectUI::Render()
{
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());
    //½¦ÀÌ´õ FVF »ý¼º
    m_pGraphic_Device->CreateVertexDeclaration(decl, &m_pDecl);

    LPDIRECT3DBASETEXTURE9 BackTex = {};
    m_pGraphic_Device->GetTexture(7, &BackTex);

    LPDIRECT3DBASETEXTURE9 Tex = {};
    m_pTexture_Com->Set_Texture(0, 0);
    m_pGraphic_Device->GetTexture(0, &Tex);
    LPDIRECT3DBASETEXTURE9 Tex2 = {};
    m_pTexture_Com->Set_Texture(1, 0);
    m_pGraphic_Device->GetTexture(0, &Tex2);
    LPDIRECT3DBASETEXTURE9 Tex3 = {};
    m_pTexture_Com->Set_Texture(2, 0);
    m_pGraphic_Device->GetTexture(0, &Tex3);
    m_pEffect->SetFloat("Alpha", m_Alpha);
    m_pEffect->SetFloat("Uv", m_fUVTime);
    m_pEffect->SetFloat("Power", m_fPower);
    m_pEffect->SetFloat("End", m_fEnd);
    m_pEffect->SetFloat("Ending", m_fEnding);
    m_pEffect->SetFloat("PlayerHP", m_pPlayer->Get_Char()->iHp);

    m_pEffect->SetTexture("TexSrc", BackTex);
    m_pEffect->SetTexture("TexDst", Tex);
    m_pEffect->SetTexture("TexArg", Tex2);
    m_pEffect->SetTexture("TexEnd", Tex3);

   m_pGraphic_Device->SetVertexDeclaration(m_pDecl);
   m_pEffect->Begin(NULL, 0);
   m_pEffect->BeginPass(0);

    m_pVIBuffer_Com->Render();

    m_pEffect->EndPass();
    m_pEffect->End();

    Safe_Release(Tex);
    Safe_Release(BackTex);

    return S_OK;
}

void CDamageEffectUI::ActiveEffect(_float Hp)
{
    m_bActive = true;
    m_Alpha = 0.f;
    m_bInverse = false;
}

void CDamageEffectUI::SetEnd()
{
    m_bEnd = true;
}

HRESULT CDamageEffectUI::ADD_Compoenets()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), (CComponent**)&m_pTransform_Com)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer_Com)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_DamageUI"),
        TEXT("Com_Texture"), (CComponent**)&m_pTexture_Com)))
        return E_FAIL;

    return S_OK;
}

CDamageEffectUI* CDamageEffectUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CDamageEffectUI* pInstance = new CDamageEffectUI(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : DAMAGE EFFECT");
    }
    return pInstance;
}

CUserInterface* CDamageEffectUI::Clone(void* pArg)
{
    CDamageEffectUI* pInstance = new CDamageEffectUI(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : DAMAGE EFFECT");
    }
    return pInstance;
}

void CDamageEffectUI::Free()
{
    __super::Free();

    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);
    Safe_Release(m_pTexture_Com);
}

void CDamageEffectUI::ResetData()
{
    m_Alpha = 0.f;
    m_bInverse = false;
    m_bActive = false;
}
