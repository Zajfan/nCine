set(RUNTIME_INSTALL_DESTINATION bin)
set(LIBRARY_INSTALL_DESTINATION lib)
set(ARCHIVE_INSTALL_DESTINATION lib)
set(INCLUDE_INSTALL_DESTINATION include)

if(MSVC OR APPLE OR EMSCRIPTEN)
	set(README_INSTALL_DESTINATION .)
	set(DATA_INSTALL_DESTINATION data)
	set(SHADERS_INSTALL_DESTINATION data/shaders)
	set(NCINE_CONFIG_INSTALL_DESTINATION cmake)
	set(DOCUMENTATION_INSTALL_DESTINATION docs)
	set(ANDROID_INSTALL_DESTINATION android)
else()
	set(README_INSTALL_DESTINATION share/ncine)
	set(DATA_INSTALL_DESTINATION share/ncine/data)
	set(SHADERS_INSTALL_DESTINATION share/ncine/shaders)
	set(NCINE_CONFIG_INSTALL_DESTINATION lib/cmake/nCine)
	set(DOCUMENTATION_INSTALL_DESTINATION share/doc/ncine)
	set(ANDROID_INSTALL_DESTINATION share/ncine/android)
endif()

set(CPACK_PACKAGE_NAME "nCine")
set(CPACK_PACKAGE_VENDOR "Angelo Theodorou")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A cross-platform 2D game engine")
set(CPACK_PACKAGE_DESCRIPTION "nCine is a 2D game engine for Linux, Windows, macOS, Android and Emscripten")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://ncine.github.io")
set(CPACK_PACKAGE_CONTACT "encelo@gmail.com")
set(CPACK_PACKAGE_VERSION ${NCINE_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${NCINE_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${NCINE_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${NCINE_VERSION_PATCH})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "nCine")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_CHECKSUM MD5)

if(MSVC)
	set(CPACK_GENERATOR NSIS ZIP)
	set(CPACK_NSIS_MUI_ICON "${NCINE_ICONS_DIR}/icon.ico")
	set(CPACK_NSIS_MENU_LINKS ${CPACK_PACKAGE_HOMEPAGE_URL} "nCine Website")
	if(NCINE_INSTALL_DEV_SUPPORT)
		if(NCINE_BUILD_DOCUMENTATION)
			list(APPEND CPACK_NSIS_MENU_LINKS "${DOCUMENTATION_INSTALL_DESTINATION}/index.html;nCine Documentation")
		else()
			if(NOT GIT_NO_TAG)
				set(DOCS_DIRECTORY ${GIT_TAG_NAME})
			else()
				set(DOCS_DIRECTORY ${GIT_BRANCH_NAME})
			endif()
			list(APPEND CPACK_NSIS_MENU_LINKS "${CPACK_PACKAGE_HOMEPAGE_URL}/docs/${DOCS_DIRECTORY}/index.html;nCine Documentation")
			list(APPEND CPACK_NSIS_MENU_LINKS "${CPACK_PACKAGE_HOMEPAGE_URL}/docs/lua_api_${DOCS_DIRECTORY}.html;nCine Lua API")
		endif()
	endif()
	if(NCINE_BUILD_TESTS)
		# Custom NSIS commands needed in order to set the "Start in" property of the start menu shortcut
		set(CPACK_NSIS_CREATE_ICONS_EXTRA
			"SetOutPath '$INSTDIR\\\\bin'
			CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\nCine Test.lnk' '$INSTDIR\\\\bin\\\\${NCINE_STARTUP_TEST}.exe'
			CreateShortCut '$DESKTOP\\\\nCine Test.lnk' '$INSTDIR\\\\bin\\\\${NCINE_STARTUP_TEST}.exe'
			SetOutPath '$INSTDIR'")
		set(CPACK_NSIS_DELETE_ICONS_EXTRA
			"Delete '$SMPROGRAMS\\\\$MUI_TEMP\\\\nCine Test.lnk'
			Delete '$DESKTOP\\\\nCine Test.lnk'")
	endif()
	set(CPACK_NSIS_COMPRESSOR "/SOLID lzma")
	set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)

	set(NCINE_SYSTEM_NAME "Win64")
	if("${CMAKE_GENERATOR}" STREQUAL "Visual Studio 16 2019")
		set(NCINE_COMPILER "VS2019")
	elseif("${CMAKE_GENERATOR}" STREQUAL "Visual Studio 15 2017")
		set(NCINE_COMPILER "VS2017")
	endif()
elseif(APPLE)
	set(CPACK_GENERATOR "Bundle")
	set(CPACK_BUNDLE_NAME nCine)
	set(FRAMEWORKS_INSTALL_DESTINATION ../Frameworks)

	configure_file(${CMAKE_SOURCE_DIR}/Info.plist.in ${CMAKE_BINARY_DIR}/Info.plist @ONLY)
	set(CPACK_BUNDLE_PLIST ${CMAKE_BINARY_DIR}/Info.plist)

	file(RELATIVE_PATH RELPATH_TO_BIN ${CMAKE_INSTALL_PREFIX}/MacOS ${CMAKE_INSTALL_PREFIX}/Resources/${RUNTIME_INSTALL_DESTINATION})
	file(WRITE ${CMAKE_BINARY_DIR}/bundle_executable "#!/usr/bin/env sh\ncd \"$(dirname \"$0\")\" \ncd ${RELPATH_TO_BIN} &&./${NCINE_STARTUP_TEST}")
	install(FILES ${CMAKE_BINARY_DIR}/bundle_executable DESTINATION ../MacOS/ RENAME ${CPACK_BUNDLE_NAME}
		PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE COMPONENT tests)

	if(EXISTS ${NCINE_ICONS_DIR}/icon1024.png)
		add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/${CPACK_BUNDLE_NAME}.iconset
			COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/${CPACK_BUNDLE_NAME}.iconset
			COMMAND ${CMAKE_COMMAND} -E copy_if_different ${NCINE_ICONS_DIR}/icon1024.png ${CMAKE_BINARY_DIR}/${CPACK_BUNDLE_NAME}.iconset/icon_512x512@2x.png
			COMMAND sips -z 512 512 ${NCINE_ICONS_DIR}/icon1024.png --out ${CMAKE_BINARY_DIR}/${CPACK_BUNDLE_NAME}.iconset/icon_512x512.png
			COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/${CPACK_BUNDLE_NAME}.iconset/icon_512x512.png ${CMAKE_BINARY_DIR}/${CPACK_BUNDLE_NAME}.iconset/icon_256x256@2x.png
			COMMAND sips -z 256 256 ${NCINE_ICONS_DIR}/icon1024.png --out ${CMAKE_BINARY_DIR}/${CPACK_BUNDLE_NAME}.iconset/icon_256x256.png
			COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/${CPACK_BUNDLE_NAME}.iconset/icon_256x256.png ${CMAKE_BINARY_DIR}/${CPACK_BUNDLE_NAME}.iconset/icon_128x128@2x.png
			COMMAND sips -z 128 128 ${NCINE_ICONS_DIR}/icon1024.png --out ${CMAKE_BINARY_DIR}/${CPACK_BUNDLE_NAME}.iconset/icon_128x128.png
			COMMAND sips -z 64 64 ${NCINE_ICONS_DIR}/icon1024.png --out ${CMAKE_BINARY_DIR}/${CPACK_BUNDLE_NAME}.iconset/icon_32x32@2x.png
			COMMAND sips -z 32 32 ${NCINE_ICONS_DIR}/icon1024.png --out ${CMAKE_BINARY_DIR}/${CPACK_BUNDLE_NAME}.iconset/icon_32x32.png
			COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/${CPACK_BUNDLE_NAME}.iconset/icon_32x32.png ${CMAKE_BINARY_DIR}/${CPACK_BUNDLE_NAME}.iconset/icon_16x16@2x.png
			COMMAND sips -z 16 16 ${NCINE_ICONS_DIR}/icon1024.png --out ${CMAKE_BINARY_DIR}/${CPACK_BUNDLE_NAME}.iconset/icon_16x16.png
			COMMAND iconutil --convert icns --output ${CMAKE_BINARY_DIR}/${CPACK_BUNDLE_NAME}.icns ${CMAKE_BINARY_DIR}/${CPACK_BUNDLE_NAME}.iconset)
		add_custom_target(iconutil_convert ALL DEPENDS ${CMAKE_BINARY_DIR}/${CPACK_BUNDLE_NAME}.iconset)
		set(CPACK_BUNDLE_ICON ${CMAKE_BINARY_DIR}/${CPACK_BUNDLE_NAME}.icns)
	endif()
elseif(EMSCRIPTEN)
	if(CMAKE_HOST_WIN32)
		set(CPACK_GENERATOR ZIP)
	else()
		set(CPACK_GENERATOR TGZ)
	endif()
elseif(UNIX AND NOT APPLE)
	set(CPACK_GENERATOR TGZ)
	if(NCINE_BUILD_TESTS)
		set(ICONS_INSTALL_DESTINATION share/icons/hicolor)

		if(EXISTS ${NCINE_DATA_DIR}/svg/icon.svg)
			install(FILES ${NCINE_DATA_DIR}/svg/icon.svg DESTINATION ${ICONS_INSTALL_DESTINATION}/scalable/apps/ RENAME nCine.svg COMPONENT data)
		endif()

		if(EXISTS ${NCINE_ICONS_DIR}/icon1024.png)
			install(FILES ${NCINE_ICONS_DIR}/icon1024.png DESTINATION ${ICONS_INSTALL_DESTINATION}/1024x1024/apps/ RENAME nCine.png COMPONENT data)
		endif()
		if(EXISTS ${NCINE_ICONS_DIR}/icon192.png)
			install(FILES ${NCINE_ICONS_DIR}/icon192.png DESTINATION ${ICONS_INSTALL_DESTINATION}/192x192/apps/ RENAME nCine.png COMPONENT data)
		endif()
		if(EXISTS ${NCINE_ICONS_DIR}/icon96.png)
			install(FILES ${NCINE_ICONS_DIR}/icon96.png DESTINATION ${ICONS_INSTALL_DESTINATION}/96x96/apps/ RENAME nCine.png COMPONENT data)
		endif()
		if(EXISTS ${NCINE_ICONS_DIR}/icon72.png)
			install(FILES ${NCINE_ICONS_DIR}/icon72.png DESTINATION ${ICONS_INSTALL_DESTINATION}/72x72/apps/ RENAME nCine.png COMPONENT data)
		endif()
		if(EXISTS ${NCINE_ICONS_DIR}/icon48.png)
			install(FILES ${NCINE_ICONS_DIR}/icon48.png DESTINATION ${ICONS_INSTALL_DESTINATION}/48x48/apps/ RENAME nCine.png COMPONENT data)
		endif()

		configure_file(${CMAKE_SOURCE_DIR}/io.github.ncine.desktop.in ${CMAKE_BINARY_DIR}/io.github.ncine.desktop @ONLY)
		install(FILES ${CMAKE_BINARY_DIR}/io.github.ncine.desktop DESTINATION share/applications COMPONENT tests)
	endif()

	set(NCINE_SYSTEM_NAME "Linux")
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		set(NCINE_COMPILER "GCC")
	elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		set(NCINE_COMPILER "Clang")
	endif()
elseif(MINGW)
	set(CPACK_GENERATOR TGZ)

	set(NCINE_SYSTEM_NAME "MinGW")
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		set(NCINE_COMPILER "GCC")
	elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		set(NCINE_COMPILER "Clang")
	endif()
endif()

# Custom override of CPack package name
if(DEFINED NCINE_SYSTEM_NAME)
	if(DEFINED NCINE_COMPILER)
		set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${NCINE_SYSTEM_NAME}-${NCINE_COMPILER})
	else()
		set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${NCINE_SYSTEM_NAME})
	endif()
