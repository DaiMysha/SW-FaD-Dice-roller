cmake_minimum_required (VERSION 2.8.8)

include(cmake/macro.cmake)

set(PROJECT_NAME "SWFaD_DiceRoller")
project (${PROJECT_NAME})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(LIBS "")

find_package(SFML 2 COMPONENTS system graphics window REQUIRED)
if(NOT SFML_FOUND)
	message(FATAL_ERROR "SFML Modules not found. Please set the SFML_ROOT variable to your SFML root installation directory and retry")
endif()

include_directories(${SFML_INCLUDE_DIR})

list(APPEND LIBS
	${LIBS}
	${SFML_LIBRARIES}
	${SFML_DEPENDENCIES}
)

set(FILES_HEADER
)

set(FILES_SRC
	main.cpp
)
	
add_executable (${PROJECT_NAME}
	${FILES_HEADER}
	${FILES_SRC}
)
target_link_libraries (SWFaD_DiceRoller ${LIBS})
