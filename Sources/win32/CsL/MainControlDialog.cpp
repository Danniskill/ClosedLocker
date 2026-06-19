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
#include "MainControlDialog.h"
#include "AppDelegate.h"
#include <Shlobj.h>

static Uint16 _lastKeyCode;

MainControlDialog::MainControlDialog(const HINSTANCE & hInstance, const int & resourceId) 
	: BaseDialog(hInstance, resourceId) {
}

MainControlDialog::~MainControlDialog() {
}

void MainControlDialog::initDialog() {
	HINSTANCE hIns = GetModuleHandleW(NULL);
	//dialog icon
	SET_DIALOG_ICON(IDI_APP_ICON);

	//set title version
	TCHAR title[256];
	TCHAR titleBuffer[256];
	LoadString(hIns, IDS_MAIN_DIALOG_TITLE, title, 256);
	wsprintfW(titleBuffer, title, OpenKeyHelper::getVersionString().c_str());
	SetWindowText(hDlg, titleBuffer);

	//input type
	comboBoxInputType = GetDlgItem(hDlg, IDC_COMBO_INPUT_TYPE);
	vector<LPCTSTR>& inputType = OpenKeyManager::getInputType();
	for (int i = 0; i < inputType.size(); i++) {
		SendMessage(comboBoxInputType, CB_ADDSTRING, i, reinterpret_cast<LPARAM>(inputType[i]));
	}
	createToolTip(comboBoxInputType, IDS_STRING_INPUT);

	//code table
	comboBoxTableCode = GetDlgItem(hDlg, IDC_COMBO_TABLE_CODE);
	vector<LPCTSTR>& tableCode = OpenKeyManager::getTableCode();
	for (int i = 0; i < tableCode.size(); i++) {
		SendMessage(comboBoxTableCode, CB_ADDSTRING, i, reinterpret_cast<LPARAM>(tableCode[i]));
	}
	createToolTip(comboBoxTableCode, IDS_STRING_CODE);

	checkCtrl = GetDlgItem(hDlg, IDC_CHECK_SWITCH_KEY_CTRL);
	createToolTip(checkCtrl, IDS_STRING_CTRL);

	checkAlt = GetDlgItem(hDlg, IDC_CHECK_SWITCH_KEY_ALT);
	createToolTip(checkAlt, IDS_STRING_ALT);

	checkWin = GetDlgItem(hDlg, IDC_CHECK_SWITCH_KEY_WIN); 
	createToolTip(checkWin, IDS_STRING_WIN);

	checkShift = GetDlgItem(hDlg, IDC_CHECK_SWITCH_KEY_SHIFT); 
	createToolTip(checkShift, IDS_STRING_SHIFT);

	textSwitchKey = GetDlgItem(hDlg, IDC_SWITCH_KEY_KEY);
	createToolTip(textSwitchKey, IDS_STRING_SWITCH_KEY);

	checkBeep = GetDlgItem(hDlg, IDC_CHECK_SWITCH_KEY_BEEP);
	createToolTip(checkBeep, IDS_STRING_BEEP);

	checkVietnamese = GetDlgItem(hDlg, IDC_RADIO_METHOD_VIETNAMESE); 
	createToolTip(checkVietnamese, IDS_STRING_VIET);

	checkEnglish = GetDlgItem(hDlg, IDC_RADIO_METHOD_ENGLISH);
	createToolTip(checkEnglish, IDS_STRING_ENG);

	/*--------end common---------*/

	checkModernOrthorgraphy = GetDlgItem(hDlg, IDC_CHECK_MODERN_ORTHORGRAPHY);
	createToolTip(checkModernOrthorgraphy, IDS_STRING_MORDEN_ORTHORGRAPHY);

	checkFixRecommendBrowser = GetDlgItem(hDlg, IDC_CHECK_FIX_RECOMMEND_BROWSER);
	createToolTip(checkFixRecommendBrowser, IDS_STRING_FIX_BROWSER);

	checkCapsFirstChar = GetDlgItem(hDlg, IDC_CHECK_CAPS_FIRST_CHAR);
	createToolTip(checkCapsFirstChar, IDS_STRING_CAPS_FIRST_CHAR);

	hUpdateButton = GetDlgItem(hDlg, IDC_BUTTON_CHECK_UPDATE);

	checkModernIcon = GetDlgItem(hDlg, IDC_CHECK_MODERN_ICON);
	createToolTip(checkModernIcon, IDS_STRING_MODERN_ICON);

	checkShowOnStartup = GetDlgItem(hDlg, IDC_CHECK_SHOW_ON_STARTUP);
	createToolTip(checkShowOnStartup, IDS_STRING_SHOW_ON_STARTUP);

	checkRunWithWindows = GetDlgItem(hDlg, IDC_CHECK_RUN_WITH_WINDOWS);
	createToolTip(checkRunWithWindows, IDS_STRING_RUN_ON_STARTUP);

	checkCheckNewVersion = GetDlgItem(hDlg, IDC_CHECK_CHECK_UPDATE);
	createToolTip(checkCheckNewVersion, IDS_STRING_CHECK_UPDATE);

	checkSupportMetroApp = GetDlgItem(hDlg, IDC_CHECK_SUPPORT_METRO_APP);
	createToolTip(checkSupportMetroApp, IDS_STRING_SUPPORT_METRO);

	checkUseClipboard = GetDlgItem(hDlg, IDC_CHECK_USE_CLIPBOARD);
	createToolTip(checkUseClipboard, IDS_STRING_USE_CLIPBOARD);

	checkFixChromium = GetDlgItem(hDlg, IDC_CHECK_FIX_CHROMIUM);
	createToolTip(checkFixChromium, IDS_STRING_FIX_CHROMIUM);

	/*------------end tab 3----------------*/

	SendDlgItemMessage(hDlg, IDBUTTON_OK, BM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(hIns, MAKEINTRESOURCEW(IDI_ICON_OK_BUTTON)));
	SendDlgItemMessage(hDlg, ID_BTN_DEFAULT, BM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(hIns, MAKEINTRESOURCEW(IDI_ICON_DEFAULT_BUTTON)));
	SendDlgItemMessage(hDlg, IDBUTTON_EXIT, BM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(hIns, MAKEINTRESOURCEW(IDI_ICON_EXIT_BUTTON)));
	fillData();
}

