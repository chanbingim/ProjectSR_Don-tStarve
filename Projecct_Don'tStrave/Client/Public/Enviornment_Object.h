#pragma once

#include "Client_Defines.h"
#include "LandObject.h"

NS_BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CCollision_Component;
class CAnimController;
NS_END

NS_BEGIN(Client)
class CEnviornment_Object : public CLandObject
{
public:
	enum class Enviornment_STATE { IDLE, DAMAGED, BROKEN, END };

protected :
	CEnviornment_Object(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEnviornment_Object(const CEnviornment_Object& rhs);
	virtual ~CEnviornment_Object() = default;

public :
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

protected :
	_matrix				m_WorldMat = {};

	//클라이언트에서 정의되어있는 아이템 출력 버퍼
	CTexture*				m_Idle_pTexture_Com = { nullptr };
	CVIBuffer_Rect*			m_pVIBuffer_Com = { nullptr };
	CCollision_Component*	m_pCollision_Com = { nullptr };

public :
	static		CEnviornment_Object* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual		CGameObject*		Clone(void* pArg) override;
	virtual		void				Free();

};
NS_END