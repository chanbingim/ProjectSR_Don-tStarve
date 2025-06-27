#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CEnviornment_Object : public CGameObject
{
private :
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
	//클라이언트에서 정의되어있는 아이템 출력 버퍼
	CTexture*			m_pTexture_Com = { nullptr };
	CVIBuffer_Rect*		m_pVIBuffer_Com = { nullptr };

private :
	HRESULT				ADD_Components();
	
public :
	static		CEnviornment_Object* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual		CGameObject*		Clone(void* pArg) override;
	virtual		void				Free();

};
NS_END