INT_PTR MainControlDialog::eventProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
		this->hDlg = hDlg;
		initDialog();
		return TRUE;
	case WM_CTLCOLORSTATIC:
		SetBkMode((HDC)wParam, TRANSPARENT);
		return (INT_PTR)GetSysColorBrush(COLOR_BTNFACE);
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		switch (wmId) {
		case IDBUTTON_OK:
			AppDelegate::getInstance()->closeDialog(this);
			break;
		case IDBUTTON_EXIT:
			AppDelegate::getInstance()->onOpenKeyExit();
			break;
		case ID_BTN_DEFAULT: {
			int msgboxID = MessageBox(
				hDlg,
				_T("Bạn có chắc chắn muốn thiết lập lại cài đặt gốc?"),
				_T("CsL"),
				MB_ICONEXCLAMATION | MB_YESNO
			);
			if (msgboxID == IDYES) {
				AppDelegate::getInstance()->onDefaultConfig();
			}
			break;
		}
		case IDC_BUTTON_MACRO_TABLE:
			AppDelegate::getInstance()->onMacroTable();
			break;
		case IDC_BUTTON_CHECK_UPDATE:
			onUpdateButton();
			break;
		case IDC_BUTTON_GO_SOURCE_CODE:
			ShellExecute(NULL, _T("open"), _T("https://github.com/tuyenvm/OpenKey"), NULL, NULL, SW_SHOWNORMAL);
			break;
		default:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				this->onComboBoxSelected((HWND)lParam, LOWORD(wParam));
			}
			else if (HIWORD(wParam) == BN_CLICKED) {
				this->onCheckboxClicked((HWND)lParam);
			}
			else if (HIWORD(wParam) == EN_CHANGE) {
				_lastKeyCode = OpenKeyManager::_lastKeyCode;
				if (_lastKeyCode > 0) {
					OpenKeyManager::_lastKeyCode = 0;
					this->onCharacter((HWND)lParam, _lastKeyCode);
				}
			}
			break;
		}
		break;
	}
	case WM_NOTIFY: {
		switch (((LPNMHDR)lParam)->code) {
		case NM_CLICK:
		case NM_RETURN: {
			PNMLINK link = (PNMLINK)lParam;
			if (link->hdr.idFrom == IDC_SYSLINK_HOME_PAGE)
				ShellExecute(NULL, _T("open"), _T("https://open-key.org"), NULL, NULL, SW_SHOWNORMAL);
			else if (link->hdr.idFrom == IDC_SYSLINK_FANPAGE)
				ShellExecute(NULL, _T("open"), _T("https://www.facebook.com/OpenKeyVN"), NULL, NULL, SW_SHOWNORMAL);
			else if (link->hdr.idFrom == IDC_SYSLINK_AUTHOR_EMAIL)
				ShellExecute(NULL, _T("open"), _T("mailto:maivutuyen.91@gmail.com"), NULL, NULL, SW_SHOWNORMAL);
			break;
		}
		}
		break;
	}
	}

	return FALSE;
}



