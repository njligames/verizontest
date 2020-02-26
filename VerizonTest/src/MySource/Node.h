//
//  Node.hpp
//  VerizonTest
//
//  Created by James Folk on 6/21/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#ifndef Node_hpp
#define Node_hpp


#include <vector>
#include <string>

#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

#include "glm/gtx/transform.hpp"

namespace njli
{
    class Camera;
    class Geometry;
    
    class TornadoData;
    
    class Node
    {
        friend class Geometry;
        friend class Scene;
        
    public:
        /* members */
        Node();
        Node(const Node &rhs);
        const Node &operator=(const Node &rhs);
        ~Node();
        
        void setName(const std::string &name);
        const std::string &getName()const;
        
        void addCamera(Camera *camera);
        void removeCamera();
        Camera* getCamera();
        
        void addGeometry(Geometry * body);
        void removeGeometry();
        Geometry *const getGeometry()const;
        
        void enableHideGeometry(bool hidden = true);
        bool isHiddenGeometry()const;
        
        void setOpacity(float opacity);
        float getOpacity()const;
        
        void setNormalMatrix(const glm::mat3x3 &mtx);
        
        const glm::mat3x3 &getNormalMatrix()const;
        
        void setColorBase(const glm::vec4 &color);
        
        const glm::vec4 &getColorBase()const;
    public:
        Node* getParentNode();
        const Node* getParentNode() const;
        bool hasParentNode() const;
        void setParentNode(Node * parent);
        void removeParentNode();
        bool removeFromParentNode();
        Node* findChildNode(const std::string &name);
        const Node* findChildNode(const std::string &name) const;
        Node* getChildNode(const unsigned long index);
        const Node* getChildNode(const unsigned long index) const;
        void getChildrenNodes(std::vector<Node*> & children) const;
        unsigned long getChildNodeIndex(Node * object) const;
        bool hasChildNode(Node * object) const;
        bool hasChildrenNodes() const;
        void addChildNode(Node * object);
        void removeChildNode(const unsigned long index);
        void removeChildNode(Node * object);
        void removeChildrenNodes();
        unsigned long numberOfChildrenNodes() const;
        void replaceChildNode(Node * oldChild, Node * newChild);
    public:
        glm::mat4x4 getWorldTransform() const;
        
        const glm::mat4x4& getTransform() const;
        
        void setTransform(const glm::mat4x4& transform);
        
        glm::vec3 getOrigin() const;
        
        void setOrigin(const glm::vec3 &origin);
        
        void setOrigin(const glm::vec2& origin);
        
        glm::quat getRotation() const;
        
        void setRotation(const glm::quat& rotation);
        
        
        const glm::vec3 &getScale()const;
        
        void setScale(const glm::vec3 &scale);
        
        void setScale(const float scale);
    public:
        
        TornadoData *const getTornadoData()const;
        
    protected:
        void setGeometryIndex(unsigned long index);
        unsigned long getGeometryIndex() const;
        void clearGeometryIndex();
        
        bool isTransformDirty()const;
        void resetTransformDirty();
        
        void update(float timestep);
        void render(Geometry *const geometry);
    private:
        std::string m_Name;
        
        glm::vec3 *mScale;
        
        glm::mat4x4 *mTransform;
        
        
        Node* m_ParentNode;
        std::vector<Node*> m_ChildrenNodes;
        
        Camera *m_Camera;
        Geometry *m_Geometry;
        unsigned long m_GeometryIndex;
        
        bool m_HideGeometry;
        float m_Opacity;
        
        glm::mat3x3 *mNormalMatrix;
        
        glm::vec4 *mColorbase;
        
        
        bool m_TransformDirty;
        bool m_NormalMatrixDirty;
        bool m_OpacityDirty;
        bool m_HiddenDirty;
        bool m_ColorBaseDirty;
        
        
        float m_MaxSpeed;
        
    };
}

#endif /* Node_hpp */
