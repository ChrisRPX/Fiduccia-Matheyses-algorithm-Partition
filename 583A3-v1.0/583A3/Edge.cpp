//
//  Edge.cpp
//  583A3
//
//  Created by Peixu Ren on 2018-02-19.
//  Copyright © 2018 Peixu Ren. All rights reserved.
//

#include "Edge.hpp"
Edge::Edge()
{
    src_No = -1;
    sink_No = -1;
    net_No = -1;
    
    crossing = false;
    //involved = false;
    //cutsize = -1;
}

Edge::~Edge()
{
    
}