void MainControlDialog::fillData() {
	SendMessage(comboBoxInputType, CB_SETCURSEL, vInputType, 0);
	SendMessage(comboBoxTableCode, CB_SETCURSEL, vCodeTable, 0);
	
	SendMessage(checkCtrl, BM_SETCHECK, HAS_CONTROL(vSwitchKeyStatus) ? 1 : 0, 0);
	SendMessage(checkAlt, BM_SETCHECK, HAS_OPTION(vSwitchKeyStatus) ? 1 : 0, 0);
	SendMessage(checkWin, BM_SETCHECK, HAS_COMMAND(vSwitchKeyStatus) ? 1 : 0, 0);
	SendMessage(checkShift, BM_SETCHECK, HAS_SHIFT(vSwitchKeyStatus) ? 1 : 0, 0);
	setSwitchKeyText(textSwitchKey, (vSwitchKeyStatus >> 24) & 0xFF);
	SendMessage(checkBeep, BM_SETCHECK, HAS_BEEP(vSwitchKeyStatus) ? 1 : 0, 0);

	SendMessage(checkVietnamese, BM_SETCHECK, vLanguage, 0);
	SendMessage(checkEnglish, BM_SETCHECK, !vLanguage, 0);
	
	SendMessage(checkModernOrthorgraphy, BM_SETCHECK, vUseModernOrthography ? 1 : 0, 0);
	SendMessage(checkFixRecommendBrowser, BM_SETCHECK, vFixRecommendBrowser ? 1 : 0, 0);
	SendMessage(checkShowOnStartup, BM_SETCHECK, vShowOnStartUp ? 1 : 0, 0);
	SendMessage(checkRunWithWindows, BM_SETCHECK, vRunWithWindows ? 1 : 0, 0);
	SendMessage(checkModernIcon, BM_SETCHECK, vUseGrayIcon ? 1 : 0, 0);
	SendMessage(checkCapsFirstChar, BM_SETCHECK, vUpperCaseFirstChar ? 1 : 0, 0);
	SendMessage(checkSupportMetroApp, BM_SETCHECK, vSupportMetroApp ? 1 : 0, 0);
	SendMessage(checkCheckNewVersion, BM_SETCHECK, vCheckNewVersion ? 1 : 0, 0);
	SendMessage(checkUseClipboard, BM_SETCHECK, vSendKeyStepByStep ? 0 : 1, 0);
	SendMessage(checkFixChromium, BM_SETCHECK, vFixChromiumBrowser ? 1 : 0, 0);

	EnableWindow(checkFixChromium, vFixRecommendBrowser);
}

void MainControlDialog::setSwitchKey(const unsigned short& code) {
	vSwitchKeyStatus &= 0xFFFFFF00;
	vSwitchKeyStatus |= code;
	vSwitchKeyStatus &= 0x00FFFFFF;
	vSwitchKeyStatus |= ((unsigned int)code << 24);
	APP_SET_DATA(vSwitchKeyStatus, vSwitchKeyStatus);
}

