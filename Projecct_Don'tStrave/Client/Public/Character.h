#pragma once

#include "Client_Defines.h"
#include "AlphaObject.h"
#include "UserInterface.h"
#include "PlayerAnim.h"
#include "Character_Manager.h"

NS_BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CAnimController;
class CCollision_Component;
NS_END

NS_BEGIN(Client)

class CTerrian_Manager;

enum DIR {
	DOWN,
	SIDE,
	UP,
	DIR_END
};

enum MOVE_DIR {
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_END
};

class CCharacter : public CAlphaObject
{
	typedef struct Object_desc {
		_uint iFolder;
		_uint iFile;
		_float2 fPos;
		_float fAngle;
		_float2 fScale;
	}OBJECT_DESC;

	typedef struct Object_Ref_desc {
		_uint iTimeline;
		_uint iZindex;
	}OBJECT_REF_DESC;

	typedef struct Key {
		_uint iId;
		_uint iTime;
		OBJECT_DESC tObj;
	}KEY_DESC;

	typedef struct MainKey {
		_uint iTime;
		vector<OBJECT_REF_DESC> tRefVec;
	}MAINKEY_DESC;

	typedef struct Mainline {
		vector<MAINKEY_DESC> tKeysVec;
	}MAINLINE_DESC;

	typedef struct Timeline {
		_uint	iId;
		wstring szName;
		vector<KEY_DESC> tKeysVec;
	}TIMELINE_DESC;

	typedef struct Scml_Animation {
		wstring szName;
		_uint iLength;
		MAINLINE_DESC tMainlinesVec;
		vector<TIMELINE_DESC> tTimelinesVec;
	}SCML_ANIMATION_DESC;

	typedef struct Entity {
		wstring szName;
		vector<SCML_ANIMATION_DESC> tAnimationsVec;
	}ENTITY_DESC;

	typedef struct Image_File {
		_uint iId;
		wstring szName;
		CTexture* pTexture;
		_float2 fSize;
		_float2 fPivot;
	}IMAGE_FILE_DESC;

	typedef struct Image_Folder {
		_uint iId;
		wstring szName;
		vector<IMAGE_FILE_DESC> tFilesVec;
	}IMAGE_FOLDER_DESC;

	typedef struct Character_desc {
		_uint			m_iMaxHp = {};
		_uint			m_iTemp = {};
		_uint			m_iAtk = {};
		_uint			m_iDef = {};
		_uint			m_iMaxHit = {};
	}CHARACTER_DESC;
protected:
	CCharacter(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCharacter(const CCharacter& Prototype);
	virtual ~CCharacter() = default;
private:
	wstring Get_wstring(const char* str) {
		int len = MultiByteToWideChar(CP_ACP, 0, str, -1, nullptr, 0);
		wstring wFileName(len, 0);
		MultiByteToWideChar(CP_ACP, 0, str, -1, &wFileName[0], len);
		return wFileName;
	}
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void		Attack() = 0;
	virtual void		Get_Damage(_uint iAtk);
	void		SetDir();
	HRESULT AddTexture(const _char* pScmlFilePath, const _tchar* pTextureFilePath);
	HRESULT LoadImageFile();
	HRESULT LoadScml(const char* filename);
	void RenderAnimation(const wstring& animName);

protected:
	_uint			m_iMaxHp = {};
	_uint			m_iTemp = {};
	_uint			m_iAtk = {};
	_float			m_fSpeed = {};
	_uint			m_iMaxHit = {};
	_float3			m_fMoving = {};
	_float			m_fAngle = {};
	_float			m_fCamDistance = {};
	MOVE_DIR			m_tMoveDIr = {};
	DIR				m_tDir = {};
	_uint			m_iDir = {};
	_int				m_iHp = {};
	_int				m_iHit = {};
	_uint			m_iLength = {};
	_bool			m_bAttack;
	_float			m_fAniTime = {};
	wstring			m_sAnim;

	Entity						m_tAnimation = {};
	vector<IMAGE_FOLDER_DESC>	m_tImageVec = {};
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	CCharacter_Manager*			m_pCharacterInstance = { nullptr };
	CTerrian_Manager*			m_pTerrian_Manager = { nullptr };
	CCollision_Component*		m_pCollision_Com = { nullptr };

public:
	HRESULT Ready_Components();
	virtual void Free() override;
};
NS_END