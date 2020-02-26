//
//  Node.cpp
//  VerizonTest
//
//  Created by James Folk on 6/21/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#include "Node.hpp"
#include "Geometry.hpp"
#include "Camera.hpp"

#include <assert.h>
#include <iostream>

namespace njli
{
    Node::Node():
    m_Name("NODE"),
mScale(new glm::vec3(1.0f, 1.0f, 1.0f)),
mTransform(new glm::mat4(1)),
    m_ParentNode(NULL),
    m_Camera(NULL),
    m_Geometry(NULL),
    m_GeometryIndex(-1),
    m_HideGeometry(false),
    m_Opacity(1.0f),
mNormalMatrix(new glm::mat3x3(1)),
mColorbase(new glm::vec4(1, 1, 1, 1)),
    m_TransformDirty(true),
    m_NormalMatrixDirty(true),
    m_OpacityDirty(true),
    m_HiddenDirty(true),
    m_ColorBaseDirty(true)
    {
        
    }
    
    Node::~Node()
    {
        
        delete mColorbase;
        mColorbase = NULL;
        
        delete mNormalMatrix;
        mNormalMatrix = NULL;
        
        m_Camera = NULL;
        m_ParentNode = NULL;

        
        delete mTransform;
        mTransform = NULL;
        
        delete mScale;
        mScale = NULL;
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
        if(hidden != m_HideGeometry)
            m_HiddenDirty = true;
        m_HideGeometry = hidden;
        
    }
    
    bool Node::isHiddenGeometry()const
    {
        return m_HideGeometry;
    }
    
    void Node::setOpacity(float opacity)
    {
        if(opacity != m_Opacity)
            m_OpacityDirty = true;
        
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

void Node::setNormalMatrix(const glm::mat3x3 &mtx)
{
    *mNormalMatrix = mtx;
    m_NormalMatrixDirty = true;
}

const glm::mat3x3 &Node::getNormalMatrix()const
{
    return *mNormalMatrix;
}

void Node::setColorBase(const glm::vec4 &color)
{
    if(*mColorbase != color)
        m_ColorBaseDirty = true;
    *mColorbase = color;
}

const glm::vec4 &Node::getColorBase()const
{
    return *mColorbase;
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
    

glm::mat4x4 Node::getWorldTransform() const {
    glm::mat4x4 transform(glm::scale(getTransform(), getScale()));
    
    if(getParentNode())
    {
        return (getParentNode()->getWorldTransform() * transform);
    }
    return (transform);
    
}
    
    
const glm::mat4x4& Node::getTransform() const {
    return *mTransform;
}
    

void Node::setTransform(const glm::mat4x4& transform) {
    m_TransformDirty = true;
    *mTransform = transform;
}
    

glm::vec3 Node::getOrigin()const
{
    glm::mat4 transform(getTransform());
    return glm::vec3(transform[3][0],
                     transform[3][1],
                     transform[3][2]);
}
    

void Node::setOrigin(const glm::vec3 &origin)
{
    glm::mat4 t(getTransform());
    
    t[3][0] = origin.x;
    t[3][1] = origin.y;
    t[3][2] = origin.z;
    
    setTransform(t);
}
    
void Node::setOrigin(const glm::vec2 &origin)
{
    glm::vec3 _origin(origin.x, origin.y, 0);
    Node::setOrigin(_origin);
}
    

glm::quat Node::getRotation()const
{
    return glm::quat_cast(getTransform());
//    return getWorldTransform().getRotation();
}
    

 
void Node::setRotation(const glm::quat &rotation)
{
    glm::mat4 t(glm::mat3_cast(rotation));
    
    glm::translate(t, getOrigin());
    
    setTransform(t);
}
    
const glm::vec3 &Node::getScale()const {
    return *mScale;
}
    
void Node::setScale(const glm::vec3 &scale) {
    *mScale = scale;
}
    
    void Node::setScale(const float scale)
    {
        glm::vec3 _scale(scale, scale, scale);
        Node::setScale(_scale);
    }
    

    
    void Node::setGeometryIndex(unsigned long index)
    {
        assert(index >= 0 && index < getGeometry()->numberOfInstances());
        
        m_GeometryIndex = index;
        
        m_NormalMatrixDirty= true;
//        m_ColorTransformDirty = true;
        m_OpacityDirty = true;
        m_HiddenDirty = true;
        m_ColorBaseDirty = true;
    }
    
    unsigned long Node::getGeometryIndex()const
    {
        return m_GeometryIndex;
    }
    
    void Node::clearGeometryIndex()
    {
        m_GeometryIndex = -1;
    }
    
    bool Node::isTransformDirty()const
    {
        
        if(getParentNode())
            return m_TransformDirty || getParentNode()->isTransformDirty();
        return m_TransformDirty;
    }
    
    void Node::resetTransformDirty()
    {
        m_TransformDirty = false;
    }
    
    
    void Node::update(float timestep)
    {
//        float mass = 1.0f;
        
//        *m_ImpulseForce += *m_GravityForce;
        
//        btVector3 acceleration((*m_ImpulseForce) / mass);
//
//        setVelocity(getVelocity() + acceleration * timestep);
//        if(getVelocity().length() > getMaxSpeed())
//            setVelocity(getVelocity().normalized() * getMaxSpeed());
//
//        setOrigin(getTransform().getOrigin() + (*m_CurrentVelocity) * timestep);
//
//        if(m_CurrentVelocity->length() > 0.00000001)
//        {
//            *m_HeadingVector = m_CurrentVelocity->normalized();
//        }
//        *m_ImpulseForce = btVector3(0,0,0);
    }
    void Node::render(Geometry *const geometry)
    {
        if(geometry)
        {
            const unsigned long geometryIndex = getGeometryIndex();
            
            if(isTransformDirty())
                geometry->setTransform(geometryIndex, getWorldTransform());
            
//            if(m_ColorTransformDirty)
//            {
//                geometry->setColorTransform(geometryIndex, getColorTransform());
//                m_ColorTransformDirty = false;
//            }
            
            if(m_NormalMatrixDirty)
            {
                
                geometry->setNormalMatrixTransform(geometryIndex, getNormalMatrix());
                m_NormalMatrixDirty = false;
            }
            
            if(m_HiddenDirty)
            {
                geometry->setHidden(this);
                m_HiddenDirty = false;
            }
            
            if(m_OpacityDirty)
            {
                geometry->setOpacity(this);
                m_OpacityDirty = false;
            }
            
            if(m_ColorBaseDirty)
            {
                geometry->setColorBase(this);
                m_ColorBaseDirty = false;
            }
            
        }
    }
}
