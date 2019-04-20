#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <string>
#include <psapi.h>
#include <chrono>
#include <fstream>
#include <codecvt>

std::string ws2utf8(std::wstring &input) {
 std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8conv;
 return utf8conv.to_bytes(input);
}

std::wstring utf82ws(std::string &input) {
 std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8conv;
 return utf8conv.from_bytes(input);
}

double previousTime = clock();

namespace ErwinUtils {
	double getDifferenceBetweenClocks(double clock1, double clock2) {
		return (clock1 - clock2) / CLOCKS_PER_SEC * 1000;
	}

	void writeToFile(std::string name, std::string content, bool append = true, bool newLine = true) {
		std::ofstream soutfile;

		if (append) {
			soutfile.open(name, std::ios_base::app);
		} else {
			soutfile.open(name);
		}

		content = content.append(newLine ? "\n" : "");

		std::wbuffer_convert<std::codecvt_utf8<wchar_t>> converter(soutfile.rdbuf());
		std::wostream out(&converter);
		out << utf82ws(content);
		soutfile.close();
	}

	std::wstring getActiveWindowProcessName(HWND hwnd) {
		if (!hwnd) {
			return NULL;
		}

		std::wstring processName;
		DWORD dwPID;
		GetWindowThreadProcessId(hwnd, &dwPID);
		HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPID);

		if (handle) {
			TCHAR path[MAX_PATH];

			if (GetModuleFileNameEx(handle, 0, path, MAX_PATH)) {
				processName = path;
				std::string::size_type idx = processName.rfind(L"\\");

				if (idx != std::string::npos) {
					processName = processName.substr(idx + 1);
				}
			}

			CloseHandle(handle);
		}

		return processName;
	}
}
