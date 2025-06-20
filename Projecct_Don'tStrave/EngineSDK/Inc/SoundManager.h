#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CSoundManager : public CBase
{
private :
	CSoundManager() = default;
	virtual ~CSoundManager() = default;

public:
	void Initialize();

public:
	void Manager_PlaySound(const TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void Manager_PlayBGM(const TCHAR* pSoundKey, float fVolume);
	void Manager_StopSound(CHANNELID eID);
	void Manager_StopAll();
	void Manager_SetChannelVolume(CHANNELID eID, float fVolume);

private:
	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD::Sound*> m_mapSound;
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD::Channel* m_pChannelArr[SOUND_END];
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD::System* m_pSystem;

private :
	void LoadSoundFile();

public :
	static	CSoundManager*	Create();
	virtual void			Free() override;

};
NS_END