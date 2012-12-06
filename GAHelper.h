#pragma once

#include <sstream>

// put your GA info here
const std::wstring var_utmac = L"UA-11111111-11";
const std::wstring var_utmhn = L"xxx.xxx.com";

class GAParameters;
class GAHelper {
public:
	static void GAHelper::PerformGARequest(const GAParameters& param);
	static void GAHelper::PerformGARequest(const std::wstring& label, const std::wstring& refer);
private:
	GAHelper(void);
};

class GAParameters{
public:
	GAParameters(	std::wstring utmac,
					std::wstring utmhn,
					std::wstring utmp = L"",
					std::wstring utmr = L"",					
					std::wstring source = L"",
					std::wstring media = L"",
					std::wstring campaign = L""):
						utmac_(utmac),
						utmhn_(utmhn),
						utmp_(utmp),
						utmr_(utmr),
						source_(source),
						media_(media),
						campaign_(campaign){
	}
	
	
	std::wstring utmac_;
	std::wstring utmhn_;
	std::wstring source_;
	std::wstring media_;
	std::wstring campaign_;
	std::wstring utmp_;
	std::wstring utmr_;
};