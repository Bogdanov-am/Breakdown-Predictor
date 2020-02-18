cmake_minimum_required(VERSION 2.8)		# Проверка версии CMake.
										# Если версия установленой программы
										# старее указаной, произайдёт аварийный выход.

project(BreakdownPredict)			# Название проекта


set(SOURCE_EXE src/main.cpp src/MyAdd.cpp src/compress.cpp src/correlation.cpp  src/CreateData.cpp)			# Установка переменной со списком исходников


#include_directories(library)			# Расположение заголовочных файлов
include_directories(headers)

set(CMAKE_CXX_STANDART 17)	

add_executable(BreakdownPredict ${SOURCE_EXE})		# Создает исполняемый файл с именем main

#add_subdirectory(library)			# Добавление подпроекта, указывается имя дирректории
#target_link_libraries(main)		# Линковка программы с библиотекой