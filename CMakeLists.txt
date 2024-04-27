cmake_minimum_required(VERSION 3.19)
project(lexical-analysis)

set(CMAKE_CXX_STANDARD 14)

set(PROJECT_INC_DIR ${PROJECT_SOURCE_DIR}/include)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_INC_DIR}
        )
link_directories(${PROJECT_SOURCE_DIR}/lib)


## 生成dll
aux_source_directory(./source SOURCE)
aux_source_directory(./include INCLUDE)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,10000000")

add_library(${PROJECT_NAME} STATIC ${SOURCE} ${INCLUDE})
install(TARGETS ${PROJECT_NAME} DESTINATION  ${PROJECT_SOURCE_DIR}/lib)



aux_source_directory(./lib LIBS)
add_executable(regular-test test/regular-test.cpp)
target_link_libraries(regular-test
        ${PROJECT_NAME}
        ${LIBS}
        ${NEEDED_LIB}
        )

add_executable(graph-test test/graph-test.cpp)
target_link_libraries(graph-test
        ${PROJECT_NAME}
        ${LIBS}
        ${NEEDED_LIB}
        )

add_executable(dfa-test test/dfa-test.cpp)
target_link_libraries(dfa-test
        ${PROJECT_NAME}
        ${LIBS}
        ${NEEDED_LIB}
        )

add_executable(utils-test test/utils-test.cpp)
target_link_libraries(utils-test
        ${PROJECT_NAME}
        ${LIBS}
        ${NEEDED_LIB}
        )

add_executable(re2nfa example/re2nfa.cpp)
target_link_libraries(re2nfa
        ${PROJECT_NAME}
        ${LIBS}
        ${NEEDED_LIB}
        )

add_executable(nfa2dfa example/nfa2dfa.cpp)
target_link_libraries(nfa2dfa
        ${PROJECT_NAME}
        ${LIBS}
        ${NEEDED_LIB}
        )

add_executable(dfa2minidfa example/dfa2minidfa.cpp)
target_link_libraries(dfa2minidfa
        ${PROJECT_NAME}
        ${LIBS}
        ${NEEDED_LIB}
        )

# 程序文件
add_executable(program-01-regex2dfa example/program-01-regex2dfa.cpp)
target_link_libraries(program-01-regex2dfa
        ${PROJECT_NAME}
        ${LIBS}
        ${NEEDED_LIB}
        )

add_executable(program-02-source2token example/program-02-source2token.cpp)
target_link_libraries(program-02-source2token
        ${PROJECT_NAME}
        ${LIBS}
        ${NEEDED_LIB}
        )