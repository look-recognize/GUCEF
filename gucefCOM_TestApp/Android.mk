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

LOCAL_MODULE := gucefCOM_TestApp
@echo Module name: $(LOCAL_MODULE)

LOCAL_SRC_FILES := \
  $(MY_MODULE_PATH)/src/gucefCOM_TestApp_main.cpp \
  $(MY_MODULE_PATH)/src/TestCode_CHTTPClient.cpp \
  $(MY_MODULE_PATH)/src/TestCode_CHTTPServer.cpp

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

include $(BUILD_EXECUTABLE)

