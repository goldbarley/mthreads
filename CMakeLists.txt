cmake_minimum_required(VERSION 3.15.7)
project(mthread C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE_RELEASE)
endif() # CMAKE_BUILD_TYPE

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	set(MTHREAD_IS_TOP_LVL ON)
else()
	set(MTHREAD_IS_TOP_LVL OFF)
endif() # MTHREAD_IS_TOP_LVL

if (MTHREAD_IS_TOP_LVL)
	set(MTHREAD_DEFAULT_LIB_TYPE "EXEC")
else()
	set(MTHREAD_DEFAULT_LIB_TYPE "STATIC")
endif() # MTHREAD_DEFAULT_LIB_TYPE

set(MTHREAD_LIB_TYPE "${MTHREAD_DEFAULT_LIB_TYPE}" CACHE STRING "Library type.")

if (MTHREAD_LIB_TYPE STREQUAL "SHARED")
        add_library(${PROJECT_NAME} SHARED)
        target_compile_definitions(${PROJECT_NAME} PUBLIC MTHREAD_LIB_SHARED)
elseif (MTHREAD_LIB_TYPE STREQUAL "EXEC")
        add_executable(${PROJECT_NAME}
                ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.c)
        target_compile_definitions(${PROJECT_NAME} PUBLIC MTHREAD_LIB_EXEC)
else ()
        add_library(${PROJECT_NAME} STATIC)
        target_compile_definitions(${PROJECT_NAME} PUBLIC MTHREAD_LIB_STATIC)
endif() # MTHREAD_LIB_TYPE

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include)

if (UNIX)
	target_sources(${PROJECT_NAME} PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src/mthread_posix/mthread_cond.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/mthread_posix/mthread_mutex.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/mthread_posix/mthread_posix.c)
	target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
endif() # Platform
