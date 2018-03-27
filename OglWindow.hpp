#pragma once

#include<Windows.h>
#include<iostream>
#include<vector>

using namespace std;

struct ALLMONITORINFO
{
	HMONITOR hMonitor;
	RECT     rect;
	bool     isPrimary;
};

class oglWindow
{
public:
	oglWindow()
	{
		_hWnd = NULL;
		szTitle = L"��ʾ����";
		szWindowClass = L"oglWindow";
	}

	~oglWindow()
	{
		//Close();
	}

	HINSTANCE _hInst;					// ��ǰʵ��
	TCHAR* szTitle;					// �������ı�
	TCHAR* szWindowClass;			// ����������
									//���ɵĴ��ھ��
	HWND _hWnd;
	int width;
	int height;

	///-------------------------------------------------------------------------------------------------
	/// <summary> ����һ������. </summary>
	/// <returns>
	/// true if it succeeds, false if it fails.
	/// </returns>
	///-------------------------------------------------------------------------------------------------
	BOOL Create(int w,int h,WNDPROC proc= NULL)
	{
		if (_hWnd != NULL)//����Ѿ�������һ��������ô�����ظ�����
		{
			return FALSE;
		}

		//����ģ��ľ����������.exeģ��ľ������.dllģ��ľ����
		//����������ΪNULL���򷵻ص��øô��ڵ�.exeģ������
		//Ҳ�������������û�ȡģ����GetModuleHandle("mydll.dll")
		_hInst = GetModuleHandle(NULL);

		WNDCLASSEX wcex;
		memset(&wcex, 0, sizeof(wcex));

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;// | CS_DBLCLKS | CS_OWNDC;

											 //������ص�����
		if (proc != NULL)
			wcex.lpfnWndProc = proc;
		else
			wcex.lpfnWndProc = this->MyWndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = _hInst;

		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(107));
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(108));
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		//wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WIN01);//��Ҫ�˵���
		wcex.lpszClassName = szWindowClass;//����������

		RegisterClassEx(&wcex);

		int startx = 0, starty = 0;


		//if (mInfo.size() == 1)
		//{
		//	RECT rect = mInfo[0].rect;
		//	width = rect.right - rect.left;
		//	height = rect.bottom - rect.top;
		//	startx = rect.left;
		//	starty = rect.top;
		//}
		//else
		//{
		//	for (int i = 0; i < mInfo.size(); i++)
		//	{
		//		//��ȡ�ض���ʾ������Ϣ������Ҫ���Ƿֱ��ʣ���ʼ����ȡ�
		//		if (!mInfo[i].isPrimary)
		//		{
		//			RECT rect = mInfo[0].rect;
		//			width = rect.right - rect.left;
		//			height = rect.bottom - rect.top;
		//			startx = rect.left;
		//			starty = rect.top;
		//			break;
		//		}
		//	}
		//}

		width = w;
		height = h;
		//��ȡ������Ϊ��λ����Ļ�ߴ硣
		//��ӡ�ó�ȷʵ�õ��˳���
		//cout << "width:" << width << "  height:" << height << endl;
		_hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
			startx, starty, width, height, NULL, NULL, _hInst, NULL);

		if (!_hWnd)
		{
			//Debug::GetInst()->Log("��������ʧ�ܣ�%d", GetLastError());
			return FALSE;
		}
		//ToggleFullScreen();

		ShowWindow(_hWnd, SW_SHOW);
		//ShowWindow(_hWnd, SW_MAXIMIZE);
		UpdateWindow(_hWnd);

		return TRUE;
	}

	void Close()
	{
		if (_hWnd)
		{
			//CloseWindow(_hWnd);
			//CloseWindow destroy window
			BOOL result = DestroyWindow(_hWnd);
			DWORD error = GetLastError();
			//Debug::GetInst()->Log("Error:%d.", error);
		}
		_hWnd = NULL;

	}

private:

	///-------------------------------------------------------------------------------------------------
	/// <summary> һ��WndProc��Ϣ������ģ��. </summary>
	/// <returns> A CALLBACK. </returns>
	///-------------------------------------------------------------------------------------------------
	static	LRESULT CALLBACK  MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_COMMAND:
			break;
		case WM_PAINT:
			//hdc = BeginPaint(hWnd, &ps);
			//// TODO:  �ڴ���������ͼ����...
			//EndPaint(hWnd, &ps);
			break;

		case WM_KEYDOWN:
			//���������ĳһ�����µ���Ϣ

			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}

	///----------------------------------------------------------------------------------------
	///��ȡ������ʾ������Ϣ���������Ǵ洢��vector�����ڡ�
	///�÷�������ΪEnumDisplayMonitors�Ļص���������vector����Ҳ���Դ���ʽ����ȥ��
	///
	///----------------------------------------------------------------------------------------
	static BOOL CALLBACK MonitorEnumProc(__in  HMONITOR hMonitor, __in  HDC hdcMonitor, __in  LPRECT lprcMonitor, __in  LPARAM dwData)
	{
		vector<ALLMONITORINFO>& infoArray = *reinterpret_cast<vector<ALLMONITORINFO>* >(dwData);

		ALLMONITORINFO monitorInfo;
		monitorInfo.hMonitor = hMonitor;
		monitorInfo.rect = *lprcMonitor;

		HMONITOR priMonitor = MonitorFromWindow(nullptr, MONITOR_DEFAULTTOPRIMARY);
		if (priMonitor == hMonitor)
			monitorInfo.isPrimary = true;
		else
			monitorInfo.isPrimary = false;

		infoArray.push_back(monitorInfo);
		return TRUE;
	}

};