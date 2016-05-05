TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++14
INCLUDEPATH += C:\lib\cpp\catch\single_include

SOURCES += \
    src/main.cpp \
    src/Lexer/sourcestream.cpp \
    src/Lexer/symbol.cpp \
    src/Lexer/token.cpp \
    src/Lexer/lexer.cpp

HEADERS += \
    lib/catch/single_include/catch.hpp \
    src/Lexer/sourcestream.h \
    src/Lexer/symbol.h \
    src/Lexer/token.h \
    src/Lexer/lexer.h

