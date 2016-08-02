CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Rather complicated CMake code for setting the logging level
SET(LOGGING_LEVEL "ORCH_WARNING" CACHE STRING "Orchestrator logging levels: ORCH_DEBUG, ORCH_DEBUG_INFO (recommended), ORCH_WARNING, ORCH_ERROR, ORCH_INFO")
SET(LOGGING_LEVEL_VALUES "ORCH_DEBUG" "ORCH_DEBUG_INFO" "ORCH_WARNING" "ORCH_ERROR" "ORCH_INFO")
SET_PROPERTY(CACHE LOGGING_LEVEL PROPERTY STRINGS ${LOGGING_LEVEL_VALUES})
LIST(FIND LOGGING_LEVEL_VALUES ${LOGGING_LEVEL} LOGGING_LEVEL_INDEX)

IF(${LOGGING_LEVEL_INDEX} EQUAL -1)
	message(FATAL_ERROR "Option ${LOGGING_LEVEL} not supported, valid entries are ${LOGGING_LEVEL_VALUES}")
ENDIF()
IF(${LOGGING_LEVEL_INDEX} EQUAL 0)
	ADD_DEFINITIONS(-DLOGGING_LEVEL=ORCH_DEBUG)
ENDIF()
IF(${LOGGING_LEVEL_INDEX} EQUAL 1)
	ADD_DEFINITIONS(-DLOGGING_LEVEL=ORCH_DEBUG_INFO)
ENDIF()
IF(${LOGGING_LEVEL_INDEX} EQUAL 2)
	ADD_DEFINITIONS(-DLOGGING_LEVEL=ORCH_WARNING)
ENDIF()
IF(${LOGGING_LEVEL_INDEX} EQUAL 3)
	ADD_DEFINITIONS(-DLOGGING_LEVEL=ORCH_ERROR)
ENDIF()
IF(${LOGGING_LEVEL_INDEX} EQUAL 4)
	ADD_DEFINITIONS(-DLOGGING_LEVEL=ORCH_INFO)
ENDIF()
# End of the rather complicated CMake code for setting the logging level

OPTION(BUILD_ExternalProjects "Build external projects (json-spirit, rofl, ...)" OFF)
OPTION(BUILD_Orchestrator "Build orchestrator" ON)
OPTION(BUILD_NameResolver "Build name-resolver" ON)
OPTION(BUILD_DBInitializer "Build db_initializer (need if if you plan to use authentication on the orchestrator)" OFF)

include (ExternalProject)
IF(BUILD_ExternalProjects)
# json-spirit: extract, build, install (locally in build-dir):
ExternalProject_Add(
	JSON_Spirit
	URL ${CMAKE_SOURCE_DIR}/contrib/json-spirit.zip
	URL_MD5 728c247d012bb999dca38629bb4649aa
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/externals
	# unfortunately json-spirit lacks a top-level cmakelists.txt:
	PATCH_COMMAND printf "CMAKE_MINIMUM_REQUIRED(VERSION 2.8)\\nADD_SUBDIRECTORY(build)\\n" > CMakeLists.txt
)
ENDIF(BUILD_ExternalProjects)

IF(BUILD_Orchestrator)
	ADD_SUBDIRECTORY(orchestrator)
ENDIF(BUILD_Orchestrator)

IF(BUILD_NameResolver)
	ADD_SUBDIRECTORY(name-resolver)
ENDIF(BUILD_NameResolver)

IF(BUILD_DBInit)
	ADD_SUBDIRECTORY(db_initializer)
ENDIF(BUILD_DBInit)
