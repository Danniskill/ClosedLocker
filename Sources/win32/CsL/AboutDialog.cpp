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
#include "stdafx.h"
#include "AboutDialog.h"
#include "AppDelegate.h"

AboutDialog::AboutDialog(const HINSTANCE & hInstance, const int & resourceId)
	: BaseDialog(hInstance, resourceId) {
}

AboutDialog::~AboutDialog() {
	if (_hBoldFont) {
		DeleteObject(_hBoldFont);
	}
}

void AboutDialog::fillData() {
}

INT_PTR AboutDialog::eventProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
		this->hDlg = hDlg;
		initDialog();
		return TRUE;
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		switch (wmId) {
		case IDM_EXIT:
		case IDBUTTON_OK:
			AppDelegate::getInstance()->closeDialog(this);
			break;
		}
		break;
	}
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code) {
		case NM_CLICK:
		case NM_RETURN: {
			PNMLINK link = (PNMLINK)lParam;
			if (link->hdr.idFrom == IDC_SYSLINK_HOME_PAGE)
				ShellExecute(NULL, _T("open"), _T("https://github.com/tuyenvm/OpenKey"), NULL, NULL, SW_SHOWNORMAL);
			else if (link->hdr.idFrom == IDC_SYSLINK_NEW_VERSION)
				ShellExecute(NULL, _T("open"), _T("https://github.com/tuyenvm/OpenKey/releases"), NULL, NULL, SW_SHOWNORMAL);
			else if (link->hdr.idFrom == IDC_SYSLINK_FANPAGE)
				ShellExecute(NULL, _T("open"), _T("https://www.facebook.com/OpenKeyVN"), NULL, NULL, SW_SHOWNORMAL);
		}
		break;
		}
	}
	return FALSE;
}

void AboutDialog::initDialog() {
	//dialog icon
	SET_DIALOG_ICON(IDI_APP_ICON);
	
	wchar_t titleBuffer[256];
#ifdef _WIN64
	wsprintfW(titleBuffer, _T("CsLite (64 bit) - Bộ gõ tiếng Việt tối giản"));
#else
	wsprintfW(titleBuffer, _T("CsLite (32 bit) - Bộ gõ tiếng Việt tối giản"));
#endif
	SendDlgItemMessage(hDlg, IDC_STATIC_APP_TITLE, WM_SETTEXT, 0, LPARAM(titleBuffer));

	wchar_t versionBuffer[256];
	wsprintfW(versionBuffer, _T("Phiên bản: %s"), OpenKeyHelper::getVersionString().c_str());
	SendDlgItemMessage(hDlg, IDC_STATIC_APP_VERSION, WM_SETTEXT, 0, LPARAM(versionBuffer));

	wchar_t captionBuffer[256];
	wsprintfW(captionBuffer, _T("Thông tin CsLite %s"), OpenKeyHelper::getVersionString().c_str());
	SetWindowText(hDlg, captionBuffer);

	// Set bold font for authors
	HFONT hFont = (HFONT)SendDlgItemMessage(hDlg, IDC_STATIC_AUTHOR1, WM_GETFONT, 0, 0);
	if (hFont) {
		LOGFONT lf;
		GetObject(hFont, sizeof(LOGFONT), &lf);
		lf.lfWeight = FW_BOLD;
		_hBoldFont = CreateFontIndirect(&lf);
		SendDlgItemMessage(hDlg, IDC_STATIC_AUTHOR1, WM_SETFONT, (WPARAM)_hBoldFont, TRUE);
		SendDlgItemMessage(hDlg, IDC_STATIC_AUTHOR2, WM_SETFONT, (WPARAM)_hBoldFont, TRUE);
	}

	HINSTANCE hIns = (HINSTANCE)GetWindowLongPtr(hDlg, GWLP_HINSTANCE);
	SendDlgItemMessage(hDlg, IDBUTTON_OK, BM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(hIns, MAKEINTRESOURCEW(IDI_ICON_OK_BUTTON)));
}


