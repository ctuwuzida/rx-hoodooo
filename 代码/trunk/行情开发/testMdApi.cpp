// testTraderApi.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <assert.h>
#include <signal.h>

#include "MdSpi.h"
#define DIM(_Array) (sizeof(_Array) / sizeof(_Array[0]))

// ���ò���
char FRONT_ADDR[MAX_PATH] = {0};		// ǰ�õ�ַ
TThostFtdcBrokerIDType	BROKER_ID = {0};				// ���͹�˾����
TThostFtdcInvestorIDType INVESTOR_ID = {0};			// Ͷ���ߴ���
TThostFtdcPasswordType  PASSWORD = {0};;			// �û�����
char *ppInstrumentID[] = {"cu0912", "cu1001"};			// ���鶩���б�
int iInstrumentID = 2;									// ���鶩������

// UserApi����
CThostFtdcMdApi* g_pUserApi = NULL;

// UserSpi����
CThostFtdcMdSpi* g_pUserSpi = NULL;

// ������
int g_nRequestID = 0;


#define MD_FLOWPATH ".\\MdFlowDir"
#define CONFIG_FILE ".\\Config.ini"

//Ctrl+C��Ϣ����
static void _signalsiginthandler(int signal)
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

static BOOL _initconfig(void)
{
	char szTemp[MAX_PATH] = {0};
	DWORD dwLen = 0;

	dwLen = GetPrivateProfileStringA("Config", "INVESTOR_ID", NULL, szTemp, MAX_PATH, CONFIG_FILE);
	if (dwLen>DIM(INVESTOR_ID))
	{
		_tprintf(_T("Error of INVESTOR_ID!!!"));
		return FALSE;
	}
	strncpy_s(INVESTOR_ID, szTemp, DIM(INVESTOR_ID));

	dwLen = GetPrivateProfileStringA("Config", "PASSWORD", NULL, szTemp, MAX_PATH, CONFIG_FILE);
	if (dwLen>DIM(PASSWORD))
	{
		_tprintf(_T("Error of PASSWORD!!!"));
		return FALSE;
	}
	strncpy_s(PASSWORD, szTemp, DIM(PASSWORD));


	dwLen = GetPrivateProfileStringA("Config", "BROKER_ID", NULL, szTemp, MAX_PATH, CONFIG_FILE);
	if (dwLen>DIM(BROKER_ID))
	{
		_tprintf(_T("Error of BROKER_ID!!!"));
		return FALSE;
	}
	strncpy_s(BROKER_ID, szTemp, DIM(BROKER_ID));

	dwLen = GetPrivateProfileStringA("Config", "FRONT_ADDR", NULL, szTemp, MAX_PATH, CONFIG_FILE);
	if (dwLen>DIM(FRONT_ADDR))
	{
		_tprintf(_T("Error of FRONT_ADDR!!!"));
		return FALSE;
	}
	strncpy_s(FRONT_ADDR, szTemp, DIM(FRONT_ADDR));

	return TRUE;
}

void main(void)
{
	if (!_initconfig())
	{
		_tprintf(_T("Error of Config/InstrumentID.txt!!!"));
		return;
	}
	
	// ��ʼ��UserApi
	g_pUserApi = CThostFtdcMdApi::CreateFtdcMdApi(MD_FLOWPATH);			// ����UserApi
	g_pUserSpi = new CMdSpi();

	assert(NULL!=g_pUserApi);
	assert(NULL!=g_pUserSpi);

	g_pUserApi->RegisterSpi(g_pUserSpi);						// ע���¼���
	g_pUserApi->RegisterFront(FRONT_ADDR);					// connect
	g_pUserApi->Init();

	_tprintf(_T("Ctrl + C exit process!\n"));
	signal(SIGINT, _signalsiginthandler);

	g_pUserApi->Join();//wait Ctrl + C to run SignalSIGINTHandler()

	//while g_pUserApi->Release() called!
	g_pUserApi = NULL;
	delete g_pUserSpi;
	g_pUserSpi = NULL;
}