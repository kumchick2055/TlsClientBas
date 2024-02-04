#include "cookie.h"

Cookie::Cookie(const std::string& cookieData) {
    if (!isValidSetCookie(cookieData)) {
        return;
    }
    auto tokens = splitString(cookieData, "; ");
    auto splitData = splitString(tokens.at(0), "=");

    name = splitData.at(0);
    value = join(std::vector<std::string>(splitData.begin() + 1, splitData.end()), "=");

    for (const auto& t : tokens) {
        if (t.find('=') != std::string::npos) {
            auto splitdata = splitString(t, "=");

            if (splitdata.size() == 2) {
                if (splitdata.at(0) == "expires") {
                    std::string datestring = splitdata.at(1);

                    std::tm time = {};
                    std::istringstream ss(datestring);

                    ss >> std::get_time(&time, "%a, %d-%b-%Y %H:%M:%S %Z");

                    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&time));
                    auto duration = tp.time_since_epoch();

                    expires = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
                }
                if (splitdata.at(0) == "path") {
                    path = splitdata.at(1);
                }
                if (splitdata.at(0) == "domain") {
                    domain = splitdata.at(1);
                }
            }
        }
        if (t == "Secure") {
            secure = true;
        }
        else if (t == "HttpOnly") {
            httpOnly = true;
        }
    }
}

bool Cookie::isValidSetCookie(const std::string& cookie) {
    std::regex pattern("^[^=]+=[^;]+;.*$");
    return std::regex_match(cookie, pattern);
}

std::string Cookie::toString() {
    return name + "=" + value;
}


bool CookieJar::isCookieForDomain(const Cookie& cookie, const std::string& url) {
    std::vector<std::string> domainSplit = splitString(cookie.domain, ".");

    if (domainSplit.size() > 1) {
        std::string domain = domainSplit.rbegin()[1] + "." + domainSplit.rbegin()[0];
        std::string currentUrlWihoutProtocol = splitString(url, "://").at(1);
        std::string currentUrl = splitString(currentUrlWihoutProtocol, "/").at(0);

        
        if (currentUrlWihoutProtocol.back() != '/') {
            currentUrlWihoutProtocol += "/";
        }

        int temp1 = countSubstring(currentUrl, ".");
        int temp2 = countSubstring(cookie.domain, ".");

        if (domain == getDomain(url) && temp1 == temp2 && currentUrlWihoutProtocol.find(cookie.path) != std::string::npos) {
            return url.find(cookie.domain) != std::string::npos;
        }
    }

    return false;
}

void CookieJar::addCookie(const Cookie& cookie) {
    auto it = std::find_if(cookies.begin(), cookies.end(), [&](const Cookie& c) {
        return c.name == cookie.name;
    });

    if (it != cookies.end()) {
        *it = cookie;
        return;
    }

    cookies.push_back(cookie);
}

std::string CookieJar::getCookiesForUrl(const std::string& url) {
    std::vector<Cookie> cookiesForUrl;
    std::vector<std::string> result;

    if (!isValidUrl(url)) {
        return "";
    }

    for (const auto& cookie : cookies) {
        if (isCookieForDomain(cookie, url)) {
            cookiesForUrl.push_back(cookie);
        }
    }

    for (const auto& cookie : cookiesForUrl) {
        if (isCookieForDomain(cookie, url)) {
            result.push_back(cookie.name + "=" + cookie.value);
        }
    }

    return join(result, "; ");
}

std::string CookieJar::toString() {
    std::vector<std::string> temp;
    for (const auto& it : cookies) {
        temp.push_back(it.name + "=" + it.value);
    }

    std::string cookiesString = join(temp, "; ");
    return cookiesString;
}

std::string CookieJar::toBasString() {
    nlohmann::json j = nlohmann::json::parse(R"(
        {
            "cookies": []
        }
    )");

    for (const auto& cookie : cookies) {
        j["cookies"].push_back({ 
            {"domain", cookie.domain},
            {"expires", cookie.expires},
            {"httpOnly",cookie.httpOnly},
            {"name",cookie.name},
            {"path",cookie.path},
            {"secure",cookie.secure},
            {"value",cookie.value}
        });
    }

    return j.dump();
}

bool CookieJar::isValidUrl(const std::string& url) {
    std::regex url_regex("^(https?|ftp)://[a-zA-Z0-9]+(\\.[a-zA-Z]{2,})+(/[a-zA-Z0-9#]+?)*$");

    if (std::regex_match(url, url_regex)) {
        return true;
    }

    return false;
}

size_t CookieJar::length() {
    return cookies.size();
}

std::string CookieJar::getDomain(const std::string& currentUrl) {
    std::string url = currentUrl;

    std::string delimiter = ".";
    size_t pos = 0;
    std::string token;
    std::vector<std::string> tokens;
    std::string resultString = "";

    while ((pos = url.find(delimiter)) != std::string::npos) {
        token = url.substr(0, pos);
        tokens.push_back(token);
        url.erase(0, pos + delimiter.length());
    }
    tokens.push_back(url);

    if (tokens.size() >= 2) {
        resultString = tokens[tokens.size() - 2] + "." + tokens[tokens.size() - 1];
    }

    if (resultString.find("://") != std::string::npos) {
        resultString = splitString(resultString, "://").at(1);
    }

    return resultString;
}

void CookieJar::clear() {
    cookies.clear();
}

//int main()
//{
//    std::string cookie1 = "NID=511=cbIvahRL_xsfdgMQdTYCe7UVrKtbYGC4UDSxTcylQxMACi-Xy8kwwjbQFSYFZfKYwE00BbVj6eLxNagOaDRBir2kD-F4_eLjNQTaYw6B4QiUOGl9MDQc2py11BXmKXZoS7wNw24psZuY10SLgbj_NCJmOYcdwT-OmpnDm2GU90o; expires=Thu, 01-Aug-2024 13:58:50 GMT; path=/; domain=.google.com; Secure; HttpOnly; SameSite=none";
//    std::string cookie2 = "1P_JAR=2024-01-31-13; expires=Fri, 01-Mar-2024 13:58:51 GMT; path=/; domain=google.com; Secure; SameSite=none";
//    std::string cookie3 = "AEC=Ae3NU9PlTefIUgOhIrXaW-EW21sx7NUj2EWoQmaiVRvLkNBp72Aln2CPTtM; expires=Mon, 29-Jul-2024 13:58:51 GMT; path=/; domain=.google.com; Secure; HttpOnly; SameSite=lax";
//
//    CookieJar jar;
//    jar.addCookie(Cookie(cookie1));
//    jar.addCookie(Cookie(cookie2));
//    jar.addCookie(Cookie(cookie3));
//
//    std::vector<Cookie> cookies = jar.getCookiesForUrl("https://www.google.com");
//
//    for (auto& cookie : cookies) {
//        std::cout << cookie.toString() << "\n" << std::endl;
//    }
//}
