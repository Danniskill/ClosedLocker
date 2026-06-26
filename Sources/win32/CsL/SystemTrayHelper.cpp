/*----------------------------------------------------------
OpenKey - The Cross platform Open source Vietnamese Keyboard application.

Copyright (C) 2019 Mai Vu Tuyen
Contact: maivutuyen.91@gmail.com
Github: https://github.com/tuyenvm/OpenKey
Fanpage: https://www.facebook.com/OpenKeyVN

This file is belong to the OpenKey project, Win32 version
which is released under GPL license.
You can fork, modify, improve this program. If you
redistribute your new version, it MUST be open source.
-----------------------------------------------------------*/
#include "SystemTrayHelper.h"
#include "AppDelegate.h"

#define WM_TRAYMESSAGE (WM_USER + 1)
#define TRAY_ICONUID 100

#define POPUP_VIET_ON_OFF 900
#define POPUP_SPELLING 901
#define POPUP_SMART_SWITCH 902
#define POPUP_USE_MACRO 903

#define POPUP_TELEX 910
#define POPUP_VNI 911
#define POPUP_SIMPLE_TELEX 912

#define POPUP_UNICODE 930
#define POPUP_TCVN3 931
#define POPUP_VNI_WINDOWS 932
#define POPUP_UNICODE_COMPOUND 933
#define POPUP_VN_LOCALE_1258 934

#define POPUP_CONVERT_TOOL 980
#define POPUP_QUICK_CONVERT 981

#define POPUP_MACRO_TABLE 990

// Switch keys
#define POPUP_SWITCH_CTRL_SHIFT 1101
#define POPUP_SWITCH_ALT_Z 1102
#define POPUP_SWITCH_CTRL_SPACE 1103
#define POPUP_SWITCH_SHIFT_SPACE 1104

// Typing Options
#define POPUP_OPT_MODERN_ORTHO 1201
#define POPUP_OPT_CAPS_FIRST 1202
#define POPUP_OPT_USE_CLIPBOARD 1203
#define POPUP_OPT_FIX_BROWSER 1204
#define POPUP_OPT_FIX_CHROMIUM 1205

// System
#define POPUP_SYS_STARTUP 1301
#define POPUP_SYS_METRO 1302
#define POPUP_SYS_MODERN_ICON 1303
#define POPUP_SYS_BEEP 1304

#define POPUP_ABOUT_OPENKEY 2010
#define POPUP_OPENKEY_EXIT 2000

extern int vUseModernOrthography;
extern int vUpperCaseFirstChar;
extern int vFixRecommendBrowser;
extern int vFixChromiumBrowser;
extern int vSendKeyStepByStep;
extern int vRunWithWindows;
extern int vSupportMetroApp;
extern int vUseGrayIcon;
extern int vSwitchKeyStatus;
extern int vLanguage;
extern int vInputType;
extern int vCodeTable;
#include "../../engine/ConvertTool.h"

#define MODIFY_MENU(MENU, COMMAND, DATA) ModifyMenu(MENU, COMMAND, \
											MF_BYCOMMAND | (DATA ? MF_CHECKED : MF_UNCHECKED), \
											COMMAND, \
											menuData[COMMAND]);

static HMENU popupMenu;
static HMENU menuInputType;
static HMENU menuTableCode;
static HMENU menuSwitchKey;
static HMENU menuTypingOpt;
static HMENU menuSystem;
static HMENU otherCode;

static NOTIFYICONDATA nid;

