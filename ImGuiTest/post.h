#ifndef _POST_H_
#define _POST_H_

#include "utils.h"


std::string createUrlEncoded(const std::map<std::string, std::string>& payload);
std::string createJsonBody(const std::map<std::string, std::string>& payload);
std::vector<std::string> generateMultipartFormData(const std::map<std::string, std::string>& payload);

#endif