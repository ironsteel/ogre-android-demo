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


#ifndef ANDROIDKEYBOARD_H
#define ANDROIDKEYBOARD_H

#include <OIS.h>

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

#endif // ANDROIDKEYBOARD_H
