# Registers the Chrome Native Messaging host manifest for com.keypr.native,
# pointing it at the freshly built qt_client binary.
#
# Invoked as a POST_BUILD step on the qt_client target with:
#   cmake -DEXE_PATH=<path to built executable> -DSOURCE_DIR=<repo root> -P register_native_host.cmake

if(NOT DEFINED EXE_PATH)
    message(FATAL_ERROR "EXE_PATH must be set")
endif()

if(NOT DEFINED SOURCE_DIR)
    message(FATAL_ERROR "SOURCE_DIR must be set")
endif()

set(TEMPLATE_FILE "${SOURCE_DIR}/installer/native-messaging/com.keypr.native.json.in")

if(WIN32)
    # JSON requires backslashes to be escaped.
    string(REPLACE "\\" "\\\\" NATIVE_HOST_PATH "${EXE_PATH}")
elseif(APPLE)
    set(NATIVE_HOST_PATH "${EXE_PATH}")
elseif(UNIX)
    set(NATIVE_HOST_PATH "${EXE_PATH}")
else()
    message(WARNING "Unsupported platform for Native Messaging host registration")
    return()
endif()

configure_file("${TEMPLATE_FILE}" "${CMAKE_CURRENT_BINARY_DIR}/com.keypr.native.generated.json" @ONLY)
set(GENERATED_MANIFEST "${CMAKE_CURRENT_BINARY_DIR}/com.keypr.native.generated.json")

if(WIN32)
    get_filename_component(EXE_DIR "${EXE_PATH}" DIRECTORY)
    set(MANIFEST_DESTINATION "${EXE_DIR}/com.keypr.native.json")
elseif(APPLE)
    set(MANIFEST_DESTINATION "$ENV{HOME}/Library/Application Support/Google/Chrome/NativeMessagingHosts/com.keypr.native.json")
else()
    set(MANIFEST_DESTINATION "$ENV{HOME}/.config/google-chrome/NativeMessagingHosts/com.keypr.native.json")
endif()

get_filename_component(MANIFEST_DESTINATION_DIR "${MANIFEST_DESTINATION}" DIRECTORY)
file(MAKE_DIRECTORY "${MANIFEST_DESTINATION_DIR}")
file(COPY_FILE "${GENERATED_MANIFEST}" "${MANIFEST_DESTINATION}")

message(STATUS "Native Messaging host manifest written to: ${MANIFEST_DESTINATION}")

if(WIN32)
    file(TO_NATIVE_PATH "${MANIFEST_DESTINATION}" MANIFEST_DESTINATION_NATIVE)
    execute_process(
        COMMAND reg add "HKCU\\Software\\Google\\Chrome\\NativeMessagingHosts\\com.keypr.native" /ve /t REG_SZ /d "${MANIFEST_DESTINATION_NATIVE}" /f
        RESULT_VARIABLE REG_RESULT
    )
    if(NOT REG_RESULT EQUAL 0)
        message(WARNING "Failed to register Native Messaging host in the Windows registry")
    endif()
endif()
