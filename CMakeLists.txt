cmake_minimum_required(VERSION 2.8)

project(dicomPlugin)

# Necessary variables
if(DEFINED ENV{G4APP_PATH})
    set(G4APP_PATH $ENV{G4APP_PATH})
endif()
if(DEFINED ENV{G4APP_SOURCE_PATH})
    set(G4APP_SOURCE_PATH $ENV{G4APP_SOURCE_PATH})
endif()

# G4APP_PATH 
if(DEFINED G4APP_PATH)
    message(STATUS "Using install location: $ENV{G4APP_PATH}" )
    set(CMAKE_INSTALL_PREFIX ${G4APP_PATH})
endif()

# Use the FindG4Application.cmake file
set(CMAKE_MODULE_PATH "${G4APP_SOURCE_PATH}/cmake/" ${CMAKE_MODULE_PATH} cmake)

# Find the application library & Geant4
find_package(G4Application REQUIRED)

# Boost
find_package(Boost 1.49 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

# JsonCpp - for parsing several options
# Answer from: http://stackoverflow.com/questions/18005880/how-to-writing-a-cmake-module-for-jsoncpp
find_package(JsonCpp)
if(JSONCPP_FOUND)
    message(STATUS "Using system-wide Json-Cpp")
    include_directories(${Jsoncpp_INCLUDE_DIR})
    target_link_libraries(dicomPlugin ${JsonCpp_LIBRARY})
else(JSONCPP_FOUND)
    message(STATUS "Using g4application-built-in Json-Cpp")
    include_directories("${G4APP_SOURCE_PATH}/lib/json/")
endif(JSONCPP_FOUND)

# Geant4
find_package(Geant4 REQUIRED)
find_package(Geant4 COMPONENTS qt)
include(${Geant4_USE_FILE})

# Set GDCM libraries
# Answer from: http://stackoverflow.com/questions/21116319/using-gdcm-via-cmake
find_package(GDCM REQUIRED)
if(GDCM_FOUND)
    include(${GDCM_USE_FILE})
    set(GDCM_LIBRARIES 
        gdcmCommon
        gdcmDICT
        gdcmDSED
        gdcmIOD
        gdcmjpeg12
        gdcmjpeg16
        gdcmjpeg8
        gdcmMEXD
        gdcmMSFF
        socketxx)
else(GDCM_FOUND)
    message(FATAL_ERROR "Cannot find GDCM, did you set GDCM_DIR?")
endif(GDCM_FOUND)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

include_directories(${G4Application_INCLUDE_DIR} include)
add_library(dicomPlugin SHARED ${sources} ${headers})

target_link_libraries(dicomPlugin
    ${Geant4_LIBRARIES}
    ${G4Application_LIBRARY}
    ${GDCM_LIBRARIES}
)

install(TARGETS dicomPlugin DESTINATION .)

set(CMAKE_CXX_FLAGS "-std=c++0x")
