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
set(CMAKE_MODULE_PATH "${G4APP_SOURCE_PATH}/cmake/" ${CMAKE_MODULE_PATH})

# Find the application library & Geant4
find_package(G4Application REQUIRED)
find_package(GDCM REQUIRED)
find_package(Geant4 REQUIRED)
find_package(Geant4 COMPONENTS qt)

include(${Geant4_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

include_directories(${G4Application_INCLUDE_DIR} include)
add_library(dicomPlugin SHARED ${sources} ${headers})

target_link_libraries(dicomPlugin ${Geant4_LIBRARIES} ${G4Application_LIBRARY} ${GDCM_LIBRARY})

install(TARGETS dicomPlugin DESTINATION .)

set(CMAKE_CXX_FLAGS "-std=c++0x")



 