map<UINT, LPCTSTR> menuData = {
	{POPUP_VIET_ON_OFF, _T("Bật Tiếng Việt")},
	{POPUP_TELEX, _T("Kiểu gõ Telex")},
	{POPUP_VNI, _T("Kiểu gõ VNI")},
	{POPUP_SIMPLE_TELEX, _T("Kiểu gõ Simple Telex")},
	{POPUP_UNICODE, _T("Unicode dựng sẵn")},
	{POPUP_TCVN3, _T("TCVN3 (ABC)")},
	{POPUP_VNI_WINDOWS, _T("VNI Windows")},
	{POPUP_UNICODE_COMPOUND, _T("Unicode tổ hợp")},
	{POPUP_VN_LOCALE_1258, _T("Vietnamese locale CP 1258")},
	{POPUP_CONVERT_TOOL, _T("Công cụ chuyển mã...")},
	{POPUP_QUICK_CONVERT, _T("Chuyển mã nhanh")},
	{POPUP_MACRO_TABLE, _T("Bảng gõ tắt...")},

	{POPUP_SWITCH_CTRL_SHIFT, _T("Ctrl + Shift")},
	{POPUP_SWITCH_ALT_Z, _T("Alt + Z")},
	{POPUP_SWITCH_CTRL_SPACE, _T("Ctrl + Space")},
	{POPUP_SWITCH_SHIFT_SPACE, _T("Shift + Space")},

	{POPUP_OPT_MODERN_ORTHO, _T("Đặt dấu oà, uý (thay vì òa, úy)")},
	{POPUP_OPT_CAPS_FIRST, _T("Viết hoa chữ cái đầu tiên")},
	{POPUP_OPT_USE_CLIPBOARD, _T("Sử dụng Clipboard để gửi phím")},
	{POPUP_OPT_FIX_BROWSER, _T("Sửa lỗi gợi ý trên trình duyệt")},
	{POPUP_OPT_FIX_CHROMIUM, _T("Sửa lỗi Chromium")},

	{POPUP_SYS_STARTUP, _T("Khởi động cùng Windows")},
	{POPUP_SYS_METRO, _T("Hỗ trợ ứng dụng Windows Store")},
	{POPUP_SYS_MODERN_ICON, _T("Sử dụng biểu tượng hiện đại")},
	{POPUP_SYS_BEEP, _T("Phát âm báo khi chuyển chế độ")},

	{POPUP_ABOUT_OPENKEY, _T("Thông tin...")},
	{POPUP_OPENKEY_EXIT, _T("Thoát")},
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_USER+2019:
		// do nothing
		break;
	case WM_TRAYMESSAGE: {
		if (lParam == WM_LBUTTONDBLCLK) {
			AppDelegate::getInstance()->onToggleVietnamese();
			SystemTrayHelper::updateData();
		}
		if (lParam == WM_LBUTTONUP) {
			AppDelegate::getInstance()->onToggleVietnamese();
			SystemTrayHelper::updateData();
		} else if (lParam == WM_RBUTTONDOWN) {
			POINT curPoint;
			GetCursorPos(&curPoint);
			SetForegroundWindow(hWnd);
			UINT commandId = TrackPopupMenu(
				popupMenu,
				TPM_RETURNCMD | TPM_NONOTIFY,
				curPoint.x,
				curPoint.y,
				0,
				hWnd,
				NULL
			);
			switch (commandId) {
			case POPUP_VIET_ON_OFF:
				AppDelegate::getInstance()->onToggleVietnamese();
				break;
			case POPUP_CONVERT_TOOL:
				AppDelegate::getInstance()->onConvertTool();
				break;
			case POPUP_QUICK_CONVERT:
				AppDelegate::getInstance()->onQuickConvert();
				break;
			case POPUP_TELEX:
				AppDelegate::getInstance()->onInputType(0);
				break;
			case POPUP_VNI:
				AppDelegate::getInstance()->onInputType(1);
				break;
			case POPUP_SIMPLE_TELEX:
				AppDelegate::getInstance()->onInputType(2);
				break;
			case POPUP_UNICODE:
				AppDelegate::getInstance()->onTableCode(0);
				break;
			case POPUP_TCVN3:
				AppDelegate::getInstance()->onTableCode(1);
				break;
			case POPUP_VNI_WINDOWS:
				AppDelegate::getInstance()->onTableCode(2);
				break;
			case POPUP_UNICODE_COMPOUND:
				AppDelegate::getInstance()->onTableCode(3);
				break;
			case POPUP_VN_LOCALE_1258:
				AppDelegate::getInstance()->onTableCode(4);
				break;
			case POPUP_MACRO_TABLE:
				AppDelegate::getInstance()->onMacroTable();
				break;
			case POPUP_SWITCH_CTRL_SHIFT:
				AppDelegate::getInstance()->onSetSwitchKey((vSwitchKeyStatus & 0x0000FF00) | 0x5A000900 | 0xFE); // 0x5A is version/magic, 09 is Ctrl+Shift (1|8)
				break;
			case POPUP_SWITCH_ALT_Z:
				AppDelegate::getInstance()->onSetSwitchKey((vSwitchKeyStatus & 0x0000FF00) | 0x5A000200 | 'Z'); // 02 is Alt
				break;
			case POPUP_SWITCH_CTRL_SPACE:
				AppDelegate::getInstance()->onSetSwitchKey((vSwitchKeyStatus & 0x0000FF00) | 0x5A000100 | VK_SPACE); // 01 is Ctrl
				break;
			case POPUP_SWITCH_SHIFT_SPACE:
				AppDelegate::getInstance()->onSetSwitchKey((vSwitchKeyStatus & 0x0000FF00) | 0x5A000800 | VK_SPACE); // 08 is Shift
				break;
			case POPUP_OPT_MODERN_ORTHO:
				AppDelegate::getInstance()->onToggleModernOrtho();
				break;
			case POPUP_OPT_CAPS_FIRST:
				AppDelegate::getInstance()->onToggleCapsFirst();
				break;
			case POPUP_OPT_USE_CLIPBOARD:
				AppDelegate::getInstance()->onToggleUseClipboard();
				break;
			case POPUP_OPT_FIX_BROWSER:
				AppDelegate::getInstance()->onToggleFixBrowser();
				break;
			case POPUP_OPT_FIX_CHROMIUM:
				AppDelegate::getInstance()->onToggleFixChromium();
				break;
			case POPUP_SYS_STARTUP:
				AppDelegate::getInstance()->onToggleStartup();
				break;
			case POPUP_SYS_METRO:
				AppDelegate::getInstance()->onToggleMetro();
				break;
			case POPUP_SYS_MODERN_ICON:
				AppDelegate::getInstance()->onToggleModernIcon();
				break;
			case POPUP_SYS_BEEP:
				AppDelegate::getInstance()->onToggleBeep();
				break;
			case POPUP_ABOUT_OPENKEY:
				AppDelegate::getInstance()->onOpenKeyAbout();
				break;
			case POPUP_OPENKEY_EXIT:
				AppDelegate::getInstance()->onOpenKeyExit();
				break;
			}
			SystemTrayHelper::updateData();
		}
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

HWND SystemTrayHelper::createFakeWindow(const HINSTANCE & hIns) {
	//create fake window
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hIns;
	wcex.hIcon = LoadIcon(hIns, MAKEINTRESOURCE(IDI_APP_ICON));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_OPENKEY);
	wcex.lpszClassName = APP_CLASS;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
	ATOM atom = RegisterClassExW(&wcex);
	HWND hWnd = CreateWindowW(APP_CLASS, _T(""), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hIns, nullptr);
	if (!hWnd) {
		return NULL;
	}
	ShowWindow(hWnd, 0);
	UpdateWindow(hWnd);
	return hWnd;
}

void SystemTrayHelper::createPopupMenu() {
	popupMenu = CreatePopupMenu();
	
	AppendMenu(popupMenu, MF_CHECKED, POPUP_VIET_ON_OFF, menuData[POPUP_VIET_ON_OFF]);
	AppendMenu(popupMenu, MF_SEPARATOR, 0, 0);

	// Kiểu gõ
	menuInputType = CreatePopupMenu();
	AppendMenu(menuInputType, MF_CHECKED, POPUP_TELEX, menuData[POPUP_TELEX]);
	AppendMenu(menuInputType, MF_CHECKED, POPUP_VNI, menuData[POPUP_VNI]);
	AppendMenu(menuInputType, MF_CHECKED, POPUP_SIMPLE_TELEX, menuData[POPUP_SIMPLE_TELEX]);
	AppendMenu(popupMenu, MF_POPUP, (UINT_PTR)menuInputType, _T("Kiểu gõ"));

	// Bảng mã
	menuTableCode = CreatePopupMenu();
	AppendMenu(menuTableCode, MF_UNCHECKED, POPUP_UNICODE, menuData[POPUP_UNICODE]);
	AppendMenu(menuTableCode, MF_UNCHECKED, POPUP_TCVN3, menuData[POPUP_TCVN3]);
	AppendMenu(menuTableCode, MF_UNCHECKED, POPUP_VNI_WINDOWS, menuData[POPUP_VNI_WINDOWS]);
	otherCode = CreatePopupMenu();
	AppendMenu(otherCode, MF_CHECKED, POPUP_UNICODE_COMPOUND, menuData[POPUP_UNICODE_COMPOUND]);
	AppendMenu(otherCode, MF_CHECKED, POPUP_VN_LOCALE_1258, menuData[POPUP_VN_LOCALE_1258]);
	AppendMenu(menuTableCode, MF_POPUP, (UINT_PTR)otherCode, _T("Bảng mã khác"));
	AppendMenu(popupMenu, MF_POPUP, (UINT_PTR)menuTableCode, _T("Bảng mã"));

	// Phím chuyển ngữ
	menuSwitchKey = CreatePopupMenu();
	AppendMenu(menuSwitchKey, MF_UNCHECKED, POPUP_SWITCH_CTRL_SHIFT, menuData[POPUP_SWITCH_CTRL_SHIFT]);
	AppendMenu(menuSwitchKey, MF_UNCHECKED, POPUP_SWITCH_ALT_Z, menuData[POPUP_SWITCH_ALT_Z]);
	AppendMenu(menuSwitchKey, MF_UNCHECKED, POPUP_SWITCH_CTRL_SPACE, menuData[POPUP_SWITCH_CTRL_SPACE]);
	AppendMenu(menuSwitchKey, MF_UNCHECKED, POPUP_SWITCH_SHIFT_SPACE, menuData[POPUP_SWITCH_SHIFT_SPACE]);
	AppendMenu(popupMenu, MF_POPUP, (UINT_PTR)menuSwitchKey, _T("Phím chuyển ngữ"));

	// Tùy chọn gõ
	menuTypingOpt = CreatePopupMenu();
	AppendMenu(menuTypingOpt, MF_UNCHECKED, POPUP_OPT_MODERN_ORTHO, menuData[POPUP_OPT_MODERN_ORTHO]);
	AppendMenu(menuTypingOpt, MF_UNCHECKED, POPUP_OPT_CAPS_FIRST, menuData[POPUP_OPT_CAPS_FIRST]);
	AppendMenu(menuTypingOpt, MF_UNCHECKED, POPUP_OPT_USE_CLIPBOARD, menuData[POPUP_OPT_USE_CLIPBOARD]);
	AppendMenu(menuTypingOpt, MF_UNCHECKED, POPUP_OPT_FIX_BROWSER, menuData[POPUP_OPT_FIX_BROWSER]);
	AppendMenu(menuTypingOpt, MF_UNCHECKED, POPUP_OPT_FIX_CHROMIUM, menuData[POPUP_OPT_FIX_CHROMIUM]);
	AppendMenu(popupMenu, MF_POPUP, (UINT_PTR)menuTypingOpt, _T("Tùy chọn gõ"));

	// Hệ thống
	menuSystem = CreatePopupMenu();
	AppendMenu(menuSystem, MF_UNCHECKED, POPUP_SYS_STARTUP, menuData[POPUP_SYS_STARTUP]);
	AppendMenu(menuSystem, MF_UNCHECKED, POPUP_SYS_METRO, menuData[POPUP_SYS_METRO]);
	AppendMenu(menuSystem, MF_UNCHECKED, POPUP_SYS_MODERN_ICON, menuData[POPUP_SYS_MODERN_ICON]);
	AppendMenu(menuSystem, MF_UNCHECKED, POPUP_SYS_BEEP, menuData[POPUP_SYS_BEEP]);
	AppendMenu(popupMenu, MF_POPUP, (UINT_PTR)menuSystem, _T("Hệ thống"));

	AppendMenu(popupMenu, MF_SEPARATOR, 0, 0);

	AppendMenu(popupMenu, MF_STRING, POPUP_MACRO_TABLE, menuData[POPUP_MACRO_TABLE]);
	AppendMenu(popupMenu, MF_STRING, POPUP_CONVERT_TOOL, menuData[POPUP_CONVERT_TOOL]);
	AppendMenu(popupMenu, MF_STRING, POPUP_ABOUT_OPENKEY, menuData[POPUP_ABOUT_OPENKEY]);
	AppendMenu(popupMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(popupMenu, MF_UNCHECKED, POPUP_OPENKEY_EXIT, menuData[POPUP_OPENKEY_EXIT]);

	SetMenuDefaultItem(popupMenu, POPUP_VIET_ON_OFF, false);
}

static void loadTrayIcon() {
	int icon = 0;
	if (vLanguage) {
		icon = vUseGrayIcon ? IDI_ICON_STATUS_VIET_10 : IDI_ICON_STATUS_VIET;
		LoadString(GetModuleHandle(0), IDS_TRAY_TITLE_2, nid.szTip, 128);
	}
	else {
		icon = vUseGrayIcon ? IDI_ICON_STATUS_ENG_10 : IDI_ICON_STATUS_ENG;
		LoadString(GetModuleHandle(0), IDS_TRAY_TITLE, nid.szTip, 128);
	}
	nid.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(icon));
}

void SystemTrayHelper::updateData() {
	loadTrayIcon();
	Shell_NotifyIcon(NIM_MODIFY, &nid);

	MODIFY_MENU(popupMenu, POPUP_VIET_ON_OFF, vLanguage);

	MODIFY_MENU(menuInputType, POPUP_TELEX, vInputType == 0);
	MODIFY_MENU(menuInputType, POPUP_VNI, vInputType == 1);
	MODIFY_MENU(menuInputType, POPUP_SIMPLE_TELEX, vInputType == 2);

	MODIFY_MENU(menuTableCode, POPUP_UNICODE, vCodeTable == 0);
	MODIFY_MENU(menuTableCode, POPUP_TCVN3, vCodeTable == 1);
	MODIFY_MENU(menuTableCode, POPUP_VNI_WINDOWS, vCodeTable == 2);
	MODIFY_MENU(otherCode, POPUP_UNICODE_COMPOUND, vCodeTable == 3);
	MODIFY_MENU(otherCode, POPUP_VN_LOCALE_1258, vCodeTable == 4);

	bool isCtrlShift = (vSwitchKeyStatus & 0xFFFFFF00) == 0x5A000900; // Ctrl + Shift + NoKey?
	// Wait, we need a better way to represent shortcut keys.
	// For now let's just check standard masks.
	bool hasCtrl = (vSwitchKeyStatus & 0x100) != 0;
	bool hasAlt = (vSwitchKeyStatus & 0x200) != 0;
	bool hasWin = (vSwitchKeyStatus & 0x400) != 0;
	bool hasShift = (vSwitchKeyStatus & 0x800) != 0;
	unsigned short key = ((vSwitchKeyStatus >> 24) & 0xFF);

	MODIFY_MENU(menuSwitchKey, POPUP_SWITCH_CTRL_SHIFT, hasCtrl && hasShift && !hasAlt && !hasWin && key == 0xFE);
	MODIFY_MENU(menuSwitchKey, POPUP_SWITCH_ALT_Z, hasAlt && !hasCtrl && !hasShift && !hasWin && key == 'Z');
	MODIFY_MENU(menuSwitchKey, POPUP_SWITCH_CTRL_SPACE, hasCtrl && !hasShift && !hasAlt && !hasWin && key == VK_SPACE);
	MODIFY_MENU(menuSwitchKey, POPUP_SWITCH_SHIFT_SPACE, hasShift && !hasCtrl && !hasAlt && !hasWin && key == VK_SPACE);

	MODIFY_MENU(menuTypingOpt, POPUP_OPT_MODERN_ORTHO, vUseModernOrthography);
	MODIFY_MENU(menuTypingOpt, POPUP_OPT_CAPS_FIRST, vUpperCaseFirstChar);
	MODIFY_MENU(menuTypingOpt, POPUP_OPT_USE_CLIPBOARD, vSendKeyStepByStep == 0);
	MODIFY_MENU(menuTypingOpt, POPUP_OPT_FIX_BROWSER, vFixRecommendBrowser);
	MODIFY_MENU(menuTypingOpt, POPUP_OPT_FIX_CHROMIUM, vFixChromiumBrowser);

	MODIFY_MENU(menuSystem, POPUP_SYS_STARTUP, vRunWithWindows);
	MODIFY_MENU(menuSystem, POPUP_SYS_METRO, vSupportMetroApp);
	MODIFY_MENU(menuSystem, POPUP_SYS_MODERN_ICON, vUseGrayIcon);
	bool hasBeep = (vSwitchKeyStatus & 0x1000) != 0;
	MODIFY_MENU(menuSystem, POPUP_SYS_BEEP, hasBeep);

	wstring hotkey = L"";
	bool hasAdd = false;
	if (convertToolHotKey & 0x100) {
		hotkey += L"Ctrl";
		hasAdd = true;
	}
	if (convertToolHotKey & 0x200) {
		if (hasAdd)
			hotkey += L" + ";
		hotkey += L"Alt";
		hasAdd = true;
	}
	if (convertToolHotKey & 0x400) {
		if (hasAdd)
			hotkey += L" + ";
		hotkey += L"Win";
		hasAdd = true;
	}
	if (convertToolHotKey & 0x800) {
		if (hasAdd)
			hotkey += L" + ";
		hotkey += L"Shift";
		hasAdd = true;
	}

	unsigned short k = ((convertToolHotKey >> 24) & 0xFF);
	if (k != 0xFE) {
		if (hasAdd)
			hotkey += L" + ";
		if (k == VK_SPACE)
			hotkey += L"Space";
		else
			hotkey += (wchar_t)k;
	}

	wstring hotKeyString = menuData[POPUP_QUICK_CONVERT];
	if (hasAdd) {
		hotKeyString += L" - [";
		hotKeyString += hotkey;
		hotKeyString += L"]";
	}
	ModifyMenu(popupMenu, POPUP_QUICK_CONVERT, MF_BYCOMMAND | MF_UNCHECKED, POPUP_QUICK_CONVERT, hotKeyString.c_str());
}

void SystemTrayHelper::createSystemTrayIcon(const HINSTANCE& hIns) {
	HWND hWnd = createFakeWindow(hIns);
	createPopupMenu();

	//create system tray
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hWnd;
	nid.uID = TRAY_ICONUID;
	nid.uVersion = NOTIFYICON_VERSION;
	nid.uCallbackMessage = WM_TRAYMESSAGE;
	loadTrayIcon();
	LoadString(hIns, IDS_APP_TITLE, nid.szTip, 128);
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD, &nid);
}

void SystemTrayHelper::removeSystemTray() {
	Shell_NotifyIcon(NIM_DELETE, &nid);
}