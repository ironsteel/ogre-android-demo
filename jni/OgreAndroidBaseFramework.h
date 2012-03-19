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
#include "AndroidLogListener.h"

class OgreAndroidBaseFramework
{

public:
    OgreAndroidBaseFramework()
    {
        mRenderWindow = NULL;
        mGles2Plugin = NULL;
        mPfxPlugin = NULL;
        mLastTime = 0;
    }
    
    virtual ~OgreAndroidBaseFramework()
    {
        destroyRenderWindow();
        if(mRoot) delete mRoot;
        if(mGles2Plugin) delete mGles2Plugin;
        if(mPfxPlugin) delete mPfxPlugin;
        
    }
    
    void destroyRenderWindow()
    {
        
        if(mRenderWindow) {
            mRenderWindow->destroy();
            Ogre::Root::getSingleton().detachRenderTarget(mRenderWindow);
            
            mRenderWindow = NULL; 
        }
    }
    
    bool initOgreRoot();
    
    
private:
    Ogre::Root *mRoot;
    Ogre::GLES2Plugin *mGles2Plugin;
    Ogre::ParticleFXPlugin *mPfxPlugin;
    AndroidLogListener *mAndroidLogListener;
    Ogre::RenderWindow *mRenderWindow; // The main render window

    Ogre::Timer mTimer;
    unsigned long mLastTime;
    
};

#endif // OGREANDROIDBASEFRAMEWORK_H
