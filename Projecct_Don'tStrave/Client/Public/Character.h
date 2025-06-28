#pragma once

#include "Client_Defines.h"
#include "LandObject.h"
#include "UserInterface.h"
#include "PlayerAnim.h"


NS_BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CAnimController;
NS_END

NS_BEGIN(Client)

enum DIR {
	DOWN,
	SIDE,
	UP,
	DIR_END
};

class CCharacter : public CLandObject
{
protected:
	CCharacter(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCharacter(const CCharacter& Prototype);
	virtual ~CCharacter() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void		Damage() = 0;
	virtual void		Death() = 0;
	virtual void		Attack() = 0;
	void		Get_Damage(_uint iAtk);
protected:
	_uint			m_iMaxHp = {};
	_uint			m_iTemp = {};
	_uint			m_iAtk = {};
	_uint			m_iDef = {};
	_uint			m_iMaxHit = {};
	_int				m_iHp = {};
	_int				m_iHit = {};
	_bool			m_bAttack;
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CAnimController* m_pAnimController = { nullptr };
	CTransform* m_pAnimTransformCom = { nullptr };
public:
	virtual void Free() override;
};
NS_END