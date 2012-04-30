#ifndef SHADERGENERATORRESOLVERLISTENER_H
#define SHADERGENERATORRESOLVERLISTENER_H
#include "OgreMaterialManager.h"
#include "OgreRTShaderSystem.h"


class ShaderGeneratorResolverListener : public Ogre::MaterialManager::Listener
{
public:
	ShaderGeneratorResolverListener(Ogre::RTShader::ShaderGenerator *shaderGenerator);
	virtual Ogre::Technique* handleSchemeNotFound(unsigned short schemeIndex, 
	                                              const Ogre::String &schemeName, 
	                                              Ogre::Material *originalMaterial, 
	                                              unsigned short lodIndex, 
	                                              const Ogre::Renderable *rend);
	
protected:
	Ogre::RTShader::ShaderGenerator* mShaderGenerator;
};

#endif // SHADERGENERATORRESOLVERLISTENER_H
