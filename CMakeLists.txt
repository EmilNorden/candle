cmake_minimum_required (VERSION 2.8)
project (candle)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Set warning level
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread -Wall -Wno-long-long -pedantic")

 # Set C++11
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()


file (GLOB candle_SRC src/*.cpp)

add_executable (candle ${candle_SRC})

find_package(FreeImage REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)

add_subdirectory("lib/assimp-3.2")

message(STATUS "boost--------${Boost_SYSTEM_LIBRARY}---------")

include_directories(
	"lib/assimp-3.2/include"
	${FreeImage_INCLUDE_DIRS}
)

target_link_libraries(candle
	assimp
	${FreeImage_LIBRARIES}
	${Boost_FILESYSTEM_LIBRARY}
	${Boost_SYSTEM_LIBRARY}
)	