endif()

set(CPACK_COMPONENTS_ALL libraries)
set(CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "Libraries")

if(NCINE_BUILD_TESTS)
	set(CPACK_COMPONENTS_ALL "${CPACK_COMPONENTS_ALL};data;tests")
	set(CPACK_COMPONENT_DATA_DEPENDS libraries)
	set(CPACK_COMPONENT_TESTS_DEPENDS data)

	set(CPACK_COMPONENT_DATA_DISPLAY_NAME "Data")
	set(CPACK_COMPONENT_DATA_DESCRIPTION "Data files for tests")
	set(CPACK_COMPONENT_TESTS_DISPLAY_NAME "Executables")
	set(CPACK_COMPONENT_TESTS_DESCRIPTION "Test executables")

	set(CPACK_COMPONENT_DATA_GROUP testgroup)
	set(CPACK_COMPONENT_TESTS_GROUP testgroup)
	set(CPACK_COMPONENT_GROUP_TESTGROUP_DISPLAY_NAME "Test projects")
	set(CPACK_COMPONENT_GROUP_TESTGROUP_DESCRIPTION "Test projects and their data")
endif()

if(NCINE_INSTALL_DEV_SUPPORT)
	set(CPACK_COMPONENTS_ALL "${CPACK_COMPONENTS_ALL};android;devsupport")
	set(CPACK_COMPONENT_DEVSUPPORT_DEPENDS libraries)

	set(CPACK_COMPONENT_ANDROID_DISPLAY_NAME "Android")
	set(CPACK_COMPONENT_ANDROID_DESCRIPTION "Android development support")
	set(CPACK_COMPONENT_DEVSUPPORT_DISPLAY_NAME "Headers")
	set(CPACK_COMPONENT_DEVSUPPORT_DESCRIPTION "Headers and additional files to support development")
	set(CPACK_COMPONENT_LIBRARIES_DESCRIPTION "Run-time and development libraries")

	set(CPACK_COMPONENT_DEVSUPPORT_GROUP devgroup)
	set(CPACK_COMPONENT_DOCUMENTATION_GROUP devgroup)
	set(CPACK_COMPONENT_GROUP_DEVGROUP_DISPLAY_NAME "Development")
	set(CPACK_COMPONENT_GROUP_DEVGROUP_DESCRIPTION "Development support")

	if(NCINE_BUILD_DOCUMENTATION)
		set(CPACK_COMPONENTS_ALL "${CPACK_COMPONENTS_ALL};documentation")
		set(CPACK_COMPONENT_DOCUMENTATION_DEPENDS devsupport)
		set(CPACK_COMPONENT_DOCUMENTATION_DISPLAY_NAME "Documentation")
		set(CPACK_COMPONENT_DOCUMENTATION_DESCRIPTION "Doxygen generated documentation")
	endif()
