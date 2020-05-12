
#include<Windows.h>
#include<digitalv.h>
#include<mmsystem.h>
#include<Mmsystem.h>
#include<string>
#include<iostream>
#include<atlstr.h>
#pragma comment(lib,"WINMM.LIB")

using namespace std;
MCIDEVICEID wDevioceId;


DWORD MCIPlay()
{
	MCI_PLAY_PARMS mciPP;
	DWORD opReturn;
	opReturn = mciSendCommand(wDevioceId, MCI_PLAY, MCI_NOTIFY, (DWORD)&mciPP);
	return opReturn;

}

DWORD MCIStatus_music_length()
{
	MCI_STATUS_PARMS mciStatus;
	mciStatus.dwItem = MCI_STATUS_LENGTH;
	DWORD staReturn = mciSendCommand(wDevioceId, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatus);
	DWORD music_alltime = mciStatus.dwReturn / 1000; //������ʱ��
	return music_alltime;
}

DWORD MCIStatus_music_pos()
{
	MCI_STATUS_PARMS mciStatus;
	mciStatus.dwItem = MCI_STATUS_POSITION;
	DWORD stalReturn = mciSendCommand(wDevioceId, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatus);
	DWORD music_timeing = mciStatus.dwReturn / 1000;//���ֵ�ǰ����ʱ��
	return music_timeing;
}

void send_music_from_pos()
{
	MCI_STATUS_PARMS mciStatus;
	mciStatus.dwItem = MCI_STATUS_POSITION;
	DWORD stalReturn = mciSendCommand(wDevioceId, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatus);
	DWORD music_timeing = mciStatus.dwReturn / 1000; //���ֵ�ǰ����ʱ��
	MCI_PLAY_PARMS PlayParms;
	PlayParms.dwFrom = music_timeing; //ָ����ʲôʱ�䲥��
	DWORD ret1 = mciSendCommand(wDevioceId, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&PlayParms);
}

void MCISeek(int time)
{
	DWORD music_timeing =time / 1000; //���ֵ�ǰ����ʱ��
	MCI_PLAY_PARMS PlayParms;
	PlayParms.dwFrom = music_timeing; //ָ����ʲôʱ�䲥��
	DWORD ret1 = mciSendCommand(wDevioceId, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&PlayParms);

}

void close_mci()
{
	DWORD retclose = mciSendCommand(wDevioceId, MCI_CLOSE, NULL, NULL);
}

//��������
DWORD MCISetVolume(DWORD dwVol)
{
	MCI_DGV_SETAUDIO_PARMS mciSetAudioPara;
	mciSetAudioPara.dwCallback = NULL;
	mciSetAudioPara.dwItem = MCI_DGV_SETAUDIO_VOLUME;
	mciSetAudioPara.dwValue = dwVol; //����0-1000
	DWORD dwReturn = mciSendCommand(wDevioceId,MCI_SETAUDIO, MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD)(LPVOID)&mciSetAudioPara);
	return dwReturn;
}


DWORD MCIOpen(const CString& strPath)
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

		//�����豸��Ϣ
		MCI_SET_PARMS mciSet;
		mciSet.dwTimeFormat = MCI_FORMAT_MILLISECONDS;  //����ʱ�䵥λΪ����
		mciSendCommand(wDevioceId, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD)(LPVOID)&mciSet);

		//���õ�ǰ����λ����ת60��
		//MCISeek(100);

		//��Ƶ����
		MCIPlay();

		MCI_STATUS_PARMS mciStatus;
		mciStatus.dwItem = MCI_STATUS_TIME_FORMAT;
		DWORD sta2Return = mciSendCommand(wIDDevioce, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatus);
		DWORD music_playstyle = mciStatus.dwReturn; //��õ�ǰ���Ź���ģʽ ���� 526 ��ͣ 525
	}
	else
		wIDDevioce = NULL;
	return opReturn;

}



int main()
{
	cout << MCIOpen("sound/starlight.mp3") << endl;

	while (1);

	return 0;
}