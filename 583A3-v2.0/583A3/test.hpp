//
//  test.hpp
//  583A3
//
//  Created by Peixu Ren on 2018-02-22.
//  Copyright © 2018 Peixu Ren. All rights reserved.
//

#ifndef test_hpp
#define test_hpp

#include <stdio.h>
#include "Grids.hpp"
#include "Edge.hpp"
#include "Net.hpp"
#include "Placement_Net.hpp"

void test_node_status(PLC &plcnet,int cell_No);
void test_crossing(PLC &plcnet);
void test_node_gain(PLC &plcnet);
void test_sort(PLC &plcnet);
void test_locked_num(PLC &plcnet);
void test_finalpart(PLC &plcnet);

#endif /* test_hpp */
