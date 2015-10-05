// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef MAIN_STDAFX_H
#define MAIN_STDAFX_H

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windowsx.h>

// crt memory leaks detection
#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>    
	#define DEBUG_NEW   new( _CLIENT_BLOCK, __FILE__, __LINE__)
	#define new DEBUG_NEW
#endif 

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here
#include <memory>
#include <functional>
#include <stdexcept>
#include <list>
#include <map>
#include <string>

#endif // MAIN_STDAFX_H

