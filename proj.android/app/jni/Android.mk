LOCAL_PATH := $(call my-dir)

# The path to the Firebase C++ SDK, in the project's root directory.
FIREBASE_CPP_SDK_DIR := ../../../firebase_cpp_sdk

APP_ABI := armeabi-v7a x86
STL := $(firstword $(subst _, ,$(APP_STL)))
FIREBASE_LIBRARY_PATH := $(FIREBASE_CPP_SDK_DIR)/libs/android/$(TARGET_ARCH_ABI)/$(STL)

include $(CLEAR_VARS)
LOCAL_MODULE := firebase_app
LOCAL_SRC_FILES := $(FIREBASE_LIBRARY_PATH)/libfirebase_app.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(FIREBASE_CPP_SDK_DIR)/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := firebase_feature
LOCAL_SRC_FILES := $(FIREBASE_LIBRARY_PATH)/libfirebase_admob.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(FIREBASE_CPP_SDK_DIR)/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := $(LOCAL_PATH)/hellocpp/main.cpp \
                   $(LOCAL_PATH)/../../../Classes/AppDelegate.cpp \
                   $(LOCAL_PATH)/../../../Classes/HelloWorldScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/Finished/FinishedScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/Title/TitleScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/FirebaseHelper.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cc_static
LOCAL_STATIC_LIBRARIES += firebase_app
LOCAL_STATIC_LIBRARIES += firebase_feature

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module, cocos)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
