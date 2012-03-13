LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    	:= libogresamplebrowser
LOCAL_C_INCLUDES	:= $(OGRE_ROOT)/include \
		$(OGRE_ROOT)/OgreMain/include \
	 	$(LOCAL_PATH)/nv_util \
	 	$(OGRE_ROOT)/RenderSystems/GLES2/include \
	 	$(OGRE_ROOT)/RenderSystems/GLES2/include/Android \
	 	$(OIS_ROOT)/includes \
    $(OGRE_ROOT)/PlugIns/ParticleFX/include \
	 	$(OGRE_ROOT)/Samples/Browser/include/ \
	 	$(OGRE_ROOT)/Samples/Common/include
									

LOCAL_CFLAGS		+= -DFREEIMAGE_LIB=1 -DPNG_STATIC=1 -DOGRE_STATIC_LIB=1 -DUSE_RTSHADER_SYSTEM=1
LOCAL_LDLIBS		:= -llog -lz 
LOCAL_SRC_FILES 	:= acpwrapper.cpp ACPChunk.cpp ACPFile.cpp AndroidLogListener.cpp AndroidArchive.cpp ogrewrapper.cpp ogresamplebrowser.cpp  FileSystemLayerImpl_Android.cpp  nv_util/nv_util.cpp SdkCameraMan.h

LOCAL_SHARED_LIBRARIES := ois ogre

include $(BUILD_SHARED_LIBRARY)

$(call import-module, lib-android-ogre-3d/lib)
$(call import-module, lib-android-ois/lib)
