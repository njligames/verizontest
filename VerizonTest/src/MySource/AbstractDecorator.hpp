//
//  AbstractDecorator.h
//  JLIGameEngineTest
//
//  Created by James Folk on 11/15/14.
//  Copyright (c) 2014 James Folk. All rights reserved.
//

#ifndef AbstractDecorator_h
#define AbstractDecorator_h

#include <vector>
#include <string>

namespace njli
{
    class AbstractDecorator
    {
    protected:
        AbstractDecorator();
        virtual ~AbstractDecorator();

        AbstractDecorator *getParent();
        const AbstractDecorator *getParent() const;
        bool hasParent()const;
        virtual void setParent(AbstractDecorator *parent);
        void removeParent();
        bool removeFromParent();
        
        AbstractDecorator *findChild(const std::string &name);
        const AbstractDecorator *findChild(const std::string &name) const;
        AbstractDecorator* getChild(const unsigned long index);
        const AbstractDecorator *getChild(const unsigned long index) const;
        void getChildren(std::vector<AbstractDecorator*> &children)const;
        unsigned long getChildIndex(AbstractDecorator *object)const;
        unsigned long getChildIndex(const AbstractDecorator *object)const;
        bool hasChild(AbstractDecorator *object)const;
        bool hasChildren()const;
        void addChild(AbstractDecorator *object);
        void removeChild(const unsigned long index);
        void removeChild(AbstractDecorator *object);
        void removeChildren();
        unsigned long numberOfChildren()const;
        void replaceChild(AbstractDecorator *oldChild, AbstractDecorator *newChild);
    public:
        /**
         *  <#Description#>
         *
         *  @param name <#name description#>
         */
        void setName(const std::string &name);
        /**
         *  <#Description#>
         *
         *  @return <#return value description#>
         */
        const std::string &getName()const;
    private:
        AbstractDecorator(const AbstractDecorator &);
        AbstractDecorator &operator=(const AbstractDecorator &rhs);
    
        std::vector<AbstractDecorator*> m_Children;
        AbstractDecorator *m_pParent;
        
        std::string m_Name;
};
}

#endif
