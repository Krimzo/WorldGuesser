#include "utility/web.h"

#include <windows.h>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")


std::string kl::web::siteData(const std::string& url, uint bufferSize) {
	// Create browser
	HINTERNET connection = InternetOpenA("Browser", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0);
	if (!connection) {
		printf("Failed to connect!\n");
		return "";
	}

	// Open url
	HINTERNET openAddress = InternetOpenUrlA(connection, url.c_str(), nullptr, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);
	if (!openAddress) {
		printf("Failed to open \"%s\"!\n", url.c_str());
		return "";
	}

	// Download data
	std::string finalData;
	std::string dataBuffer;
	dataBuffer.resize(bufferSize);
	DWORD byteReadCount = 0;
	while (InternetReadFile(openAddress, &dataBuffer[0], (DWORD)dataBuffer.size(), &byteReadCount) && byteReadCount) {
		for (DWORD i = 0; i < byteReadCount; i++) {
			finalData.push_back(dataBuffer[i]);
		}
	}

	// Close browser and url
	InternetCloseHandle(openAddress);
	InternetCloseHandle(connection);

	// Return data
	return finalData;
}

void kl::web::toFile(const std::string& url, const std::string& fileName) {
	kl::file::writeString(fileName, kl::web::siteData(url));
}
