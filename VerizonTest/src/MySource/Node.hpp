//
//  Node.hpp
//  VerizonTest
//
//  Created by James Folk on 6/21/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#ifndef Node_hpp
#define Node_hpp

namespace NJLI
{
    class Node
    {
    public:
        /* members */
        Node();
        Node(const Node &rhs);
        const Node &operator=(const Node &rhs);
        ~Node();
    protected:
    private:
    };
}

#endif /* Node_hpp */
