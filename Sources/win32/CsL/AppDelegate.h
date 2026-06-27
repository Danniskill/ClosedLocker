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
#pragma once
#include "stdafx.h"
class AppDelegate {
private:
	HINSTANCE hInstance;
private:
public:
	AppDelegate();
	static AppDelegate* getInstance();
	int run(HINSTANCE hInstance);
public: //event
	void onInputMethodChangedFromHotKey();
	void onDefaultConfig();

	void onToggleVietnamese();
	void onToggleCheckSpelling();
	void onToggleUseSmartSwitchKey();
	void onToggleUseMacro();
	
	// New menu toggles
	void onSetSwitchKey(const unsigned int& keyStatus);
	void onToggleModernOrtho();
	void onToggleCapsFirst();
	void onToggleUseClipboard();
	void onToggleFixBrowser();
	void onToggleFixChromium();
	
	void onToggleStartup();
	void onToggleMetro();
	void onToggleModernIcon();
	void onToggleBeep();

	void onQuickConvert();

	void onInputType(const int& type);
	void onTableCode(const int& code);

	void onOpenKeyExit();
};