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
#include "OgreRTShaderSystem.h"
template <> OgreAndroidBaseFramework* Ogre::Singleton<OgreAndroidBaseFramework>::msSingleton = 0;

OgreAndroidBaseFramework* OgreAndroidBaseFramework::getSingletonPtr(void) 
{
	if(msSingleton)
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
		
		mAndroidArchiveFactory = new AndroidArchiveFactory(mJni);
		Ogre::ArchiveManager::getSingletonPtr()->addArchiveFactory(mAndroidArchiveFactory);
		
		createResourceGroup("Essential");
		
		// From android assets
		addResourceLocation("RTShaderLib.mp3", "Essential", "Android");
		
		// From filesystem
		addResourceLocation("/sdcard/ogre_media/textures", "Essential", "FileSystem");
		addResourceLocation("/sdcard/ogre_media/shaders", "Essential", "FileSystem");
		addResourceLocation("/sdcard/ogre_media/materials", "Essential", "FileSystem");
		addResourceLocation("/sdcard/ogre_media/meshes", "Essential", "FileSystem");
		addResourceLocation("/sdcard/ogre_media/trays", "Essential", "FileSystem");
		addResourceLocation("/sdcard/ogre_media/font-defs", "Essential", "FileSystem");
		addResourceLocation("/sdcard/ogre_media/trays-materials", "Essential", "FileSystem");
		
	    mRoot->addFrameListener(this);
		mKeyboard = new AndroidKeyboard();
		return true;
		
	} catch(Ogre::Exception &e) {
	}
	
	return false;
}


void OgreAndroidBaseFramework::createScene()
{
	 mSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC);
	 Ogre::TextureManager::getSingletonPtr()->setDefaultNumMipmaps(0);
	 mCamera = mSceneManager->createCamera("Camera");
	 mViewport = mRenderWindow->addViewport(mCamera);
	 
	 mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight())); 
	 
	 mViewport->setCamera(mCamera);
	 if(Ogre::RTShader::ShaderGenerator::initialize()) {
		 Ogre::RTShader::ShaderGenerator* shaderGen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
		 shaderGen->addSceneManager(mSceneManager);
	 }
	 mTechniqueResolver = new ShaderGeneratorResolverListener(Ogre::RTShader::ShaderGenerator::getSingletonPtr());
	 Ogre::MaterialManager::getSingletonPtr()->addListener(mTechniqueResolver);
   
	 
	 mCameraMan = new OgreBites::SdkCameraMan(mCamera);
	 mCameraMan->setStyle(OgreBites::CS_MANUAL);
	 mTrays = new OgreBites::SdkTrayManager("Sdk", mRenderWindow, NULL);
	 mTrays->setListener(this);
	 mTrays->hideLogo();
	 mTrays->showCursor();

	 mTrays->createButton(OgreBites::TL_BOTTOMRIGHT, "Forward", "Forward");
	 mTrays->createButton(OgreBites::TL_BOTTOMLEFT, "Back", "Back");
	 mSceneManager->setSkyBox(true, "SkyBox", 30, true);
	 mCharacter = new SinbadCharacterController(mCamera);
}



void OgreAndroidBaseFramework::initializeResourceGroups()
{
	
	for(Ogre::map<Location, Value>::type::iterator i = mResourceMap.begin(); i != mResourceMap.end(); ++i)
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(i->first, i->second.second, i->second.first);  
    }
	
	Ogre::ResourceGroupManager::getSingletonPtr()->initialiseAllResourceGroups();
	
}

void OgreAndroidBaseFramework::buttonHit(Button *button)
{
	if(button->getName() == "Forward") {
		OIS::KeyEvent evt(mKeyboard, OIS::KC_W, 0);
		mCharacter->injectKeyDown(evt);
		return;
	}
	
	if(button->getName() == "Back") {
		OIS::KeyEvent evt(mKeyboard, OIS::KC_S, 0);
		mCharacter->injectKeyDown(evt);
		return;
	}
}

void OgreAndroidBaseFramework::addResourceLocation(Location path, GroupName name, ArchiveType type)
{
	
	mResourceMap[path] = Value(name, type);
}

void OgreAndroidBaseFramework::injectTouchDown(OIS::MultiTouchEvent &evt)
{
	
	if(mTrays) {
		mTrays->injectMouseDown(evt);
	}
	
	if(mCharacter) {
		mCharacter->injectMouseDown(evt);
	}
	/*if(mCameraMan) {
		mCameraMan->injectMouseDown(evt);
	}*/
}

void OgreAndroidBaseFramework::injectTouchUp(OIS::MultiTouchEvent &evt)
{
	if(mTrays) {
		mTrays->injectMouseUp(evt);
	}
	if(mCameraMan) {
		mCameraMan->injectMouseUp(evt);
	}

}

void OgreAndroidBaseFramework::injectTouchMove(OIS::MultiTouchEvent &evt)
{
	if(mTrays) {
		mTrays->injectMouseMove(evt);
	}
	if(mCharacter) {
		mCharacter->injectMouseMove(evt);
	}
	/*if(mCameraMan) {
		mCameraMan->injectMouseMove(evt);
	}*/
}




void OgreAndroidBaseFramework::initRenderWindow(unsigned int windowHandle, unsigned int width, unsigned int height, unsigned int contextHandle)
{
	if(!mRenderWindow) {
		Ogre::NameValuePairList params;
		params["externalWindowHandle"] = Ogre::StringConverter::toString(windowHandle);
		params["externalGLContext"] = Ogre::StringConverter::toString(contextHandle);
		
		mRenderWindow = mRoot->createRenderWindow("OgreAndroidPrimary", width, height, true, &params);
	}
}

