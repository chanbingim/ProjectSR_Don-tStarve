#include "AnimationUI.h"
#include "GameInstance.h"
#include "UI_MoveAnim.h"

CAnimationUI::CAnimationUI(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CUserInterface(pGraphic_Device)
{
}

CAnimationUI::CAnimationUI(const CAnimationUI& rhs) : 
    CUserInterface(rhs)
{
}

HRESULT CAnimationUI::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CAnimationUI::Initialize(void* pArg)
{
    UIOBJECT_DESC	Desc{};
    Desc.fSizeX = 200.0f;
    Desc.fSizeY = 200.0f;
    Desc.fX = 100.0f;
    Desc.fY = 100.0f;
    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    CUI_MoveAnim::UI_MOVE_DESC MoveDesc;
    MoveDesc.pTransformCom = m_pTransform_Com;
    MoveDesc.Frame.pAnimTexture = m_pTexture_Com;
    MoveDesc.StartPoint = _float3(300.f, 200.f, 0.f);
    MoveDesc.EndPoint   = _float3(100.f, 100.f, 0.f);

    MoveDesc.fDurTime = 0.3f;
    MoveDesc.Frame.bIsLoop = true;
    MoveDesc.Frame.iStartFrame = 0;
    MoveDesc.Frame.iEndFrame = 37;
    MoveDesc.Frame.fTimeRate = 1.0f;

    m_pUIMoveAnim = CUI_MoveAnim::Create(&MoveDesc);
    m_pAnimController->ChangeState(m_pUIMoveAnim);

    return S_OK;
}

void CAnimationUI::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);

    //이거 m_fx, m_fy로 UI를 제어해서 바꿀방법이 딱히 생각나지않음
    //이대로 해야할거같기도하고
    m_pAnimController->Tick(fTimeDelta);
    m_fX = m_pTransform_Com->GetWorldState(WORLDSTATE::POSITION).x;
    m_fY = m_pTransform_Com->GetWorldState(WORLDSTATE::POSITION).y;
}

void CAnimationUI::Update(_float fTimeDelta)
{
    //위치 변경하면 변경된 후에 호출해줘야 함
    __super::Update(fTimeDelta);
}

void CAnimationUI::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);
}

HRESULT CAnimationUI::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;

    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());
    m_pAnimController->Render();
    m_pVIBuffer_Com->Render();

    return E_NOTIMPL;
}

void CAnimationUI::HoverEevent()
{

}

void CAnimationUI::ClickedEevent()
{

}

HRESULT CAnimationUI::ADD_Components()
{
    CTransform::Transform_Desc Transform_Desc;
    Transform_Desc.m_MoveSpeedSec = 2.f;
    Transform_Desc.m_TurnSpeedSec = 2.f;
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransform_Com, &Transform_Desc)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer_Com)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_AnimController"), TEXT("Com_AnimController"), (CComponent**)&m_pAnimController)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Loading"), TEXT("Com_Texture"), (CComponent**)&m_pTexture_Com)))
        return E_FAIL;

    return S_OK;
}

CAnimationUI* CAnimationUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CAnimationUI* pInstance = new CAnimationUI(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : ANIMATION USER INTERFACE");
    }

    return pInstance;
}

CGameObject* CAnimationUI::Clone(void* pArg)
{
    CAnimationUI* pInstance = new CAnimationUI(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : ANIMATION USER INTERFACE");
    }

    return pInstance;
}

void CAnimationUI::Free()
{
    __super::Free();

    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);
    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pAnimController);
    Safe_Release(m_pUIMoveAnim);
}