else()
	set(CPACK_COMPONENT_LIBRARIES_DESCRIPTION "Run-time libraries")
endif()

include(CPack)

if(WIN32)
	if(NCINE_INSTALL_DEV_SUPPORT)
		install(TARGETS ncine EXPORT nCineTargets RUNTIME DESTINATION ${RUNTIME_INSTALL_DESTINATION} COMPONENT libraries
			ARCHIVE DESTINATION ${ARCHIVE_INSTALL_DESTINATION} COMPONENT libraries)
	else()
		install(TARGETS ncine EXPORT nCineTargets RUNTIME DESTINATION ${RUNTIME_INSTALL_DESTINATION} COMPONENT libraries)
	endif()
else()
	install(TARGETS ncine EXPORT nCineTargets DESTINATION ${LIBRARY_INSTALL_DESTINATION} COMPONENT libraries)
endif()

install(FILES README.md DESTINATION ${README_INSTALL_DESTINATION} COMPONENT libraries)
if(MSVC OR APPLE)
	install(FILES LICENSE DESTINATION ${README_INSTALL_DESTINATION} COMPONENT libraries RENAME LICENSE.txt)
endif()

if(MSVC)
	if(ANGLE_FOUND)
		get_target_property(EGL_IMPORTED_LOCATION EGL::EGL IMPORTED_LOCATION)
		install(FILES ${EGL_IMPORTED_LOCATION} DESTINATION ${RUNTIME_INSTALL_DESTINATION} COMPONENT libraries)
		get_target_property(GLES_IMPORTED_LOCATION OpenGLES::GLES IMPORTED_LOCATION)
		install(FILES ${GLES_IMPORTED_LOCATION} DESTINATION ${RUNTIME_INSTALL_DESTINATION} COMPONENT libraries)
	endif()
	if(GLEW_FOUND)
		get_target_property(GLEW_IMPORTED_LOCATION GLEW::GLEW IMPORTED_LOCATION)
		install(FILES ${GLEW_IMPORTED_LOCATION} DESTINATION ${RUNTIME_INSTALL_DESTINATION} COMPONENT libraries)
	endif()
	if(GLFW_FOUND)
		get_target_property(GLFW_IMPORTED_LOCATION GLFW::GLFW IMPORTED_LOCATION)
		install(FILES ${GLFW_IMPORTED_LOCATION} DESTINATION ${RUNTIME_INSTALL_DESTINATION} COMPONENT libraries)
	endif()
	if(SDL2_FOUND)
		get_target_property(SDL2_IMPORTED_LOCATION SDL2::SDL2 IMPORTED_LOCATION)
		install(FILES ${SDL2_IMPORTED_LOCATION} DESTINATION ${RUNTIME_INSTALL_DESTINATION} COMPONENT libraries)
	endif()
	if(PNG_FOUND)
		get_target_property(ZLIB_IMPORTED_LOCATION ZLIB::ZLIB IMPORTED_LOCATION)
		install(FILES ${ZLIB_IMPORTED_LOCATION} DESTINATION ${RUNTIME_INSTALL_DESTINATION} COMPONENT libraries)
		get_target_property(PNG_IMPORTED_LOCATION PNG::PNG IMPORTED_LOCATION)
		install(FILES ${PNG_IMPORTED_LOCATION} DESTINATION ${RUNTIME_INSTALL_DESTINATION} COMPONENT libraries)
	endif()
	if(WEBP_FOUND)
		get_target_property(OPENAL_IMPORTED_LOCATION WebP::WebP IMPORTED_LOCATION)
		install(FILES ${OPENAL_IMPORTED_LOCATION} DESTINATION ${RUNTIME_INSTALL_DESTINATION} COMPONENT libraries)
	endif()
	if(OPENAL_FOUND)
		get_target_property(OPENAL_IMPORTED_LOCATION OpenAL::AL IMPORTED_LOCATION)
		install(FILES ${OPENAL_IMPORTED_LOCATION} DESTINATION ${RUNTIME_INSTALL_DESTINATION} COMPONENT libraries)

		if(VORBIS_FOUND)
			get_target_property(OGG_IMPORTED_LOCATION Ogg::Ogg IMPORTED_LOCATION)
			install(FILES ${OGG_IMPORTED_LOCATION} DESTINATION ${RUNTIME_INSTALL_DESTINATION} COMPONENT libraries)
			get_target_property(VORBIS_IMPORTED_LOCATION Vorbis::Vorbis IMPORTED_LOCATION)
			install(FILES ${VORBIS_IMPORTED_LOCATION} DESTINATION ${RUNTIME_INSTALL_DESTINATION} COMPONENT libraries)
			get_target_property(VORBISFILE_IMPORTED_LOCATION Vorbis::Vorbisfile IMPORTED_LOCATION)
			install(FILES ${VORBISFILE_IMPORTED_LOCATION} DESTINATION ${RUNTIME_INSTALL_DESTINATION} COMPONENT libraries)
		endif()
	endif()
	if(LUA_FOUND)
		get_target_property(LUA_IMPORTED_LOCATION Lua::Lua IMPORTED_LOCATION)
		install(FILES ${LUA_IMPORTED_LOCATION} DESTINATION ${RUNTIME_INSTALL_DESTINATION} COMPONENT libraries)
	endif()
