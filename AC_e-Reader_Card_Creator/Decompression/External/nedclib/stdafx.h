// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
inline int fopen_s(FILE **handle, const char *path, const char *mode) {
    FILE *h = fopen(path, mode);
    if (!h)
        return 1;
    *handle = h;
    return 0;
}
#define sprintf_s(buf, n, format, ...) snprintf(buf, n, format, __VA_ARGS__)
#define vsprintf_s(buf, n, format, args) vsnprintf(buf, n, format, args)
#define _stricmp(a, b) strcmp(a, b)
#define stricmp(a, b) strcmp(a, b)
#else
#include <windows.h>
#endif
// TODO: reference additional headers your program requires here
