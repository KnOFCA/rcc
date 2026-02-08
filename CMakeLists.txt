# -*- mode:cmake -*-

cmake_minimum_required(VERSION 3.12)
project(rcc LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT UNIX)
  message(WARNING "Unsupported operating system")
endif()

# C++标准设置
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 定义生成的ANTLR文件
set(GENERATED_ANTLR_DIR ${PROJECT_SOURCE_DIR}/generated/ANTLR)

set(ANTLR_GENERATED_SRC
   ${GENERATED_ANTLR_DIR}/TLexer.cpp 
   ${GENERATED_ANTLR_DIR}/TLexer.h
   ${GENERATED_ANTLR_DIR}/TParser.cpp
   ${GENERATED_ANTLR_DIR}/TParser.h
   ${GENERATED_ANTLR_DIR}/TParserBaseListener.cpp
   ${GENERATED_ANTLR_DIR}/TParserBaseListener.h
   ${GENERATED_ANTLR_DIR}/TParserBaseVisitor.cpp
   ${GENERATED_ANTLR_DIR}/TParserBaseVisitor.h
   ${GENERATED_ANTLR_DIR}/TParserListener.cpp
   ${GENERATED_ANTLR_DIR}/TParserListener.h
   ${GENERATED_ANTLR_DIR}/TParserVisitor.cpp
   ${GENERATED_ANTLR_DIR}/TParserVisitor.h
)

# 标记所有生成的文件为GENERATED，这样CMake在配置阶段不会检查它们是否存在
set_source_files_properties(${ANTLR_GENERATED_SRC} PROPERTIES GENERATED TRUE)

# 查找Java
find_package(Java COMPONENTS Runtime REQUIRED)

# 假设ANTLR_JAR_LOCATION已在环境中设置，否则提供默认路径
if(NOT DEFINED ANTLR_JAR_LOCATION)
    message(WARNING "ANTLR_JAR_LOCATION is not defined. Using default path.")
    set(ANTLR_JAR_LOCATION "/usr/local/lib/antlr-4.13.2-complete.jar")
endif()

# 自定义命令来生成ANTLR代码
add_custom_command(
    OUTPUT ${ANTLR_GENERATED_SRC}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${GENERATED_ANTLR_DIR}
    COMMAND ${Java_JAVA_EXECUTABLE} -jar ${ANTLR_JAR_LOCATION}
            -Werror
            -Dlanguage=Cpp
            -listener
            -visitor
            -o ${GENERATED_ANTLR_DIR}
            -package antlrcpptest
            ${PROJECT_SOURCE_DIR}/ANTLR/TLexer.g4
            ${PROJECT_SOURCE_DIR}/ANTLR/TParser.g4
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS 
        ${PROJECT_SOURCE_DIR}/ANTLR/TLexer.g4 
        ${PROJECT_SOURCE_DIR}/ANTLR/TParser.g4
        ${ANTLR_JAR_LOCATION}
    COMMENT "Generating ANTLR C++ parser and lexer"
)

# 创建自定义目标
add_custom_target(GenerateParser ALL DEPENDS ${ANTLR_GENERATED_SRC})

# 收集项目源文件
set(MAIN_SRC ${PROJECT_SOURCE_DIR}/src/main.cpp)

# 使用CONFIGURE_DEPENDS自动检测新文件
file(GLOB FRONT_SRCS CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/front/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/front/*.cxx"
)

# 组合所有源文件
set(ANTLR4_DEMO_SRC
    ${MAIN_SRC}
    ${ANTLR_GENERATED_SRC}
    ${FRONT_SRCS}
)

# 添加可执行文件
add_executable(antlr4-demo ${ANTLR4_DEMO_SRC})

# 设置目标属性
set_target_properties(antlr4-demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# 添加依赖关系
add_dependencies(antlr4-demo GenerateParser)

# 包含目录 - 使用现代target_include_directories
target_include_directories(antlr4-demo PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/generated
    ${PROJECT_SOURCE_DIR}/runtime/src
    ${PROJECT_SOURCE_DIR}/runtime/src/misc
    ${PROJECT_SOURCE_DIR}/runtime/src/atn
    ${PROJECT_SOURCE_DIR}/runtime/src/dfa
    ${PROJECT_SOURCE_DIR}/runtime/src/tree
    ${PROJECT_SOURCE_DIR}/runtime/src/support
    ${GENERATED_ANTLR_DIR}
)

# 编译器特定设置
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(antlr4-demo PRIVATE 
        "/MP"           # 启用多处理器编译
        "/MT$<$<CONFIG:Debug>:d>"
        "/wd4251"       # 禁用特定的警告
    )
    target_compile_definitions(antlr4-demo PRIVATE 
        _CRT_SECURE_NO_WARNINGS
    )
else()
    # GCC/Clang编译器选项
    target_compile_options(antlr4-demo PRIVATE
        -Wall
        -Wextra
        -Wno-overloaded-virtual  # 抑制ANTLR可能产生的虚函数重载警告
        -Wno-unused-parameter
    )
    
    # 如果是Clang，添加更多警告选项
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(antlr4-demo PRIVATE
            -Wno-c++98-compat
            -Wno-c++98-compat-pedantic
        )
    endif()
endif()

# 链接库和目录
target_link_directories(antlr4-demo PRIVATE
    ${PROJECT_SOURCE_DIR}/lib
)

target_link_libraries(antlr4-demo antlr4-runtime)

# 安装规则
install(TARGETS antlr4-demo 
        DESTINATION bin
        COMPONENT runtime
)

# 可选：添加测试支持
option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    
    # 这里可以添加测试配置
    # add_subdirectory(tests)
endif()

# 可选：添加性能优化选项
option(ENABLE_OPTIMIZATIONS "Enable compiler optimizations" OFF)
if(ENABLE_OPTIMIZATIONS AND NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(antlr4-demo PRIVATE -O3 -march=native)
endif()

# 添加调试信息选项
option(WITH_DEBUG_INFO "Build with debug information" ON)
if(WITH_DEBUG_INFO)
    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        target_compile_options(antlr4-demo PRIVATE /Zi)
        target_link_options(antlr4-demo PRIVATE /DEBUG)
    else()
        target_compile_options(antlr4-demo PRIVATE -g3)
    endif()
endif()
