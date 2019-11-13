#cmake 最低版本要求，低于2.6 构建过程会被终止
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

#定义工程名称
PROJECT(server)

set(SRCS main.cpp waitconnect.cpp)
add_executable(server ${SRCS})

#打印相关消息消息
MESSAGE(STATUS "Project: server")
MESSAGE(STATUS "Project Directory: ${PROJECT_SOURCE_DIR}")

#指定编译类型
SET(CMAKE_BUILE_TYPE DEBUG)

#需要pthread库
find_package(Threads)
target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT})

#指定编译器
SET(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")

#设置可执行文件的输出目录
SET(EXECUTABLE_OUTPUT_PATH $ENV{BIN})

#把可执行文件放到文件夹下,加上下面这两句话
#SET(ENV{BIN} OUTBIN)
MESSAGE(STATUS "env outbin path::: $ENV{BIN}")
set( CMAKE_INSTALL_PREFIX "$ENV{BIN}")
#MESSAGE(STATUS "make install path::: ${CMAKE_INSTALL_PREFIX}")
#install(TARGETS client RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})

#添加子目录
#ADD_SUBDIRECTORY(utility)

#ADD_SUBDIRECTORY(server)