void MainControlDialog::onComboBoxSelected(const HWND & hCombobox, const int & comboboxId) {
	if (hCombobox == comboBoxInputType) {
		APP_SET_DATA(vInputType, (int)SendMessage(hCombobox, CB_GETCURSEL, 0, 0));
	} else if (hCombobox == comboBoxTableCode) {
		APP_SET_DATA(vCodeTable, (int)SendMessage(hCombobox, CB_GETCURSEL, 0, 0));
		if (vRememberCode) {
			setAppInputMethodStatus(OpenKeyHelper::getFrontMostAppExecuteName(), vLanguage | (vCodeTable << 1));
			saveSmartSwitchKeyData();
		}
	}
	SystemTrayHelper::updateData();
}

void MainControlDialog::onCheckboxClicked(const HWND & hWnd) {
	int val = 0;
	if (hWnd == checkCtrl) {
		val = (int)SendMessage(checkCtrl, BM_GETCHECK, 0, 0);
		vSwitchKeyStatus &= (~0x100);
		vSwitchKeyStatus |= val << 8;
		APP_SET_DATA(vSwitchKeyStatus, vSwitchKeyStatus);
	} else if (hWnd == checkAlt) {
		val = (int)SendMessage(checkAlt, BM_GETCHECK, 0, 0);
		vSwitchKeyStatus &= (~0x200);
		vSwitchKeyStatus |= val << 9;
		APP_SET_DATA(vSwitchKeyStatus, vSwitchKeyStatus);
	} else if (hWnd == checkWin) {
		val = (int)SendMessage(checkWin, BM_GETCHECK, 0, 0);
		vSwitchKeyStatus &= (~0x400);
		vSwitchKeyStatus |= val << 10;
		APP_SET_DATA(vSwitchKeyStatus, vSwitchKeyStatus);
	} else if (hWnd == checkShift) {
		val = (int)SendMessage(checkShift, BM_GETCHECK, 0, 0);
		vSwitchKeyStatus &= (~0x800);
		vSwitchKeyStatus |= val << 11;
		APP_SET_DATA(vSwitchKeyStatus, vSwitchKeyStatus);
	} else if (hWnd == checkBeep) {
		val = (int)SendMessage(checkBeep, BM_GETCHECK, 0, 0);
		vSwitchKeyStatus &= (~0x8000);
		vSwitchKeyStatus |= val << 15;
		APP_SET_DATA(vSwitchKeyStatus, vSwitchKeyStatus);
	} else if (hWnd == checkVietnamese) {
		val = (int)SendMessage(checkVietnamese, BM_GETCHECK, 0, 0);
		APP_SET_DATA(vLanguage, val ? 1 : 0);
		if (vUseSmartSwitchKey) {
			setAppInputMethodStatus(OpenKeyHelper::getFrontMostAppExecuteName(), vLanguage | (vCodeTable << 1));
			saveSmartSwitchKeyData();
		}
	} else if (hWnd == checkEnglish) {
		val = (int)SendMessage(checkVietnamese, BM_GETCHECK, 0, 0);
		APP_SET_DATA(vLanguage, val ? 1 : 0);
		if (vUseSmartSwitchKey) {
			setAppInputMethodStatus(OpenKeyHelper::getFrontMostAppExecuteName(), vLanguage | (vCodeTable << 1));
			saveSmartSwitchKeyData();
		}
	} else if (hWnd == checkModernOrthorgraphy) {
		val = (int)SendMessage(hWnd, BM_GETCHECK, 0, 0);
		APP_SET_DATA(vUseModernOrthography, val ? 1 : 0);
	} else if (hWnd == checkFixRecommendBrowser) {
		val = (int)SendMessage(hWnd, BM_GETCHECK, 0, 0);
		APP_SET_DATA(vFixRecommendBrowser, val ? 1 : 0);
		EnableWindow(checkFixChromium, vFixRecommendBrowser);
	} else if (hWnd == checkShowOnStartup) {
		val = (int)SendMessage(hWnd, BM_GETCHECK, 0, 0);
		APP_SET_DATA(vShowOnStartUp, val ? 1 : 0);
	} else if (hWnd == checkRunWithWindows) {
		val = (int)SendMessage(hWnd, BM_GETCHECK, 0, 0);
		APP_SET_DATA(vRunWithWindows, val ? 1 : 0);
		OpenKeyHelper::registerRunOnStartup(vRunWithWindows);
	} else if (hWnd == checkUseClipboard) {
		val = (int)SendMessage(hWnd, BM_GETCHECK, 0, 0);
		APP_SET_DATA(vSendKeyStepByStep, val ? 0 : 1);
	} else if (hWnd == checkCapsFirstChar) {
		val = (int)SendMessage(hWnd, BM_GETCHECK, 0, 0);
		APP_SET_DATA(vUpperCaseFirstChar, val ? 1 : 0);
	} else if (hWnd == checkModernIcon) {
		val = (int)SendMessage(hWnd, BM_GETCHECK, 0, 0);
		APP_SET_DATA(vUseGrayIcon, val ? 1 : 0);
	} else if (hWnd == checkSupportMetroApp) {
		val = (int)SendMessage(hWnd, BM_GETCHECK, 0, 0);
		APP_SET_DATA(vSupportMetroApp, val ? 1 : 0);
	} else if (hWnd == checkCheckNewVersion) {
		val = (int)SendMessage(hWnd, BM_GETCHECK, 0, 0);
		APP_SET_DATA(vCheckNewVersion, val ? 1 : 0);
	} else if (hWnd == checkFixChromium) {
		val = (int)SendMessage(hWnd, BM_GETCHECK, 0, 0);
		APP_SET_DATA(vFixChromiumBrowser, val ? 1 : 0);
	}
	SystemTrayHelper::updateData();
}

