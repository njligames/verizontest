//
//  TornadoData.cpp
//  VerizonTest
//
//  Created by James Folk on 6/25/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#include "TornadoData.hpp"
#include "btTransform.h"

namespace njli
{
    TornadoData::TornadoData():
    m_Rotation(0),
    m_BaseTranslation(new btTransform(btTransform::getIdentity())),
    m_TranslationOffset(new btVector3(0,0,0)),
    m_MaxDegrees(10.0f),
    m_BaseDegrees(0.0f)
    {
        m_BaseTranslation->setOrigin(*m_TranslationOffset);
    }
    
    TornadoData::~TornadoData()
    {
        delete m_TranslationOffset;
        m_TranslationOffset = NULL;
        
        delete m_BaseTranslation;
        m_BaseTranslation = NULL;
    }
    
    void TornadoData::update(float timestep)
    {
        m_BaseTranslation->setRotation(btQuaternion(btVector3(0.0f, 1.0f, 0.0f), m_Rotation));
        
        m_Rotation += (timestep * 0.001f);
        if(m_Rotation > btRadians(m_MaxDegrees))
            m_Rotation = btRadians(m_MaxDegrees);
    }
    
    void TornadoData::setTranslationOffset(const btVector3 &offset)
    {
        *m_TranslationOffset = offset;
        m_BaseTranslation->setOrigin(*m_TranslationOffset);
    }
    
    const btTransform &TornadoData::getBaseTransform()const
    {
        return *m_BaseTranslation;
    }
    
    void TornadoData::setBaseDegreesPerTimeStep(const float base)
    {
        m_BaseDegrees = base;
    }
    
    float TornadoData::getBaseDegreesPerTimeStep()const
    {
        return m_BaseDegrees;
    }
    
    void TornadoData::setMaxDegreesPerTimestep(const float max)
    {
        m_MaxDegrees = max;
    }
    
    float TornadoData::getMaxDegreesPerTimestep()const
    {
        return m_MaxDegrees;
    }
}
