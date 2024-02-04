#ifndef _COOKIEJAR_H_
#define _COOKIEJAR_H_

#include "utils.h"
#include <algorithm>

class Cookie {
public:
    std::string domain;
    long long expires;
    bool httpOnly = false;
    std::string name;
    std::string path;
    bool secure = false;
    std::string value;

    Cookie(const std::string& cookieData);

    bool isValidSetCookie(const std::string& cookie);
    std::string toString();
};

class CookieJar {
private:
    std::vector<Cookie> cookies;

    bool isCookieForDomain(const Cookie& cookie, const std::string& url);
public:
    void addCookie(const Cookie& cookie);
    std::string getCookiesForUrl(const std::string& url);
    bool isValidUrl(const std::string& url);
    std::string getDomain(const std::string& currentUrl);
    std::string toString();
    std::string toBasString();
    size_t length();

    void clear();
};

#endif