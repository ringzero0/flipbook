//
// Created by Praveen P on 23-11-2025.
//

#ifndef FLIPBOOK_PROMPTREQ_H
#define FLIPBOOK_PROMPTREQ_H
#include <string>

std::string json_escape(const std::string& s);
std::string strip_markdown(std::string text);
std::string extract_solution(const std::string& json_response);
std::string prompt(std::string user_request);

#endif //FLIPBOOK_PROMPTREQ_H