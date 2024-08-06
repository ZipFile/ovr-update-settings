#include <openvr.h>
#include <iostream>
#include <string>
#include <vector>
#include "ops.h"


int main(int argc, char* argv[]) {
  OPMap opMap = makeOpMap();
  std::vector<std::string> arguments(argv + 1, argv + argc);

  if (!arguments.size()) {
    std::cerr << "Usage: " << argv[0] << " get|set|del|add|neg ARGS" << std::endl;
		return 1;
	}

  OPMap::iterator it = opMap.find(arguments[0]);

  if (it == opMap.end()) {
		std::cerr << "Unknown operation: " << arguments[0] << std::endl;
		return 1;
	}

  OP op = it->second;

  vr::EVRInitError eError = vr::VRInitError_None;
  vr::VR_Init(&eError, vr::VRApplication_Utility);

  if (eError != vr::VRInitError_None) {
    std::cerr << "Unable to initialize VR: " << vr::VR_GetVRInitErrorAsEnglishDescription(eError) << std::endl;
    return 2;
  }

  vr::IVRSettings* pSettings = vr::VRSettings();
  
  if (!pSettings) {
    std::cerr << "Failed to get VR settings interface" << std::endl;
    vr::VR_Shutdown();
    return 3;
  }

  try {
    op(pSettings, {std::next(std::begin(arguments)), std::end(arguments)});
  }
  catch (...) {
    std::cerr << "An error occurred while processing the operation" << std::endl;
  }

  vr::VR_Shutdown();

  return 0;
}