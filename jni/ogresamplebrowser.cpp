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

#define  LOG_TAG    "libogresamplebrowser"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

class AndroidMultiTouch : public OIS::MultiTouch
{
public:
	AndroidMultiTouch():OIS::MultiTouch("DWM", false, 0, 0){}
	
	/** @copydoc Object::setBuffered */
	virtual void setBuffered(bool buffered){}

	/** @copydoc Object::capture */
	virtual void capture(){}

	/** @copydoc Object::queryInterface */
	virtual OIS::Interface* queryInterface(OIS::Interface::IType type) {return 0;}

	/** @copydoc Object::_initialize */
	virtual void _initialize(){}
	
	OIS::MultiTouchState &getMultiTouchState(int i){
		while(i >= mStates.size()){
			OIS::MultiTouchState state;
			state.width = getRenderWindow()->getWidth();
			state.height = getRenderWindow()->getHeight();
			mStates.push_back(state);
		}
		return mStates[i];
	}
};

class AndroidKeyboard : public OIS::Keyboard
{
public:
	AndroidKeyboard():OIS::Keyboard("DWM", false, 1, 0){}
	
	/** @copydoc Object::setBuffered */
	virtual void setBuffered(bool buffered){}

	/** @copydoc Object::capture */
	virtual void capture(){}

	/** @copydoc Object::queryInterface */
	virtual OIS::Interface* queryInterface(OIS::Interface::IType type) {return 0;}

	/** @copydoc Object::_initialize */
	virtual void _initialize(){}
	
	virtual bool isKeyDown( OIS::KeyCode key ) const{
		return false;
	}
	
	virtual const std::string& getAsString( OIS::KeyCode kc ){
		static std::string defstr = "";
		return defstr;
	}
	
	virtual void copyKeyStates( char keys[256] ) const{
	
	}
};


static AndroidMultiTouch *g_multiTouch = 0;
static AndroidKeyboard *g_keyboard = 0;
static bool g_rootInit = false;
static int g_xOffset = 0, g_yOffset = 0;
static Ogre::AndroidArchiveFactory *g_archiveFactory = 0;
static Ogre::map<Ogre::String,Ogre::String>::type g_resourceMap;
static Ogre::SceneManager *sceneManager = 0;
static Ogre::Camera*       m_pCamera = 0;
static Ogre::Viewport*       m_pViewport = 0;
static Ogre::SceneNode*      m_pCubeNode;
static Ogre::Entity*       m_pCubeEntity;
static OgreBites::SdkCameraMan *cameraMan = 0;


static void injectTouchEvent(int pointerId, int action, float x, float y){
  if(cameraMan){
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
			state.X.abs = g_xOffset + (int)-x;
			state.X.rel = state.X.abs - last;
			
			last = state.Y.abs;
			state.Y.abs = g_yOffset + (int)-y;
			state.Y.rel = state.Y.abs - last;
			
			//last = state.Z.abs;
			state.Z.abs = 0;
			state.Z.rel = 0;
			
			OIS::MultiTouchEvent evt(g_multiTouch, state);
			
			switch(state.touchType){
			case OIS::MT_Pressed:
			      cameraMan->injectMouseDown(evt);
				break;
			case OIS::MT_Released:
			      cameraMan->injectMouseUp(evt);
				break;
			case OIS::MT_Moved:
			      cameraMan->injectMouseMove(evt);
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

jboolean render(JNIEnv* env, jobject thiz, jint drawWidth, jint drawHeight, jboolean forceRedraw)
{
	// Check that a render window even exists
	if(getRenderWindow() == 0){
		initRenderWindow(0, drawWidth, drawHeight, 0);
	}
	
	// Initialize the sample browser
	if(g_multiTouch == 0){
		g_multiTouch = new AndroidMultiTouch();
	}
	
	if(g_keyboard == 0){
		g_keyboard = new AndroidKeyboard();
	}

	if(sceneManager == 0) {
        Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Essential");
        sceneManager = Ogre::Root::getSingletonPtr()->createSceneManager(Ogre::ST_GENERIC, "SceneManager");  
        sceneManager->setAmbientLight(Ogre::ColourValue(0.0f, 0.0f, 1.0f));
    
        m_pCamera = sceneManager->createCamera("Camera");
        m_pCamera->setNearClipDistance(1);
        m_pCamera->setFarClipDistance(1000);
        m_pCamera->setPosition(Ogre::Vector3(0, 60, -220));
        m_pCamera->lookAt(Ogre::Vector3(0, 0, 0)); 
        
        m_pViewport = getRenderWindow()->addViewport(m_pCamera);
        m_pCamera->setAspectRatio(Ogre::Real(m_pViewport->getActualWidth()) / Ogre::Real(m_pViewport->getActualHeight())); 

    //m_pViewport->setBackgroundColour(Ogre::ColourValue(0.0f, 1.0f, 0.0f, 1.0f));
        m_pViewport->setCamera(m_pCamera);

        if(cameraMan == 0) {
        cameraMan = new OgreBites::SdkCameraMan(m_pCamera);
        cameraMan->setStyle(OgreBites::CS_ORBIT);
        }

        sceneManager->createLight("Light1")->setPosition(-150 , -150 , 350);
        sceneManager->getLight("Light1")->setDirection(Ogre::Vector3(0, 0, 0));
        sceneManager->getLight("Light1")->setDiffuseColour(Ogre::ColourValue(1.0, 0.0, 0.0));
        Ogre::MaterialPtr material = Ogre::MaterialManager::getSingletonPtr()->getByName("bumpmap");
        Ogre::String vertexProgram =  material.getPointer()->getTechnique(0)->getPass(0)->getVertexProgramName();
        LOGI("VertexProgramName");
        LOGI("====================================================================");
        LOGI(vertexProgram.c_str());
        LOGI("====================================================================");
        Ogre::MaterialPtr simpleMat = Ogre::MaterialManager::getSingletonPtr()->getByName("bumpmap");
        m_pCubeEntity = sceneManager->createEntity("Cube", "athene.mesh");
        m_pCubeEntity->setMaterial(simpleMat);
        m_pCubeNode = sceneManager->getRootSceneNode()->createChildSceneNode("CubeNode");
        m_pCubeNode->attachObject(m_pCubeEntity);
        cameraMan->setTarget(m_pCubeNode);

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


