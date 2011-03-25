#-------------------------------------------------------------------
# This file has been automatically generated by ProjectGenerator    
# which is part of a build system designed for GUCEF                
#     (Galaxy Unlimited Framework)                                  
# For the latest info, see http://www.VanvelzenSoftware.com/        
#                                                                   
# The contents of this file are placed in the public domain. Feel   
# free to make use of it in any way you like.                       
#-------------------------------------------------------------------


ifndef $(MY_MODULE_PATH)
  MY_MODULE_PATH := $(call my-dir)
endif
LOCAL_PATH := $(MY_MODULE_PATH)

include $(CLEAR_VARS)

@echo Module path: $(MY_MODULE_PATH)
MY_MODULE_PATH := $(LOCAL_PATH)

LOCAL_MODULE := gucefPATCHER
@echo Module name: $(LOCAL_MODULE)

LOCAL_SRC_FILES := \
  $(MY_MODULE_PATH)/src/gucefPATCHER.cpp \
  $(MY_MODULE_PATH)/src/gucefPATCHER_CModule.cpp \
  $(MY_MODULE_PATH)/src/gucefPATCHER_CPatchConfig.cpp \
  $(MY_MODULE_PATH)/src/gucefPATCHER_CPatchEngine.cpp \
  $(MY_MODULE_PATH)/src/gucefPATCHER_CPatchListEngine.cpp \
  $(MY_MODULE_PATH)/src/gucefPATCHER_CPatchListEngineEvents.cpp \
  $(MY_MODULE_PATH)/src/gucefPATCHER_CPatchListParser.cpp \
  $(MY_MODULE_PATH)/src/gucefPATCHER_CPatchManager.cpp \
  $(MY_MODULE_PATH)/src/gucefPATCHER_CPatchSetDirEngine.cpp \
  $(MY_MODULE_PATH)/src/gucefPATCHER_CPatchSetDirEngineEvents.cpp \
  $(MY_MODULE_PATH)/src/gucefPATCHER_CPatchSetEngine.cpp \
  $(MY_MODULE_PATH)/src/gucefPATCHER_CPatchSetEngineEvents.cpp \
  $(MY_MODULE_PATH)/src/gucefPATCHER_CPatchSetFileEngine.cpp \
  $(MY_MODULE_PATH)/src/gucefPATCHER_CPatchSetFileEngineEvents.cpp \
  $(MY_MODULE_PATH)/src/gucefPATCHER_CPatchSetGenerator.cpp \
  $(MY_MODULE_PATH)/src/gucefPATCHER_CPatchSetParser.cpp \
  $(MY_MODULE_PATH)/src/gucefPATCHER_CPatchTaskConsumer.cpp \
  $(MY_MODULE_PATH)/src/gucefPATCHER_CPatchTaskData.cpp

LOCAL_C_INCLUDES := \
  $(MY_MODULE_PATH)/include \
  $(MY_MODULE_PATH)/../gucefCOM/include \
  $(MY_MODULE_PATH)/../gucefCOMCORE/include \
  $(MY_MODULE_PATH)/../gucefCORE/include \
  $(MY_MODULE_PATH)/../gucefMT/include


LOCAL_SHARED_LIBRARIES := \
  gucefCOM \
  gucefCOMCORE \
  gucefCORE \
  gucefMT

include $(BUILD_SHARED_LIBRARY)

