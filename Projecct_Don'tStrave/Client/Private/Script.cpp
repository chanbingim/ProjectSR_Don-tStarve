#include "Script.h"
#include "GameInstance.h"

CScript::CScript(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CUserInterface(pGraphic_Device)
{
}

CScript::CScript(const CScript& rhs) :
    CUserInterface(rhs)
{
}

HRESULT CScript::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CScript::Initialize(void* pArg)
{
    if (FAILED(ADD_Components()))
        return E_FAIL;

    UIOBJECT_DESC   Info = {};
    Info.fSizeX = 900.f;
    Info.fSizeY = 400.f;
    Info.fX = 640;
    Info.fY = 520.f;
  
    if(FAILED(__super::Initialize(&Info)))
        return E_FAIL;

    SCRIPT_DESC* Desc  = static_cast<SCRIPT_DESC*>(pArg);
    m_AllText = Desc->DataScript;
    m_FontPoint = { m_fX - (m_fSizeX * 0.4f) , m_fY + 30 };

    m_ZOrder = 2;
    return S_OK;
}

void CScript::Priority_Update(_float fTimeDelta)
{
    if(1.f > m_fAnimationTime)
        m_fAnimationTime += fTimeDelta * 2.f;
}

void CScript::Update(_float fTimeDelta)
{
}

void CScript::Late_Update(_float fTimeDelta)
{
    UpdatePosition();
    if (!m_StartTextView)
    {
        PlayAnimation(0);
    }
    else
    {
        size_t MaxLength = m_AllText[m_TextIndex].length();
        if (MaxLength >= m_iCnt)
        {
            m_PrintText += (m_AllText[m_TextIndex])[m_iCnt];
            m_iCnt++;
        }
        else
        {
            if (m_pGameInstance->KeyDown(VK_RETURN))
            {
                m_iCnt = 0;
                if ((size_t)m_TextIndex < m_AllText.size() - 1)
                {
                    m_TextIndex++;
                    m_PrintText = {};
                }
                else
                {
                    if(m_pGameInstance->GettSinematick())
                        m_pGameInstance->SetSinematick(false);

                    m_isDead = true;
                }
            }
        }    
    }

    m_FontRect = {
          static_cast<long>(m_FontPoint.x),
          static_cast<long>(m_FontPoint.y),
          static_cast<long>(m_FontPoint.x + (m_PrintText.length() * 30)),
          static_cast<long>(m_FontPoint.y + 70),
    };

    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);
}

HRESULT CScript::Render()
{
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());
    m_pTexture_Com->Set_Texture(0);
    m_pVIBuffer_Com->Render();

    D3DXCOLOR white = { 1.f,1.f,1.f,1.f };
    m_pGameInstance->Render_Font(TEXT("Date_40"), m_PrintText.c_str(), &m_FontRect, white, DT_LEFT);

    return S_OK;
}

void CScript::PlayAnimation(_uint type)
{
    switch(type)
    {
    case 0 :
        MovePositionAnim();
        break;

    case 1 :
        ChangeScaleAnim();
        break;
    }
}

void CScript::MovePositionAnim()
{
    _float3 vPos = m_pTransform_Com->GetWorldState(WORLDSTATE::POSITION);
    _float3 TargetPos = vPos - _float3(0.f , 30.f, 0.f);

    _float3 NewPos = {};
    D3DXVec3Lerp(&NewPos, &TargetPos, &vPos, m_fAnimationTime);
    m_pTransform_Com->SetPosition(NewPos);
    m_StartTextView = true;
}

void CScript::ChangeScaleAnim()
{
    _float3 vScale = m_pTransform_Com->GetScale();
    _float3 TargetScale{};

    _float3 NewPos = {};
    D3DXVec3Lerp(&NewPos, &TargetScale, &vScale, m_fAnimationTime);
    m_pTransform_Com->SetScale(NewPos);
    m_StartTextView = true;
}

HRESULT CScript::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransform_Com)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer_Com)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_ScriptFrame"), TEXT("Com_Texture"), (CComponent**)&m_pTexture_Com)))
        return E_FAIL;

    return S_OK;
}

CScript* CScript::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CScript* pInstance = new CScript(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : SCRIPT OBJECT");
    }

    return pInstance;
}

CGameObject* CScript::Clone(void* pArg)
{
    CScript* pInstance = new CScript(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : SCRIPT OBJECT");
    }

    return pInstance;
}

void CScript::Free()
{
    __super::Free();

    Safe_Release(m_pVIBuffer_Com);
    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pTexture_Com);
}
