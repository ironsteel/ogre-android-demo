#include "ShaderGeneratorResolverListener.h"

ShaderGeneratorResolverListener::ShaderGeneratorResolverListener(Ogre::RTShader::ShaderGenerator *shaderGenerator)
{
	mShaderGenerator = shaderGenerator;
}

Ogre::Technique* ShaderGeneratorResolverListener::handleSchemeNotFound(unsigned short schemeIndex, 
                                                      const Ogre::String &schemeName, 
                                                      Ogre::Material *originalMaterial, 
                                                      unsigned short lodIndex, 
                                                      const Ogre::Renderable *rend)
{
	
	Ogre::Technique* generatedTech = NULL;

	// Case this is the default shader generator scheme.
	if (schemeName == Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME)
	{
		bool techniqueCreated;

		// Create shader generated technique for this material.
		techniqueCreated = mShaderGenerator->createShaderBasedTechnique(
			originalMaterial->getName(), 
			Ogre::MaterialManager::DEFAULT_SCHEME_NAME, 
			schemeName);	

		// Case technique registration succeeded.
		if (techniqueCreated)
		{
			// Force creating the shaders for the generated technique.
			mShaderGenerator->validateMaterial(schemeName, originalMaterial->getName());
			
			// Grab the generated technique.
			Ogre::Material::TechniqueIterator itTech = originalMaterial->getTechniqueIterator();

			while (itTech.hasMoreElements())
			{
				Ogre::Technique* curTech = itTech.getNext();

				if (curTech->getSchemeName() == schemeName)
				{
					generatedTech = curTech;
					break;
				}
			}				
		}
	}

	return generatedTech;
}
