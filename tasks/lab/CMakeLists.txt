cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.
# Если версия установленой программы
# старее указаной, произайдёт аварийный выход.

add_executable(main lab_second.c)		# Создает исполняемый файл с именем main
# из исходника main.cpp

find_package(MPI REQUIRED)

target_link_libraries(main
 PRIVATE
 ${MPI_C_LIBRARIES})

target_include_directories(main
 PRIVATE
 ${MPI_C_INCLUDE_PATH})
