/*
    Copyright 2012 <copyright holder> <email>

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/


#ifndef OGREANDROIDBASEFRAMEWORK_H
#define OGREANDROIDBASEFRAMEWORK_H

#include <Ogre.h>
#include <OgreGLES2Plugin.h>
#include <OgreParticleFXPlugin.h>
#include <OgreRenderTargetListener.h>
#include "AndroidLogListener.h"
#include "SdkCameraMan.h"
#include "OISMultiTouch.h"

class OgreAndroidBaseFramework : public Ogre::Singleton<OgreAndroidBaseFramework>, Ogre::FrameListener
{

public:
    OgreAndroidBaseFramework()
    {
        mRenderWindow = NULL;
        mGles2Plugin = NULL;
        mPfxPlugin = NULL;
		mLogManager = NULL;
		mSceneManager = NULL;
        mLastTime = 0;
    }
    
    virtual ~OgreAndroidBaseFramework()
    {
        destroyRenderWindow();
		
        if(mRoot) delete mRoot;
        if(mGles2Plugin) delete mGles2Plugin;
        if(mPfxPlugin) delete mPfxPlugin;
		if(mLogManager) delete mLogManager;
		if(mCameraMan) delete mCameraMan;
        
    }
    
    void destroyRenderWindow()
    {
        
        if(mRenderWindow) {
            mRenderWindow->destroy();
            Ogre::Root::getSingleton().detachRenderTarget(mRenderWindow);
            mRenderWindow = NULL; 
        }
    }
	
	Ogre::RenderWindow* getRenderWindow() 
	{
		return mRenderWindow;
	}
    
    bool initOgreRoot();
	
	void initRenderWindow(unsigned int windowHandle, unsigned int width, unsigned int height, unsigned int contextHandle);
	
	
	void addResourceLocation(Ogre::String path, Ogre::String resourceGroup);
	
	void injectTouchUp(OIS::MultiTouchEvent &evt);
	void injectTouchDown(OIS::MultiTouchEvent &evt);
	void injectTouchMove(OIS::MultiTouchEvent &evt);
	
	void createResourceGroup(Ogre::String resourceGroup) 
	{
		Ogre::ResourceGroupManager::getSingletonPtr()->createResourceGroup(resourceGroup);
	}
	
	void initializeResourceGroups();
	
	void renderOneFrame()
	{
		if(!mSceneManager) {
			initializeResourceGroups();
			createScene();
		}
		mRoot->renderOneFrame();
	}
	
	void createScene();
	
	
	
	
	static OgreAndroidBaseFramework* getSingletonPtr(void);
    
private:
    Ogre::Root *mRoot;
    Ogre::GLES2Plugin *mGles2Plugin;
    Ogre::ParticleFXPlugin *mPfxPlugin;
    AndroidLogListener *mAndroidLogListener;
    Ogre::RenderWindow *mRenderWindow; // The main render window
	Ogre::LogManager *mLogManager;
	Ogre::SceneManager *mSceneManager;
	Ogre::Camera *mCamera;
	Ogre::Viewport *mViewport;
	OgreBites::SdkCameraMan *mCameraMan;
	
	Ogre::Entity *mRazorEntity;
	Ogre::SceneNode *mRazorNode;
	
	Ogre::map<Ogre::String,Ogre::String>::type mResourceMap;
	
    Ogre::Timer mTimer;
	
	
    unsigned long mLastTime;
};

#endif // OGREANDROIDBASEFRAMEWORK_H
