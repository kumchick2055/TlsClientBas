#pragma execution_character_set("utf-8")
#include "post.h"
#include <filesystem>
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

		std::ifstream iff(wstr, std::ios::binary);
		std::string line;
		std::string content;

		if (!iff.bad()) {
			if (iff.is_open()) {
				std::vector<char> buffer((std::istreambuf_iterator<char>(iff)), std::istreambuf_iterator<char>());
				std::string content(buffer.begin(), buffer.end());
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
	nlohmann::json jsonResult = nlohmann::json::object();

	for (auto it = payload.begin(); it != payload.end(); ++it) {
		std::string first = it->first;
		std::string second = it->second;

		second = checkBase64OrFile(second);

		jsonResult[first] = second;
	}

	result = jsonResult.dump();

	return result;
}


std::vector<std::string> generateMultipartFormData(const std::map<std::string, std::string>& payload) {
	std::string boundaryPrefix = "---------------------------";
	std::string randChars = "0123456789";

	std::string boundary = boundaryPrefix;

	for (int i = 0; i < 29; ++i) {
		char randChar = getRandomChar(randChars);
		std::string charToString(1, randChar);

		boundary += charToString;
	}

	std::string boundaryContent;

	for (auto it = payload.begin(); it != payload.end(); ++it) {
		std::string first = it->first;
		std::string second = it->second;

		if (second.find("file://") != std::string::npos) {
			std::string filePathRaw = splitString(second, "file://").at(1);

			int wchars_num = MultiByteToWideChar(CP_UTF8, 0, filePathRaw.c_str(), -1, NULL, 0);
			wchar_t* wstr = new wchar_t[wchars_num];
			MultiByteToWideChar(CP_UTF8, 0, filePathRaw.c_str(), -1, wstr, wchars_num);

			std::filesystem::path filePath = wstr;

			if (!std::filesystem::exists(filePath)) {
				continue;
			}

			std::string contentType = "application/octet-stream";

			auto fileExt = filePath.extension();
			std::string fileName = filePath.filename().string();

			if (fileExt == ".jpg" || fileExt == ".jpeg") {
				contentType = "image/jpg";
			}
			if (fileExt == ".png") {
				contentType = "image/png";
			}
			if (fileExt == ".pdf") {
				contentType = "application/pdf";
			}
			if (fileExt == ".txt" || fileExt == ".log") {
				contentType = "text/plain";
			}
			
			boundaryContent += "--" + boundary + "\n";
			boundaryContent += "Content-Disposition: form-data; name=\"" + first + "\"; filename=\"" + fileName + "\"\nContent-Type: " + contentType + "\n\n";
			boundaryContent += checkBase64OrFile(second) + "\n";

			delete[] wstr;
		}
		else {
			boundaryContent += "--" + boundary + "\n";
			boundaryContent += "Content-Disposition: form-data; name=\"" + first + "\"\n\n";
			boundaryContent += checkBase64OrFile(second) + "\n";
		}
	}

	boundaryContent += "--" + boundary + "--\n";

	std::vector<std::string> returnData;
	returnData.push_back(boundary);
	returnData.push_back(boundaryContent);

	return returnData;
}