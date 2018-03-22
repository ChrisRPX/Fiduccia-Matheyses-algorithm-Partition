//
//  Edge.hpp
//  583A3
//
//  Created by Peixu Ren on 2018-02-19.
//  Copyright © 2018 Peixu Ren. All rights reserved.
//

#ifndef Edge_hpp
#define Edge_hpp

#include <stdio.h>
#include "Grids.hpp"
using namespace std;

class Edge
{
public:
    int src_No;
    int sink_No;
    int net_No;
    
    bool crossing;
    //bool involved;
    //double cutsize;
    
    Edge();
    ~Edge();
};
#endif /* Edge_hpp */
