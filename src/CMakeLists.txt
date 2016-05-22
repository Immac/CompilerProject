cmake_minimum_required(VERSION 3.5)
include_directories(C:/lib/cpp/catch/include)
project(src)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES
    Lexer/lexer.cpp
    Lexer/lexer.h
    Lexer/sourcestream.cpp
    Lexer/sourcestream.h
    Lexer/symbol.h
    Lexer/token.cpp
    Lexer/token.h
    main.cpp Util/utilities.cpp Util/utilities.h Lexer/Parser.cpp Lexer/Parser.h)

add_executable(src ${SOURCE_FILES})