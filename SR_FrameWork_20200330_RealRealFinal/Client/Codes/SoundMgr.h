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
	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD_SOUND*> m_mapSound; 
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL]; 
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem; 

};

#define __SOUNDMGR_H__
#endif // !__SOUNDMGR_H__
