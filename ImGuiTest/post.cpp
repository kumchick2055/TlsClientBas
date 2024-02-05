#include "post.h"

std::string checkBase64OrFile(const std::string& input) {
	std::string result = input;
	if (input.find("base64://") != std::string::npos) {
		std::string payloadBase64 = splitString(input, "base64://").at(1);
		std::vector<BYTE> decodeResult = base64_decode(payloadBase64);
		result = std::string(decodeResult.begin(), decodeResult.end());
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
