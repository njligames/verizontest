//
//  Camera.hpp
//  VerizonTest
//
//  Created by James Folk on 6/21/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

namespace NJLI
{
    class Camera
    {
    public:
        /* members */
        Camera();
        Camera(const Camera &rhs);
        const Camera &operator=(const Camera &rhs);
        ~Camera();
    protected:
    private:
    };
}

#endif /* Camera_hpp */
