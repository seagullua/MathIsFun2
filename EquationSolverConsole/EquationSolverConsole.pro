TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
INCLUDEPATH += ../Classes
INCLUDEPATH += ../Classes/Math
QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp

SOURCES += main.cpp \
    EquationSolver.cpp \
    ../Classes/Math/Term.cpp \
    ../Classes/Math/Symbol.cpp \
    ../Classes/Math/Solution.cpp \
    ../Classes/Math/Node.cpp \
    ../Classes/Math/ExpressionParser.cpp \
    ../Classes/Math/Equation.cpp

HEADERS += \
    EquationSolver.h \
    ../Classes/Math/Term.h \
    ../Classes/Math/Symbol.h \
    ../Classes/Math/Solution.h \
    ../Classes/Math/Node.h \
    ../Classes/Math/ExpressionParser.h \
    ../Classes/Math/Equation.h

