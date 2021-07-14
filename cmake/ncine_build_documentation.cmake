# Creating a Doxygen configuration file in the build directory anyway
find_package(Doxygen)

file(RELATIVE_PATH DOXYGEN_RELATIVE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR})
if("${DOXYGEN_RELATIVE_PATH}" STREQUAL "")
	set(DOXYGEN_RELATIVE_PATH ".")
endif()

set(DOXYGEN_INPUT_FILES "${DOXYGEN_RELATIVE_PATH}/include ${DOXYGEN_RELATIVE_PATH}/src")
set(DOXYGEN_EXCLUDE_FILES "${DOXYGEN_RELATIVE_PATH}/src/tests")
if(NOT NCINE_IMPLEMENTATION_DOCUMENTATION)
	set(DOXYGEN_EXCLUDE_FILES "${DOXYGEN_EXCLUDE_FILES} ${DOXYGEN_RELATIVE_PATH}/src/include")
endif()
set(DOXYGEN_OUTPUT_DIR docs)

if(EXISTS ${NCINE_ICONS_DIR}/icon72.png)
	set(DOXYGEN_PROJECT_LOGO ${NCINE_ICONS_DIR}/icon72.png)
endif()

set(NCINE_GENERATE_QHP OFF)
if(NCINE_GENERATE_QHP)
	set(DOXYGEN_GENERATE_QHP "YES")
	set(DOXYGEN_QHP_NAMESPACE "io.github.ncine.${NCINE_VERSION}")
endif()

set(DOXYGEN_WARNINGS "NO")

set(DOXYFILE_IN ${CMAKE_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE ${CMAKE_BINARY_DIR}/Doxyfile)
configure_file(${DOXYFILE_IN} ${DOXYFILE} @ONLY)

if(NCINE_BUILD_DOCUMENTATION)
	if(NOT DOXYGEN_FOUND)
		message(FATAL_ERROR "Doxygen is needed to build the documentation")
	endif()

	add_custom_target(documentation ALL DEPENDS ${DOXYGEN_OUTPUT_DIR})
	add_custom_command(OUTPUT ${DOXYGEN_OUTPUT_DIR}
		COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		COMMENT "Generating documentation with Doxygen"
	)

	if(NCINE_INSTALL_DEV_SUPPORT)
		install(DIRECTORY ${CMAKE_BINARY_DIR}/${DOXYGEN_OUTPUT_DIR}/html/ DESTINATION ${DOCUMENTATION_INSTALL_DESTINATION} COMPONENT documentation)
	endif()
endif()
