TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    ../../Classes/Logic/Stamp.cpp \
    ../../Classes/Logic/Level.cpp \
    ../../Classes/Logic/Collection.cpp \
    ../../Classes/Math/Term.cpp \
    ../../Classes/Math/Symbol.cpp \
    ../../Classes/Math/Solution.cpp \
    ../../Classes/Math/Node.cpp \
    ../../Classes/Math/ExpressionParser.cpp \
    ../../Classes/Math/Equation.cpp \
    LevelsDefinition.cpp \
    ../../Classes/Logic/RW_BuildLevels.cpp
INCLUDEPATH += ../BinaryRW/
INCLUDEPATH += ../../Classes/Binary/
INCLUDEPATH += ../../Classes/
INCLUDEPATH += ../../libraries/ADLib/src

DESTDIR = $$PWD/bin/




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
    col7.collection \
    col8.collection \
    col9.collection \
    col10.collection \
    mif-multiplication/mul10.collection \
    mif-multiplication/mul9.collection \
    mif-multiplication/mul8.collection \
    mif-multiplication/mul7.collection \
    mif-multiplication/mul6.collection \
    mif-multiplication/mul5.collection \
    mif-multiplication/mul4.collection \
    mif-multiplication/mul3.collection \
    mif-multiplication/mul2.collection \
    mif-multiplication/mul1.collection \
    mif-50/col50_10.collection \
    mif-50/col50_9.collection \
    mif-50/col50_8.collection \
    mif-50/col50_7.collection \
    mif-50/col50_6.collection \
    mif-50/col50_5.collection \
    mif-50/col50_4.collection \
    mif-50/col50_3.collection \
    mif-50/col50_2.collection \
    mif-50/col50_1.collection \
    mif-100/col100_10.collection \
    mif-100/col100_9.collection \
    mif-100/col100_8.collection \
    mif-100/col100_7.collection \
    mif-100/col100_6.collection \
    mif-100/col100_5.collection \
    mif-100/col100_4.collection \
    mif-100/col100_3.collection \
    mif-100/col100_2.collection \
    mif-100/col100_1.collection

