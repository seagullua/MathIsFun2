LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := hellocpp_shared

LOCAL_MODULE_FILENAME := libhellocpp

   
FILE_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Binary/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Core/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Layers/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Logic/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Math/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Scenes/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../libraries/ADLib/platform/Android/cpp/ADJNI.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../libraries/ADLib/platform/Android/cpp/ADOnActivityResult.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../libraries/ADLib/platform/Android/cpp/Device/ADAds_AdMob.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../libraries/ADLib/platform/Android/cpp/Device/ADBrowser.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../libraries/ADLib/platform/Android/cpp/Device/ADInApp_Amazon.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../libraries/ADLib/platform/Android/cpp/Device/ADInfo_Store_AmazonStore.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../libraries/ADLib/platform/Android/cpp/Device/ADLanguage.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../libraries/ADLib/platform/Android/cpp/Device/ADNotification.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../libraries/ADLib/platform/Android/cpp/Device/ADStatistics_Flurry.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../libraries/ADLib/platform/Android/cpp/Device/ADUIThread.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../libraries/ADLib/src/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../libraries/ADLib/src/ADLib/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../libraries/ADLib/src/ADLib/Device/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../libraries/ADLib/src/ADLib/Generic/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../libraries/ADLib/src/ADLib/PlatformImpl/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../libraries/ADLib/src/ADLib/Rendering/*.cpp)


LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../libraries/ADLib/platform/Android/cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../libraries/ADLib/platform/Android/java
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../libraries/ADLib/platform/Android/libs
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../libraries/ADLib/platform/Android/res
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../libraries/ADLib/src

LOCAL_CPPFLAGS += -std=c++11
LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, G:/cocos2d-x-new/)
$(call import-add-path, G:/cocos2d-x-new/cocos2dx/platform/third_party/android/prebuilt)
$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
