TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt
INCLUDEPATH += ../../../cocos2dx \
    ../../../cocos2dx/include \
    ../../../cocos2dx/kazmath/include \
    ../../../cocos2dx/platform/win32 \
    ../../../cocos2dx/platform/third_party/win32/OGLES \
    ../Classes \
    ../../../CocosDenshion/include

include(../libraries/ADLib/ADLib.pri)

#LIBS += -lopengl32
#DEFINES +=NDEBUG
CONFIG(debug, debug|release) {
    LIBS += -lOpenGL32
    LIBS += -L../../../../Debug.win32/ -llibcocos2d
    LIBS += -L../../../../Debug.win32/ -lglew32
    LIBS += -L../../../../Debug.win32/ -llibCocosDenshion
} else {
    LIBS += -L../../../Release.win32/ -llibcocos2d
    LIBS += -L../../../Release.win32/ -lglew32
    LIBS += -L../../../Release.win32/ -llibCocosDenshion
    LIBS += -lopengl32
    DEFINES +=NDEBUG
}
LIBS += -luser32
QMAKE_CXXFLAGS_DEBUG += -wd4267 -wd4251 -wd4244
QMAKE_CXXFLAGS_RELEASE += -wd4267 -wd4251 -wd4244

DEFINES += WIN32 \
            _DEBUG \
            _WINDOWS \
            COCOS2D_DEBUG="1" \
            _CRT_SECURE_NO_WARNINGS \
            _SCL_SECURE_NO_WARNINGS

SOURCES += main.cpp \
    ../Classes/AppDelegate.cpp \
    ../Classes/Core/AnimatedMenuItem.cpp \
    ../Classes/Core/GraphicsManager.cpp \
    ../Classes/Scenes/MainMenu.cpp \
    ../Classes/Core/MenuSpriteBatch.cpp \
    ../Classes/Scenes/SelectCollection.cpp \
    ../Classes/Layers/BackgroundHolder.cpp \
    ../Classes/Core/CCScrollView.cpp \
    ../Classes/Layers/LevelKeyboard.cpp \
    ../Classes/Math/Term.cpp \
    ../Classes/Math/Symbol.cpp \
    ../Classes/Math/Node.cpp \
    ../Classes/Math/ExpressionParser.cpp \
    ../Classes/Math/Equation.cpp \
    ../Classes/Layers/EquationDrawer.cpp \
    ../Classes/Layers/DebugLayer.cpp \
    ../Classes/Scenes/SelectLevel.cpp \
    ../Classes/Logic/Level.cpp \
    ../Classes/Logic/Collection.cpp \
    ../Classes/Scenes/LevelScene.cpp \
    ../Classes/Logic/RW.cpp \
    ../Classes/Math/Solution.cpp \
    ../Classes/Layers/LevelScenePopUp.cpp \
    ../Classes/Logic/Stamp.cpp \
    ../Classes/Scenes/Loading.cpp \
    ../Classes/Core/CCTime.cpp \
    ../Classes/Core/CCManualFont.cpp \
    ../Classes/Core/CCTime.cpp \
    ../Classes/Scenes/Settings.cpp \
    ../Classes/Scenes/Developers.cpp \
    ../Classes/Core/MusicSettings.cpp \
    ../Classes/Layers/LevelStop.cpp \
    ../Classes/Logic/Tutorial.cpp \
    ../Classes/Layers/DisplaySolution.cpp \
    ../Classes/Layers/FoundSolutionsLayer.cpp \
    ../Classes/PurchaseHandler.cpp \
    ../Classes/Layers/PopUpWindow.cpp \
    ../Classes/Store.cpp \
    ../Classes/Layers/YesNoDialog.cpp \
    ../Classes/Core/Ads.cpp \
    PlatformEmulator.cpp

HEADERS += \
    ../Classes/AppMacros.h \
    ../Classes/AppDelegate.h \
    main.h \
    ../Classes/Core/AnimatedMenuItem.h \
    ../Classes/Core/GraphicsManager.h \
    ../Classes/cocos2d-A.h \
    ../Classes/Scenes/MainMenu.h \
    ../Classes/Core/MenuSpriteBatch.h \
    ../Classes/Scenes/SelectCollection.h \
    ../Classes/Layers/BackgroundHolder.h \
    ../Classes/Core/CCScrollView.h \
    ../Classes/Layers/LevelKeyboard.h \
    ../Classes/Math/Term.h \
    ../Classes/Math/Symbol.h \
    ../Classes/Math/Node.h \
    ../Classes/Math/ExpressionParser.h \
    ../Classes/Math/Equation.h \
    ../Classes/Layers/EquationDrawer.h \
    ../Classes/Layers/DebugLayer.h \
    ../Classes/Scenes/SelectLevel.h \
    ../Classes/Logic/Level.h \
    ../Classes/Logic/Collection.h \
    ../Classes/Scenes/LevelScene.h \
    ../Classes/Logic/RW.h \
    ../Classes/Math/Solution.h \
    ../Classes/Layers/LevelScenePopUp.h \
    ../Classes/Logic/Stamp.h \
    ../Classes/Scenes/Loading.h \
    ../Classes/Core/CCTime.h \
    ../Classes/Core/CCNodeWithOpacity.h \
    ../Classes/Core/CCManualFont.h \
    ../Classes/Scenes/Settings.h \
    ../Classes/Scenes/Developers.h \
    ../Classes/Core/MusicSettings.h \
    ../Classes/Layers/LevelStop.h \
    ../Classes/Logic/Tutorial.h \
    ../Classes/Layers/DisplaySolution.h \
    ../Classes/Layers/FoundSolutionsLayer.h \
    ../Classes/Logic/Language.h \
    ../Classes/PurchaseHandler.h \
    ../Classes/Scenes/SelectCollection_UnlockAnimator.h \
    ../Classes/Layers/PopUpWindow.h \
    ../Classes/Store.h \
    ../Classes/Core/Browser.h \
    ../Classes/Core/Statistics.h \
    ../Classes/Layers/YesNoDialog.h \
    ../Classes/Core/Notification.h \
    ../Classes/Core/Ads.h \
    ../Classes/Core/RealLog.h \
    ../Classes/Core/Native.h \
    ../Classes/Core/Distributor.h \
    ../Classes/Core/Screen.h

