#pragma once

#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Engine)
class CTransform;
class CVIBuffer;
class CTexture;
NS_END

NS_BEGIN(Client)
class  CBossHpBar final : public CUserInterface
{
private:
	CBossHpBar(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBossHpBar(const CBossHpBar& rhs);
	virtual ~CBossHpBar() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Priority_Update(_float fTimeDelta);
	virtual void		Update(_float fTimeDelta);
	virtual void		Late_Update(_float fTimeDelta);
	virtual HRESULT		Render();

protected:
	CVIBuffer* m_pVIBuffer_Com = { nullptr };
	CTexture* m_pHpBarTex_Com = { nullptr };
	CTexture* m_pHpTex_Com = { nullptr };
	MONSTER_DATA* m_pBossData = { nullptr };
	wstring m_strName = {};
	RECT m_FontRect = {};
private:
	
public:
	HRESULT Ready_Components();
	static	CBossHpBar* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};
NS_END