void MainControlDialog::onCharacter(const HWND& hWnd, const UINT16 & keyCode) {
	if (keyCode == 0) return;
	if (hWnd == textSwitchKey) {
		UINT16 code = GET_SWITCH_KEY(vSwitchKeyStatus);
		if (keyCode == VK_DELETE || keyCode == VK_BACK) {
			code = 0xFE;
		}
		else if (keyCodeToCharacter(keyCode) != 0) {
			code = keyCode;
		}
		setSwitchKey(code);
		setSwitchKeyText(hWnd, code);
	}
}

void MainControlDialog::setSwitchKeyText(const HWND & hWnd, const UINT16 & keyCode) {
	if (keyCode == KEY_SPACE) {
		SetWindowText(hWnd, _T("Space"));
	} else if (keyCode == 0xFE) {
		SetWindowText(hWnd, _T(""));
	} else {
		Uint16 key[] = { keyCode, 0 };
		SetWindowText(hWnd, (LPCWSTR)&key);
	}
}

void MainControlDialog::onUpdateButton() {
	EnableWindow(hUpdateButton, false);
	string newVersion;
	if (OpenKeyManager::checkUpdate(newVersion)) {
		WCHAR msg[256];
		wsprintf(msg,
			TEXT("CsL Có phiên bản mới (%s), bạn có muốn cập nhật không?"),
			utf8ToWideString(newVersion).c_str());

		int msgboxID = MessageBox(
			hDlg,
			msg,
			_T("CsL Update"),
			MB_ICONEXCLAMATION | MB_YESNO
		);
		if (msgboxID == IDYES) {
			//Call OpenKeyUpdate
			WCHAR path[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, path);
			wsprintf(path, TEXT("%s\\CsLUpdate.exe"), path);
			ShellExecute(0, L"", path, 0, 0, SW_SHOWNORMAL);

			AppDelegate::getInstance()->onOpenKeyExit();
		}

	}
	else {
		MessageBox(hDlg, _T("Bạn đang dùng phiên bản mới nhất!"), _T("CsL Update"), MB_OK);
	}
	EnableWindow(hUpdateButton, true);
}

void MainControlDialog::requestRestartAsAdmin() {
	OpenKeyHelper::registerRunOnStartup(false);
	if (vRunAsAdmin && !IsUserAnAdmin()) {
		int msgboxID = MessageBox(
			hDlg,
			_T("Bạn cần phải khởi động lại CsL để kích hoạt chế độ Admin!\nBạn có muốn khởi động lại CsL không?"),
			_T("CsL"),
			MB_ICONEXCLAMATION | MB_YESNO
		);
		if (msgboxID == IDYES) {
			PostQuitMessage(0);
			ShellExecute(0, L"runas", OpenKeyHelper::getFullPath().c_str(), 0, 0, SW_SHOWNORMAL);
		}
	} else {
		OpenKeyHelper::registerRunOnStartup(vRunWithWindows);
	}
}
