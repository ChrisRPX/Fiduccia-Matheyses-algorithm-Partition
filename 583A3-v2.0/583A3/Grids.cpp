//
//  Grids.cpp
//  583A2
//
//  Created by Peixu Ren on 2018-02-06.
//  Copyright © 2018 Peixu Ren. All rights reserved.
//

#include "Grids.hpp"

Grids::Grids()
{
    cell_No = -1;
    x = -1;
    y = -1;
    partA_B = -1;
    gain = 0;
    locked = false;
    
    net_num = 0;
    net_No = NULL;
    
}

Grids::~Grids()
{
    if(net_No)
        delete [] net_No;
}
