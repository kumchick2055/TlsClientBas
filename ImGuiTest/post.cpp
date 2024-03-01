#pragma execution_character_set("utf-8")
#include "post.h"
#include <fstream>
#include <Windows.h>

std::string checkBase64OrFile(const std::string& input) {
	std::string result = input;
	if (input.find("base64://") != std::string::npos) {
		std::string payloadBase64 = splitString(input, "base64://").at(1);
		std::vector<BYTE> decodeResult = base64_decode(payloadBase64);
		result = std::string(decodeResult.begin(), decodeResult.end());
	}

	if (input.find("file://") != std::string::npos) {
		std::string filePath = splitString(input, "file://").at(1);

		int wchars_num = MultiByteToWideChar(CP_UTF8, 0, filePath.c_str(), -1, NULL, 0);
		wchar_t* wstr = new wchar_t[wchars_num];
		MultiByteToWideChar(CP_UTF8, 0, filePath.c_str(), -1, wstr, wchars_num);

		std::ifstream iff(wstr, std::ios::in);
		std::string line;
		std::string content;

		if (!iff.bad()) {
			if (iff.is_open()) {
				content = std::string(std::istreambuf_iterator<char>(iff), std::istreambuf_iterator<char>());
				result = content;
				
				iff.close();
			}
		}

		delete[] wstr;
	}
	
	return result;
}

std::string createUrlEncoded(const std::map<std::string, std::string>& payload) {
	std::vector<std::string> result;

	for (auto it = payload.begin(); it != payload.end(); ++it) {
		std::string first = urlEncode(it->first);
		std::string second = it->second;

		second = checkBase64OrFile(second);

		first = urlEncode(first);
		second = urlEncode(second);

		result.push_back(first + "=" + second);
	}

	return join(result, "&");
}

std::string createJsonBody(const std::map<std::string, std::string>& payload) {
	std::string result;
	nlohmann::json jsonResult = nlohmann::json::parse(R"({})");

	for (auto it = payload.begin(); it != payload.end(); ++it) {
		std::string first = it->first;
		std::string second = it->second;

		second = checkBase64OrFile(second);

		jsonResult[first] = second;
	}

	result = jsonResult.dump();

	return result;
}


std::string generateMultipartFormData(const std::map<std::string, std::string>& payload) {

}