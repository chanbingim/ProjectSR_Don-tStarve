#include "MouseManager.h"
#include "MouseSlotUI.h"
#include "Picking.h"

HRESULT CMouseManager::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	Mouse_Mgr_Desc* MouseMgr_Data = static_cast<Mouse_Mgr_Desc*>(pArg);
	if (nullptr == MouseMgr_Data)
		return E_FAIL;

	m_MouseSlot = MouseMgr_Data->pMouseSlot;
	m_MouseData = CPicking::Create(MouseMgr_Data->pGraphic_Device, MouseMgr_Data->handle);
	return S_OK;
}

void CMouseManager::Update()
{
	if (m_MouseData)
		m_MouseData->Update();
}

void CMouseManager::Render()
{

}

void CMouseManager::Transform_ToLocalSpace(const _float4x4* pWorldMatrixInverse)
{
	if (nullptr == m_MouseData)
		return;

	m_MouseData->Transform_ToLocalSpace(pWorldMatrixInverse);
}

_float3 CMouseManager::GetMousePosition(_uint ID)
{
	if (nullptr == m_MouseData)
		return _float3(FLT_MAX, FLT_MAX, FLT_MAX);

	return m_MouseData->GetMousePosition(ID);
}

_bool CMouseManager::Picking_InWorldSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut)
{
	if (nullptr == m_MouseData)
		return false;

	return m_MouseData->Picking_InWorldSpace(vPointA, vPointB, vPointC, pOut);
}

_bool CMouseManager::Picking_InLocalSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut)
{
	if (nullptr == m_MouseData)
		return false;

	return m_MouseData->Picking_InLocalSpace(vPointA, vPointB, vPointC,pOut);
}

CMouseManager* CMouseManager::Create(void* pArg)
{
	CMouseManager* pInstance = new CMouseManager();
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("CREATE FAILED : MOUSE_MANAGER");
	}

	return pInstance;
}

void CMouseManager::Free()
{
	__super::Free();

	Safe_Release(m_MouseData);
	Safe_Release(m_MouseSlot);
}
