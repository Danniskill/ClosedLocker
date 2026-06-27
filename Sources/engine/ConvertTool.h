//
//  ConvertTool.h
//  OpenKey
//
//  Created by Tuyen on 9/4/19.
//  Copyright © 2019 Tuyen Mai. All rights reserved.
//

#ifndef ConvertTool_h
#define ConvertTool_h

#include "DataType.h"
#include <string>
using namespace std;

extern int convertToolDontAlertWhenCompleted;
extern int convertToolToAllCaps;
extern int convertToolToAllNonCaps;
extern int convertToolToCapsFirstLetter;
extern int convertToolToCapsEachWord;
extern int convertToolRemoveMark;
extern Uint8 convertToolFromCode;
extern Uint8 convertToolToCode;
extern int convertToolHotKey;

string convertUtil(const string& sourceString);

#endif /* ConvertTool_h */
