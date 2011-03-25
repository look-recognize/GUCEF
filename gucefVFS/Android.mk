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

LOCAL_MODULE := gucefVFS
@echo Module name: $(LOCAL_MODULE)

LOCAL_SRC_FILES := \
  $(MY_MODULE_PATH)/src/gucefVFS.cpp \
  $(MY_MODULE_PATH)/src/gucefVFS_CFileSystemArchive.cpp \
  $(MY_MODULE_PATH)/src/gucefVFS_CIArchive.cpp \
  $(MY_MODULE_PATH)/src/gucefVFS_CModule.cpp \
  $(MY_MODULE_PATH)/src/gucefVFS_CVFS.cpp \
  $(MY_MODULE_PATH)/src/gucefVFS_CVFSHandle.cpp \
  $(MY_MODULE_PATH)/src/gucefVFS_CVFSURLHandler.cpp

LOCAL_C_INCLUDES := \
  $(MY_MODULE_PATH)/include \
  $(MY_MODULE_PATH)/../gucefCORE/include \
  $(MY_MODULE_PATH)/../gucefMT/include


LOCAL_SHARED_LIBRARIES := \
  gucefCORE \
  gucefMT

include $(BUILD_SHARED_LIBRARY)

