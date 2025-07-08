#include "BirchnutLeaf.h"
#include "GameInstance.h"

#include "XML_Manager.h"

CBirchnutLeaf::CBirchnutLeaf(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CEnviornment_Object(pGraphic_Device)
{
}

CBirchnutLeaf::CBirchnutLeaf(const CEnviornment_Object& rhs) :
    CEnviornment_Object(rhs)
{
}

HRESULT CBirchnutLeaf::Initialize_Prototype()
{
    auto XML_Instance = CXML_Manager::GetInstance();
    XML_Instance->AddTexture("../Bin/Resources/Textures/Objects/Evergreen/evergreen_new.scml", L"../Bin/Resources/Textures/Objects/Evergreen/", &m_tImageVec);
    XML_Instance->LoadScml("../Bin/Resources/Textures/Objects/Evergreen/evergreen_new.scml", &m_tAnimation);


    return S_OK;
}

HRESULT CBirchnutLeaf::Initialize(void* pArg)
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
        m_FrontName = Leaf_Desc->szFrontName;
        m_TailName = Leaf_Desc->szTailName;
    }
    m_pOwner->GetState();
    Safe_AddRef(m_pOwner);

	m_EnviromentState = Enviornment_STATE::IDLE;

    return S_OK;
}

void CBirchnutLeaf::Priority_Update(_float fTimeDelta)
{
}

void CBirchnutLeaf::Update(_float fTimeDelta)
{
    //부모의 모션을 따라감
}

void CBirchnutLeaf::Late_Update(_float fTimeDelta)
{
}

void CBirchnutLeaf::Reset_State()
{
}

HRESULT CBirchnutLeaf::Render()
{
    __super::Render();
    return S_OK;
}

HRESULT CBirchnutLeaf::ADD_Components()
{
    /*Transform Com*/
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
        return E_FAIL;

    /*VI_Buffer Com*/
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    return S_OK;
}

CBirchnutLeaf* CBirchnutLeaf::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _wstring FolderName, const _wstring FilePath)
{
    CBirchnutLeaf* pInstance = new CBirchnutLeaf(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : BIRCHNUT LEAF");
    }
    return pInstance;
}

CGameObject* CBirchnutLeaf::Clone(void* pArg)
{
    CBirchnutLeaf* pInstance = new CBirchnutLeaf(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : BIRCHNUT LEAF");
    }
    return pInstance;
}

void CBirchnutLeaf::Free()
{
    __super::Free();
    Safe_Release(m_pOwner);
}
