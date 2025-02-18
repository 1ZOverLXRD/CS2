export module ProcessUtil;

import <iostream>;
import <Windows.h>;
import <string>;
import PConfig;
export namespace ProcessUtils {
	HANDLE gHandle;

	DWORD GetProcessIdByName(const std::string& processName);
	uintptr_t GetModuleBaseAddress(DWORD pid, const std::string& moduleName);
	bool TestGetModuleBaseAddress(DWORD pid);
	bool attachProcess(DWORD pid) {
		gHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
		if (gHandle == NULL || gHandle == INVALID_HANDLE_VALUE) return false;
		return true;
	}
	void awaitProcess();

	template <typename T>
	T rm(uintptr_t addres) {
		T result{};
		ReadProcessMemory(gHandle, (LPCVOID)addres, &result, sizeof(T), 0);
		return result;
	}

	template <typename type>
	bool readArray(uintptr_t address, type* buffer, size_t size) {
		SIZE_T bytesRead;
		if (!ReadProcessMemory(gHandle, (LPCVOID)(address), buffer, size, &bytesRead)) {
			//cerr << "Could not read array at address " << hex << address << ". Error: " << GetLastError() << endl;
			return false;
		}
		return bytesRead == size;
	}

	template <typename type>
	bool wm(uintptr_t address, const type& value) {
		SIZE_T bytesWritten;
		if (!WriteProcessMemory(gHandle, (LPVOID)(address), &value, sizeof(type), &bytesWritten)) {
			//cerr << "Could not write memory at address " << hex << address << ". Error: " << GetLastError() << endl;
			return false;
		}

		if (bytesWritten != sizeof(type)) {
			//cerr << "Unexpected number of bytes written. Expected: " << sizeof(type) << ", Actual: " << bytesWritten << endl;
			return false;
		}
	}

}