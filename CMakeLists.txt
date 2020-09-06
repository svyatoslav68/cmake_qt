cmake_minimum_required(VERSION 3.10)

project(qholiday VERSION 0.0.1)

configure_file(
	${CMAKE_HOME_DIRECTORY}/version.hpp.in
	${CMAKE_CURRENT_BINARY_DIR}/version.hpp
)

if (APPLE)
	message("Сборка запущена на ${CMAKE_SYSTEM_NAME}")
	message("Устанавливается расположение библиотеки Qt для MacOS")
	set(CMAKE_PREFIX_PATH /opt/local/libexec/qt5)
endif()

set(HEADER_DIR $ENV{HOME}/mylib/include)
set(LIB_DIR $ENV{HOME}/mylib/lib)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

configure_file(
	${CMAKE_HOME_DIRECTORY}/version.hpp.in
	${CMAKE_CURRENT_BINARY_DIR}/version.hpp
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB HOLIDAY_SRC
		${SOURCE_DIR}/*.cpp)

#set(CMAKE_BINARY_DIR ./build)

list(APPEND CMAKE_MODULE_PATH $ENV{HOME}/cmake_modules)

# В добавление в список поиска каталога с заголовками mysql
find_package(MariaDBClient REQUIRED)
if(MariaDBClient_FOUND)
	include_directories(${MariaDBClient_INCLUDE_DIR})
	message("MariaDBClient_LIBRARIES = ${MariaDBClient_LIBRARIES}")
	message("MariaDBClient_INCLUDE_DIR = ${MariaDBClient_INCLUDE_DIR}")
else()
	message("Не найден MariaDBClient,\nбудет предпринята попытка найти MySQL-клиент.")
	find_package(MySql REQUIRED)
	if(MYSQL_FOUND)
		include_directories(${MYSQL_INCLUDE_DIR})
		message("Найден MySQL header ${MYSQL_INCLUDE_DIR}")
	else()
		message("Никто не найден. (MariaDB, MYSQL)")
		exit()
	endif()
	exit()
endif()


find_package(Boost REQUIRED COMPONENTS program_options date_time)
find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui) 

set(TARGET_APP qHoliday)

add_executable(${TARGET_APP} ${HOLIDAY_SRC})

if(Boost_FOUND)
	message("Boost_LIBRARIES = " ${Boost_LIBRARIES})
	target_link_libraries(${TARGET_APP} ${Boost_LIBRARIES})
	#target_include_directories(${TARGET} PUBLIC ${Boost_INCLUDE_DIRS})
else()
	message("Не получится собрать приложение\n, не найден Boost")
	exit()
endif()

target_include_directories(${TARGET_APP} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${TARGET_APP} PUBLIC ${HEADER_DIR})
if(Qt5_FOUND)
	message("Qt_LIBRARIES = " ${Qt5_LIBRARIES})
	target_link_libraries(${TARGET_APP} Qt5::Widgets Qt5::Core Qt5::Gui)
else()
	message("Не получится собрать приложение\n, не найден Qt5")
	exit()
endif()

target_link_libraries(${TARGET_APP} ${LIB_DIR}/libgetstr.a)
target_link_libraries(${TARGET_APP} ${LIB_DIR}/libconnection.a)
target_link_libraries(${TARGET_APP} ${MariaDBClient_LIBRARIES})
#target_link_libraries(${TARGET_APP} ${MYSQL_LIBRARIES})

set_target_properties(
	${TARGET_APP} PROPERTIES
	CXX_STANDARD 17	#Собирать в соответствии со стандартом c17
	C_STANDARD	11
	)

if (EXISTS ${SOURCE_DIR}/settings.xml) 
	if (NOT EXISTS ${CMAKE_BINARY_DIR}/settings.xml) 
		add_custom_command(TARGET ${TARGET_APP} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/settings.xml ${CMAKE_BINARY_DIR}/settings.xml) 
	endif()	
endif()
#message("Исполняемый файл ${CMAKE_CURRENT_SOURCE_DIR} помещается в каталог:  ${CMAKE_BINARY_DIR}")
