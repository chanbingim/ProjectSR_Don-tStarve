#include "Light_Manager.h"
#include "LightComponent.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CLight_Manager);

CLight_Manager::CLight_Manager()
{
	
}

HRESULT CLight_Manager::Initialize(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	m_pGraphic_Device = pGraphic_Device;
	Safe_AddRef(m_pGraphic_Device);

	m_pGraphic_Device->LightEnable(0, true);

	return S_OK;
}

void CLight_Manager::UpdateLight()
{
	Sorting_Light(1);
}

HRESULT CLight_Manager::ADD_Light(LIGHT_TYPE LightType, CLightComponent* pLight)
{
	_uint Index = ENUM_CLASS(LightType);

	auto iter = find(m_Lightlist[Index].begin(), m_Lightlist[Index].end(), pLight);
	if (iter != m_Lightlist[Index].end())
		return E_FAIL;

	m_Lightlist[Index].push_back(pLight);

	return S_OK;
}

void CLight_Manager::DeadLight(LIGHT_TYPE LightType, CLightComponent* pLight)
{
	_uint Index = ENUM_CLASS(LightType);
	auto iter = find(m_Lightlist[Index].begin(), m_Lightlist[Index].end(), pLight);
	if (iter == m_Lightlist[Index].end())
		return;

	m_Lightlist[Index].erase(iter);
}

void CLight_Manager::Enable_Light()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);
	if(0 < m_Lightlist[ENUM_CLASS(LIGHT_TYPE::DIRECATION)].size())
	(*m_Lightlist[ENUM_CLASS(LIGHT_TYPE::DIRECATION)].begin())->Render_Light();

	//Spot조명 적용
	auto iter = m_Lightlist[ENUM_CLASS(LIGHT_TYPE::POINT)].begin();
	auto ApplyLight_End = m_Lightlist[ENUM_CLASS(LIGHT_TYPE::POINT)].end();
	for (_uint i = 1; i <= 7; ++i)
	{
		if (iter == ApplyLight_End)
		{
			for(int j = i; j <= 7; ++j)
				m_pGraphic_Device->LightEnable(j, false);
			break;
		}

		m_pGraphic_Device->LightEnable(i, true);
		(*iter)->Render_Light(i);
		iter++;
	}
}

void CLight_Manager::UnEnable_Light()
{
}

void CLight_Manager::Sorting_Light(_uint type)
{
	m_Lightlist[ENUM_CLASS(LIGHT_TYPE::POINT)].sort([&](CLightComponent* pSrc, CLightComponent* pDst)
		{
			if (1 == type)
				return pSrc->GetOwner()->Get_CameraDistance() < pDst->GetOwner()->Get_CameraDistance();
			else
				return pSrc->GetOwner()->Get_CameraDistance() > pDst->GetOwner()->Get_CameraDistance();
		});
}

void CLight_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pGraphic_Device);

	for (_uint i = 0; i < ENUM_CLASS(LIGHT_TYPE::END); ++i)
		m_Lightlist[i].clear();
}
