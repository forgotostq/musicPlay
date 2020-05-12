
#include<Windows.h>
#include<digitalv.h>
#include<mmsystem.h>
#include<conio.h>
#include<Mmsystem.h>
#include<string>
#include<iostream>
#include<atlstr.h>
#include<thread>
#include<vector>
#pragma comment(lib,"WINMM.LIB")

//�ο����� https://www.cnblogs.com/kuangke/p/6626845.html?tdsourcetag=s_pctim_aiomsg

using namespace std;
MCIDEVICEID wDevioceId;



int BackgroundMusicVol = 500;
int SoundEffectVol = 500;
bool endflag = false;
enum MusicType { backgroundMusic, soundEffect };
class SoundManager
{
public:
	void playSound(const CString& strPath, MusicType musicType);
	void SetBackgroundMusicVol(int vol); 
	void SetSoundEffectVol(int vol);

	vector<thread> musicList;

	~SoundManager();
};

 SoundManager::~SoundManager()
{
	 endflag = true;
	 for (int i = 0; i < musicList.size(); i++)
	 {
		 musicList[i].join();
	 }
	 musicList.clear();
}


DWORD MCIPlay(DWORD DeviceID, MusicType musicType)
{
	MCI_PLAY_PARMS mciPP;
	DWORD opReturn;
	if (musicType == soundEffect)
	{
		opReturn = mciSendCommand(wDevioceId, MCI_PLAY, MCI_NOTIFY, (DWORD)&mciPP);
		return opReturn;
	}
	else if (musicType == backgroundMusic)
	{
		opReturn = mciSendCommand(wDevioceId, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)&mciPP);
		return opReturn;
	}

}

void Close_MCI(DWORD DeviceID)
{
	DWORD retclose = mciSendCommand(DeviceID, MCI_CLOSE, NULL, NULL);
}

//��������
DWORD MCISetVolume(DWORD dwVol,DWORD DeviceID)
{
	MCI_DGV_SETAUDIO_PARMS mciSetAudioPara;
	mciSetAudioPara.dwCallback = NULL;
	mciSetAudioPara.dwItem = MCI_DGV_SETAUDIO_VOLUME;
	mciSetAudioPara.dwValue = dwVol; //����0-1000
	DWORD dwReturn = mciSendCommand(DeviceID,MCI_SETAUDIO, MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD)(LPVOID)&mciSetAudioPara);
	return dwReturn;
}



DWORD SOUND_RUN(const CString& strPath,MusicType musicType)
{
	//��MCI�豸
	MCI_OPEN_PARMS mciOP;
	DWORD opReturn;
	MCIDEVICEID wIDDevioce; //�����豸ID
	

	mciOP.lpstrDeviceType = NULL;
	mciOP.lpstrElementName = strPath; //���ô򿪵��ļ�������
	opReturn = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&mciOP);
	if (opReturn == 0)
	{
		wIDDevioce = mciOP.wDeviceID; //�����豸ID
		wDevioceId = mciOP.wDeviceID;

		//���õ�ǰ����λ����ת60��
		//MCISeek(100);


		//����һ��������С
		int vol;
		vol = (musicType == backgroundMusic) ? BackgroundMusicVol : SoundEffectVol;
		MCISetVolume(vol, wIDDevioce);

		//��Ƶ����
		MCIPlay(wIDDevioce,musicType);
		while (1)
		{
			MCI_STATUS_PARMS mciStatus;
			mciStatus.dwItem = MCI_STATUS_MODE;
			DWORD sta2Return = mciSendCommand(wIDDevioce, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatus);
			DWORD music_playstyle = mciStatus.dwReturn; //��õ�ǰ���Ź���ģʽ ���� 526 ��ͣ 525
			cout << wIDDevioce << ":" << music_playstyle << endl;
			if (music_playstyle == 525)
			{
				if (musicType == soundEffect)
				{
					Close_MCI(wIDDevioce);
					return 0;  //�������
				}

			}
			//�������������С�������ģ����޸�����������С
			if (musicType == backgroundMusic)
			{
				if (vol != BackgroundMusicVol)
				{
					vol = BackgroundMusicVol;
					MCISetVolume(vol, wIDDevioce);
				}
			}
			else if (musicType == soundEffect)
			{
				if (vol != SoundEffectVol)
				{
					vol = SoundEffectVol;
					MCISetVolume(vol, wIDDevioce);
				}
			}
			if (endflag == true)  //������ֹ��׼λ���˳�
			{
				Close_MCI(wIDDevioce);
				return 0;
			}

			Sleep(100);
		}

	}
	else
		wIDDevioce = NULL;
	return opReturn;

}


void SoundManager::playSound(const CString& strPath, MusicType musicType)
{
	musicList.push_back(thread(SOUND_RUN, strPath, musicType));

}

void SoundManager::SetBackgroundMusicVol(int vol)
{
	if (vol > 1000)
	{
		vol = 1000;
	}
	if (vol < 0)
	{
		vol = 0;
	}
	BackgroundMusicVol = vol;
}

void SoundManager::SetSoundEffectVol(int vol)
{
	if (vol > 1000)
	{
		vol = 1000;
	}
	if (vol < 0)
	{
		vol = 0;
	}
	SoundEffectVol = vol;
}



int main()
{
	{
		SoundManager SM;

		//musicList.push_back(thread(SOUND_RUN, "sound/starlight.mp3",backgroundMusic));
		SM.playSound("sound/starlight.mp3", backgroundMusic);

		while (1)
		{
			char command;
			command = _getch();
			if (command == 'r')
			{
				SM.playSound("sound/getRice.mp3", soundEffect);

				cout << "New Music" << endl;
			}
			if (command == 'm')
			{
				SM.playSound("sound/makeDeal.mp3", soundEffect);

				cout << "New Music" << endl;
			}

			if (command == 'u')
			{

				BackgroundMusicVol += 100;
				if (BackgroundMusicVol > 1000)
				{
					BackgroundMusicVol = 1000;
				}
				SM.SetBackgroundMusicVol(BackgroundMusicVol);
			}
			if (command == 'd')
			{
				BackgroundMusicVol -= 100;
				if (BackgroundMusicVol < 0)
				{
					BackgroundMusicVol = 0;
				}
				SM.SetBackgroundMusicVol(BackgroundMusicVol);
			}
			if (command == 'w')
			{
				SoundEffectVol += 100;
				if (SoundEffectVol > 1000)
				{
					SoundEffectVol = 1000;
				}
				SM.SetSoundEffectVol(SoundEffectVol);
			}
			if (command == 's')
			{
				SoundEffectVol -= 100;
				if (SoundEffectVol < 0)
				{
					SoundEffectVol = 0;
				}
				SM.SetSoundEffectVol(SoundEffectVol);
			}
			if (command == 'e')
			{
				break;
			}


		}
	}
	

	return 0;
}
