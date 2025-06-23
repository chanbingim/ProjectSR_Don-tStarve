#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CVIBuffer;
class CTexture;

class CPicking final : public CBase
{
private:
	CPicking(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual ~CPicking() = default;

public:
	HRESULT Initialize(HWND hWnd);
	void	Update();
	void	Transform_ToLocalSpace(const _float4x4* pWorldMatrixInverse);

	// 0 - ��ũ�� ��ǥ, 1 - �� �����̽� ��ǥ, 2 - ���� ��ǥ
	_float3	GetMousePosition(_uint ID);

	_bool	Picking_InWorldSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut);
	_bool	Picking_InLocalSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut);

private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = { nullptr };
	CVIBuffer*			m_VIBuffer_Com = { nullptr };
	CTexture*			m_Texture_Com = { nullptr };

	HWND				m_hWnd = {};
	_uint				m_iWinSizeX{}, m_iWinSizeY{};

	_float3				m_vRayDir[ENUM_CLASS(RAY::END)];
	_float3				m_vRayPos[ENUM_CLASS(RAY::END)];

public:
	static	CPicking*	Create(LPDIRECT3DDEVICE9 pGraphicDevice, HWND Handle);
	virtual void		Free() override;
};

NS_END