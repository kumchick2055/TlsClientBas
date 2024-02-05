#include <filesystem>
#include <iostream>
#include <random>
#include <sstream>
#include "main.h"
#include <map>
#include "gui.h"
#include <regex>
#include <locale>
#include "utils.h"
#include "cookie.h"
#include "post.h"
#include <fstream>

struct HttpClient {
	std::string lastResponse;
	std::string threadSession;
	std::string clientIdentifier = "chrome_103";
	std::string lastHeadersStr = "{}";
	std::map<std::string, std::string> headers;
	std::string proxy;
	int status;
	int timeout = 30;
	CookieJar jar;
};

typedef void (*ClearAll)();

extern "C" {

	void* StartDll() {                                                        
		std::string tlsClientDll = searchForTlsClientDll(std::filesystem::current_path());

		if (!tlsClientDll.empty()) {
			HINSTANCE loadLibrary = LoadLibraryA(tlsClientDll.c_str());

			if (loadLibrary != NULL) {
				return loadLibrary;
			}
		}

		return 0;
	}

	void EndDll(void* DllData) {
		stopGui();

		HINSTANCE library = reinterpret_cast<HINSTANCE>(DllData);

		if (library != NULL) {
			ClearAll destroySession = reinterpret_cast<ClearAll>(GetProcAddress(library, "destroyAll"));
			destroySession();
		}

	}

	void* StartThread() {
		HttpClient* client = new HttpClient;
		client->threadSession = getUuid();

		return static_cast<void*>(client);
	}

	void EndThread(void* ThreadData) {
		HttpClient* client = static_cast<HttpClient*>(ThreadData);
		delete client;
	}

	void OpenSniffer(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError) {
		startGui();
	}

	void SetFingerprint(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError) {
		HttpClient* client = static_cast<HttpClient*>(ThreadData);

		client->clientIdentifier = InputJson;
	}

	void SetTimeout(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError) {
		HttpClient* client = static_cast<HttpClient*>(ThreadData);

		std::string cppString(InputJson);
		std::istringstream iss(cppString);

		int number;
		if (iss >> number) {
			if (number > 0) {
				client->timeout = number;
			}
		}
	}

	void Reset(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError) {
		HINSTANCE library = reinterpret_cast<HINSTANCE>(DllData);

		HttpClient* client = static_cast<HttpClient*>(ThreadData);

		if (library != NULL) {
			nlohmann::json requestJson = nlohmann::json::parse(R"(
				{
					"sessionId": ""
				}
			)");

			client->timeout = 30;
			client->clientIdentifier = "chrome_103";
			client->lastResponse = "";
			client->lastHeadersStr = "{}";
			client->headers.clear();
			client->proxy = "";
			client->jar.clear();

			requestJson["sessionId"] = client->threadSession;

			DoRequest destroySession = reinterpret_cast<DoRequest>(GetProcAddress(library, "destroySession"));
			destroySession(const_cast<char*>(requestJson.dump().c_str()));
	}
	}


	void GetHeaders(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError) {
		HttpClient* client = static_cast<HttpClient*>(ThreadData);
		std::string inputJson = InputJson;

		nlohmann::json lastHeaders = nlohmann::json::parse(client->lastHeadersStr);

		if (lastHeaders.is_object()) {
			if (inputJson == "all") {
				std::string tempData = lastHeaders.dump();
				
				const char* allHeaders = tempData.c_str();

				char* ResMemory = AllocateSpace(static_cast<int>(strlen(allHeaders)), AllocateData);
				memcpy(ResMemory, allHeaders, strlen(allHeaders));
			}
			else {
				if (lastHeaders.find(inputJson) != lastHeaders.end()) {
					std::string tempData = lastHeaders[inputJson][0].get<std::string>();

					const char* currentHeader = tempData.c_str();

					char* ResMemory = AllocateSpace(static_cast<int>(strlen(currentHeader)), AllocateData);
					memcpy(ResMemory, currentHeader, strlen(currentHeader));
				}
			}
		}

	}

	void GetStatus(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError) {
		HttpClient* client = static_cast<HttpClient*>(ThreadData);

		std::string statusStr = std::to_string(client->status);
		const char* statusChar = statusStr.c_str();

		char* ResMemory = AllocateSpace(static_cast<int>(strlen(statusChar)), AllocateData);
		memcpy(ResMemory, statusChar, strlen(statusChar));
	}

	void SetHeader(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError) {
		HttpClient* client = static_cast<HttpClient*>(ThreadData);
		nlohmann::json inputJson = nlohmann::json::parse(InputJson);

		std::string key = "";
		std::string value = "";

		if (inputJson.find("key") != inputJson.end()) {
			if (inputJson.find("value") != inputJson.end()) {
				key = inputJson["key"].get<std::string>();
				value = inputJson["value"].get<std::string>();

				std::locale loc;

				for (auto& c : key)
					c = std::tolower(c, loc);

				client->headers[key] = value;
			}
		}
	}

	void ClearHeaders(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError) {
		HttpClient* client = static_cast<HttpClient*>(ThreadData);

		client->headers.clear();
	}

	void SetProxy(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError) {
		HttpClient* client = static_cast<HttpClient*>(ThreadData);

		nlohmann::json inputJson = nlohmann::json::parse(InputJson);

		if (inputJson.is_object()) {
			std::string proxyText = getValueOrDefault(inputJson, "proxyText", "");
			std::string proxyType = getValueOrDefault(inputJson, "proxyType", "");
			std::string proxyLogin = getValueOrDefault(inputJson, "proxyLogin", "");
			std::string proxyPassword = getValueOrDefault(inputJson, "proxyPassword", "");

			if (proxyText.empty() || proxyType.empty()) {
				return;
			}

			if (!proxyLogin.empty() && !proxyPassword.empty()) {
				client->proxy = proxyType + "://" + proxyLogin + ":" + proxyPassword + "@" + proxyText;
			}
			else {
				client->proxy = proxyType + "://" + proxyText;
			}

		}
	}

	void SaveCookies(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError) {
		HttpClient* client = static_cast<HttpClient*>(ThreadData);
		
		std::string cookiesJson = client->jar.toBasString();
		const char* tmp = cookiesJson.c_str();

		char* ResMemory = AllocateSpace(static_cast<int>(strlen(tmp)), AllocateData);
		memcpy(ResMemory, tmp, strlen(tmp));
	}

	void LoadCookies(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError) {
		HttpClient* client = static_cast<HttpClient*>(ThreadData);

		nlohmann::json cookiesJson = nlohmann::json::parse(InputJson);

		//client->jar.clear();

		if (cookiesJson.find("cookies") != cookiesJson.end()) {
			if (cookiesJson["cookies"].is_array()) {
				for (const auto& cookieData : cookiesJson["cookies"]) {
					Cookie cookie("");

					if (cookieData.find("domain") == cookieData.end() ||
						cookieData.find("expires") == cookieData.end() ||
						cookieData.find("httpOnly") == cookieData.end() ||
						cookieData.find("name") == cookieData.end() ||
						cookieData.find("path") == cookieData.end() ||
						cookieData.find("secure") == cookieData.end()) {
						continue;
					}

					cookie.domain = cookieData["domain"].get<std::string>();
					cookie.expires = cookieData["expires"].get<long long>();
					cookie.httpOnly = cookieData["httpOnly"].get<bool>();
					cookie.name = cookieData["name"].get<std::string>();
					cookie.path = cookieData["path"].get<std::string>();
					cookie.secure = cookieData["secure"].get<bool>();
					cookie.value = cookieData["value"].get<std::string>();

					client->jar.addCookie(cookie);
				}
			}
		}
	}

	void Request(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError) {
		HttpClient* client = static_cast<HttpClient*>(ThreadData);
		HINSTANCE library = reinterpret_cast<HINSTANCE>(DllData);


		if (library != NULL) {
			nlohmann::json requestJson = nlohmann::json::parse(R"(
				{
					"tlsClientIdentifier": "",
					"followRedirects": false,
					"insecureSkipVerify": false,
					"withoutCookieJar": false,
					"withDefaultCookieJar": false,
					"forceHttp1": false,
					"withDebug": false,
					"withRandomTLSExtensionOrder": false,
					"isByteResponse": false,
					"isByteRequest": false,
					"catchPanics": false,
					"timeoutSeconds": 30,
					"timeoutMilliseconds": 0,
					"sessionId": "",
					"proxyUrl": "",
					"isRotatingProxy": false,
					"certificatePinningHosts": {},
					"headers": {

					},
					"headerOrder": [

					] ,
					"requestUrl": "",
					"requestMethod": "GET"
				}
			)");

			nlohmann::json inputJson = nlohmann::json::parse(InputJson);

			std::string currentUrl = inputJson["url"].get<std::string>();
			std::string method = inputJson["method"].get<std::string>();
			std::string payloadConstructor = "";
			bool isConstructor = false;
			std::string payloadRaw = "";
			std::string contentTypeRaw = "";
			std::string contentTypeConstructor = "";

			if(inputJson.find("payloadConstructor") != inputJson.end())
				payloadConstructor = inputJson["payloadConstructor"].get<std::string>();

			if (inputJson.find("isConstructor") != inputJson.end())
				isConstructor = inputJson["isConstructor"].get<bool>();

			if (inputJson.find("payloadRaw") != inputJson.end())
				payloadRaw = inputJson["payloadRaw"].get<std::string>();

			if (inputJson.find("contentTypeRaw") != inputJson.end())
				contentTypeRaw = inputJson["contentTypeRaw"].get<std::string>();

			if (inputJson.find("contentTypeConstructor") != inputJson.end())
				contentTypeConstructor = inputJson["contentTypeConstructor"].get<std::string>();

			requestJson["tlsClientIdentifier"] = client->clientIdentifier;
			requestJson["sessionId"] = client->threadSession;
			requestJson["timeoutSeconds"] = client->timeout;
			requestJson["proxyUrl"] = client->proxy;

			requestJson["requestUrl"] = inputJson["url"].get<std::string>();
			requestJson["requestMethod"] = method;
			requestJson["isByteResponse"] = inputJson["isByteResponse"].get<bool>();
			requestJson["followRedirects"] = inputJson["redirect"].get<bool>();
			std::string headersStr = inputJson["headers"].get<std::string>() + "\r\n";

			nlohmann::json& requestHeaders = requestJson["headers"];
			nlohmann::json& requestHeadersOrder = requestJson["headerOrder"];

			std::vector<std::string> splitHeaders = split(headersStr, "\r\n");

			std::regex pattern("([^:]+):\\s*([^\\r\\n]+)");
			std::smatch match;

			for (const auto& header : splitHeaders) {
				if (std::regex_search(header, match, pattern)) {
					std::string key = match[1].str();
					std::string value = match[2].str();
					std::locale loc;
					
					for (auto& c : key)
						c = std::tolower(c, loc);

					requestHeaders[key] = value;

				}
			}

			for (auto it = client->headers.begin(); it != client->headers.end(); ++it) {
				requestHeaders[it->first] = it->second;
			}

			if (client->jar.length() > 0) {
				requestHeaders["cookie"] = client->jar.getCookiesForUrl(currentUrl);
			}

			// Generate the Body for the request
			if (method == "POST" || method == "PUT" || method == "PATCH") {
				if (isConstructor) {
					std::map<std::string, std::string> payload = parsePayload(payloadConstructor);
					std::string result = "";

					// UrlEncoded
					if (contentTypeConstructor == "urlencoded") {
						requestJson["requestBody"] = createUrlEncoded(payload);
						requestHeaders["content-type"] = "application/x-www-form-urlencoded";
					}
					// Json
					if (contentTypeConstructor == "json") {
						requestJson["requestBody"] = createJsonBody(payload);
						requestHeaders["content-type"] = "application/json";
					}
				}
				else {
					requestJson["requestBody"] = payloadRaw;
					requestHeaders["content-type"] = contentTypeRaw;
				}
			}

			for (auto& header : requestHeaders.items()) {
				requestHeadersOrder.push_back(header.key());
			}


			std::string updatedJsonString = requestJson.dump();

			DoRequest req = reinterpret_cast<DoRequest>(GetProcAddress(library, "request"));

			char* res = req(updatedJsonString.data());

			std::string responseStr = res;
			std::string responseId = responseStr.substr(7, 36);

			nlohmann::json currentResJson = nlohmann::json::parse(responseStr);

			if (currentResJson.find("headers") != currentResJson.end()) {
				client->lastHeadersStr = currentResJson["headers"].dump();

				nlohmann::json& reqHeaders = currentResJson["headers"];
				if (reqHeaders.find("Set-Cookie") != reqHeaders.end()) {
					for (const auto& it : reqHeaders["Set-Cookie"]) {
						client->jar.addCookie(Cookie(it));
					}
				}
			}

			client->status = currentResJson["status"].get<int>();

			// Add the Response to the container
			addHttpData(requestJson.dump().data(), res);

			std::string response = res;
			client->lastResponse = response;

			char* ResMemory = AllocateSpace(static_cast<int>(strlen(res)), AllocateData);
			memcpy(ResMemory, res, strlen(res));

			// Free Memory
			DoRequest freeMemory = reinterpret_cast<DoRequest>(GetProcAddress(library, "freeMemory"));
			freeMemory(const_cast<char*>(responseId.c_str()));
		}
	}

	void GetResponse(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError) {
		HttpClient* client = static_cast<HttpClient*>(ThreadData);
		HINSTANCE library = reinterpret_cast<HINSTANCE>(DllData);

		if (library != NULL) {
			char* res = client->lastResponse.data();

			char* resMemory = AllocateSpace(static_cast<int>(strlen(res)), AllocateData);
			memcpy(resMemory, res, strlen(res));
		}
	}
}