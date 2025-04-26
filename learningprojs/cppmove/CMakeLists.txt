cmake_minimum_required(VERSION 3.10) # Lowered based on your file, but >=3.15 recommended for modern CMake
project(cppmove)

set(CMAKE_CXX_STANDARD 20)

# --- Global CMake Settings ---
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)
# Enable debug logging for find_package calls
set(CMAKE_FIND_DEBUG_MODE TRUE)

message(STATUS "CMAKE_PREFIX_PATH set to: ${CMAKE_PREFIX_PATH}")
message(STATUS "CMAKE_FIND_PACKAGE_PREFER_CONFIG set to: ${CMAKE_FIND_PACKAGE_PREFER_CONFIG}")

# --- Find Dependencies Explicitly First ---
message(STATUS "Attempting to find Boost...")
find_package(Boost CONFIG REQUIRED)
if(Boost_FOUND)
    message(STATUS "Boost found (DIR: ${Boost_DIR})")
else()
    message(FATAL_ERROR "Boost package was not found.")
endif()

message(STATUS "Attempting to find glog...")
find_package(glog CONFIG REQUIRED)
if(glog_FOUND)
    message(STATUS "glog found (DIR: ${glog_DIR})")
else()
    message(FATAL_ERROR "glog package was not found.")
endif()

message(STATUS "Attempting to find gflags...")
# gflags might use NAMESPACE GFlags:: or gflags::
find_package(gflags CONFIG REQUIRED)
if(gflags_FOUND)
    message(STATUS "gflags found (DIR: ${gflags_DIR})")
else()
    message(FATAL_ERROR "gflags package was not found.")
endif()

message(STATUS "Attempting to find double-conversion...")
find_package(double-conversion CONFIG REQUIRED)
if(double-conversion_FOUND)
    message(STATUS "double-conversion found (DIR: ${double-conversion_DIR})")
else()
    message(FATAL_ERROR "double-conversion package was not found.")
endif()

# Add other potential dependencies if needed (e.g., fmt, libevent)
# find_package(fmt CONFIG REQUIRED)
# find_package(Libevent CONFIG REQUIRED)

# --- Find Folly ---
message(STATUS "Attempting to find Folly package...")
find_package(folly CONFIG REQUIRED)

if(folly_FOUND)
    message(STATUS "Folly package found (DIR: ${folly_DIR}).")
    # Check again for the target immediately after find_package
    if(TARGET folly::folly)
        message(STATUS "Found folly::folly target immediately after find_package.")
    else()
        message(WARNING "Could NOT find folly::folly target immediately after find_package. Folly's CMake scripts likely failed internally.")
        # You can inspect Folly's CMake files in ${folly_DIR}
    endif()
else()
    message(FATAL_ERROR "Folly package was not found by find_package.")
endif()

# --- Build Target ---
add_executable(cppmove main.cpp
        customer.h
        Shapes.h)

# --- Link Libraries ---
# This will still fail if folly::folly wasn't created.
# We link dependencies explicitly in case Folly doesn't export them correctly.
message(STATUS "Attempting to link libraries...")
target_link_libraries(cppmove PRIVATE
        Boost::boost
        glog::glog          # Common target name for glog
        gflags::gflags      # Common target name for gflags
        double-conversion::double-conversion # Common target name
        # fmt::fmt          # If using fmt
        # Libevent::libevent # If using Libevent
        folly::folly        # This is the one causing the error if not found
)
message(STATUS "Linking setup complete.")

# Disable debug logging after find calls if desired
set(CMAKE_FIND_DEBUG_MODE FALSE)