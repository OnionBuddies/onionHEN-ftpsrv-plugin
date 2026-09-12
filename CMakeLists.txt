cmake_minimum_required(VERSION 3.20)

project(OnionHENFtpServerPlugin VERSION 1.0.0 LANGUAGES C)

set(ONION_PLUGIN_TARGET ftpsrv)
set(ONION_PLUGIN_ID FTPS00001)
set(ONION_PLUGIN_VERSION 1.00)
set(ONION_PLUGIN_NAME "FTP Server")

set(ONIONHEN_PLUGIN_SDK_SOURCE "" CACHE PATH
    "Use a local onionHEN-plugin-sdk checkout instead of downloading it")
set(ONIONHEN_PLUGIN_SDK_GIT_REPOSITORY
    "https://github.com/OnionBuddies/onionHEN-plugin-sdk.git" CACHE STRING
    "OnionHEN Plugin SDK repository")
set(ONIONHEN_PLUGIN_SDK_GIT_TAG
    "0cf4346536705fb271ba0eb2226d97ebcb23559c" CACHE STRING
    "Pinned OnionHEN Plugin SDK revision")

if(NOT ONIONHEN_PLUGIN_SDK_SOURCE AND
   DEFINED ENV{ONIONHEN_PLUGIN_SDK_SOURCE})
    set(ONIONHEN_PLUGIN_SDK_SOURCE "$ENV{ONIONHEN_PLUGIN_SDK_SOURCE}")
endif()

if(ONIONHEN_PLUGIN_SDK_SOURCE)
    if(NOT EXISTS "${ONIONHEN_PLUGIN_SDK_SOURCE}/CMakeLists.txt")
        message(FATAL_ERROR
            "ONIONHEN_PLUGIN_SDK_SOURCE is not an SDK checkout: "
            "${ONIONHEN_PLUGIN_SDK_SOURCE}")
    endif()
    add_subdirectory(
        "${ONIONHEN_PLUGIN_SDK_SOURCE}"
        "${CMAKE_BINARY_DIR}/_deps/onionhen-plugin-sdk-build"
        EXCLUDE_FROM_ALL)
else()
    include(FetchContent)
    FetchContent_Declare(onionhen_plugin_sdk
        GIT_REPOSITORY "${ONIONHEN_PLUGIN_SDK_GIT_REPOSITORY}"
        GIT_TAG "${ONIONHEN_PLUGIN_SDK_GIT_TAG}"
        GIT_SHALLOW FALSE)
    FetchContent_MakeAvailable(onionhen_plugin_sdk)
endif()

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/plugin_config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/plugin_config.h"
    @ONLY)

set(FTPSRV_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/ftpsrv")
set(FTPSRV_SOURCES
    "${FTPSRV_SOURCE_DIR}/srv.c"
    "${FTPSRV_SOURCE_DIR}/cmd.c"
    "${FTPSRV_SOURCE_DIR}/cmd-proc.c"
    "${FTPSRV_SOURCE_DIR}/cmd-comp.c"
    "${FTPSRV_SOURCE_DIR}/cmd-prospero.c"
    "${FTPSRV_SOURCE_DIR}/notify.c"
    "${FTPSRV_SOURCE_DIR}/io.c"
    "${FTPSRV_SOURCE_DIR}/io-freebsd.c"
    "${FTPSRV_SOURCE_DIR}/self.c"
    "${FTPSRV_SOURCE_DIR}/self-prospero.c"
    "${FTPSRV_SOURCE_DIR}/sha256.c")

onion_add_plugin(
    NAME ${ONION_PLUGIN_TARGET}
    TITLE_ID ${ONION_PLUGIN_ID}
    VERSION ${ONION_PLUGIN_VERSION}
    SOURCES
        source/main.c
        source/ftp_i18n.c
        source/ftp_service.c
        source/plugin_descriptor.c
        source/plugin_settings.c
        source/plugin_ui.c
        ${FTPSRV_SOURCES}
    INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_BINARY_DIR}/generated"
        "${FTPSRV_SOURCE_DIR}"
    LIBRARIES
        SceLibcInternal
        SceSystemService
        SceNotification
        kernel_sys)

set_target_properties(${ONION_PLUGIN_TARGET} PROPERTIES
    OUTPUT_NAME "${ONION_PLUGIN_ID}.elf")
target_compile_definitions(${ONION_PLUGIN_TARGET} PRIVATE
    VERSION_TAG="OnionHEN FTP plugin ${ONION_PLUGIN_VERSION}")
set_source_files_properties(${FTPSRV_SOURCES} PROPERTIES
    COMPILE_FLAGS
    "-Wno-error -Wno-unused-function -Wno-unused-parameter -Wno-sign-compare")
