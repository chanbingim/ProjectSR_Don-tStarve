#pragma once

#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Engine)
class CTransform;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

struct	CQuestData;
class	CEventButton;

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

	void							SetQuestData(_uint QuestIndex, void* pArg);
	void							EntryClickEvent();
	void							EntryCancelClickEvent();

private :
	_uint							m_EntryIndex = { 0 };
	CTransform*						m_pParentTransform = { nullptr };

	_uint							m_QuestListType = {};
	CQuestData*						m_pQuestData = { nullptr };

	_float2							m_fRelativePos;
	RECT							m_FontRect = { 0, 0, 100, 100 };

	CEventButton*					m_AcceptBut = { nullptr };
	CEventButton*					m_CancelBut = { nullptr };
	
	_bool							m_IsShowAcceptBut = { false };

private:
	HRESULT							ADD_Components();
	HRESULT							ADD_Buttons();

	void							AcceptQuest();
	void							ClearQuest();
	void							ShowScriptData();

public:
	static	CQuestBoxEntry*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};

NS_END