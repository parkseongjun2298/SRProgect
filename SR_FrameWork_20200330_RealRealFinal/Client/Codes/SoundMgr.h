#pragma once
#ifndef __SOUNDMGR_H__

class CSoundMgr
{
public:
	DECLARE_SINGLETON(CSoundMgr)

public:
	enum CHANNELID {BGM, MINIGAMEBGM, PLAYER, MONSTER, RIDES, EFFECT, UI, MAXCHANNEL};
private:
	CSoundMgr();
	~CSoundMgr();

public:
	void Initialize(); 

	void Release(); 
public:
	void PlaySound(TCHAR* pSoundKey, CHANNELID eID);
	void PlayBGM(TCHAR* pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SoundControl(CHANNELID eID, float fVolume);
	inline void Update() { FMOD_System_Update(m_pSystem); }
private:
	void LoadSoundFile(); 

private:
	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound; 
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL]; 
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem; 

};

#define __SOUNDMGR_H__
#endif // !__SOUNDMGR_H__
