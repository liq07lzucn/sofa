/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2016 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to the Free Software Foundation,     *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
*******************************************************************************
*                              SOFA :: Framework                              *
*                                                                             *
* Authors: The SOFA Team (see Authors.txt)                                    *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/

#ifndef SOFA_CORE_LOADER_MATERIAL_H_
#define SOFA_CORE_LOADER_MATERIAL_H_

#include <sofa/core/core.h>
#include <sofa/defaulttype/Vec.h>
#include <sofa/core/objectmodel/DataFileName.h>
#include <sofa/helper/system/FileRepository.h>

namespace sofa
{

namespace core
{

namespace loader
{

class Material
{
public:
    std::string 	name;		/* name of material */
    defaulttype::Vec4f  diffuse ;	/* diffuse component */
    defaulttype::Vec4f  ambient ;	/* ambient component */
    defaulttype::Vec4f  specular;	/* specular component */
    defaulttype::Vec4f  emissive;	/* emmissive component */
    float  shininess;	/* specular exponent */
    bool   useDiffuse;
    bool   useSpecular;
    bool   useAmbient;
    bool   useEmissive;
    bool   useShininess;
    bool   useTexture;
    bool   useBumpMapping;
    bool   activated;
    std::string   textureFilename; // path to the texture linked to the material
    std::string   bumpTextureFilename; // path to the bump texture linked to the material

    void setColor(float r, float g, float b, float a)
    {
        ambient = defaulttype::Vec4f(r*0.2f,g*0.2f,b*0.2f,a);
        diffuse = defaulttype::Vec4f(r,g,b,a);
        specular = defaulttype::Vec4f(r,g,b,a);
        emissive = defaulttype::Vec4f(r,g,b,a);
    }

    inline friend std::ostream& operator << (std::ostream& out, const Material& m )
    {
        out   << m.name         << " ";
        out  << "Diffuse"       << " " <<  m.useDiffuse   << " " <<  m.diffuse      << " ";
        out  << "Ambient"       << " " <<  m.useAmbient   << " " <<  m.ambient      << " ";
        out  << "Specular"      << " " <<  m.useSpecular  << " " <<  m.specular     << " ";
        out  << "Emissive"      << " " <<  m.useEmissive  << " " <<  m.emissive     << " ";
        out  << "Shininess"     << " " <<  m.useShininess << " " <<  m.shininess   << " ";
        /*
              if (m.useTexture)
              {
                  out << "Texture linked to the material : " << m.textureFilename << " ";
              }
              else
              {
                  out << "No texture linked to the material ";
              }

              if (m.useBumpMapping)
              {
                  out << "Bump texture linked to the material : " << m.bumpTextureFilename << " ";
              }
              else
              {
                  out << "No bump texture linked to the material ";
              }
        */
        return out;
    }
    inline friend std::istream& operator >> (std::istream& in, Material &m )
    {

        std::string element;
        in  >>  m.name ;
        for (unsigned int i=0; i<5; ++i)
        {
            in  >>  element;
            if      (element == std::string("Diffuse")   || element == std::string("diffuse")   ) { in  >>  m.useDiffuse   ; in >> m.diffuse;   }
            else if (element == std::string("Ambient")   || element == std::string("ambient")   ) { in  >>  m.useAmbient   ; in >> m.ambient;   }
            else if (element == std::string("Specular")  || element == std::string("specular")  ) { in  >>  m.useSpecular  ; in >> m.specular;  }
            else if (element == std::string("Emissive")  || element == std::string("emissive")  ) { in  >>  m.useEmissive  ; in >> m.emissive;  }
            else if (element == std::string("Shininess") || element == std::string("shininess") ) { in  >>  m.useShininess ; in >> m.shininess; }
        }
        return in;
    }

    Material()
    {
        ambient =  defaulttype::Vec4f( 0.2f,0.2f,0.2f,1.0f);
        diffuse =  defaulttype::Vec4f( 0.75f,0.75f,0.75f,1.0f);
        specular =  defaulttype::Vec4f( 1.0f,1.0f,1.0f,1.0f);
        emissive =  defaulttype::Vec4f( 0.0f,0.0f,0.0f,0.0f);

        shininess =  45.0f;
        name = "Default";
        useAmbient =  true;
        useDiffuse =  true;
        useSpecular =  false;
        useEmissive =  false;
        useShininess =  false;
        activated = false;

        useTexture = false;
        textureFilename ="DEFAULT";

        useBumpMapping = false;
        bumpTextureFilename ="DEFAULT";
    }

    Material(const Material& mat)
    {
        ambient =  mat.ambient;
        diffuse =  mat.diffuse;
        specular =  mat.specular;
        emissive =  mat.emissive;

        shininess =  mat.shininess;
        name = mat.name;
        useAmbient =  mat.useAmbient;
        useDiffuse =  mat.useDiffuse ;
        useSpecular =  mat.useSpecular ;
        useEmissive =  mat.useEmissive;
        useShininess =  mat.useShininess ;
        activated = mat.activated;

        useTexture = mat.useTexture;
        textureFilename = mat.textureFilename;

        useBumpMapping = mat.useBumpMapping;
        bumpTextureFilename = mat.bumpTextureFilename;
    }


    friend size_t hash_value(const Material& c)
    {
        size_t hash = boost::hash<std::string>()(c.name);
        boost::hash_combine( hash, c.diffuse );
        boost::hash_combine( hash, c.ambient );
        boost::hash_combine( hash, c.specular );
        boost::hash_combine( hash, c.emissive );
        boost::hash_combine( hash, c.shininess );
        boost::hash_combine( hash, c.useDiffuse );
        boost::hash_combine( hash, c.useSpecular );
        boost::hash_combine( hash, c.useAmbient );
        boost::hash_combine( hash, c.useEmissive );
        boost::hash_combine( hash, c.useShininess );
        boost::hash_combine( hash, c.useTexture );
        boost::hash_combine( hash, c.useBumpMapping );
        boost::hash_combine( hash, c.activated );
        boost::hash_combine( hash, c.textureFilename );
        boost::hash_combine( hash, c.bumpTextureFilename );
        return hash;
    }
};

} // namespace loader

} // namespace core

} // namespace sofa

#endif /* SOFA_CORE_LOADER_MATERIAL_H_ */