elseif(APPLE)
	file(RELATIVE_PATH RELPATH_TO_FRAMEWORKS ${CMAKE_INSTALL_PREFIX}/${LIBRARY_INSTALL_DESTINATION} ${CMAKE_INSTALL_PREFIX}/${FRAMEWORKS_INSTALL_DESTINATION})
	set_target_properties(ncine PROPERTIES INSTALL_RPATH "@loader_path/${RELPATH_TO_FRAMEWORKS}")

	if(GLEW_FOUND)
		install(DIRECTORY ${CMAKE_FRAMEWORK_PATH}/glew.framework DESTINATION ${FRAMEWORKS_INSTALL_DESTINATION} COMPONENT libraries)
	endif()
	if(GLFW_FOUND)
		install(DIRECTORY ${CMAKE_FRAMEWORK_PATH}/glfw.framework DESTINATION ${FRAMEWORKS_INSTALL_DESTINATION} COMPONENT libraries)
	endif()
	if(SDL2_FOUND)
		install(DIRECTORY ${CMAKE_FRAMEWORK_PATH}/sdl2.framework DESTINATION ${FRAMEWORKS_INSTALL_DESTINATION} COMPONENT libraries)
	endif()
	if(PNG_FOUND)
		install(DIRECTORY ${CMAKE_FRAMEWORK_PATH}/zlib.framework DESTINATION ${FRAMEWORKS_INSTALL_DESTINATION} COMPONENT libraries)
		install(DIRECTORY ${CMAKE_FRAMEWORK_PATH}/png.framework DESTINATION ${FRAMEWORKS_INSTALL_DESTINATION} COMPONENT libraries)
	endif()
	if(WEBP_FOUND)
		install(DIRECTORY ${CMAKE_FRAMEWORK_PATH}/webp.framework DESTINATION ${FRAMEWORKS_INSTALL_DESTINATION} COMPONENT libraries)
	endif()
	if(OPENAL_FOUND)
		install(DIRECTORY ${CMAKE_FRAMEWORK_PATH}/openal.framework DESTINATION ${FRAMEWORKS_INSTALL_DESTINATION} COMPONENT libraries)

		if(VORBIS_FOUND)
			install(DIRECTORY ${CMAKE_FRAMEWORK_PATH}/ogg.framework DESTINATION ${FRAMEWORKS_INSTALL_DESTINATION} COMPONENT libraries)
			install(DIRECTORY ${CMAKE_FRAMEWORK_PATH}/vorbis.framework DESTINATION ${FRAMEWORKS_INSTALL_DESTINATION} COMPONENT libraries)
			install(DIRECTORY ${CMAKE_FRAMEWORK_PATH}/vorbisfile.framework DESTINATION ${FRAMEWORKS_INSTALL_DESTINATION} COMPONENT libraries)
		endif()
	endif()
	if(LUA_FOUND)
		install(DIRECTORY ${CMAKE_FRAMEWORK_PATH}/lua.framework DESTINATION ${FRAMEWORKS_INSTALL_DESTINATION} COMPONENT libraries)
	endif()
