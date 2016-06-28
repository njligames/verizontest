//
//  Node.cpp
//  VerizonTest
//
//  Created by James Folk on 6/21/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#include "Node.hpp"
#include "Geometry.hpp"
#include "TornadoData.hpp"
#include "Camera.hpp"

#include <assert.h>

namespace njli
{
    Node::Node():
    m_Name("NODE"),
    m_Scale(new btVector3(1.0f, 1.0f, 1.0f)),
    m_Transform(new btTransform(btTransform::getIdentity())),
    m_ColorTransform(new btTransform(btTransform::getIdentity())),
    m_Orientation(new btQuaternion()),
    m_ParentNode(NULL),
    m_Camera(NULL),
    m_Geometry(NULL),
    m_GeometryIndex(-1),
    m_TornadoData(new TornadoData()),
    m_HideGeometry(false),
    m_Opacity(1.0f),
    m_NormalMatrix(new btMatrix3x3(btMatrix3x3::getIdentity())),
    m_Colorbase(new btVector4(1, 1, 1, 1))
    {
        
    }
    
    Node::~Node()
    {
        delete m_Colorbase;
        m_Colorbase = NULL;
        
        delete m_NormalMatrix;
        m_NormalMatrix = NULL;
        
        delete m_TornadoData;
        m_TornadoData = NULL;
        
        m_Camera = NULL;
        m_ParentNode = NULL;
        
        delete m_Orientation;
        m_Orientation = NULL;
        
        delete m_ColorTransform;
        m_ColorTransform = NULL;
        
        delete m_Transform;
        m_Transform = NULL;
        
        delete m_Scale;
        m_Scale = NULL;
    }
    
    void Node::setName(const std::string &name)
    {
        m_Name = name;
    }
    
    const std::string &Node::getName()const
    {
        return m_Name;
    }
    
    void Node::addCamera(Camera *camera)
    {
        m_Camera = camera;
        camera->setNodeOwner(this);
        
    }
    
    void Node::removeCamera()
    {
        m_Camera = NULL;
    }
    
    Camera* Node::getCamera()
    {
        return m_Camera;
    }
    
    void Node::addGeometry(Geometry * geometry)
    {
        assert(geometry);
        
        removeGeometry();
        
        m_Geometry = geometry;
        m_Geometry->addReference(this);
    }
    
    void Node::removeGeometry()
    {
        Geometry *geometry = getGeometry();
        if(geometry)
            geometry->removeReference(this);
    }
    
    Geometry *const Node::getGeometry()const
    {
        return m_Geometry;
    }
    
    void Node::enableHideGeometry(bool hidden)
    {
        m_HideGeometry = hidden;
    }
    
    bool Node::isHiddenGeometry()const
    {
        return m_HideGeometry;
    }
    
    void Node::setOpacity(float opacity)
    {
        if(opacity<0.0f)
            m_Opacity = 0.0f;
        else if(opacity > 1.0f)
            m_Opacity = 1.0f;
        else
            m_Opacity = opacity;
    }
    
    float Node::getOpacity()const
    {
        return m_Opacity;
    }
    
    void Node::setNormalMatrix(const btMatrix3x3 &mtx)
    {
        *m_NormalMatrix = mtx;
    }
    
    const btMatrix3x3 &Node::getNormalMatrix()const
    {
        return *m_NormalMatrix;
    }
    
    void Node::setColorBase(const btVector4 &color)
    {
        *m_Colorbase = color;
    }
    
    const btVector4 &Node::getColorBase()const
    {
        return *m_Colorbase;
    }
    
    Node *Node::getParentNode()
    {
        return m_ParentNode;
    }
    
    const Node *Node::getParentNode() const
    {
        return m_ParentNode;
    }
    
    bool Node::hasParentNode()const
    {
        return (NULL != m_ParentNode);
    }
    
    void Node::setParentNode(Node *parent)
    {
        assert(parent);
        
        m_ParentNode = parent;
    }
    
    void Node::removeParentNode()
    {
        m_ParentNode = NULL;
    }
    
    bool Node::removeFromParentNode()
    {
        Node *parent = dynamic_cast<Node*>(getParentNode());
        
        if(parent && parent->hasChildNode(this))
        {
            parent->removeChildNode(this);
            return true;
        }
        return false;
    }
    
    Node *Node::findChildNode(const std::string &name)
    {
        if(getName() == name)
        {
            return this;
        }
        else
        {
            for (std::vector<Node*>::const_iterator iter = m_ChildrenNodes.begin();
                 iter != m_ChildrenNodes.end();
                 ++iter)
            {
                Node *n = (*iter)->findChildNode(name);
                if(NULL != n)
                {
                    return n;
                }
            }
        }
        return NULL;
    }
    
    const Node *Node::findChildNode(const std::string &name) const
    {
        if(getName() == name)
        {
            return this;
        }
        else
        {
            for (std::vector<Node*>::const_iterator iter = m_ChildrenNodes.begin();
                 iter != m_ChildrenNodes.end();
                 ++iter)
            {
                Node *n = (*iter)->findChildNode(name);
                if(NULL != n)
                {
                    return n;
                }
            }
        }
        return NULL;
    }
    
    Node*	Node::getChildNode(const unsigned long index)
    {
        assert(index < numberOfChildrenNodes());
        
        return m_ChildrenNodes.at(index);
    }
    
    const Node*	Node::getChildNode(const unsigned long index)const
    {
        assert(index < numberOfChildrenNodes());
        return m_ChildrenNodes.at(index);
    }
    
    void Node::getChildrenNodes(std::vector<Node*> &children)const
    {
        children = m_ChildrenNodes;
    }
    
