# Registers the Chrome/Chromium Native Messaging host manifest for
# com.keypr.native, pointing it at the freshly built qt_client binary.
#
# Invoked as a POST_BUILD step on the qt_client target with:
#   cmake -DEXE_PATH=<path to built executable> \
#         -DSOURCE_DIR=<repo root> \
#         -P register_native_host.cmake

if(NOT DEFINED EXE_PATH)
  message(FATAL_ERROR "EXE_PATH must be set")
endif()

if(NOT DEFINED SOURCE_DIR)
  message(FATAL_ERROR "SOURCE_DIR must be set")
endif()

set(TEMPLATE_FILE
    "${SOURCE_DIR}/installer/native-messaging/com.keypr.native.json.in")

if(NOT EXISTS "${TEMPLATE_FILE}")
  message(FATAL_ERROR "Native Messaging template not found: ${TEMPLATE_FILE}")
endif()

# JSON requires Windows backslashes to be escaped.
if(WIN32)
  string(REPLACE "\\" "\\\\" NATIVE_HOST_PATH "${EXE_PATH}")
else()
  set(NATIVE_HOST_PATH "${EXE_PATH}")
endif()

set(GENERATED_MANIFEST
    "${CMAKE_CURRENT_BINARY_DIR}/com.keypr.native.generated.json")

configure_file(
  "${TEMPLATE_FILE}"
  "${GENERATED_MANIFEST}"
  @ONLY
)

function(install_native_manifest MANIFEST_DIRECTORY)
  if(NOT IS_DIRECTORY "${MANIFEST_DIRECTORY}")
    file(MAKE_DIRECTORY "${MANIFEST_DIRECTORY}")
  endif()

  set(MANIFEST_DESTINATION
      "${MANIFEST_DIRECTORY}/com.keypr.native.json")

  file(COPY_FILE
    "${GENERATED_MANIFEST}"
    "${MANIFEST_DESTINATION}"
    ONLY_IF_DIFFERENT
  )

  message(STATUS
    "Native Messaging host manifest written to: "
    "${MANIFEST_DESTINATION}"
  )
endfunction()

if(WIN32)
  # Windows Chromium-based browsers use the registry to locate the manifest.
  # The manifest itself can be stored next to the executable.
  get_filename_component(EXE_DIR "${EXE_PATH}" DIRECTORY)
  set(MANIFEST_DESTINATION
      "${EXE_DIR}/com.keypr.native.json")

  file(COPY_FILE
    "${GENERATED_MANIFEST}"
    "${MANIFEST_DESTINATION}"
    ONLY_IF_DIFFERENT
  )

  file(TO_NATIVE_PATH
    "${MANIFEST_DESTINATION}"
    MANIFEST_DESTINATION_NATIVE
  )

  # Register with all supported Chromium-based browsers whose registry
  # installation roots are present.
  set(BROWSER_REGISTRY_ROOTS
    "Google\\Chrome"
    "Google\\Chrome Beta"
    "Google\\Chrome Dev"
    "Google\\Chrome SxS"
    "Microsoft\\Edge"
    "Chromium"
    "BraveSoftware\\Brave-Browser"
    "Vivaldi"
    "Opera Software\\Opera Stable"
  )

  set(REGISTERED_BROWSER_COUNT 0)

  foreach(BROWSER_REGISTRY_ROOT IN LISTS BROWSER_REGISTRY_ROOTS)
    # Check whether the browser has a registry installation entry.
    execute_process(
      COMMAND reg query
        "HKCU\\Software\\${BROWSER_REGISTRY_ROOT}"
      RESULT_VARIABLE HKCU_RESULT
      OUTPUT_QUIET
      ERROR_QUIET
    )

    execute_process(
      COMMAND reg query
        "HKLM\\Software\\${BROWSER_REGISTRY_ROOT}"
      RESULT_VARIABLE HKLM_RESULT
      OUTPUT_QUIET
      ERROR_QUIET
    )

    execute_process(
      COMMAND reg query
        "HKLM\\Software\\WOW6432Node\\${BROWSER_REGISTRY_ROOT}"
      RESULT_VARIABLE HKLM_WOW6432_RESULT
      OUTPUT_QUIET
      ERROR_QUIET
    )

    if(HKCU_RESULT EQUAL 0 OR
       HKLM_RESULT EQUAL 0 OR
       HKLM_WOW6432_RESULT EQUAL 0)

      execute_process(
        COMMAND reg add
          "HKCU\\Software\\${BROWSER_REGISTRY_ROOT}\\NativeMessagingHosts\\com.keypr.native"
          /ve
          /t REG_SZ
          /d "${MANIFEST_DESTINATION_NATIVE}"
          /f
        RESULT_VARIABLE REG_RESULT
        OUTPUT_QUIET
        ERROR_QUIET
      )

      if(REG_RESULT EQUAL 0)
        math(EXPR REGISTERED_BROWSER_COUNT
          "${REGISTERED_BROWSER_COUNT} + 1")

        message(STATUS
          "Registered native host for ${BROWSER_REGISTRY_ROOT}"
        )
      else()
        message(WARNING
          "Failed to register native host for ${BROWSER_REGISTRY_ROOT}"
        )
      endif()
    endif()
  endforeach()

  if(REGISTERED_BROWSER_COUNT EQUAL 0)
    message(WARNING
      "No supported Chromium-based browser installation was detected"
    )
  endif()

