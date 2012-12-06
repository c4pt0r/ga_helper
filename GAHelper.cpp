#include "StdAfx.h"
#include "GAHelper.h"

#include <sstream>
#include <time.h>
#include <algorithm>
#include <cctype>
#include <Winhttp.h> 

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Winhttp.lib")


const std::wstring GA_HOST = L"www.google-analytics.com";
const std::wstring GA_HEAD = L"/__utm.gif?utmwv=1";


DWORD WINAPI SendProc(LPVOID data);

long long BigRand(){
	long long num = rand();
	num = (num << 32 | rand());
	num = (num % (9999999999 - 1000000000)) + 1000000000;
	return num;
}
void GAHelper::PerformGARequest(const std::wstring& label, const std::wstring& refer) {
	GAParameters params(var_utmac, var_utmhn, label, refer);
	GAHelper::PerformGARequest(params);
}

void GAHelper::PerformGARequest(const GAParameters& param){

	std::wostringstream convert; 
	convert<<BigRand();
	std::wstring var_utmn = convert.str();

	convert.clear();
	convert.seekp(0);
	convert<<rand();
	std::wstring var_cookie = convert.str();

	convert.clear();
	convert.seekp(0);
	convert<<rand();
	std::wstring var_random = convert.str();

	convert.clear();
	convert.seekp(0);
	time_t seconds = time(NULL);
	convert<<seconds;
	std::wstring var_today = convert.str();

	std::wstring var_uservar = L"-"; 

	std::wostringstream ga_url_stream;
	ga_url_stream << GA_HEAD;
	ga_url_stream << L"&utmn="<< var_utmn;
	ga_url_stream << L"&utmsr=-&utmsc=-&utmul=-&utmje=0&utmfl=-&utmdt=-";
	ga_url_stream << L"&utmhn=" << param.utmhn_;
	ga_url_stream << L"&utm_source=&utm_medium=&utm_campaign=";
	ga_url_stream << L"&utmr=" << L"wdj.com";
	ga_url_stream << L"&utmp=" << param.utmp_;
	ga_url_stream << L"&utmac=" << param.utmac_;
	ga_url_stream << L"&utmcc=__utma%3D" <<var_cookie;
	ga_url_stream << L"."<< var_random<<L"."<<var_today<<L"."<<var_today<<L"."<<var_today;
	ga_url_stream << L".2%3B%2B__utmb%3D" << var_cookie;
	ga_url_stream << L"%3B%2B__utmc%3D"<< var_cookie;
	ga_url_stream << L"%3B%2B__utmz%3D" << var_cookie <<L"."<<var_today;
	ga_url_stream << L".2.2.utmccn%3D(direct)%7Cutmcsr%3D(direct)%7Cutmcmd%3D(none)%3B%2B__utmv%3D";
	ga_url_stream << var_cookie<<L"."<< var_uservar <<L"%3B";

	std::wstring* wurl = new std::wstring(ga_url_stream.str());
	//LOG((LPCWSTR)wurl->c_str());
	DWORD threadId;
	if (CreateThread(NULL, 0,
					(LPTHREAD_START_ROUTINE)&SendProc,
					(LPVOID) wurl,
					0,
					&threadId 
					) == NULL) {
	}
		
}

DWORD WINAPI SendProc(LPVOID data){
	DWORD dataSize = 0;
	DWORD bytesDownloaded = 0;
	LPSTR outBuffer;
	BOOL  result = FALSE;
	HINTERNET session = NULL,connection = NULL,request = NULL;

	std::wstring *wurl = (std::wstring*)data;
	if( wurl == NULL ){
		return result;
	}
	session = WinHttpOpen(GA_HOST.c_str(),  
						  WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
						  WINHTTP_NO_PROXY_NAME, 
						  WINHTTP_NO_PROXY_BYPASS, 0 );

	if( session )
		connection = WinHttpConnect( session, GA_HOST.c_str(),
									INTERNET_DEFAULT_HTTP_PORT, 0 );

	if( connection )
		request = WinHttpOpenRequest( connection, L"GET", wurl->c_str(),
								   NULL, WINHTTP_NO_REFERER, 
								   WINHTTP_DEFAULT_ACCEPT_TYPES, 
								   0 );

	if( request )
		result = WinHttpSendRequest( request,
								   WINHTTP_NO_ADDITIONAL_HEADERS, 0,
								   WINHTTP_NO_REQUEST_DATA, 0, 
								   0, 0 );
	//DWORD n = GetLastError();
	if( result )
		result = WinHttpReceiveResponse( request, NULL );

	if( result ) {
		do {
			  dataSize = 0;
			  if( !WinHttpQueryDataAvailable( request, &dataSize ) )
				return FALSE;

			  outBuffer = new char[dataSize+1];
			  if( !outBuffer ) {
				return false;
			  } else {
				ZeroMemory( outBuffer, dataSize+1 );
				if( !WinHttpReadData( request, (LPVOID)outBuffer, dataSize, &bytesDownloaded ) ) {
					delete []outBuffer;
					return false;
				} 
				// TODO: if you care about the response, please handle it here...
				//
				delete[] outBuffer;
			}
		} while( dataSize > 0 );
	}
	// clean up
	if( request ) WinHttpCloseHandle( request );
	if( connection ) WinHttpCloseHandle( connection );
	if( session ) WinHttpCloseHandle( session );

	delete wurl;
	return result;

}
