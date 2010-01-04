// testTraderApi.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <assert.h>
#include <signal.h>

#include "MdSpi.h"
#define DIM(_Array) (sizeof(_Array) / sizeof(_Array[0]))

// 配置参数
char FRONT_ADDR[MAX_PATH] = {0};		// 前置地址
TThostFtdcBrokerIDType	BROKER_ID = {0};				// 经纪公司代码
TThostFtdcInvestorIDType INVESTOR_ID = {0};			// 投资者代码
TThostFtdcPasswordType  PASSWORD = {0};;			// 用户密码
char *ppInstrumentID[] = {"cu0912", "cu1001"};			// 行情订阅列表
int iInstrumentID = 2;									// 行情订阅数量

// UserApi对象
CThostFtdcMdApi* g_pUserApi = NULL;

// UserSpi对象
CThostFtdcMdSpi* g_pUserSpi = NULL;

// 请求编号
int g_nRequestID = 0;


#define MD_FLOWPATH ".\\MdFlowDir"
#define CONFIG_FILE ".\\Config.ini"

//Ctrl+C消息处理
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
	
	// 初始化UserApi
	g_pUserApi = CThostFtdcMdApi::CreateFtdcMdApi(MD_FLOWPATH);			// 创建UserApi
	g_pUserSpi = new CMdSpi();

	assert(NULL!=g_pUserApi);
	assert(NULL!=g_pUserSpi);

	g_pUserApi->RegisterSpi(g_pUserSpi);						// 注册事件类
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