cmake_minimum_required (VERSION 2.8)

project (WebService)
add_definitions("-std=c++11 -lpthread -lmysqlclient")
aux_source_directory(../FastNet/src BASE_DIR)
aux_source_directory(. BASE_DIR2)
aux_source_directory(../CJsonObject/ BASE_DIR3)
link_directories("/usr/lib64/mysql")
include_directories("/usr/include/mysql")
find_package (Threads)
add_executable(main ${BASE_DIR} ${BASE_DIR2} ${BASE_DIR3} headerParse/requestParser.cc)
target_link_libraries (main ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (main mysqlclient)



