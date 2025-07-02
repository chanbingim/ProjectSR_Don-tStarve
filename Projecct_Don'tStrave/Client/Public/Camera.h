#pragma once

#include "Client_Defines.h"
#include "FrustomCamera.h"

#include "GameInstance.h"

NS_BEGIN(Client)

class CCamera final : public CFrustomCamera
{
public:
	typedef struct tagCameraDesc : public CTransform::TRANSFORM_DESC
	{
		_float3		vAt{};
		_float3		vEye{};

		_float		fFov{};
		_float		fNear{}, fFar{};
	}CAMERA_DESC;
private:
	CCamera(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCamera(const CCamera& Prototype);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Initialize_Late() override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	class CTransform*				m_pPlayerTransformCom = { nullptr };
	class CCamera_Button*			m_pButton_Left = { nullptr };
	class CCamera_Button*			m_pButton_Right = { nullptr };

private:
	_float2							m_vOldMouse = {};
	_float							m_fSensor = { 0.2f };

private:
	HRESULT Ready_Components(void* pArg);
public:
	static CCamera* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END