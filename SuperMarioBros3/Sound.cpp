
/*
#include "Sound.h"

Sound* Sound::__Instance = NULL;

Sound::Sound()
{
	Init();
	LoadResourceSound();
}



void Sound::LoadResourceSound()
{
	/*
	AddSound(1, L"Res/sound/usingwhip.wav");
	*/
/*

}
bool Sound::isPlaying(int type)
{
	return _ArrSound[type]->IsSoundPlaying();
}

void Sound::StopAll()
{
	for (auto& x : _ArrSound)
	{
		if (x.second->IsSoundPlaying())
			x.second->Stop();
	}
}


void Sound::AddSound(int type, LPTSTR filename)
{
	CSound* wave;
	HRESULT result = dsound->Create(&wave, filename);
	if (result != DS_OK)
	{
		return;
	}
	_ArrSound[type] = wave;
}

void Sound::Play(int type, bool isLoop, LONG lVolume)
{
	_ArrSound[type]->Play(0, isLoop, lVolume);
	//DSBPLAY_LOOPING             0x00000001 -> int = 1

}

void Sound::Stop(int type)
{
	_ArrSound[type]->Stop();
}



void Sound::Init()
{
	//dsound = new CSoundManager();
	//dsound->Initialize(CGame::GetInstance()->GetHWND(), DSSCL_PRIORITY);
	//dsound->SetPrimaryBufferFormat(2, 22050, 16);
}

Sound* Sound::GetInstance()
{
	if (__Instance == NULL)
		__Instance = new Sound();
	return __Instance;
}
*/
