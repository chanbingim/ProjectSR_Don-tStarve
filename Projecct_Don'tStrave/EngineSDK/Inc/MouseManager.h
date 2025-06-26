#pragma once

#include "Base.h"

NS_BEGIN(Engine)
class CPicking;
class CMouseSlotUI;

class CMouseManager : public CBase
{
public :
	typedef struct Mouse_Mgr_Desc
	{
		CMouseSlotUI*			pMouseSlot;

		LPDIRECT3DDEVICE9		pGraphic_Device = { nullptr };
		HWND					handle = { nullptr };
	}MOUSE_MANGER_DESC;

private :
	CMouseManager() = default;
	virtual ~CMouseManager() = default;

public:
	CMouseSlotUI* Chagne_Slot(CMouseSlotUI* pSlot);

public:
	HRESULT						Initialize(void* pArg);
	void						Update();
	void						Render();

#pragma region PICKING
	void						Transform_ToLocalSpace(const _float4x4* pWorldMatrixInverse);
	// 0 - 스크린 좌표, 1 - 뷰 스페이스 좌표, 2 - 월드 좌표
	_float3						GetMousePosition(_uint ID);

	_bool						Picking_InWorldSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut);
	_bool						Picking_InLocalSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut);
#pragma endregion

private :
	CMouseSlotUI*				m_MouseSlot = { nullptr };
	CPicking*					m_MouseData = { nullptr };

	_bool						m_bHover = { false };
	_bool						m_bPicking = { false };

public :
	//마우스 객체 만들어서 던져주면 그 마우스 객체UI사용가능
	static		CMouseManager*		Create(void* pArg);
	virtual		void				Free() override;
};
NS_END