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
#include "post.h"
#include <fstream>

struct HttpClient {
	std::string lastResponse;
	std::string threadSession;
	std::string clientIdentifier = "chrome_120";
	std::string lastHeadersStr = "{}";
	std::map<std::string, std::string> headers;
	std::string proxy;
	int status;
	int timeout = 30;
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
		client->headers["accept"] = "*/*";
		client->headers["accept-encoding"] = "gzip, deflate, br";
		client->headers["connection"] = "keep-alive";
		client->headers["user-agent"] = "tls-client/1.0";

		return static_cast<void*>(client);
	}

	void EndThread(void* ThreadData) {
		HttpClient* client = static_cast<HttpClient*>(ThreadData);

		std::string tlsClientDll = searchForTlsClientDll(std::filesystem::current_path());
		nlohmann::json requestJson = nlohmann::json::object();

		requestJson["sessionId"] = client->threadSession;

		if (!tlsClientDll.empty()) {
			HINSTANCE loadLibrary = LoadLibraryA(tlsClientDll.c_str());

			if (loadLibrary != NULL) {
				DoRequest destroySession = reinterpret_cast<DoRequest>(GetProcAddress(loadLibrary, "destroySession"));

				destroySession(const_cast<char*>(requestJson.dump().c_str()));
			}
		}

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
			client->clientIdentifier = "chrome_120";
			client->lastResponse = "";
			client->lastHeadersStr = "{}";
			client->headers.clear();
			client->proxy = "";

			requestJson["sessionId"] = client->threadSession;

			DoRequest destroySession = reinterpret_cast<DoRequest>(GetProcAddress(library, "destroySession"));
			destroySession(const_cast<char*>(requestJson.dump().c_str()));

			client->threadSession = getUuid();

			client->headers["accept"] = "*/*";
			client->headers["accept-encoding"] = "gzip, deflate, br";
			client->headers["connection"] = "keep-alive";
			client->headers["user-agent"] = "tls-client/1.0";
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
					std::string tempData = lastHeaders[inputJson].dump();

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
		HINSTANCE library = reinterpret_cast<HINSTANCE>(DllData);
		HttpClient* client = static_cast<HttpClient*>(ThreadData);

		nlohmann::json requestJson = nlohmann::json::parse(R"({
			"sessionId": "",
			"url": ""
		})");

		requestJson["sessionId"] = client->threadSession;
		requestJson["url"] = InputJson;

		DoRequest getCookiesFromSession = reinterpret_cast<DoRequest>(GetProcAddress(library, "getCookiesFromSession"));
		char* result = getCookiesFromSession(const_cast<char*>(requestJson.dump().c_str()));
	

		char* ResMemory = AllocateSpace(static_cast<int>(strlen(result)), AllocateData);
		memcpy(ResMemory, result, strlen(result));
	}

	void LoadCookies(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError) {
		HINSTANCE library = reinterpret_cast<HINSTANCE>(DllData);
		HttpClient* client = static_cast<HttpClient*>(ThreadData);

		nlohmann::json inputJson = nlohmann::json::parse(InputJson, nullptr, false);

		if (inputJson.is_discarded()) {
			return;
		}

		nlohmann::json requestJson = nlohmann::json::object();

		requestJson["sessionId"] = client->threadSession;
		
		if (inputJson.find("url") != inputJson.end()) {
			requestJson["url"] = inputJson["url"];
		}
		else {
			return;
		}
		
		if (inputJson.find("cookies") != inputJson.end()) {

			if (inputJson["cookies"].is_string()) {
				nlohmann::json cookiesList = nlohmann::json::parse(inputJson["cookies"].get<std::string>(), nullptr, false);

				if (cookiesList.is_discarded()) {
					return;
				}

				if (cookiesList.find("cookies") != cookiesList.end()) {

					if (cookiesList["cookies"].is_array()) {
						for (auto& cookie : cookiesList["cookies"]) {
							if (cookie.find("expires") != cookie.end()) {
								if (cookie["expires"].is_string()) {
									std::string expiresString = cookie["expires"];
									std::time_t expiresTime = parseDateTime(expiresString);

									cookie["expires"] = expiresTime;
								}
							}
						}

						requestJson["cookies"] = cookiesList["cookies"];
					}
				}
			}
		}
		else {
			return;
		}

		DoRequest addCookiesToSession = reinterpret_cast<DoRequest>(GetProcAddress(library, "addCookiesToSession"));
		char* result = addCookiesToSession(const_cast<char*>(requestJson.dump().c_str()));

		char* ResMemory = AllocateSpace(static_cast<int>(strlen(result)), AllocateData);
		memcpy(ResMemory, result, strlen(result));
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
					"withRandomTLSExtensionOrder": true,
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
			requestJson["isByteResponse"] = true;
			requestJson["isByteRequest"] = true;
			requestJson["followRedirects"] = inputJson["redirect"].get<bool>();
			std::string headersStr = inputJson["headers"].get<std::string>() + "\r\n";

			nlohmann::json& requestHeaders = requestJson["headers"];

			std::vector<std::string> splitHeaders = split(headersStr, "\r\n");

			for (auto it = client->headers.begin(); it != client->headers.end(); ++it) {
				requestHeaders[it->first] = it->second;
			}

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


			nlohmann::json requestJsonCookie = nlohmann::json::parse(R"({
				"sessionId": "",
				"url": ""
			})");

			requestJsonCookie["sessionId"] = client->threadSession;
			requestJsonCookie["url"] = currentUrl;

			DoRequest getCookiesFromSession = reinterpret_cast<DoRequest>(GetProcAddress(library, "getCookiesFromSession"));
			char* result = getCookiesFromSession(const_cast<char*>(requestJsonCookie.dump().c_str()));

			nlohmann::json cookiesForUrl = nlohmann::json::parse(result);

			if (cookiesForUrl.find("cookies") != cookiesForUrl.end()) {
				if (cookiesForUrl["cookies"].is_array()) {
					nlohmann::json& cookiesForUrlList = cookiesForUrl["cookies"];
					std::string cookiesStr = "";

					for (const auto& it : cookiesForUrlList) {
						cookiesStr += it["name"].get<std::string>() + "=" + it["value"].get<std::string>() + "; ";
					}

					if (cookiesStr.size() != 0) {
						cookiesStr = cookiesStr.substr(0, cookiesStr.length() - 2);;
						requestHeaders["cookie"] = cookiesStr;
					}

				}
			}
			
			if (requestHeaders.find("cookie") != requestHeaders.end()) {
				if (requestHeaders["cookie"].size() == 0) {
					requestHeaders.erase("cookie");
				}
			}

			// Generate the Body for the request
			if (method == "POST" || method == "PUT" || method == "PATCH") {
				if (isConstructor) {
					std::map<std::string, std::string> payload = parsePayload(payloadConstructor);
					std::string result = "";

					// UrlEncoded
					if (contentTypeConstructor == "urlencode") {
						std::string urlencodeBody = createUrlEncoded(payload);
						std::vector<BYTE> bodyByte(urlencodeBody.begin(), urlencodeBody.end());

						urlencodeBody = base64_encode(bodyByte.data(), bodyByte.size());

						requestJson["requestBody"] = urlencodeBody;
						requestHeaders["content-type"] = "application/x-www-form-urlencoded";
					}
					// Json
					if (contentTypeConstructor == "json") {
						std::string jsonBody = createJsonBody(payload);
						std::vector<BYTE> bodyByte(jsonBody.begin(), jsonBody.end());

						jsonBody = base64_encode(bodyByte.data(), bodyByte.size());

						requestJson["requestBody"] = jsonBody;
						requestHeaders["content-type"] = "application/json";
					}
					// Multipart/Form-Data
					if (contentTypeConstructor == "multipart") {
						std::vector<std::string> multipartData = generateMultipartFormData(payload);
						std::string multipartBody = multipartData.at(1);

						std::vector<BYTE> bodyByte(multipartBody.begin(), multipartBody.end());

						multipartBody = base64_encode(bodyByte.data(), bodyByte.size());
						
						requestJson["requestBody"] = multipartBody;
						requestHeaders["content-type"] = "multipart/form-data; boundary=" + multipartData.at(0);
					}
				}
				else {
					std::vector<BYTE> bodyByte(payloadRaw.begin(), payloadRaw.end());
					payloadRaw = base64_encode(bodyByte.data(), bodyByte.size());

					requestJson["requestBody"] = payloadRaw;
					requestHeaders["content-type"] = contentTypeRaw;
				}
			}


			std::string updatedJsonString = requestJson.dump();

			DoRequest req = reinterpret_cast<DoRequest>(GetProcAddress(library, "request"));

			char* res = req(updatedJsonString.data());

			std::string responseStr = res;
			std::string responseId = responseStr.substr(7, 36);

			nlohmann::json currentResJson = nlohmann::json::parse(responseStr);

			client->status = currentResJson["status"].get<int>();
			if (currentResJson["headers"].is_object()) {
				client->lastHeadersStr = currentResJson["headers"].dump();
			}
			else {
				client->lastHeadersStr = "{}";
			}

			// Add the Response to the container
			addHttpData(requestJson.dump().data(), res);

			std::string response = res;
			client->lastResponse = response;

			char* ResMemory = AllocateSpace(static_cast<int>(strlen(res)), AllocateData);
			memcpy(ResMemory, res, strlen(res));

			// Free Memory
			DoRequest freeMemory = reinterpret_cast<DoRequest>(GetProcAddress(library, "freeMemory"));
			freeMemory(const_cast<char*>(responseId.c_str()));

			// Download
			if (inputJson.find("downloadPath") != inputJson.end()) {
				std::string downloadPath = inputJson["downloadPath"].get<std::string>();

				int wchars_num = MultiByteToWideChar(CP_UTF8, 0, downloadPath.c_str(), -1, NULL, 0);
				wchar_t* wstr = new wchar_t[wchars_num];
				MultiByteToWideChar(CP_UTF8, 0, downloadPath.c_str(), -1, wstr, wchars_num);

				std::filesystem::path filePath(wstr);

				if (std::filesystem::exists(filePath.parent_path())) {
					std::string responseBody;


					if (currentResJson.find("body") != currentResJson.end()) {
						responseBody = currentResJson["body"].get<std::string>();
					}

					if (responseBody.find(";base64,") != std::string::npos) {
						std::string base64Body = splitString(responseBody, ";base64,").at(1);
						std::vector<BYTE> decodeBase64Body = base64_decode(base64Body);

						responseBody = std::string(decodeBase64Body.begin(), decodeBase64Body.end());
					}

					std::ofstream file(filePath);
					if (file.is_open()) {
						file << responseBody;
					}
					file.close();

				}

				delete[] wstr;
			}
		}
	}

	void GetResponse(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError) {
		HttpClient* client = static_cast<HttpClient*>(ThreadData);
		HINSTANCE library = reinterpret_cast<HINSTANCE>(DllData);

		if (library != NULL) {
			if (client->lastResponse.size() != 0) {
				nlohmann::json resJson = nlohmann::json::parse(client->lastResponse);
				std::string inputJson = InputJson;
				std::string body;

				if (resJson.find("body") != resJson.end()) {
					body = resJson["body"].get<std::string>();

					if (inputJson == "true") {
						if (body.find(";base64,") != std::string::npos) {

							std::string base64Body = splitString(body, ";base64,").at(1);

							std::vector<BYTE> decodeBase64Body = base64_decode(base64Body);

							body = std::string(decodeBase64Body.begin(), decodeBase64Body.end());
						}
					}
				}

				char* res = body.data();
				char* resMemory = AllocateSpace(static_cast<int>(strlen(res)), AllocateData);
				memcpy(resMemory, res, strlen(res));
			}
			else {
				const char* res = "null";

				char* resMemory = AllocateSpace(static_cast<int>(strlen(res)), AllocateData);
				memcpy(resMemory, res, strlen(res));
			}

		}
	}
}