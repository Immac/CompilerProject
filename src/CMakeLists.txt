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
    Lexer/token.h
    main.cpp Util/utilities.cpp Util/utilities.h Lexer/Parser.cpp Lexer/Parser.h Tree/ExpressionNode.cpp Tree/ExpressionNode.h Tree/BinaryOperatornode.cpp Tree/BinaryOperatornode.h Tree/UnaryOperatorNode.cpp Tree/UnaryOperatorNode.h Tree/MultiplicationNode.cpp Tree/MultiplicationNode.h Tree/AdditionNode.cpp Tree/AdditionNode.h Tree/IntegerNode.cpp Tree/IntegerNode.h Semantic/SemanticException.cpp Semantic/SemanticException.h Tree/RealNode.cpp Tree/RealNode.h Tree/StringNode.cpp Tree/StringNode.h Tree/BoolNode.cpp Tree/BoolNode.h Tree/CharNode.cpp Tree/CharNode.h Tree/Accessor.cpp Tree/Accessor.h Tree/IndexAccessor.cpp Tree/IndexAccessor.h Tree/IdNode.cpp Tree/IdNode.h Tree/MemberAccessor.cpp Tree/MemberAccessor.h)

add_executable(src ${SOURCE_FILES})