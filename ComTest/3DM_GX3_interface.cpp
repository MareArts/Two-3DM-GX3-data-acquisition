#include "StdAfx.h"
#include "3DM_GX3_interface.h"


static UINT DataSendReceiveThread(LPVOID pParam);


interface_3DM_GX3::interface_3DM_GX3()
{
	pMain = NULL;

	m_ComuPort.Parent = this;
	memset(buffer,0, sizeof(char)*1024);
	memset(temp,0, sizeof(char)*1024);

	bufferindex=0;
	Parsing=0;
	Thread_3DM_GX3_OnOff=false;
}


interface_3DM_GX3::~interface_3DM_GX3(void)
{	
	GetDataThreadStop();
}

//포트설정
bool interface_3DM_GX3::OpenPort(void)
{
	int stop;
	int parity;
	int data;
	int baudrate;
	CString strPort;

	//포트 설정 INIT 값, 상수값으로 분류.
	//통신속도 9600, Data비트 8비트, 패리티 없음, 스탑 없음
	strPort="COM1";
	baudrate=115200; //9600;
	data=8;
	parity=0;
	stop=0;

	if(m_ComuPort.OpenPort(strPort, baudrate, data, stop, parity) != TRUE)		
	{
		::AfxMessageBox("Open Fail");
		return false;
	}

	return true;
}

bool interface_3DM_GX3::OpenPort2(CString strPort, int baudrate,int data,int parity,int stop)
{

	
	if(m_ComuPort.OpenPort(strPort, baudrate, data, stop, parity) != TRUE)		
	{
		sendStr.Format("3DM_GX3 Port Open Fail!");
		SendMessage(pMain->GetSafeHwnd(),UM_TEXT_OUT,(WPARAM)(LPSTR)(LPCTSTR)sendStr,NULL);

		return false;
	}
	
	sendStr.Format("3DM_GX3 Port Open Success!");
	SendMessage(pMain->GetSafeHwnd(),UM_TEXT_OUT,(WPARAM)(LPSTR)(LPCTSTR)sendStr,NULL);

	return true;

}


//#include "2011_09_16_3DM_GX3_TESTDlg.h"

union u{
	float f;
	unsigned long ul;
};

bool interface_3DM_GX3::GetData(void)
{

	BYTE aByte; 
	int iSize =  (m_ComuPort.m_QueueRead).GetSize();

	for(int i  = 0 ; i < iSize; i++)
	{			
		(m_ComuPort.m_QueueRead).GetByte(&aByte);  //글자 하나씩 읽어옴
		buffer[bufferindex] = aByte; //buffer에 넣기

		if(bufferindex == 18)
		{
			//Check Sum
			unsigned char tR = (buffer[17] << 8);
			tR += buffer[18];
			unsigned char CheckSum=0;
			for(int i=0; i<17; ++i)
				CheckSum += buffer[i];

			if( tR == CheckSum )
			{			

				BYTE p = buffer[0];				

				u uu;			
				unsigned char a,b,c,d;
				a = buffer[1];
				b = buffer[2];
				c = buffer[3];
				d = buffer[4];
				uu.ul = (a << 24) | (b << 16) | (c << 8) | d;
				ReceiveData.roll = uu.f;

				a = buffer[5];
				b = buffer[6];
				c = buffer[7];
				d = buffer[8];			
				uu.ul = (a << 24) | (b << 16) | (c << 8) | d;
				ReceiveData.pitch = uu.f;


				a = buffer[9];
				b = buffer[10];
				c = buffer[11];
				d = buffer[12];
				uu.ul = (a << 24) | (b << 16) | (c << 8) | d;
				ReceiveData.yaw = uu.f;

				if(pMain != NULL)
					//SendMessage(pMain->GetSafeHwnd(),UM_GET_DATA,NULL,NULL);
					SendMessage(pMain->GetSafeHwnd(),messageID,NULL,NULL);
				

			}	

			bufferindex=0;			

		}		

		bufferindex++;
		if(bufferindex>=1024)
			bufferindex=0;
	}



	return false;
}

bool interface_3DM_GX3::ClosePort(void)
{
	m_ComuPort.ClosePort();
	sendStr.Format("3DM_GX3 Port close");
	SendMessage(pMain->GetSafeHwnd(),UM_TEXT_OUT,(WPARAM)(LPSTR)(LPCTSTR)sendStr,NULL);

	return true;
}

int interface_3DM_GX3::SendMsg(void)
{
	return 0;
}

void interface_3DM_GX3::GetCwndPoint(CWnd* pCwnd)
{
	pMain = pCwnd;
}



int interface_3DM_GX3::SendData()
{

	memset(buffer,0, sizeof(char)*1024);
	bufferindex=0;

	//설정
	SendingCommand.Query=0xCE; //0xCC; //
	
	
	int d=m_ComuPort.WriteComm( (BYTE*)&SendingCommand, 1);

	return d;
}

void interface_3DM_GX3::GetDataThreadStart(){
	Thread_3DM_GX3_OnOff = true;
	::AfxBeginThread(DataSendReceiveThread,this);

}

void interface_3DM_GX3::GetDataThreadStop(){
	Thread_3DM_GX3_OnOff = false;

}


//::AfxBeginThread(StereoThread,this);
UINT DataSendReceiveThread(LPVOID pParam)
{

	
	while( ((interface_3DM_GX3*)pParam)->Thread_3DM_GX3_OnOff )
	{
		((interface_3DM_GX3*)pParam)->SendData();
	}	

	return 0;
}

