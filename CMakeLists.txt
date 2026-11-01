cmake_minimum_required(VERSION 3.16)
project(text_editor VERSION 1.0.0 LANGUAGES C)

set(APP_ID "text_editor")
set(APP_NAME "Text Editor")
set(APP_PKG "text-editor")
set(APP_DESC "A simple text editor")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(APP_SOURCES
	src/main.c
	src/app.c
	src/assets.c
)

set(RAVEN_SOURCE_DIR "" CACHE PATH "Path to a local Raven checkout (optional)")

find_package(raven QUIET)

if(NOT raven_FOUND)
	set(RAVEN_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	set(RAVEN_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(RAVEN_BUILD_TOOLS OFF CACHE BOOL "" FORCE)

	if(RAVEN_SOURCE_DIR)
		message(STATUS "Using Raven from ${RAVEN_SOURCE_DIR}")
		add_subdirectory("${RAVEN_SOURCE_DIR}" raven EXCLUDE_FROM_ALL)
	else()
		message(STATUS "Raven not found: downloading it from GitHub")
		include(FetchContent)
		FetchContent_Declare(raven
			GIT_REPOSITORY https://github.com/oscar-soirey/Raven.git
			GIT_TAG 1d5c735fed18ef054bc47bc08c4cd50d0e63b376)
		FetchContent_MakeAvailable(raven)
	endif()

	if(TARGET raven AND CMAKE_C_COMPILER_ID STREQUAL "GNU")
		target_compile_options(raven PRIVATE -Wno-format-truncation)
	endif()
endif()

add_executable(${APP_ID} ${APP_SOURCES})
target_link_libraries(${APP_ID} PRIVATE raven::raven)
if(MSVC)
	target_compile_definitions(${APP_ID} PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

if(MSVC)
	target_link_options(${APP_ID} PRIVATE
		"$<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>" "$<$<CONFIG:Release>:/ENTRY:mainCRTStartup>")
elseif(WIN32)
	target_link_options(${APP_ID} PRIVATE "$<$<CONFIG:Release>:-mwindows>")
endif()

include(GNUInstallDirs)

if(WIN32)
	set(APP_BIN_DIR ".")
	set(APP_ASSETS_DIR "assets")
else()
	set(APP_BIN_DIR "${CMAKE_INSTALL_BINDIR}")
	set(APP_ASSETS_DIR "${CMAKE_INSTALL_DATADIR}/${APP_ID}")
endif()

install(TARGETS ${APP_ID} RUNTIME DESTINATION "${APP_BIN_DIR}" COMPONENT app)
install(FILES assets/main.xml assets/strings.csv DESTINATION "${APP_ASSETS_DIR}" COMPONENT app)
if(UNIX AND NOT APPLE)
	install(FILES packaging/text_editor.desktop
		DESTINATION "${CMAKE_INSTALL_DATADIR}/applications" COMPONENT app)
	install(FILES packaging/text_editor.png
		DESTINATION "${CMAKE_INSTALL_DATADIR}/icons/hicolor/256x256/apps" COMPONENT app)
endif()

set(CPACK_PACKAGE_NAME "${APP_PKG}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR "Your Name")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${APP_DESC}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${APP_NAME}")
set(CPACK_COMPONENTS_ALL app)
set(CPACK_COMPONENTS_GROUPING ALL_COMPONENTS_IN_ONE)

if(WIN32)
	set(CPACK_GENERATOR "ZIP")
	set(CPACK_NSIS_DISPLAY_NAME "${APP_NAME}")
	set(CPACK_NSIS_INSTALLED_ICON_NAME "${APP_ID}.exe")
	set(CPACK_PACKAGE_EXECUTABLES "${APP_ID}" "${APP_NAME}")
else()
	set(CPACK_GENERATOR "DEB")
	set(CPACK_DEB_COMPONENT_INSTALL ON)
	set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Your Name <you@example.com>")
	set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
	set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
	set(CPACK_DEBIAN_PACKAGE_DEPENDS "libvulkan1")
	set(CPACK_DEBIAN_PACKAGE_RECOMMENDS "mesa-vulkan-drivers")
	set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
endif()

include(CPack)