elseif(EMSCRIPTEN)
	if(WEBP_FOUND)
		get_target_property(OPENAL_IMPORTED_LOCATION WebP::WebP IMPORTED_LOCATION)
		install(FILES ${OPENAL_IMPORTED_LOCATION} DESTINATION ${LIBRARY_INSTALL_DESTINATION} COMPONENT libraries)
	endif()
	if(LUA_FOUND)
		get_target_property(LUA_IMPORTED_LOCATION Lua::Lua IMPORTED_LOCATION)
		install(FILES ${LUA_IMPORTED_LOCATION} DESTINATION ${LIBRARY_INSTALL_DESTINATION} COMPONENT libraries)
	endif()
endif()

if(NCINE_INSTALL_DEV_SUPPORT)
	install(FILES ${HEADERS} DESTINATION ${INCLUDE_INSTALL_DESTINATION}/ncine COMPONENT devsupport)
	install(FILES ${NCTL_HEADERS} DESTINATION ${INCLUDE_INSTALL_DESTINATION}/nctl COMPONENT devsupport)
	if(NOT NCINE_DYNAMIC_LIBRARY)
		install(FILES ${PRIVATE_HEADERS} DESTINATION ${INCLUDE_INSTALL_DESTINATION}/ncine COMPONENT devsupport)
	endif()
	if(NCINE_WITH_TRACY)
		install(DIRECTORY ${TRACY_INCLUDE_ONLY_DIR}/tracy/ DESTINATION ${INCLUDE_INSTALL_DESTINATION}/tracy COMPONENT devsupport)
	endif()
	install(TARGETS ncine_main EXPORT nCineTargets ARCHIVE DESTINATION ${ARCHIVE_INSTALL_DESTINATION} COMPONENT devsupport)
	install(DIRECTORY project DESTINATION ${README_INSTALL_DESTINATION} COMPONENT devsupport)
	install(FILES .clang-format .editorconfig DESTINATION ${README_INSTALL_DESTINATION}/project COMPONENT devsupport)
	install(FILES cmake/FindGLFW.cmake cmake/FindSDL2.cmake cmake/FindVorbis.cmake cmake/FindWebP.cmake DESTINATION ${README_INSTALL_DESTINATION}/project/cmake COMPONENT devsupport)
