//
//  TornadoData.hpp
//  VerizonTest
//
//  Created by James Folk on 6/25/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#ifndef TornadoData_hpp
#define TornadoData_hpp

class btTransform;
class btVector3;

namespace njli
{
    class TornadoData
    {
    public:
        TornadoData();
        ~TornadoData();
        
        void update(float timestep);
        
        void setTranslationOffset(const btVector3 &offset);
        
        const btTransform &getBaseTransform()const;
        
        void setMaxDegreesPerTimestep(const float max);
    protected:
    private:
        float m_Rotation;
        btTransform *m_BaseTranslation;
        btVector3 *m_TranslationOffset;
        float m_MaxDegrees;
    };
}




#endif /* TornadoData_hpp */
