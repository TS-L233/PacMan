// pacman.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Pac-Man.h"
#include "GMap.h"
#include "GObject.h"

using namespace std;

const int WLENTH = 700;  // 高
const int WHIGHT = 740;  // 宽
const int chapter_count = 3; // 一共三关
HWND g_hwnd; // 游戏窗口句柄

#define MAX_LOADSTRING 100


// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名



// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	// 参数不再使用了
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_PACMAN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PACMAN));

	// 当前的关卡
	int n = 0; // [0, 1, 2]
	// 地图
	GMap *MapArray[chapter_count] = { new chapter_1(), new chapter_2(), new chapter_3() };

	// 玩家对象
	// 自己
	auto myself = std::make_shared<Player>(P_line, P_col);
	// 设定四个敌人对象
	auto e1 = std::make_shared<Randomer>(E_line, E_col);
	auto e2 = std::make_shared<Randomer>(E_line, E_col);
	auto e3 = std::make_shared<Guard>(E_line, E_col);
	auto e4 = std::make_shared<Tracer>(E_line, E_col);

	// 关卡
	GObject::pStage = MapArray[n]; //初始化为第一关地图

	// 设定玩家
	Enemy::player = myself;

	MSG msg;

	DWORD dwLastTime = 0;
	// 主消息循环:
	// 玩家没有被抓,并且关卡<3
	
	while (!myself->ifend() && n < chapter_count) {
		// 判断是否赢得比赛
		if (myself->ifwin()) {
			n++;
			// 重设自己和敌人位置
			myself->Setcenter(P_line, P_col);
			e1->Setcenter(E_line, E_col);
			e2->Setcenter(E_line, E_col);
			e3->Setcenter(E_line, E_col);
			e4->Setcenter(E_line, E_col);
			// 判断是否完成了3关,如果完成,退出游戏,否则进入下一关
			if (n < 3) {
				MessageBox(g_hwnd, _T("恭喜过关"), _T("吃豆子提示"), MB_OK);
				GObject::pStage = MapArray[n];
				RECT screenRect;
				screenRect.top = 0;
				screenRect.left = 0;
				screenRect.right = WLENTH;
				screenRect.bottom = WHIGHT;

				HDC hdc = GetDC(g_hwnd);
				std::shared_ptr<HDC__> dc(hdc, [](HDC hdc) {
					::ReleaseDC(g_hwnd, hdc);
				});
				::FillRect(dc.get(), &screenRect, CreateSolidBrush(RGB(255, 255, 255)));
				GObject::pStage->paintwall(hdc);
				// 继续进行循环
				continue;
			}
			else {
				// 跳出循环
				break;
			}
		}

		// 获取消息
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
			

		// 判断时间,否则画得太快
		if (GetTickCount() - dwLastTime >= 40) {
			dwLastTime = GetTickCount();
		}
		else {
			// 时间不到,本次不进行绘画
			continue;
		}

		{
			// 获得设备
			HDC hdc = ::GetDC(g_hwnd);
			// 不使用时自动释放
			std::shared_ptr<HDC__> dc(hdc, [](auto hdc) {
				::ReleaseDC(g_hwnd, hdc);
			});
			// 画豆子
			MapArray[n]->paintbean(hdc);
			// 画地图
			MapArray[n]->paintwall(hdc);

			// 画敌人及自动运动
			{

				e1->behavior();
				e1->Paintfield(hdc);
				e1->paintself(hdc);

				e2->behavior();
				e2->Paintfield(hdc);
				e2->paintself(hdc);

				e3->behavior();
				e3->Paintfield(hdc);
				e3->paintself(hdc);

				e4->behavior();
				e4->Paintfield(hdc);
				e4->paintself(hdc);
			}

			{
				// 画自己
				myself->Paintfield(hdc);
				myself->paintself(hdc);
				// 自己向前移动
				myself->behavior();

				// 获取按键 : 控制自己的方向
				if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
					myself->Setcmd(DOWN);
				}
				if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
					myself->Setcmd(LEFT);
				}
				if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
					myself->Setcmd(RIGHT);
				}
				if (GetAsyncKeyState(VK_UP) & 0x8000) {
					myself->Setcmd(UP);
				}
			}
		}
	}

	// 如果游戏结束
	if (myself->ifend()) {
		MessageBoxA(NULL, "出师未捷身先死", "吃豆子提示", MB_OK);
	}
	// 否则,提示赢得游戏
	else {
		MessageBoxA(NULL, "恭喜您赢得了胜利\r\n确定后游戏退出", "吃豆子提示", MB_OK);
	}

	return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PACMAN));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PACMAN);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	// 创建窗口
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW
		, 0 // 在屏幕中的位置 x
		, 0 // 在屏幕中的位置 y
		, WLENTH // 宽
		, WHIGHT // 高
		, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd) {
		return FALSE;
	}

	// 保存游戏窗口,后面会用到
	g_hwnd = hWnd;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		// 分析菜单选择:
		switch (wmId) {
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
					 break;
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		EndPaint(hWnd, &ps);
	}
				   break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
