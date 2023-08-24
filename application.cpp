#include <iostream>
#include <atlbase.h>

//×¢²á±í¼ü
std::wstring g_key = L"SOFTWARE\\Microsoft\\Cryptography";
//×¢²á±íÃû×Ö
std::wstring g_name = L"MachineGuid";

namespace Sky {

	std::wstring GetMachineGUID()
	{
		HKEY hKey;

#ifdef _M_IX86
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, g_key.c_str(), 0, KEY_READ | KEY_WOW64_32KEY, &hKey) != ERROR_SUCCESS)
			throw std::runtime_error("Could not open registry key");
#elif _M_X64
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, g_key.c_str(), 0, KEY_READ | KEY_WOW64_64KEY, &hKey) != ERROR_SUCCESS)
			throw std::runtime_error("Could not open registry key");
#endif		

		DWORD type;
		DWORD cbData;

		if (RegQueryValueEx(hKey, g_name.c_str(), NULL, &type, NULL, &cbData) != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			throw std::runtime_error("Could not read registry value");
		}

		if (type != REG_SZ)
		{
			RegCloseKey(hKey);
			throw "Incorrect registry value type";
		}

		std::wstring value(cbData / sizeof(wchar_t), L'\0');

		if (RegQueryValueEx(hKey, g_name.c_str(), NULL, NULL, reinterpret_cast<LPBYTE>(&value[0]), &cbData) != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			throw "Could not read registry value";
		}

		RegCloseKey(hKey);

		return value;
	}

	namespace WrongTest
	{
		void WrongJudge()
		{
			HKEY hKey;

#ifdef _WIN32
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, g_key.c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
				throw std::runtime_error("Could not open registry key");
#elif _WIN64
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, g_key.c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
				throw std::runtime_error("Could not open registry key");
#endif
		}
	}
}


int main()
{
	FILE* txtOut;

	fopen_s(&txtOut, "MachineGuid.txt", "wb");

	std::cout << "This is a Demo for getting the MachineGUID of the Windows." << std::endl;
	std::cout << std::endl;

	std::cout << "The MachineGUID is: ";
	std::wstring MachineGUID = Sky::GetMachineGUID();
	std::wcout << MachineGUID << std::endl;
	std::cout << std::endl;

	fwrite(MachineGUID.c_str(), sizeof(wchar_t), MachineGUID.size(), txtOut);

	std::cout << "The Guid txt created.";
	//std::cin.get();
	Sleep(1000);

	fclose(txtOut);
}