/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 3      *
*                (c) 2006-2008 MGH, INRIA, USTL, UJF, CNRS                    *
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
*                               SOFA :: Modules                               *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_COMPONENT_INTERACTIONFORCEFIELD_ConstantForceField_INL
#define SOFA_COMPONENT_INTERACTIONFORCEFIELD_ConstantForceField_INL

#include <sofa/core/componentmodel/behavior/ForceField.inl>
#include "ConstantForceField.h"
#include <sofa/helper/system/config.h>
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/helper/gl/template.h>
#include <assert.h>
#include <iostream>
#include <sofa/helper/gl/BasicShapes.h>




namespace sofa
{

namespace component
{

namespace forcefield
{


template<class DataTypes>
ConstantForceField<DataTypes>::ConstantForceField()
    : points(initData(&points, "points", "points where the forces are applied"))
    , forces(initData(&forces, "forces", "applied forces"))
    , arrowSizeCoef(initData(&arrowSizeCoef,0.0, "arrowSizeCoef", "Size of the drawn arrows (0->no arrows, sign->direction of drawing"))
{}


template<class DataTypes>
void ConstantForceField<DataTypes>::addForce(VecDeriv& f1, const VecCoord& p1, const VecDeriv& )
{
    f1.resize(p1.size());
    const VecIndex& indices = points.getValue();
    const VecDeriv& f = forces.getValue();
    unsigned int i = 0;
    for (; i<f.size(); i++)
    {
        f1[indices[i]]+=f[i];
    }
    for (; i<indices.size(); i++)
    {
        f1[indices[i]]+=f[f.size()-1];
    }
}



template <class DataTypes>
double ConstantForceField<DataTypes>::getPotentialEnergy(const VecCoord& x)
{
    const VecIndex& indices = points.getValue();
    const VecDeriv& f = forces.getValue();
    double e=0;
    unsigned int i = 0;
    for (; i<f.size(); i++)
    {
        e -= f[i]*x[indices[i]];
    }
    for (; i<indices.size(); i++)
    {
        e -= f[f.size()-1]*x[indices[i]];
    }
    return e;
}

template <class DataTypes>
void ConstantForceField<DataTypes>::setForce( unsigned i, const Deriv& force )
{
    VecIndex& indices = *points.beginEdit();
    VecDeriv& f = *forces.beginEdit();
    indices.push_back(i);
    f.push_back( force );
    points.endEdit();
    forces.endEdit();
}


#ifndef SOFA_FLOAT
template <>
double ConstantForceField<defaulttype::Rigid3dTypes>::getPotentialEnergy(const VecCoord& );
template <>
double ConstantForceField<defaulttype::Rigid2dTypes>::getPotentialEnergy(const VecCoord& );
#endif

#ifndef SOFA_DOUBLE
template <>
double ConstantForceField<defaulttype::Rigid3fTypes>::getPotentialEnergy(const VecCoord& );
template <>
double ConstantForceField<defaulttype::Rigid2fTypes>::getPotentialEnergy(const VecCoord& );
#endif


template<class DataTypes>
void ConstantForceField<DataTypes>::draw()
{
    if (!getContext()->getShowForceFields()) return;  /// \todo put this in the parent class
    const VecIndex& indices = points.getValue();
    const VecDeriv& f = forces.getValue();
    const VecCoord& x = *this->mstate->getX();

    double aSC = arrowSizeCoef.getValue();

    if( fabs(aSC)<1.0e-10 )
    {
        glDisable(GL_LIGHTING);
        glBegin(GL_LINES);
        glColor3f(0,1,0);
        for (unsigned int i=0; i<indices.size(); i++)
        {
            Real xx,xy,xz,fx,fy,fz;
            DataTypes::get(xx,xy,xz,x[indices[i]]);
            DataTypes::get(fx,fy,fz,f[(i<f.size()) ? i : f.size()-1]);
            glVertex3f( (GLfloat)xx, (GLfloat)xy, (GLfloat)xz );
            glVertex3f( (GLfloat)(xx+fx), (GLfloat)(xy+fy), (GLfloat)(xz+fz) );
        }
        glEnd();
    }
    else
    {
        glEnable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);
        glColor3f(1,.4,.4);
        for (unsigned int i=0; i<indices.size(); i++)
        {
            Real xx,xy,xz,fx,fy,fz;
            DataTypes::get(xx,xy,xz,x[indices[i]]);
            DataTypes::get(fx,fy,fz,f[(i<f.size()) ? i : f.size()-1]);


            defaulttype::Vec3f p1( xx, xy, xz);
            defaulttype::Vec3f p2( aSC*fx+xx, aSC*fy+xy, aSC*fz+xz );

            float norm = (p2-p1).norm();

            if( aSC > 0)
            {
                helper::gl::drawArrow( p1,p2, norm/20.0);
            }
            else
            {
                helper::gl::drawArrow( p2,p1, norm/20.0);
            }
        }
        glDisable(GL_LIGHTING);
        glDisable(GL_COLOR_MATERIAL);
    }
}


template <class DataTypes>
bool ConstantForceField<DataTypes>::addBBox(double*, double* )
{
    return false;
}

} // namespace forcefield

} // namespace component

} // namespace sofa

#endif



