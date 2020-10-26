cmake_minimum_required(VERSION 3.10)

project(date-tests)
set(TARGET date_tests)
set(SOURCE_DIR src)
set(SOURCE_APP ${CMAKE_CURRENT_SOURCE_DIR}/../src)
# set(SOURCE_APP ../src)

set(HEADER_DIR $ENV{HOME}/mylib/include)
set(LIB_DIR $ENV{HOME}/mylib/lib)

add_executable(${TARGET} ${SOURCE_DIR}/date_test.cpp)# ${SOURCE_APP}/str_from_file.cpp)

# Поиск необходимых для сборок библиотек и заголовочных файлов

list(APPEND CMAKE_MODULE_PATH $ENV{HOME}/cmake_modules)

message("CMAKE_HOME_DIRECTORY = ${CMAKE_HOME_DIRECTORY}")
message("CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")

find_package(MariaDBClient REQUIRED)
if(MariaDBClient_FOUND)
	target_include_directories(${TARGET} PUBLIC ${MariaDBClient_INCLUDE_DIR})
	target_link_libraries(${TARGET} ${MariaDBClient_LIBRARY})
	message("MariaDBClient_LIBRARIES = ${MariaDBClient_LIBRARIES}")
	message("MariaDBClient_INCLUDE_DIR = ${MariaDBClient_INCLUDE_DIR}")
else()
	message("Не получится собрать приложение,\nне найден MariaDBClient")
	exit()
endif()

target_include_directories(${TARGET} PUBLIC ${SOURCE_APP})
target_sources(${TARGET} PUBLIC ${SOURCE_APP}/tholiday.cpp)
target_sources(${TARGET} PUBLIC ${SOURCE_APP}/cl_parametrs.cpp)
target_sources(${TARGET} PUBLIC ${SOURCE_APP}/functions.cpp)

find_package(Boost REQUIRED COMPONENTS program_options date_time)

if(Boost_FOUND)
	message("Boost_INCLUDE_DIRS = " ${Boost_INCLUDE_DIRS})
	target_link_libraries(${TARGET} ${Boost_LIBRARIES})
	target_include_directories(${TARGET} PUBLIC ${Boost_INCLUDE_DIRS})
else()
	message("Не получится собрать приложение\n, не найден Boost")
endif()

target_include_directories(${TARGET} PUBLIC ${HEADER_DIR})
target_include_directories(${TARGET} PUBLIC ${SOURCE_DIR})
target_link_libraries(${TARGET} ${LIB_DIR}/libgetstr.a)
target_link_libraries(${TARGET} ${LIB_DIR}/libconnection.a)

set_target_properties(
	${TARGET} PROPERTIES
	CXX_STANDARD 17	#Собирать в соответствии со стандартом c17
	C_STANDARD	11
	)