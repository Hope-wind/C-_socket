/*************************************************************************
	> File Name: Client.cpp
	> Created Time: Sat 09 Jan 2021 06:19:19 PM CST
 ************************************************************************/

#include <stdio.h>
#include <Windows.h>
#include <string> 

#pragma comment(lib,"ws2_32.lib")
#define  PORT 4000
#define  IP_ADDRESS "10.0.2.15"
 
void autostart()
{
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


int main(int argc, char* argv[])
{
	autostart();	
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





