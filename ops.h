#pragma once

#include <vector>
#include <unordered_map>
#include <regex>

#include <openvr.h>

typedef void (*OP)(vr::IVRSettings*, std::vector<std::string>);
typedef std::unordered_map<std::string, OP> OPMap;
OPMap makeOpMap();