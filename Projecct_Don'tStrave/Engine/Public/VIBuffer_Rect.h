#pragma once

#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect final : public CVIBuffer
{
private:
	CVIBuffer_Rect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Rect(const CVIBuffer_Rect& Prototype);
	virtual ~CVIBuffer_Rect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

	virtual _bool Picking(class CTransform* pTransform, _float3* pOut = nullptr)override;
public:
	static CVIBuffer_Rect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

	void SetUV(_uint x, _uint maxX, _uint maxY, _uint frame, _uint maxFrame, _bool mirror);
};

NS_END