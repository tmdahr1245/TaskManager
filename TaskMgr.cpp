#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
//g++ -o TaskMgr TaskMgr.cpp -mwindows -municode  -lcomctl32
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPWSTR lpszClass = TEXT("Windows 작업관리자");
HWND hMain;
HWND hTab;
HWND hStatic;
const DWORD MenuNum = 6;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow){
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = (HICON)LoadImage(NULL, TEXT("Icon.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);
	hMain = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 600, 500, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hMain, nCmdShow);
	
	while(GetMessage(&Message, 0, 0, 0)){
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){
	TCITEM tie;
	TCHAR* Caption[MenuNum]={TEXT("응용 프로그램"),TEXT("프로세스"),TEXT("서비스"),TEXT("성능"),TEXT("네트워킹"),TEXT("사용자")};
	switch(iMessage){
	case WM_CREATE:
		InitCommonControls();
		hTab=CreateWindow(WC_TABCONTROL,L"",WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 0,0,0,0,hWnd,(HMENU)0,g_hInst,NULL);
		hStatic=CreateWindow(L"static",L"",WS_CHILD | WS_VISIBLE, 0,0,0,0,hWnd,(HMENU)1,g_hInst,NULL);
		tie.mask=TCIF_TEXT;
		for(DWORD i = 0; i < MenuNum ; i++){
			tie.pszText=Caption[i];
			TabCtrl_InsertItem(hTab,i,&tie);
		}
		return 0;
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code) {
		case TCN_SELCHANGE:
			DWORD CurTab=TabCtrl_GetCurSel(hTab);
			TCHAR s[10];
			wsprintf(s,L"%ld\n",CurTab);
			MessageBox(hStatic,s,TEXT("text"),MB_OK);
			//SetWindowText(hStatic,Caption[TabCtrl_GetCurSel(hTab)]);
			switch (CurTab){
				case 0:
					return 0;
				case 1:
					return 0;
				case 2:
					return 0;
				case 3:
					return 0;
				case 4:
					return 0;
				case 5:
					return 0;
			}
			break;
		}
		return 0;
	case WM_SIZE:
		MoveWindow(hTab,0,0,LOWORD(lParam),HIWORD(lParam),TRUE);
		MoveWindow(hStatic,LOWORD(lParam)/2-250,HIWORD(lParam)/2,500,25,TRUE);
		return 0;
	case WM_KEYDOWN:
            if(wParam == VK_ESCAPE)exit(0);
            break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
/*#include<windows.h>
#include<tchar.h>
#include<cstdio>
int CALLBACK foo( HWND hwnd, LPARAM param )
{
     if ( IsWindowVisible(hwnd ) == FALSE ) return 1;         // 보이지 않는 윈도우는 출력하지 않는다.
     if ( GetWindowTextLength( hwnd ) == 0 ) return 1;       // 캡션 문자열이 없으면 출력 하지 않는다.
     if ( GetParent( hwnd ) != 0 ) return 1;                         // 자식윈도우는(부모가 있다면) 출력하지 않는다.
  
     // 작업관리자는 자신의 윈도우와 바탕화면을 제외한다.

    TCHAR name[256];
    TCHAR title[256];
    GetClassName( hwnd, name, 256 );
    GetWindowText( hwnd, title, 256 );

    
    _tprintf(_T("%s - %s\n"), name, title );

     return 1; // 계속 열거하라는 의미.
}

int main()
{
     EnumWindows( foo,  // 윈도우를 찾을때 마다 이 함수를 호출해달라.
                            0 );   // 위함수로 보낼 인자.
}*/