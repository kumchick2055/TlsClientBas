#ifndef _UTILS_H_
#define _UTILS_H_

#include <filesystem>
#include <iostream>
#include <random>
#include <sstream>
#include <regex>
#include "base64.h"
#include "vendor/nlohmann/json.hpp"

std::string searchForTlsClientDll(const std::filesystem::path& dir);
std::string getUuid();
std::string updateJsonString(const std::string& jsonString, const std::string& key, const std::string& value);
std::vector<std::string> split(std::string s, std::string delimiter);
std::vector<std::string> splitString(std::string data, std::string delimiter);
std::string join(const std::vector<std::string>& vec, const std::string& delimiter);
int countSubstring(const std::string& str, const std::string& sub);
long long currentTime();
std::string getValueOrDefault(const nlohmann::json& doc, const std::string& key, const std::string& defaultValue);
std::string urlEncode(const std::string& value);
std::map<std::string, std::string> parsePayload(const std::string& payload);

#endif