    unsigned long Node::getChildNodeIndex(Node *object)const
    {
        std::vector<Node*>::const_iterator iter = std::find(m_ChildrenNodes.begin(), m_ChildrenNodes.end(), object);
        
        if (iter != m_ChildrenNodes.end())
        {
            return std::distance(m_ChildrenNodes.begin(), iter);
        }
        return -1;
    }
    
    bool Node::hasChildNode(Node *object)const
    {
        assert(object);
        
        std::vector<Node*>::const_iterator iter = std::find(m_ChildrenNodes.begin(), m_ChildrenNodes.end(), object);
        
        if(m_ChildrenNodes.end() != iter)
            return true;
        
        for (iter = m_ChildrenNodes.begin(); iter != m_ChildrenNodes.end(); ++iter)
        {
            if((*iter)->hasChildNode(object))
            {
                return true;
            }
        }
        return false;
    }
    
    bool Node::hasChildrenNodes()const
    {
        return (numberOfChildrenNodes() > 0);
    }
    
    void Node::addChildNode(Node *object)
    {
        assert(this != object);
        assert(!hasChildNode(object));
        
        object->setParentNode(this);
        m_ChildrenNodes.push_back(object);
    }
    
    void Node::removeChildNode(const unsigned long index)
    {
        assert(index < numberOfChildrenNodes());
        removeChildNode(getChildNode(index));
    }
    
    void Node::removeChildNode(Node *object)
    {
        assert(object);
        
        std::vector<Node*>::iterator iter = std::find(m_ChildrenNodes.begin(), m_ChildrenNodes.end(), object);
        
        if(iter != m_ChildrenNodes.end())
        {
            (*iter)->removeParentNode();
            m_ChildrenNodes.erase(iter);
        }
    }
    
    void Node::removeChildrenNodes()
    {
        for (unsigned long i = 0; i < m_ChildrenNodes.size(); ++i)
        {
            removeChildNode(m_ChildrenNodes.at(i));
        }
        m_ChildrenNodes.clear();
    }
    
    unsigned long Node::numberOfChildrenNodes()const
    {
        return m_ChildrenNodes.size();
    }
    
    void Node::replaceChildNode(Node *oldChild, Node *newChild)
    {
        Node *parent = oldChild->getParentNode();
        
        parent->removeChildNode(oldChild);
        parent->addChildNode(newChild);
    }
    
    btTransform Node::getWorldTransform() const
    {
//        const PhysicsBody *physicsBody = getPhysicsBody();
//
//        if(physicsBody)
//        {
//            btTransform transform(physicsBody->getWorldTransform());
//
//            transform.setBasis(transform.getBasis().scaled(getScale()));
//
//            if(getParentNode())
//            {
//                return (transform * getParentNode()->getWorldTransform());
//            }
//            return (transform);
//        }
        
        btTransform transform(getTransform());
        
        transform.setBasis(transform.getBasis().scaled(getScale()));
        
        if(getParentNode())
        {
            return (getParentNode()->getWorldTransform() * transform);
        }
        return (transform);
    }
    
    const btTransform& Node::getColorTransform() const
    {
        return *m_ColorTransform;
    }
    
    void Node::setColorTransform(const btTransform& transform)
    {
        *m_ColorTransform = transform;
    }
    
    const btTransform &Node::getTransform()const
    {
        return *m_Transform;
    }
    
    void Node::setTransform(const btTransform &transform)
    {
        *m_Transform = transform;
        
//        PhysicsBody *physicsBody = getPhysicsBody();
//        
//        if(physicsBody)
//        {
//            physicsBody->setWorldTransform(getTransform());
//        }
    }
    
    btVector3 Node::getOrigin()const
    {
        return getWorldTransform().getOrigin();
    }
    
    void Node::setOrigin(const btVector3 &origin)
    {
        btTransform t(getTransform());
        t.setOrigin(origin);
        setTransform(t);
    }
    
    void Node::setOrigin(const btVector2 &origin)
    {
        btVector3 _origin(origin.x(), origin.y(), 0);
        Node::setOrigin(_origin);
    }
    
    btQuaternion Node::getRotation()const
    {
        return getWorldTransform().getRotation();
    }
    
    void Node::setRotation(const btQuaternion &rotation)
    {
        btTransform t(getTransform());
        t.setRotation(rotation);
        setTransform(t);
    }
    
    btVector3 Node::getEulerAngles()const
    {
        float x, y, z;
        
        m_Transform->getBasis().getEulerYPR(x, y, z);
        
        btVector3 v(x,y,z);
        return v;
    }
    
    void Node::setEulerAngles(const btVector3 &angles)
    {
        btTransform t(getTransform());
        btMatrix3x3 m(t.getBasis());
        m.setEulerYPR(angles.x(), angles.y(), angles.z());
        t.setBasis(m);
        setTransform(t);
    }
    
    const btQuaternion &Node::getOrientation()const
    {
        return *m_Orientation;
    }
    
    void Node::setOrientation(const btQuaternion &orientation)
    {
        *m_Orientation = orientation;
    }
    
    const btVector3 &Node::getScale()const
    {
        return *m_Scale;
    }
    
    void Node::setScale(const btVector3 &scale)
    {
        *m_Scale = scale;
    }
    
    void Node::setScale(const float scale)
    {
        btVector3 _scale(scale,scale,scale);
        Node::setScale(_scale);
    }
    
    TornadoData *const Node::getTornadoData()const
    {
        return m_TornadoData;
    }
    
    void Node::setGeometryIndex(unsigned long index)
    {
        assert(index >= 0 && index < Geometry::MAX_CUBES);
        
        m_GeometryIndex = index;
    }
    
    unsigned long Node::getGeometryIndex()const
    {
        return m_GeometryIndex;
    }
    
    void Node::clearGeometryIndex()
    {
        m_GeometryIndex = -1;
    }
}
