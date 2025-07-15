#include "SpriteEffect.h"

#include "XML_Manager.h"
#include "GameInstance.h"
#include "CUtility.h"

CSpriteEffect::CSpriteEffect(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CAinimationObject(pGraphic_Device)
{
}

CSpriteEffect::CSpriteEffect(const CSpriteEffect& rhs) :
    CAinimationObject(rhs)
{
}

HRESULT CSpriteEffect::Initialize_Prototype(const char* FilePath, const _wstring FolderName)
{
    auto XML_Instance = CXML_Manager::GetInstance();

    WCHAR FullFilePath[MAX_PATH] = {};
    wsprintf(FullFilePath, TEXT("../Bin/Resources/Textures/Particles/%s/"), FolderName.c_str());

    char XMLFullFilePath[MAX_PATH] = {};
    CUtility::ConvertWideToUTF(FullFilePath, XMLFullFilePath);
    sprintf_s(XMLFullFilePath, "%s%s", XMLFullFilePath, FilePath);

    XML_Instance->AddTexture(XMLFullFilePath, FullFilePath, &m_tImageVec);
    XML_Instance->LoadScml(XMLFullFilePath, &m_tAnimation);

    return S_OK;
}

HRESULT CSpriteEffect::Initialize(void* pArg)
{
    if (FAILED(ADD_Component()))
        return E_FAIL;

    LoadImageFile();
    return S_OK;
}

void CSpriteEffect::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);

    if (m_fAniTime >= m_iLength)
    {
        m_isDead = true;
    }
}

void CSpriteEffect::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CSpriteEffect::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CSpriteEffect::Render()
{
    size_t delimiterPos = m_AnimName.rfind('_');
    auto Name = m_AnimName.substr(0, delimiterPos);

    XMLRenderAnimation(m_AnimName);
    return S_OK;
}

void CSpriteEffect::ReadyEffect(void* pArg)
{
    SPRITE_EFFECT* Sprite_Desc = static_cast<SPRITE_EFFECT*>(pArg);
    m_AnimName = Sprite_Desc->AnimName;
}

void CSpriteEffect::ResetObejctaData()
{
    m_fAniTime = 0;
    m_AnimName = TEXT("");
    m_isDead = false;
}

_bool CSpriteEffect::IsFinished()
{
    return m_isDead;
}

HRESULT CSpriteEffect::ADD_Component()
{
    /* Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    return S_OK;
}

CSpriteEffect* CSpriteEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const char* FilePath, const _wstring FolderName)
{
    CSpriteEffect* pInstance = new CSpriteEffect(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype(FilePath, FolderName)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : SPRITE EFFECT");
    }
    return pInstance;
}

CGameObject* CSpriteEffect::Clone(void* pArg)
{
    CSpriteEffect* pInstance = new CSpriteEffect(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : SPRITE EFFECT");
    }
    return pInstance;
}

void CSpriteEffect::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
}
