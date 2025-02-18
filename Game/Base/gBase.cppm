export module gBase;

export import ProcessUtil;
export import PConfig;
export import Offseter;
export import <Windows.h>;
export import <cstdint>;
//#include <Windows.h>
//#include <cstdint>
//
//
export extern DWORD pid;
export extern uintptr_t clientDll;
export extern uintptr_t serverDll;
export extern uintptr_t engineDll;
export extern  uintptr_t entityList;
export bool initBase();