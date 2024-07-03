#使用CMake构成一个集成编译环境
cmake_minimum_required(VERSION 3.5)
project(main)

#配置编译选项（可执行文件是可调试的）
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

#配置头文件搜索路径
#include_directories()
#配置库文件搜索路径
#link_directories()

#设置需要编译的源文件列表
set(SRC_LIST ./muduo_server.cpp)

#设置可执行文件最终存储的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)  #PROJECT_SOURCE_DIR：表示工程的根目录，Cmake中预定义的

#把.指定路径下的所有源文件名字放入变量名SRC_LIST里面
aux_source_directory(. SRC_LIST)

#生成可执行文件，由SRC_LIST变量所定义的源文件编译而来
add_executable(server ${SRC_LIST}) #或者直接写成add_executable(server muduo_server.cpp)  ${SRC_LIST}就是其中放的内容

#可执行文件server需要链接的库文件 muduo_net muduo_base pthread
target_link_libraries(server muduo_net muduo_base pthread)
