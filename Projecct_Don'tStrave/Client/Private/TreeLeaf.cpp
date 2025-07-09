#include "TreeLeaf.h"
#include "GameInstance.h"

#include "XML_Manager.h"
#include "CUtility.h"

CTreeLeaf::CTreeLeaf(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CEnviornment_Object(pGraphic_Device)
{
}

CTreeLeaf::CTreeLeaf(const CEnviornment_Object& rhs) :
    CEnviornment_Object(rhs)
{
}

HRESULT CTreeLeaf::Initialize_Prototype(const char* XMLFilePath, const _wstring FolderName)
{
    auto XML_Instance = CXML_Manager::GetInstance();

    WCHAR FullFilePath[MAX_PATH] = {};
    wsprintf(FullFilePath, TEXT("../Bin/Resources/Textures/Objects/%s/"), FolderName.c_str());

    char XMLFullFilePath[MAX_PATH] = {};
    CUtility::ConvertWideToUTF(FullFilePath, XMLFullFilePath);
    sprintf_s(XMLFullFilePath, "%s%s", XMLFullFilePath, XMLFilePath);

    XML_Instance->AddTexture(XMLFullFilePath, FullFilePath, &m_tImageVec);
    XML_Instance->LoadScml(XMLFullFilePath, &m_tAnimation);

    return S_OK;
}

HRESULT CTreeLeaf::Initialize(void* pArg)
{
	if (FAILED(ADD_Components()))
		return E_FAIL;
       

    if (nullptr == pArg)
    {

    }
    else
    {
        LEAF_DESC* Leaf_Desc = static_cast<LEAF_DESC*>(pArg);
        m_pOwner = Leaf_Desc->pOwner;
        LoadImageFile();
    }

    CLandObject::GAMEOBJECT_DESC Arg = { };
    Arg.vPosition = m_pOwner->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION);
    Arg.vScale = m_pOwner->GetTransfrom()->GetScale();
    Arg.vRotation = { 0.f, 0.f, 0.f };

    if (FAILED(__super::Initialize(&Arg)))
        return E_FAIL;

    m_TailName = TEXT("");
    m_EnviromentState =  m_pOwner->GetState();

    return S_OK;
}

void CTreeLeaf::Priority_Update(_float fTimeDelta)
{

}

void CTreeLeaf::Update(_float fTimeDelta)
{
}

void CTreeLeaf::Late_Update(_float fTimeDelta)
{
}

void CTreeLeaf::Reset_State()
{
   

}

HRESULT CTreeLeaf::Render()
{
    m_fAniTime = m_pOwner->GetAnimationFrame();
    XMLRenderAnimation(m_pOwner->GetMotionName());
    return S_OK;
}

HRESULT CTreeLeaf::ADD_Components()
{
    /*Transform Com*/
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
        return E_FAIL;

    /*VI_Buffer Com*/
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    return S_OK;
}

CTreeLeaf* CTreeLeaf::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const char* XMLFilePath, const _wstring FolderName)
{
    CTreeLeaf* pInstance = new CTreeLeaf(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype(XMLFilePath, FolderName)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : BIRCHNUT LEAF");
    }
    return pInstance;
}

CGameObject* CTreeLeaf::Clone(void* pArg)
{
    CTreeLeaf* pInstance = new CTreeLeaf(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : BIRCHNUT LEAF");
    }
    return pInstance;
}

void CTreeLeaf::Free()
{
    __super::Free();

}
