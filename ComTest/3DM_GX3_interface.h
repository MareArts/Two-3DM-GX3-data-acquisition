#pragma once

#include "CommThread_3DM_GX3.h"
#include <string>

using namespace std;

struct Command_3DM_GX3_SendData{

	BYTE Query;
};

//IGX 8Bits
struct Command_3DM_GX3_GetData{
	float roll;
	float pitch;
	float yaw;
}; 


class interface_3DM_GX3
{
public:

	//삭제 대상
	//void *pParent;
	//HWND pHand;

	interface_3DM_GX3(void);
	~interface_3DM_GX3(void);

	CCommThread_3DM_GX3 m_ComuPort;
	char buffer[1024];
	char temp[1024];
	int bufferindex;

	bool OpenPort();
	bool OpenPort2(CString strPort, int baudrate,int data,int parity,int stop);
	bool GetData(void);
	bool ClosePort(void);
	int SendMsg(void);
	void GetCwndPoint(CWnd* pCwnd);
	int SendData();
	void GetDataThreadStart();
	void GetDataThreadStop();

	int Parsing;
	unsigned int messageID;
//	PD6 pd6V;
	CWnd *pMain;
	CString sendStr;
	Command_3DM_GX3_SendData SendingCommand;
	Command_3DM_GX3_GetData ReceiveData;

	bool Thread_3DM_GX3_OnOff;
//	BufferData 3DM_GX3_BufferData;
//	CCriticalSection g_CS;


};
