#include "ops.h"
#include <iostream>
#include <openvr.h>
#include <charconv>
#include <regex>

static std::regex reInt("^\d+$");
static std::regex reFloat("^\d+\.\d+$");
static std::regex reBool("^(?:true|false)$");

void set(vr::IVRSettings* settings, std::vector<std::string> args) {
	if (args.size() != 3) {
		// TODO: throw an exception
		std::cerr << "Usage: set SECTION SUB_SECTION VALUE" << std::endl;
		return;
	}

	std::string section = args[0];
	std::string subSection = args[1];
	std::string value = args[2];

	if (std::regex_match(value, reInt)) {
		settings->SetInt32(section.c_str(), subSection.c_str(), std::stoi(value));
		return;
	} else if (std::regex_match(value, reFloat)) {
		settings->SetFloat(section.c_str(), subSection.c_str(), std::stof(value));
		return;
	} else if (value == "true") {
		settings->SetBool(section.c_str(), subSection.c_str(), true);
		return;
	} else if (value == "false") {
		settings->SetBool(section.c_str(), subSection.c_str(), false);
		return;
	}

	settings->SetString(section.c_str(), subSection.c_str(), value.c_str());
};


void get(vr::IVRSettings* settings, std::vector<std::string> args) {
	if (args.size() != 2) {
		std::cerr << "Usage: get SECTION SUB_SECTION" << std::endl;
		return;
	}

	std::string section = args[0];
	std::string subSection = args[1];

	char buffer[1024];
	vr::EVRSettingsError error = vr::VRSettingsError_None;

	settings->GetString(section.c_str(), subSection.c_str(), buffer, std::size(buffer), &error);

	if (error != vr::VRSettingsError_None) {
		std::cerr << "Error getting setting: " << settings->GetSettingsErrorNameFromEnum(error) << std::endl;
		return;
	}

	std::cout << buffer << std::endl;
}

void del(vr::IVRSettings* settings, std::vector<std::string> args) {
	if (args.size() < 1) {
		std::cerr << "Usage: del SECTION SUB_SECTION" << std::endl;
		return;
	}

	vr::EVRSettingsError error = vr::VRSettingsError_None;
	std::string section = args[0];

	if (args.size() == 1) {
		settings->RemoveSection(section.c_str(), &error);
	} else if (args.size() == 2) {
		std::string subSection = args[1];
		settings->RemoveKeyInSection(section.c_str(), subSection.c_str(), &error);
	} else if (args.size() > 2) {
		std::cerr << "Usage: del SECTION SUB_SECTION" << std::endl;
	}

	if (error != vr::VRSettingsError_None) {
		std::cerr << "Error deleting setting: " << settings->GetSettingsErrorNameFromEnum(error) << std::endl;
	}
}

void add(vr::IVRSettings* settings, std::vector<std::string> args) {
	if (args.size() != 3) {
		std::cerr << "Usage: add SECTION SUB_SECTION VALUE" << std::endl;
		return;
	}

	std::string section = args[0];
	std::string subSection = args[1];
	std::string value = args[2];
	vr::EVRSettingsError error = vr::VRSettingsError_None;

	if (std::regex_match(value, reInt)) {
		int i = 0;
		settings->GetInt32(section.c_str(), subSection.c_str(), &error);
		if (error != vr::VRSettingsError_None) goto end;
		settings->SetInt32(section.c_str(), subSection.c_str(), i + std::stoi(value), &error);
	} else if (std::regex_match(value, reFloat)) {
		float f = 0.0;
		settings->GetFloat(section.c_str(), subSection.c_str(), &error);
		if (error != vr::VRSettingsError_None) goto end;
		settings->SetFloat(section.c_str(), subSection.c_str(), f + std::stof(value), &error);
	} else {
		std::cerr << "Value " << value << " is not a number" << std::endl;
		return;
	}

end:
	if (error != vr::VRSettingsError_None) {
		std::cerr << "Error updating setting: " << settings->GetSettingsErrorNameFromEnum(error) << std::endl;
	}
}

void neg(vr::IVRSettings* settings, std::vector<std::string> args) {
	if (args.size() != 2) {
		std::cerr << "Usage: neg SECTION SUB_SECTION" << std::endl;
		return;
	}

	std::string section = args[0];
	std::string subSection = args[1];
	vr::EVRSettingsError error = vr::VRSettingsError_None;

	bool value = settings->GetBool(section.c_str(), subSection.c_str(), &error);

	if (error != vr::VRSettingsError_None) {
		std::cerr << "Error getting setting: " << settings->GetSettingsErrorNameFromEnum(error) << std::endl;
		return;
	}

	settings->SetBool(section.c_str(), subSection.c_str(), !value, &error);
}


OPMap makeOpMap() {
	return {
		{"set", set},
		{"get", get},
		{"del", del},
		{"add", add},
		{"neg", neg}
	};
}