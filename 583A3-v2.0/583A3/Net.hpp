//
//  Net.hpp
//  583A2
//
//  Created by Peixu Ren on 2018-02-06.
//  Copyright © 2018 Peixu Ren. All rights reserved.
//

#ifndef Net_hpp
#define Net_hpp

#include <stdio.h>
#include <iostream>
#include "Grids.hpp"
#include "Edge.hpp"
using namespace std;

class Net
{
public:
    int net_No;
    int cell_num;//sink_num=edge_num=cell_num-1
    
    int src_No;
    int *sink_No;
    Edge *edge;
    int involved;//0: not involved, 1: src involved, 2: sink involved.
    double cutsize;
    
    int parta_num;
    int partb_num;
    
    Net();
    ~Net();
    void create_sinks();
    void create_edges();
    //void reset_inv();
    //void find_inv_edges(int cell_No);
    
    int cal_net_cost();
    //double cal_cutsize();
    //double cal_cutsize_temp();
    
};

#endif /* Net_hpp */
