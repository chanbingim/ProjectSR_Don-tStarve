#pragma once

#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Engine)
class CTransform;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CEventButton;

class CQuestBoxEntry : public CUserInterface
{
public :
	typedef struct QuestBoxEntry_desc : public UIOBJECT_DESC
	{
		_uint					EntryIndex;
		CTransform*				pParentTransform_Com;

	} QUESTBOXENTRY_DESC;

private:
	CQuestBoxEntry(LPDIRECT3DDEVICE9 pGraphic_Device);
	CQuestBoxEntry(const CQuestBoxEntry& rhs);
	virtual ~CQuestBoxEntry() = default;

public:
	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize(void* pArg);
	virtual void					Priority_Update(_float fTimeDelta);
	virtual void					Update(_float fTimeDelta);
	virtual void					Late_Update(_float fTimeDelta);
	virtual HRESULT					Render();

private :
	_uint							m_EntryIndex = { 0 };

	CTransform*						m_pParentTransform_Com = { nullptr };
	//CVIBuffer_Rect*				m_pQuestIcon_Com = { nullptr };

	_uint							m_QuestType;
	_wstring						m_QuestTittle;

	CEventButton*					m_AcceptBut = { nullptr };
	CEventButton*					m_CancelBut = { nullptr };

private:
	HRESULT							ADD_Components();

public:
	static	CQuestBoxEntry*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};

NS_END