#include <stdio.h>
#include <Windows.h>
#include <string> 
#include <conio.h>


#define NUM 1024

int main()
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
							DCX_CACHE |  //高速缓存)
							DCX_LOCKWINDOWUPDATE);

			//创建一个内存设备上下文
			HDC hdcMem = CreateCompatibleDC(hdcSrc); 

			//切割小块的宽度

			int cx = GetSystemMetrics(SM_CXSCREEN) / 10;
			int cy = GetSystemMetrics(SM_CYSCREEN) / 10;

			//创建位图
			HBITMAP hBitmap = CreateCompatibleBitmap(hdcSrc,cx,cy);

			//位图和内存设备的关联
			SelectObject(hdcMem,hBitmap);

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
									SRCCOPY //拷贝方式
						  );

					BitBlt(hdcSrc,rects[i][2],rects[i][3],cx,cy,hdcMem,0,0,SRCCOPY);
					Sleep(1);

					//打扫门户
					DeleteDC(hdcMem);
					ReleaseDC(hwnd,hdcSrc);
					DeleteObject(hBitmap);
					

			}

		
	}
}
