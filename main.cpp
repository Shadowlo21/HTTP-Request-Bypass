bool DownloadFile(const std::wstring& url, const std::wstring& filePath)
{
    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    bool result = false;
 
    if (std::filesystem::exists(filePath))
    {
        std::filesystem::remove(filePath);
    }
 
    hSession = WinHttpOpen(L"UserAgent/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (hSession)
    {
        WINHTTP_URL_COMPONENTS urlComponents = { 0 };
        wchar_t hostName[256] = { 0 };
        wchar_t urlPath[1024] = { 0 };
 
        urlComponents.dwStructSize = sizeof(urlComponents);
        urlComponents.lpszHostName = hostName;
        urlComponents.dwHostNameLength = sizeof(hostName) / sizeof(hostName[0]);
        urlComponents.lpszUrlPath = urlPath;
        urlComponents.dwUrlPathLength = sizeof(urlPath) / sizeof(urlPath[0]);
        urlComponents.nScheme = INTERNET_SCHEME_HTTPS;
 
        if (WinHttpCrackUrl(url.c_str(), 0, 0, &urlComponents))
        {
            hConnect = WinHttpConnect(hSession, urlComponents.lpszHostName, urlComponents.nPort, 0);
            if (hConnect)
            {
                hRequest = WinHttpOpenRequest(hConnect, L"GET", urlComponents.lpszUrlPath, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
                if (hRequest)
                {
                    if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0))
                    {
                        if (WinHttpReceiveResponse(hRequest, NULL))
                        {
                            DWORD dwSize = 0;
                            DWORD dwDownloaded = 0;
                            std::vector<char> buffer;
 
                            do
                            {
                                dwSize = 0;
                                if (WinHttpQueryDataAvailable(hRequest, &dwSize))
                                {
                                    buffer.resize(dwSize);
                                    if (WinHttpReadData(hRequest, &buffer[0], dwSize, &dwDownloaded))
                                    {
                                        std::ofstream outFile(filePath, std::ios::binary | std::ios::app);
                                        if (outFile.is_open())
                                        {
                                            outFile.write(&buffer[0], dwDownloaded);
                                            outFile.close();
                                            result = true;
                                        }
                                    }
                                }
                            } while (dwSize > 0);
                        }
                    }
                }
            }
        }
    }
 
    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);
 
    return result;
}





int main(){

	std::wstring URL = L"url";
	std::wstring FilePath = pathfile;
 
	if (DownloadFile("https:\\txt.com", "C\\Test.txt"))
	{Printf("Downlaod Done");}
	else
		{Printf("Downlaod couldnt be Done");}

}