elseif(APPLE)
  # Chromium-based browser profile/configuration directories on macOS.
  set(BROWSER_CONFIG_DIRS
    "$ENV{HOME}/Library/Application Support/Google/Chrome"
    "$ENV{HOME}/Library/Application Support/Google/Chrome Beta"
    "$ENV{HOME}/Library/Application Support/Google/Chrome Dev"
    "$ENV{HOME}/Library/Application Support/Google/Chrome Canary"
    "$ENV{HOME}/Library/Application Support/Chromium"
    "$ENV{HOME}/Library/Application Support/Microsoft Edge"
    "$ENV{HOME}/Library/Application Support/BraveSoftware/Brave-Browser"
    "$ENV{HOME}/Library/Application Support/Vivaldi"
    "$ENV{HOME}/Library/Application Support/com.operasoftware.Opera"
  )

  set(DETECTED_BROWSER_COUNT 0)

  foreach(BROWSER_CONFIG_DIR IN LISTS BROWSER_CONFIG_DIRS)
    if(IS_DIRECTORY "${BROWSER_CONFIG_DIR}")
      install_native_manifest(
        "${BROWSER_CONFIG_DIR}/NativeMessagingHosts"
      )

      math(EXPR DETECTED_BROWSER_COUNT
        "${DETECTED_BROWSER_COUNT} + 1")
    endif()
  endforeach()

  if(DETECTED_BROWSER_COUNT EQUAL 0)
    message(WARNING
      "No supported Chromium-based browser configuration directory was found"
    )
  endif()

elseif(UNIX)
  # Linux follows XDG_CONFIG_HOME when it is defined.
  if(DEFINED ENV{XDG_CONFIG_HOME} AND
     NOT "$ENV{XDG_CONFIG_HOME}" STREQUAL "")
    set(CONFIG_DIR "$ENV{XDG_CONFIG_HOME}")
  else()
    set(CONFIG_DIR "$ENV{HOME}/.config")
  endif()

  set(BROWSER_CONFIG_DIRS
    "${CONFIG_DIR}/google-chrome"
    "${CONFIG_DIR}/google-chrome-beta"
    "${CONFIG_DIR}/google-chrome-unstable"
    "${CONFIG_DIR}/google-chrome-for-testing"
    "${CONFIG_DIR}/chromium"
    "${CONFIG_DIR}/microsoft-edge"
    "${CONFIG_DIR}/microsoft-edge-beta"
    "${CONFIG_DIR}/BraveSoftware/Brave-Browser"
    "${CONFIG_DIR}/vivaldi"
    "${CONFIG_DIR}/opera"
  )

  set(DETECTED_BROWSER_COUNT 0)

  foreach(BROWSER_CONFIG_DIR IN LISTS BROWSER_CONFIG_DIRS)
    if(IS_DIRECTORY "${BROWSER_CONFIG_DIR}")
      install_native_manifest(
        "${BROWSER_CONFIG_DIR}/NativeMessagingHosts"
      )

      math(EXPR DETECTED_BROWSER_COUNT
        "${DETECTED_BROWSER_COUNT} + 1")
    endif()
  endforeach()

  if(DETECTED_BROWSER_COUNT EQUAL 0)
    message(WARNING
      "No supported Chromium-based browser configuration directory was found"
    )
  endif()

else()
  message(WARNING
    "Unsupported platform for Native Messaging host registration"
  )
endif()
