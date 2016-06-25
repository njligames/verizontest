//
//  AbstractDecorator.cpp
//  JLIGameEngineTest
//
//  Created by James Folk on 3/6/15.
//  Copyright (c) 2015 James Folk. All rights reserved.
//

#include "AbstractDecorator.hpp"

#include <assert.h>

namespace njli
{
    AbstractDecorator::AbstractDecorator():
    m_pParent(NULL),
    m_Name("<UNSET>")
    {
    }

    AbstractDecorator::~AbstractDecorator()
    {
        removeFromParent();
    }

    AbstractDecorator *AbstractDecorator::getParent()
    {
        return m_pParent;
    }

    const AbstractDecorator *AbstractDecorator::getParent() const
    {
        return m_pParent;
    }

    bool AbstractDecorator::hasParent()const
    {
        return (NULL != m_pParent);
    }

    void AbstractDecorator::setParent(AbstractDecorator *parent)
    {
        assert(parent);
        
        m_pParent = parent;
    }

    void AbstractDecorator::removeParent()
    {
        m_pParent = NULL;
    }

    bool AbstractDecorator::removeFromParent()
    {
        AbstractDecorator *parent = dynamic_cast<AbstractDecorator*>(getParent());
        
        if(parent && parent->hasChildren() && parent->hasChild(this))
        {
            parent->removeChild(this);
            return true;
        }
        return false;
    }

    AbstractDecorator *AbstractDecorator::findChild(const std::string &name)
    {
        if(getName() == name)
        {
            return this;
        }
        else
        {
            for (std::vector<AbstractDecorator*>::const_iterator iter = m_Children.begin();
                 iter != m_Children.end();
                 ++iter)
            {
                AbstractDecorator *n = (*iter)->findChild(name);
                if(NULL != n)
                {
                    return n;
                }
            }
        }
        return NULL;
    }
    
    const AbstractDecorator *AbstractDecorator::findChild(const std::string &name) const
    {
        if(getName() == name)
        {
            return this;
        }
        else
        {
            for (std::vector<AbstractDecorator*>::const_iterator iter = m_Children.begin();
                 iter != m_Children.end();
                 ++iter)
            {
                AbstractDecorator *n = (*iter)->findChild(name);
                if(NULL != n)
                {
                    return n;
                }
            }
        }
        return NULL;
    }
    
    AbstractDecorator*	AbstractDecorator::getChild(const unsigned long index)
    {
        assert(index < numberOfChildren());
        return m_Children.at(index);
    }

    const AbstractDecorator*	AbstractDecorator::getChild(const unsigned long index)const
    {
        assert(index < numberOfChildren());
        return m_Children.at(index);
    }
    
    void AbstractDecorator::getChildren(std::vector<AbstractDecorator*> &children)const
    {
        children = m_Children;
    }
    
    unsigned long AbstractDecorator::getChildIndex(AbstractDecorator *object)const
    {
        if(m_Children.size() > 0)
        {
            if (object)
            {
                std::vector<AbstractDecorator*>::const_iterator iter = std::find(m_Children.begin(), m_Children.end(), object);
                
                if (iter != m_Children.end())
                {
                    return std::distance(m_Children.begin(), iter);
                }
            }
        }
        
        
        return -1;
    }
    
    unsigned long AbstractDecorator::getChildIndex(const AbstractDecorator *object)const
    {
        if(m_Children.size() > 0)
        {
            if (object)
            {
                std::vector<AbstractDecorator*>::const_iterator iter = std::find(m_Children.begin(), m_Children.end(), object);
                
                if (iter != m_Children.end())
                {
                    return std::distance(m_Children.begin(), iter);
                }
            }
        }
        
        
        return -1;
    }

    bool AbstractDecorator::hasChild(AbstractDecorator *object)const
    {
        if (object && m_Children.size() > 0)
        {
            std::vector<AbstractDecorator*>::const_iterator iter = std::find(m_Children.begin(), m_Children.end(), object);
            
            if(m_Children.end() != iter)
                return true;
            
            for (iter = m_Children.begin(); iter != m_Children.end(); ++iter)
            {
                if((*iter)->hasChild(object))
                {
                    return true;
                }
            }
        }
        
        return false;
    }

    bool AbstractDecorator::hasChildren()const
    {
        unsigned long numChildren = numberOfChildren();
        return !(numChildren <= 0);
    }

    void AbstractDecorator::addChild(AbstractDecorator *object)
    {
        assert(this != object);
        
        object->setParent(this);
        m_Children.push_back(object);
    }

    void AbstractDecorator::removeChild(const unsigned long index)
    {
        assert(index < numberOfChildren());
        removeChild(getChild(index));
    }

    void AbstractDecorator::removeChild(AbstractDecorator *object)
    {
        if (object && m_Children.size() > 0)
        {
            std::vector<AbstractDecorator*>::iterator iter = std::find(m_Children.begin(), m_Children.end(), object);
            assert(iter != m_Children.end());
            
            (*iter)->removeParent();
            m_Children.erase(iter);
        }
    }

    void AbstractDecorator::removeChildren()
    {
        m_Children.clear();
    }

    unsigned long AbstractDecorator::numberOfChildren()const
    {
        return m_Children.size();
    }

    void AbstractDecorator::replaceChild(AbstractDecorator *oldChild, AbstractDecorator *newChild)
    {
        AbstractDecorator *parent = dynamic_cast<AbstractDecorator*>(oldChild->getParent());
        
        parent->removeChild(oldChild);
        parent->addChild(newChild);
    }
    
    void AbstractDecorator::setName(const std::string &name)
    {
        m_Name = name;
    }
    
    const std::string &AbstractDecorator::getName()const
    {
        return m_Name;
    }
}
