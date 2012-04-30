#include <jni.h>
#include <stdlib.h>
#include <iostream>
#include <stdexcept>
#include <string>

#include "ogrewrapper.h"
#include <OIS.h>
#include <android/log.h>
#include "AndroidMultiTouch.h"
#include "AndroidKeyboard.h"
#include "OgreAndroidBaseFramework.h"


#define  LOG_TAG    "libogresamplebrowser"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)




static AndroidMultiTouch *g_multiTouch = 0;
static int g_xOffset = 0, g_yOffset = 0;


static void injectTouchEvent(int pointerId, int action, float x, float y){
  
		OIS::MultiTouchState &state = g_multiTouch->getMultiTouchState(pointerId);
		
		switch(action){
		case 0:
			state.touchType = OIS::MT_Pressed;
			break;
		case 1:
			state.touchType = OIS::MT_Released;
			break;
		case 2:
			state.touchType = OIS::MT_Moved;
			break;
		case 3:
			state.touchType = OIS::MT_Cancelled;
			break;
		default:
			state.touchType = OIS::MT_None;
		}
		
		if(state.touchType != OIS::MT_None){
			int last = state.X.abs;
			state.X.abs = g_xOffset + (int)x;
			state.X.rel = state.X.abs - last;
			
			last = state.Y.abs;
			state.Y.abs = g_yOffset + (int)y;
			state.Y.rel = state.Y.abs - last;
			
			//last = state.Z.abs;
			state.Z.abs = 0;
			state.Z.rel = 0;
			
			OIS::MultiTouchEvent evt(g_multiTouch, state);
			OgreAndroidBaseFramework* framework = OgreAndroidBaseFramework::getSingletonPtr();
			switch(state.touchType){
			case OIS::MT_Pressed:
			      framework->injectTouchDown(evt);
				break;
			case OIS::MT_Released:
			      framework->injectTouchUp(evt);
				break;
			case OIS::MT_Moved:
			      framework->injectTouchMove(evt);
				break;
			case OIS::MT_Cancelled:
			
				break;
			}
		}
	
}

static void injectKeyEvent(int action, int keyCode){
}
 
jboolean init(JNIEnv* env, jobject thiz)
{
	new OgreAndroidBaseFramework(env);
	
	if(!OgreAndroidBaseFramework::getSingletonPtr()->initOgreRoot()) {
		return JNI_FALSE;
	} 
	
	return true;
}

jboolean render(JNIEnv* env, jobject thiz, jint drawWidth, jint drawHeight, jboolean forceRedraw)
{

	if(g_multiTouch == 0){
		g_multiTouch = new AndroidMultiTouch();
        g_multiTouch->setWindowSize(drawWidth, drawHeight);
	}

	OgreAndroidBaseFramework::getSingletonPtr()->initRenderWindow(0, drawWidth, drawHeight, 0);
	OgreAndroidBaseFramework::getSingletonPtr()->renderOneFrame();
	
    return JNI_TRUE;
}

void cleanup(JNIEnv* env)
{
	
	if(g_multiTouch){
		delete g_multiTouch;
		g_multiTouch = 0;
	}
	delete OgreAndroidBaseFramework::getSingletonPtr();
}

jboolean inputEvent(JNIEnv* env, jobject thiz, jint action, jfloat mx, jfloat my)
{
    injectTouchEvent(0, action, mx, my);
	return JNI_TRUE;
}

jboolean keyEvent(JNIEnv* env, jobject thiz, jint action, jint unicodeChar, jint keyCode, jobject keyEvent)
{
    injectKeyEvent(action, keyCode);
    return JNI_TRUE;  
}

void setOffsets(JNIEnv* env, jobject thiz, jint x, jint y)
{
    g_xOffset = x;
	g_yOffset = y;
}

void addResourceLocation(JNIEnv *env, jobject thiz, jstring name, jstring group)
{
}

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv *env;

    LOGI("JNI_OnLoad called");
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK)
    {
    	LOGE("Failed to get the environment using GetEnv()");
        return -1;
    }
    JNINativeMethod methods[] =
    {
		{
            "init",
            "()Z",
            (void *) init
        },
        {
        	"render",
			"(IIZ)Z",
			(void *) render
        },
        {
			"inputEvent",
			"(IFFLandroid/view/MotionEvent;)Z",
			(void *) inputEvent

        },
        {
            "keyEvent",
            "(IIILandroid/view/KeyEvent;)Z",
            (void *) keyEvent
        },
        {
            "cleanup",
            "()V",
            (void *) cleanup
        },
		{
			"setOffsets",
			"(II)V",
			(void *) setOffsets
		},
		{
			"addResourceLocation",
			"(Ljava/lang/String;Ljava/lang/String;)V",
			(void *) addResourceLocation
		},
    };
    jclass k;
    k = (env)->FindClass ("org/ogre/samples/OgreSampleBrowserActivity");
    (env)->RegisterNatives(k, methods, 7);

    return JNI_VERSION_1_4;
}


