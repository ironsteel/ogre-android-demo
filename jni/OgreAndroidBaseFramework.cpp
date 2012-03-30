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


#include "OgreAndroidBaseFramework.h"

template <> OgreAndroidBaseFramework* Ogre::Singleton<OgreAndroidBaseFramework>::msSingleton = 0;

OgreAndroidBaseFramework* OgreAndroidBaseFramework::getSingletonPtr(void) 
{
	if(!msSingleton)
		msSingleton = new OgreAndroidBaseFramework();
	return msSingleton;
}


bool OgreAndroidBaseFramework::initOgreRoot()
{
	try {
		// Create logs that funnel to android logs
		mLogManager = OGRE_NEW Ogre::LogManager();
		Ogre::Log *l = mLogManager->createLog("AndroidLog", true, true, true);
		mAndroidLogListener = new AndroidLogListener();
		l->addListener(mAndroidLogListener);

		
		// Create a root object
		mRoot = new Ogre::Root("", "", "");
		
		// Register the ES2 plugin
		mGles2Plugin = new Ogre::GLES2Plugin();
		Ogre::Root::getSingleton().installPlugin(mGles2Plugin);
		
		// Register particle plugin
		mPfxPlugin = new Ogre::ParticleFXPlugin();
		Ogre::Root::getSingleton().installPlugin(mPfxPlugin);
		
		// Grab the available render systems
		const Ogre::RenderSystemList &renderSystemList = mRoot->getAvailableRenderers();
		if(renderSystemList.empty())
			return false;
		
		// Set the render system and init
		Ogre::RenderSystem *system = renderSystemList.front();
		mRoot->setRenderSystem(system);
		mRoot->initialise(false);
		
		mLastTime = mTimer.getMilliseconds();

		
//		initRenderWindow(0, mWindowWidth, mWindowHeight, 0);
	
		
		
		
		
		return true;
		
	} catch(Ogre::Exception &e) {
	}
	
	return false;
}

void OgreAndroidBaseFramework::initRenderWindow(unsigned int windowHandle, unsigned int width, unsigned int height, unsigned int contextHandle)
{
	if(!mRenderWindow) {
		Ogre::NameValuePairList params;
		params["externalWindowHandle"] = Ogre::StringConverter::toString(windowHandle);
		params["externalGLContext"] = Ogre::StringConverter::toString(contextHandle);
		
		mRenderWindow = mRoot->createRenderWindow("OgreAndroidPrimary", width, height, true, &params);
		mRenderWindow->addListener(this);
	}
}

void OgreAndroidBaseFramework::preViewportUpdate(const Ogre::RenderTargetViewportEvent &evt)
{
	
}

void OgreAndroidBaseFramework::preRenderTargetUpdate(const Ogre::RenderTargetEvent &evt)
{
	mSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC);
	
	mCamera = mSceneManager->createCamera("Camera");
    mCamera->setNearClipDistance(1);
    mCamera->setFarClipDistance(1000);
    mCamera->setPosition(Ogre::Vector3(0, 10, 0));
    mCamera->lookAt(Ogre::Vector3(0, 0, 100)); 
	mViewport = getRenderWindow()->addViewport(mCamera);
	mCamera->setAspectRatio(Ogre::Real(mWindowWidth) / Ogre::Real(mWindowHeight)); 
    mViewport->setCamera(mCamera);
	mViewport->setBackgroundColour(Ogre::ColourValue(0.0, 1.0, 0.0));
		
}
