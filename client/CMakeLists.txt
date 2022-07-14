cmake_minimum_required(VERSION 3.8.2)

# Project
project(CBORClientExample)

# Include directory
include_directories(include)
include_directories(lib/QCBOR/inc/)
include_directories(lib/cn-cbor/include)
include_directories(lib/tinycbor/src)

# Include sources 
file(GLOB QCBOR_FILES lib/QCBOR/src/*.c)
file(GLOB CNCBOR_FILES lib/cn-cbor/src/*.c)
file(GLOB TCBOR_FILES lib/tinycbor/src/*.c)

# Libcurl
link_libraries(curl)
link_libraries(m)

# Executable
add_executable(client main_og.c src/telemetry_codec.c ${QCBOR_FILES} ${CNCBOR_FILES} ${TCBOR_FILES})
