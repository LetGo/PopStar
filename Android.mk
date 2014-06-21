LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/Block.cpp \
                   ../../Classes/GameLayer.cpp \
                   ../../Classes/GameManager.cpp \
                   ../../Classes/MainScene.cpp \
                   ../../Classes/UILayer.cpp \
                   ../../Classes/SoundBank.cpp \
                   ../../Classes/TextBank.cpp \
                   ../../Classes/Rank.cpp \
                   ../../Classes/RankLayer.cpp \
                   ../../Classes/InputLayer.cpp \
                   ../../Classes/HelloWorldScene.cpp


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
                              $(LOCAL_PATH)/../../cocos2d \
                              $(LOCAL_PATH)/../../cocos2d/extensions \
                              $(LOCAL_PATH)/../../cocos2d/cocos \
                              $(LOCAL_PATH)/../../cocos2d/cocos/ui \
                              $(LOCAL_PATH)/../../cocos2d/cocos/editor-support/cocostudio 
                              
LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_ui_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)
$(call import-module,Box2D)
$(call import-module,ui)
$(call import-module,editor-support/cocostudio)