endif()
if(NOT NCINE_EMBED_SHADERS)
	install(DIRECTORY src/shaders/ DESTINATION ${SHADERS_INSTALL_DESTINATION} COMPONENT devsupport)
endif()

include(CMakePackageConfigHelpers)
configure_package_config_file(
	"${CMAKE_SOURCE_DIR}/cmake/nCineConfig.cmake.in"
	"${CMAKE_BINARY_DIR}/nCineConfig.cmake"
	INSTALL_DESTINATION "${NCINE_CONFIG_INSTALL_DESTINATION}"
	PATH_VARS CMAKE_INSTALL_PREFIX
	NO_CHECK_REQUIRED_COMPONENTS_MACRO)
write_basic_package_version_file("${CMAKE_BINARY_DIR}/nCineConfigVersion.cmake" VERSION ${NCINE_VERSION} COMPATIBILITY SameMajorVersion)
# This makes the project importable from the build directory
export(TARGETS ncine ncine_main NAMESPACE ncine:: FILE nCineTargets.cmake)
if(NCINE_INSTALL_DEV_SUPPORT)
	install(FILES ${CMAKE_BINARY_DIR}/nCineConfig.cmake DESTINATION ${NCINE_CONFIG_INSTALL_DESTINATION} COMPONENT devsupport)
	install(FILES ${CMAKE_BINARY_DIR}/nCineConfigVersion.cmake DESTINATION ${NCINE_CONFIG_INSTALL_DESTINATION} COMPONENT devsupport)
	install(EXPORT nCineTargets DESTINATION ${NCINE_CONFIG_INSTALL_DESTINATION} NAMESPACE ncine:: COMPONENT devsupport)
endif()
