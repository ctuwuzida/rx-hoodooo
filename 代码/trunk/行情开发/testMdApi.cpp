// testTraderApi.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <assert.h>
#include <signal.h>
#include "ThostFtdcMdApi.h"
#include "MdSpi.h"

// 配置参数
char FRONT_ADDR[] = "tcp://asp-sim2-md1.financial-trading-platform.com:26213";		// 前置地址
TThostFtdcBrokerIDType	BROKER_ID = "2030";				// 经纪公司代码
TThostFtdcInvestorIDType INVESTOR_ID = "81531";			// 投资者代码
TThostFtdcPasswordType  PASSWORD = "888888";			// 用户密码
char *ppInstrumentID[] = {"cu0912", "cu1001"};			// 行情订阅列表
int iInstrumentID = 2;									// 行情订阅数量




// UserApi对象
CThostFtdcMdApi* g_pUserApi = NULL;

// UserSpi对象
CThostFtdcMdSpi* g_pUserSpi = NULL;

// 请求编号
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
	// 初始化UserApi
	g_pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();			// 创建UserApi
	g_pUserSpi = new CMdSpi();

	assert(NULL!=g_pUserApi);
	assert(NULL!=g_pUserSpi);

	g_pUserApi->RegisterSpi(g_pUserSpi);						// 注册事件类
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