project(DataSvc)
cmake_minimum_required(VERSION 2.6)

# Build Type
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "DEBUG") # RELEASE, RELWITHDEBINFO MINSIZEREL
ENDIF()

# Find Package
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../CMake/Modules)

FIND_PACKAGE(Dim REQUIRED)
IF(DIM_FOUND)
	INCLUDE_DIRECTORIES(${Dim_INCLUDE_DIRS})
ELSEIF(NOT Dim_FOUND) 
	MESSAGE(FATAL_ERROR "Unable to find Dim. Did you set Dim_ROOT?")
ENDIF()

FIND_PACKAGE(SniperKernel REQUIRED)
FIND_PACKAGE(Data REQUIRED)
IF(DATA_FOUND AND SNIPERKERNEL_FOUND)
  INCLUDE_DIRECTORIES(${SniperKernel_INCLUDE_DIRS} ${Data_INCLUDE_DIRS})
ELSEIF(NOT SniperKernel_FOUND) 
  MESSAGE(FATAL_ERROR "Unable to find SniperKernel. Did you set SniperKernel_ROOT?")
ELSEIF(NOT DATA_FOUND) 
  MESSAGE(FATAL_ERROR "Unable to find Data. Did you set Data_ROOT?")
ENDIF()

# Find Package
FIND_PACKAGE(Boost)
FIND_PACKAGE(PythonLibs)
IF(Boost_FOUND AND PYTHONLIBS_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_PATH})
  SET(Boost_USE_STATIC_LIBS OFF)
  SET(Boost_USE_MULTITHREADED ON)
  SET(Boost_USE_STATIC_RUNTIME OFF)
  FIND_PACKAGE(Boost  COMPONENTS python  thread REQUIRED) 
ELSEIF(NOT Boost_FOUND)
  MESSAGE(FATAL_ERROR "Unable to find Boost. Did you set BOOST_ROOT?")
ELSEIF(NOT PYTHONLIBS_FOUND)
  MESSAGE(FATAL_ERROR "Unable to find Python. Did you set PYTHONPATH?")
ENDIF()

# Compile Flag
IF(CMAKE_COMPILER_IS_GNUCXX)
  ADD_DEFINITIONS("-Wall")
ELSE()
  MESSAGE(FATAL_ERROR "CMakeLists.txt has not been tested/written for your compiler.")
ENDIF()

# Search Source Files
include_directories(${PROJECT_SOURCE_DIR}/Data)
#include_directories(${PROJECT_SOURCE_DIR}/DataSvc)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src)
file(GLOB_RECURSE CPPLIB_SRC_LIST "src/*.cc")
MESSAGE(STATUS CPPLIB = ${CPPLIB_SRC_LIST}  )

# Compile Library for CPP
add_library(DataSvc SHARED ${CPPLIB_SRC_LIST})
target_link_libraries(DataSvc ${Boost_LIBRARIES}) 
TARGET_LINK_LIBRARIES(DataSvc ${SniperKernel_LIBRARIES})
TARGET_LINK_LIBRARIES(DataSvc ${Data_LIBRARIES})
TARGET_LINK_LIBRARIES(DataSvc ${Dim_LIBRARIES})

# Output Path and Install Area
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

INSTALL(TARGETS DataSvc LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/../SniperInstall/lib)
INSTALL(DIRECTORY DataSvc DESTINATION ${PROJECT_SOURCE_DIR}/../SniperInstall/include)
INSTALL(DIRECTORY python/DataSvc DESTINATION ${PROJECT_SOURCE_DIR}/../SniperInstall/pylib)
