# -*- mode:cmake -*-

cmake_minimum_required(VERSION 3.12)
project(rcc)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT UNIX)
  message(WARNING "Unsupported operating system")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(antlr4-demo-GENERATED_SRC
   ${PROJECT_SOURCE_DIR}/generated/ANTLR/TLexer.cpp 
   ${PROJECT_SOURCE_DIR}/generated/ANTLR/TParser.cpp
   ${PROJECT_SOURCE_DIR}/generated/ANTLR/TParserBaseListener.cpp
   ${PROJECT_SOURCE_DIR}/generated/ANTLR/TParserBaseVisitor.cpp
   ${PROJECT_SOURCE_DIR}/generated/ANTLR/TParserListener.cpp
   ${PROJECT_SOURCE_DIR}/generated/ANTLR/TParserVisitor.cpp
 )

foreach(src_file ${antlr4-demo-GENERATED_SRC})
      set_source_files_properties(
          ${src_file}
          PROPERTIES
          GENERATED TRUE
          )
endforeach(src_file ${antlr4-demo-GENERATED_SRC})

add_custom_target(GenerateParser DEPENDS ${antlr4-demo-GENERATED_SRC})
add_custom_command(OUTPUT ${antlr4-demo-GENERATED_SRC}
   COMMAND 
   ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/generated/ANTLR/
   COMMAND
   "${Java_JAVA_EXECUTABLE}" -jar ${ANTLR_JAR_LOCATION} -Werror -Dlanguage=Cpp -listener -visitor -o ${PROJECT_SOURCE_DIR}/generated/ANTLR/ -package antlrcpptest ${PROJECT_SOURCE_DIR}/ANTLR/TLexer.g4 ${PROJECT_SOURCE_DIR}/ANTLR/TParser.g4
   WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
   DEPENDS ${PROJECT_SOURCE_DIR}/ANTLR/TLexer.g4 ${PROJECT_SOURCE_DIR}/ANTLR/TParser.g4
   )

include_directories(
  ${PROJECT_SOURCE_DIR}/runtime/src
  ${PROJECT_SOURCE_DIR}/runtime/src/misc
  ${PROJECT_SOURCE_DIR}/runtime/src/atn
  ${PROJECT_SOURCE_DIR}/runtime/src/dfa
  ${PROJECT_SOURCE_DIR}/runtime/src/tree
  ${PROJECT_SOURCE_DIR}/runtime/src/support
  ${PROJECT_SOURCE_DIR}/generated
  ${PROJECT_SOURCE_DIR}/src
  )

#file(GLOB antlr4-demo_SRC "${PROJECT_SOURCE_DIR}/generated/ANTLR/*")
set(antlr4-demo_SRC 
  ${PROJECT_SOURCE_DIR}/src/main.cpp
  ${antlr4-demo-GENERATED_SRC}
  )

file(GLOB FRONT_SRCS CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/front/*.cpp"
)
list(APPEND antlr4-demo_SRC ${FRONT_SRCS})

if(NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set (flags_1 "-Wno-overloaded-virtual")
else()
  set (flags_1 "-MP /wd4251")
endif()

foreach(src_file ${antlr4-demo_SRC})
      set_source_files_properties(
          ${src_file}
          PROPERTIES
          COMPILE_FLAGS "${COMPILE_FLAGS} ${flags_1}"
          )
endforeach(src_file ${antlr4-demo_SRC})

add_executable(antlr4-demo
  ${antlr4-demo_SRC}
  )
#add_precompiled_header(antlr4-demo ${PROJECT_SOURCE_DIR}/runtime/src/antlrcpp-Prefix.h)

set_target_properties(antlr4-demo PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)


if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  target_compile_options(antlr4-demo PRIVATE "/MT$<$<CONFIG:Debug>:d>")
endif()

add_dependencies(antlr4-demo GenerateParser)

target_link_directories(antlr4-demo PRIVATE
  ${PROJECT_SOURCE_DIR}/lib
)

target_link_libraries(antlr4-demo antlr4-runtime)

install(TARGETS antlr4-demo 
        DESTINATION "share" 
        COMPONENT dev 
        )

