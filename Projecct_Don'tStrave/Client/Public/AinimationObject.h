#pragma once

#include "Client_Defines.h"

#include "LandObject.h"
#include "XMLDataStruct.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CTerrian_Manager;
class CCamera;

class CAinimationObject : public CLandObject
{
protected:
	CAinimationObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAinimationObject(const CAinimationObject& rhs);
	virtual ~CAinimationObject() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual void			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

	HRESULT					LoadImageFile(vector<IMAGE_FOLDER_DESC>* tImageVec = nullptr);

	void					XMLRenderAnimation(const wstring& animName, Entity* tEntity = nullptr, vector<IMAGE_FOLDER_DESC>* AnimVec = nullptr);
protected:
	_uint						m_fAniTime = {};
	_uint						m_iLength = {};
	_float						m_fZAngle = {};

	Entity						m_tAnimation = {};
	vector<IMAGE_FOLDER_DESC>	m_tImageVec = {};

	CCamera*					m_pCamera = { nullptr };
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	CTerrian_Manager*			m_pTerrian_Manager = { nullptr };

	_bool						m_bAnimPause = { false };
public:
	virtual		CGameObject*		Clone(void* pArg) override;
	virtual		void				Free();

};
NS_END