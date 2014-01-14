TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    ../../Classes/Logic/Stamp.cpp \
    ../../Classes/Logic/RW.cpp \
    ../../Classes/Logic/Level.cpp \
    ../../Classes/Logic/Collection.cpp \
    ../../Classes/Math/Term.cpp \
    ../../Classes/Math/Symbol.cpp \
    ../../Classes/Math/Solution.cpp \
    ../../Classes/Math/Node.cpp \
    ../../Classes/Math/ExpressionParser.cpp \
    ../../Classes/Math/Equation.cpp \
    LevelsDefinition.cpp
INCLUDEPATH += ../BinaryRW/
INCLUDEPATH += ../../Classes/Binary/
INCLUDEPATH += ../../Classes/

DEFINES += RW_DEBUG RW_BUILD_LEVELS
#DEFINES += RW_DEBUG RW_TEST_INPUT
HEADERS += \
    ../../Classes/Binary/Binary.h \
    ../../Classes/Logic/Stamp.h \
    ../../Classes/Logic/RW.h \
    ../../Classes/Logic/Level.h \
    ../../Classes/Logic/Collection.h \
    ../../Classes/Math/Term.h \
    ../../Classes/Math/Symbol.h \
    ../../Classes/Math/Solution.h \
    ../../Classes/Math/Node.h \
    ../../Classes/Math/ExpressionParser.h \
    ../../Classes/Math/Equation.h \
    DefineLevels.h \
    ../../Classes/cocos2d.h \
    cocos2d.h

OTHER_FILES += \
    col1.collection \
    col2.collection \
    col3.collection \
    col4.collection \
    col5.collection \
    col6.collection \
    easy1.levels \
    easy2.levels \
    medium3.levels \
    hard5.levels \
    medium4.levels \
    ultrahard6.levels \
    easy0.levels \
    col4000.collection \
    col3000.collection \
    col2000.collection \
    col1000.collection \
    junior1.collection \
    junior6.collection \
    junior5.collection \
    junior4.collection \
    junior3.collection \
    junior2.collection

