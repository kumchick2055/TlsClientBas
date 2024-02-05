#include "utils.h"

#include <filesystem>
#include <iostream>
#include <random>
#include <sstream>
#include "base64.h"
#include "vendor/nlohmann/json.hpp"


std::string searchForTlsClientDll(const std::filesystem::path& dir) {
	for (const auto& entry : std::filesystem::directory_iterator(dir)) {
		if (entry.is_directory()) {
			std::string result = searchForTlsClientDll(entry.path());
			if (!result.empty()) {
				return result;
			}
		}
		else if (entry.path().filename() == "tlsclient.dll") {
			return entry.path().string();
		}
	}
	return "";
}

std::string getUuid() {
	static std::random_device dev;
	static std::mt19937 rng(dev());

	std::uniform_int_distribution<int> dist(0, 15);

	const char* v = "0123456789abcdef";
	const bool dash[] = { 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0 };

	std::string res;
	for (int i = 0; i < 16; i++) {
		if (dash[i]) res += "-";
		res += v[dist(rng)];
		res += v[dist(rng)];
	}

	return res;
}

std::string updateJsonString(const std::string& jsonString, const std::string& key, const std::string& value) {
	nlohmann::json document = nlohmann::json::parse(jsonString);

	const char* keyPtr = key.c_str();
	const char* valuePtr = value.c_str();

	document[keyPtr] = valuePtr;

	return document.dump();
}

std::vector<std::string> split(std::string s, std::string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

std::vector<std::string> splitString(std::string data, std::string delimiter) {
	std::size_t pos = 0;
	std::vector<std::string> tokens;

	while ((pos = data.find(delimiter)) != std::string::npos) {
		std::string token = data.substr(0, pos);
		tokens.push_back(token);
		data.erase(0, pos + delimiter.length());
	}
	tokens.push_back(data);

	return tokens;
}

std::string join(const std::vector<std::string>& vec, const std::string& delimiter) {
	std::stringstream ss;
	for (size_t i = 0; i < vec.size(); ++i) {
		if (i > 0) {
			ss << delimiter;
		}
		ss << vec[i];
	}
	return ss.str();
}

int countSubstring(const std::string& str, const std::string& sub)
{
	if (sub.length() == 0) return 0;
	int count = 0;
	for (size_t offset = str.find(sub); offset != std::string::npos;
		offset = str.find(sub, offset + sub.length()))
	{
		++count;
	}
	return count;
}


long long currentTime() {
	auto secondsUTC = std::chrono::duration_cast<std::chrono::seconds>(
		std::chrono::system_clock::now().time_since_epoch()
		).count();

	return secondsUTC;
}

std::string getValueOrDefault(const nlohmann::json& doc, const std::string& key, const std::string& defaultValue) {
	if (doc.find(key) != doc.end()) {
		return doc[key].get<std::string>();
	}
	return defaultValue;
}

std::string urlEncode(const std::string& value) {
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;

	for (char c : value) {
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
			escaped << c;
		}
		else {
			escaped << std::uppercase;
			escaped << '%' << std::setw(2) << int((unsigned char)c);
			escaped << std::nouppercase;
		}
	}

	return escaped.str();
}

std::map<std::string, std::string> parsePayload(const std::string& payload) {
	std::map<std::string, std::string> result;
	nlohmann::json j = nlohmann::json::parse(payload);

	if (j.size() % 2 == 0) {
		auto it = j.begin();
		while (it != j.end()) {
			if (it->is_string() && std::next(it) != j.end() && std::next(it)->is_string()) {
				result[it->get<std::string>()] = std::next(it)->get<std::string>();
				it += 2;
			}
			else {
				return result;
			}
		}
	}

	return result;
}

std::vector<BYTE> string_to_byte_vector(const std::string& str) {
	std::vector<BYTE> byteVector(str.begin(), str.end());
	return byteVector;
}
