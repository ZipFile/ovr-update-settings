# CMakeList.txt : CMake project for ovr-update-settings, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("ovr-update-settings")

add_subdirectory(openvr)
include_directories(openvr/headers)
find_library(OPENVR_LIB openvr_api PATH_SUFFIXES openvr_api)

# Add source to this project's executable.
add_executable (ovr-update-settings "main.cpp" "main.h" "ops.cpp" "ops.h")

target_link_libraries(ovr-update-settings ${OPENVR_LIB})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ovr-update-settings PROPERTY CXX_STANDARD 20)
endif()
