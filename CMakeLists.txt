cmake_minimum_required(VERSION 3.10)
project(DataDistributor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Указываем флаги компиляции
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pthread")

# Добавляем libpcap
find_package(PkgConfig REQUIRED)
pkg_check_modules(PCAP REQUIRED libpcap)

include_directories(${PCAP_INCLUDE_DIRS})
link_directories(${PCAP_LIBRARY_DIRS})

# Исходные файлы
set(SOURCES
    src/main.cpp
    src/packet_processor.cpp
)

# Создание исполняемого файла
add_executable(data_distributor ${SOURCES})

# Подключаем библиотеки
target_link_libraries(data_distributor ${PCAP_LIBRARIES} pthread)
