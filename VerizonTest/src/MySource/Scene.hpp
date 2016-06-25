//
//  Scene.hpp
//  VerizonTest
//
//  Created by James Folk on 6/21/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <vector>

namespace njli
{
    class CubeGeometry;
    class Camera;
    class Node;
    
    class Scene
    {
    public:
        /* members */
        Scene();
        Scene(const Scene &rhs);
        const Scene &operator=(const Scene &rhs);
        ~Scene();
        
        void update(float timeStep, const unsigned int numSubSteps = 2);
        void render();
        
        void addActiveCamera(Camera * camera);
        void removeActiveCamera(Camera * camera);
        
        void addActiveNode(Node * node);
        void removeActiveNode(Node * node);
    protected:
    private:
        std::vector<CubeGeometry*> m_ActiveGeometries;
        std::vector<Camera*> m_ActiveCameras;
        std::vector<Node*> m_ActiveNodes;
    };
}

#endif /* Scene_hpp */
