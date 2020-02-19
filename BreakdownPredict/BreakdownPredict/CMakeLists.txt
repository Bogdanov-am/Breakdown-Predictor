cmake_minimum_required(VERSION 2.8)		# Проверка версии CMake.
										# Если версия установленой программы
										# старее указаной, произайдёт аварийный выход.

project(BreakdownPredict)			# Название проекта


set(SOURCE_EXE src/main.cpp src/MyAdd.cpp src/compress.cpp src/correlation.cpp  src/CreateData.cpp)			# Установка переменной со списком исходников

			# Расположение заголовочных файлов
include_directories(headers)

add_executable(BreakdownPredict ${SOURCE_EXE})		# Создает исполняемый файл с именем BreakdownPredict
target_link_libraries(BreakdownPredict PUBLIC OpenXLSX)
target_include_directories(BreakdownPredict PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/OpenXLSX/install/include)
set_target_properties(BreakdownPredict PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

add_subdirectory(OpenXLSX)			# Добавление подпроекта, указывается имя дирректории
