#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <vector>

#define IDC_LISTVIEW 150
#define LVIF_TEXT 151

using namespace std;
//g++ -o TaskMgr TaskMgr.cpp -mwindows -municode  -lcomctl32
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPWSTR lpszClass = TEXT("Windows 작업관리자");
HWND hMain;
HWND hTab;
HWND hStatic;
const DWORD TabNum = 6;

wchar_t windows_title[256][256];
wchar_t last_windows_title[256][256];
int title_cnt=1,last_title_cnt=1;

void make_windows_title(HWND hStatic){
	LVCOLUMN COL;
	LVITEM LI;
	HWND hList;
	hList = CreateWindow(WC_LISTVIEW,NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_SINGLESEL /*| LVS_SORTASCENDING*/, 0,0,500,500,hStatic,(HMENU)IDC_LISTVIEW,0,0);
	ListView_SetExtendedListViewStyle(hList,LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES); 
	
	COL.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	//COL.fmt = LVCFMT_LEFT;
	
	COL.cx=350;
	COL.pszText=L"작업";   
	COL.iSubItem=0;
	ListView_InsertColumn(hList,0,&COL);   
	
	COL.cx=75;
	COL.pszText=L"상태";   
	COL.iSubItem=1;
	ListView_InsertColumn(hList,1,&COL);
	//https://m.blog.naver.com/PostView.nhn?blogId=xenardy&logNo=60102016237&proxyReferer=https%3A%2F%2Fwww.google.co.kr%2F
	LI.mask = LVIF_TEXT;
	LI.iSubItem = 0;    //열
	for(DWORD i = 1; i < title_cnt ; i++){	
		LI.iItem = i+1;
		LI.pszText = windows_title[i];
		ListView_InsertItem(hList, &LI);					
		ListView_SetItemText(hList, i, 1,L""); 
	}

}
int CALLBACK foo( HWND hwnd, LPARAM param ){
     if ( IsWindowVisible(hwnd ) == FALSE ) return 1;
     if ( GetWindowTextLength( hwnd ) == 0 ) return 1;
     if ( GetParent( hwnd ) != 0 ) return 1;
	if (hwnd == hMain) return 1;
    TCHAR name[256];
    TCHAR title[256];
    GetClassName( hwnd, name, 256 );
    GetWindowText( hwnd, title, 256 );
	wcscpy(windows_title[title_cnt++],title);
     return 1;
}
DWORD WINAPI thread_proc(LPVOID lpParam){
	while(1){
		HANDLE hMutex = CreateMutex(NULL, FALSE, L"SAMPLE");
		bool change_flag=false;
		WaitForSingleObject(hMutex, INFINITE);
		EnumWindows( foo, 0 ); 
		ReleaseMutex(hMutex);
		
		Sleep(100);
		if(last_title_cnt!=title_cnt){
			change_flag=true;
		}
		else{
			for(DWORD i=0;i<title_cnt;i++){
				if(wcscmp(windows_title[i],last_windows_title[i])!=0){
					change_flag=true;
				}
			}
		}
		for(DWORD i=0;i<title_cnt;i++){
			wcscpy(last_windows_title[i],windows_title[i]);
		}
		if(change_flag){
			NMHDR nmh;
			nmh.code = TCN_SELCHANGE;
			nmh.idFrom = GetDlgCtrlID(hTab);
			nmh.hwndFrom = hTab;
			SendMessage(GetParent(hTab), WM_NOTIFY, nmh.idFrom, (LPARAM)&nmh);
		}
		last_title_cnt=title_cnt;
		title_cnt=1;
		memset(windows_title,NULL,256);
	}
}
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow){
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	
	DWORD dwThreadId = 1;
    	DWORD dwThrdParam = 1;
   	HANDLE hThreadc = CreateThread(NULL, 0, thread_proc, &dwThrdParam, 0, &dwThreadId);
	
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
	hMain = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW | WS_CAPTION, CW_USEDEFAULT, CW_USEDEFAULT, 600, 500, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hMain, nCmdShow);
	
	while(GetMessage(&Message, 0, 0, 0)){
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){
	TCITEM tie;
	TCHAR* Caption[TabNum]={TEXT("응용 프로그램"),TEXT("프로세스"),TEXT("서비스"),TEXT("성능"),TEXT("네트워킹"),TEXT("사용자")};
	switch(iMessage){
	case WM_CREATE:
		InitCommonControls();
		hTab=CreateWindow(WC_TABCONTROL,L"",WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 0,0,0,0,hWnd,(HMENU)0,g_hInst,NULL);
		hStatic=CreateWindow(L"static",L"",WS_CHILD | WS_VISIBLE, 0,0,0,0,hWnd,(HMENU)1,g_hInst,NULL);
		tie.mask=TCIF_TEXT;
		for(DWORD i = 0; i < TabNum ; i++){
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
			//SetWindowText(hStatic,Caption[TabCtrl_GetCurSel(hTab)]);
			switch (CurTab){
				case 0:
					make_windows_title(hStatic);
					return 0;
				case 1:
					//CloseWindow(hStatic);
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
	case WM_SIZE://http://www.silverwolf.co.kr/cplusplus/4903
		MoveWindow(hTab,0,0,LOWORD(lParam),HIWORD(lParam),TRUE);
		MoveWindow(hStatic,LOWORD(lParam)/2-290,HIWORD(lParam)/2-205,500,400,TRUE);
		//MoveWindow(hStatic,LOWORD(lParam)/2-250,HIWORD(lParam)/2,500,25,TRUE);
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
