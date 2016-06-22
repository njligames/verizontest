//
//  CubeGeometry.hpp
//  VerizonTest
//
//  Created by James Folk on 6/21/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#ifndef CubeGeometry_hpp
#define CubeGeometry_hpp

#import <OpenGLES/ES2/glext.h>
#import <OpenGLES/ES2/gl.h>

namespace NJLI
{
    class CubeGeometry
    {
    public:
        /* members */
        CubeGeometry();
        CubeGeometry(const CubeGeometry &rhs);
        const CubeGeometry &operator=(const CubeGeometry &rhs);
        ~CubeGeometry();
    protected:
    private:
    };
}

#endif /* CubeGeometry_hpp */
