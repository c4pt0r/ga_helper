#pragma once

#include <sstream>

class GAParameters;
class GAHelper {
public:
	static void GAHelper::PerformGARequest(const GAParameters& param);
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