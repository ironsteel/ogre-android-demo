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


#ifndef ANDROIDMULTITOUCH_H
#define ANDROIDMULTITOUCH_H

#include <OIS.h>


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
            state.width = mWindowWidth;
            state.height = mWindowHeight;
            mStates.push_back(state);
        }
        return mStates[i];
    }
    
    void setWindowSize(unsigned int width,unsigned int height)
    {
        mWindowWidth = width;
        mWindowHeight = height;
    }
    
    
private:
    unsigned int mWindowWidth;
    unsigned int mWindowHeight;
    
};
#endif // ANDROIDMULTITOUCH_H
