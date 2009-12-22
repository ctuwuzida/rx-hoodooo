// testTraderApi.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <assert.h>
#include <signal.h>
#include "ThostFtdcMdApi.h"
#include "MdSpi.h"

// ���ò���
char FRONT_ADDR[] = "tcp://asp-sim2-md1.financial-trading-platform.com:26213";		// ǰ�õ�ַ
TThostFtdcBrokerIDType	BROKER_ID = "2030";				// ���͹�˾����
TThostFtdcInvestorIDType INVESTOR_ID = "81531";			// Ͷ���ߴ���
TThostFtdcPasswordType  PASSWORD = "888888";			// �û�����
char *ppInstrumentID[] = {"cu0912", "cu1001"};			// ���鶩���б�
int iInstrumentID = 2;									// ���鶩������




// UserApi����
CThostFtdcMdApi* g_pUserApi = NULL;

// UserSpi����
CThostFtdcMdSpi* g_pUserSpi = NULL;

// ������
int g_nRequestID = 0;



void SignalSIGINTHandler(int signal)
{
	if (SIGINT==signal)
	{
		assert(NULL!=g_pUserApi);
		assert(NULL!=g_pUserSpi);
		_tprintf(_T("exit process...\n"));

		g_pUserApi->RegisterSpi(NULL);
		g_pUserApi->Release();
	}	
}

void main(void)
{
	// ��ʼ��UserApi
	g_pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();			// ����UserApi
	g_pUserSpi = new CMdSpi();

	assert(NULL!=g_pUserApi);
	assert(NULL!=g_pUserSpi);

	g_pUserApi->RegisterSpi(g_pUserSpi);						// ע���¼���
	g_pUserApi->RegisterFront(FRONT_ADDR);					// connect
	g_pUserApi->Init();

	_tprintf(_T("Ctrl + C exit process!\n"));
	signal(SIGINT, SignalSIGINTHandler);

	g_pUserApi->Join();//wait Ctrl + C to run SignalSIGINTHandler()

	//while g_pUserApi->Release() called!
	g_pUserApi = NULL;

	delete g_pUserSpi;
	g_pUserSpi = NULL;
}