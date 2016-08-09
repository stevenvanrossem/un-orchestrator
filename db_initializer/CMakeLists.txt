CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(db_initializer)

IF(CMAKE_COMPILER_IS_GNUCXX)
        ADD_DEFINITIONS(-Wall -Werror -Wno-write-strings)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)
IF(CMAKE_COMPILER_IS_GNUCC)
        ADD_DEFINITIONS(-Wall -Werror -Wno-write-strings)
ENDIF(CMAKE_COMPILER_IS_GNUCC)


# Rather complicated CMake code for setting the logging level
SET(LOGGING_LEVEL "ORCH_WARNING" CACHE STRING "Orchestrator logging levels: ORCH_DEBUG, ORCH_DEBUG_INFO, ORCH_WARNING, ORCH_ERROR, ORCH_INFO")
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

OPTION(
	LOG_ON_FILE
	"Turn on to print log messages on the file 'name-resolver.log'. If disabled, stdout is used for the log."
	OFF
)
IF(LOG_ON_FILE)
	ADD_DEFINITIONS(-DLOG_ON_FILE)
ENDIF(LOG_ON_FILE)

# Set source files
SET(SOURCES
	db_initializer.cc	
	
	../orchestrator/node_resource_manager/database_manager/SQLite/SQLiteManager.cc
	../orchestrator/node_resource_manager/database_manager/SQLite/SQLiteManager.h
	
	../orchestrator/utils/logger.c
	../orchestrator/utils/logger.h
	../orchestrator/utils/constants.h
)

INCLUDE_DIRECTORIES (
	${CMAKE_BINARY_DIR}/externals/include/
	/usr/include/boost
)

SET(CMAKE_SKIP_RPATH TRUE)
LINK_DIRECTORIES(
	${CMAKE_BINARY_DIR}/externals/lib/
	)

# Create the executable
ADD_EXECUTABLE(
	db_initializer
	${SOURCES}
)

ADD_DEPENDENCIES(db_initializer Inih)


TARGET_LINK_LIBRARIES( db_initializer
	inih
	rt
	sqlite3
	ssl
	crypto
	boost_system
)

# inih - https://github.com/benhoyt/inih
ExternalProject_Add(
	Inih
	# GIT_REPOSITORY https://github.com/benhoyt/inih
	# GIT_TAG r35
	# URL https://github.com/benhoyt/inih/archive/r35.zip
	URL ${CMAKE_SOURCE_DIR}/contrib/inih-r35.zip
	URL_MD5 e131365d1ca20cfa687e3d9284e59d39
	BUILD_IN_SOURCE 1
	PATCH_COMMAND patch -p1 < ${CMAKE_SOURCE_DIR}/contrib/inih.patch
	CONFIGURE_COMMAND ""
	BUILD_COMMAND cd extra && make -f Makefile.static
	INSTALL_COMMAND
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/externals/lib/
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/externals/include/
	COMMAND ${CMAKE_COMMAND} -E copy extra/libinih.a ${CMAKE_BINARY_DIR}/externals/lib/
	COMMAND ${CMAKE_COMMAND} -E copy ini.h ${CMAKE_BINARY_DIR}/externals/include/
	COMMAND ${CMAKE_COMMAND} -E copy cpp/INIReader.h ${CMAKE_BINARY_DIR}/externals/include/
)
