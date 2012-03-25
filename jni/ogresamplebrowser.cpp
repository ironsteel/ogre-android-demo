#include <jni.h>
#include <stdlib.h>
#include <iostream>
#include <stdexcept>
#include <string>

#include "ogrewrapper.h"
#include <OIS.h>
#include <OgreSceneManager.h>
#include "SdkCameraMan.h"
#include <android/log.h>
#include "AndroidArchive.h"
#include "AndroidMultiTouch.h"
#include "AndroidKeyboard.h"



#define  LOG_TAG    "libogresamplebrowser"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)




static AndroidMultiTouch *g_multiTouch = 0;
static AndroidKeyboard *g_keyboard = 0;
static bool g_rootInit = false;
static int g_xOffset = 0, g_yOffset = 0;
static Ogre::AndroidArchiveFactory *g_archiveFactory = 0;
static Ogre::map<Ogre::String,Ogre::String>::type g_resourceMap;
static Ogre::SceneManager *mSceneManager = 0;
static Ogre::Camera*       mCamera = 0;
static Ogre::Viewport*       mViewport = 0;
static Ogre::SceneNode*      mOgreHeadNode;
static Ogre::Entity*       mOgreHeadEntity;
static OgreBites::SdkCameraMan *mCameraMan = 0;


static void injectTouchEvent(int pointerId, int action, float x, float y){
  if(mCameraMan){
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
			
			switch(state.touchType){
			case OIS::MT_Pressed:
			      mCameraMan->injectMouseDown(evt);
				break;
			case OIS::MT_Released:
			      mCameraMan->injectMouseUp(evt);
				break;
			case OIS::MT_Moved:
			      mCameraMan->injectMouseMove(evt);
				break;
			case OIS::MT_Cancelled:
			
				break;
			}
		}
	}
}

static void injectKeyEvent(int action, int keyCode){
}
 
jboolean init(JNIEnv* env, jobject thiz)
{
	bool ret = initOgreRoot();
	if(!ret)
		return JNI_FALSE;
	
	g_rootInit = true;

	g_archiveFactory = OGRE_NEW Ogre::AndroidArchiveFactory(env);
	Ogre::ArchiveManager::getSingleton().addArchiveFactory(g_archiveFactory);
	Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Essential");
	
	for(Ogre::map<Ogre::String,Ogre::String>::type::iterator i = g_resourceMap.begin(); i != g_resourceMap.end(); ++i)
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(i->first, "FileSystem", i->second);  
	
    }

	LOGI("Adding resource locations");
}

static void init_scene_manager()
{
    mSceneManager = Ogre::Root::getSingletonPtr()->createSceneManager(Ogre::ST_GENERIC, "SceneManager");  
    mSceneManager->setAmbientLight(Ogre::ColourValue(0.0f, 1.0f, 1.0f));
}

static void init_camera()
{
    mCamera = mSceneManager->createCamera("Camera");
    mCamera->setNearClipDistance(1);
    mCamera->setFarClipDistance(1000);
    mCamera->setPosition(Ogre::Vector3(0, 10, 0));
    mCamera->lookAt(Ogre::Vector3(0, 0, 100)); 
    mViewport = getRenderWindow()->addViewport(mCamera);
    mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight())); 
    mViewport->setCamera(mCamera);
    
}

static void init_sdk_camera_manager()
{
    if(mCameraMan == 0) {
        mCameraMan = new OgreBites::SdkCameraMan(mCamera);
        mCameraMan->setStyle(OgreBites::CS_ORBIT);
    }
}

static void create_lights()
{
    mSceneManager->createLight("Light1")->setPosition(0 , 150 , 150);
	mSceneManager->getLight("Light1")->setType(Ogre::Light::LT_POINT);
    mSceneManager->getLight("Light1")->setDiffuseColour(Ogre::ColourValue(0.5, 0.5, 0.5));
}

static void create_jaiqua_node()
{
    Ogre::MaterialPtr simpleMat = Ogre::MaterialManager::getSingletonPtr()->getByName("diffuseLighting");
    mOgreHeadEntity = mSceneManager->createEntity("ogreHead", "ogrehead.mesh");
    mOgreHeadEntity->setMaterial(simpleMat);
    mOgreHeadNode = mSceneManager->getRootSceneNode()->createChildSceneNode("ogreHeadNode");
    mOgreHeadNode->attachObject(mOgreHeadEntity);
}



jboolean render(JNIEnv* env, jobject thiz, jint drawWidth, jint drawHeight, jboolean forceRedraw)
{
	// Check that a render window even exists
	if(getRenderWindow() == 0){
		initRenderWindow(0, drawWidth, drawHeight, 0);
	}
	
	// Initialize the sample browser
	if(g_multiTouch == 0){
		g_multiTouch = new AndroidMultiTouch();
        g_multiTouch->setWindowSize(drawWidth, drawHeight);
	}
	
	if(g_keyboard == 0){
		g_keyboard = new AndroidKeyboard();
	}

	if(mSceneManager == 0) {
        Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Essential");
        init_scene_manager();
        init_camera();
        init_sdk_camera_manager();
        create_lights();
        create_jaiqua_node();
        mCameraMan->setTarget(mOgreHeadNode);
    }
    
	renderOneFrame();
	
  
    return JNI_TRUE;
}

void cleanup(JNIEnv* env)
{
	
	if(g_multiTouch){
		delete g_multiTouch;
		g_multiTouch = 0;
	}
	
	if(g_keyboard){
		delete g_keyboard;
		g_keyboard = 0;
	}
	
	if(getRenderWindow()){
		destroyRenderWindow();
	}
		
	if(mCameraMan) {
        delete mCameraMan;
    }
    
	LOGI("deleting ogre root");
	if(g_rootInit){
		destroyOgreRoot();
		g_rootInit = false;
	}
	
	LOGI("deleting archive stuff");
	if(g_archiveFactory){
		OGRE_DELETE g_archiveFactory; 
		g_archiveFactory = 0;
	}
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
	LOGI("Adding resource location");
	
	const char *str1 = env->GetStringUTFChars(name, 0);
	const char *str2 = env->GetStringUTFChars(group, 0);
	g_resourceMap[str1] = str2;
	env->ReleaseStringUTFChars(name, str1);
	env->ReleaseStringUTFChars(group, str2);
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


