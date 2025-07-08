#include "BirchnutLeaf.h"

CBirchnutLeaf::CBirchnutLeaf(LPDIRECT3DDEVICE9 pGraphic_Device)
{
}

CBirchnutLeaf::CBirchnutLeaf(const CEnviornment_Object& rhs)
{
}

HRESULT CBirchnutLeaf::Initialize_Prototype()
{
    return E_NOTIMPL;
}

HRESULT CBirchnutLeaf::Initialize(void* pArg)
{
    return E_NOTIMPL;
}

void CBirchnutLeaf::Priority_Update(_float fTimeDelta)
{
}

void CBirchnutLeaf::Update(_float fTimeDelta)
{
}

void CBirchnutLeaf::Late_Update(_float fTimeDelta)
{
}

void CBirchnutLeaf::Reset_State()
{
}

HRESULT CBirchnutLeaf::Render()
{
    return E_NOTIMPL;
}

HRESULT CBirchnutLeaf::ADD_Components()
{
    return E_NOTIMPL;
}

CBirchnutLeaf* CBirchnutLeaf::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _wstring FolderName, const _wstring FilePath)
{
    return nullptr;
}

CGameObject* CBirchnutLeaf::Clone(void* pArg)
{
    return nullptr;
}

void CBirchnutLeaf::Free()
{
}
