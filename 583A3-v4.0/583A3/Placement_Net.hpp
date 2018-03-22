//
//  Placement_Net.hpp
//  581A2
//
//  Created by Peixu Ren on 2018-02-06.
//  Copyright © 2018 Peixu Ren. All rights reserved.
//

#ifndef Placement_Net_hpp
#define Placement_Net_hpp

#include <stdio.h>
#include <iostream>
#include "Net.hpp"
#include "Grids.hpp"
using namespace std;

class PLC
{
public:
    int cell_num;
    int wire_num;
    int grid_col;
    int grid_row;
    
    Net *nets;
    Grids *cells;
    int *parta;
    int *partb;
    
    int parta_num;
    int partb_num;
    int maxa_inx;
    int maxb_inx;
    int cost;
    //double cc;
    
    PLC();
    ~PLC();
    void init_partnum();
    void init_crossing();
    void re_init();
    
    bool move_cell(int max_cell);//After a swap, cost should be re-calculated "locally"
    
    void reset_inv(); //nets[i].involved should be reset before next iteration
    int find_inv_nets(int cell_No); //to find which nets are involved
    void set_inv_net();
    
    int cal_node_gain(int cell_No);
    void cal_all_gains(); //when swap happens, it only needs to calculate a "local" cost instead of global cost.
    int cal_total_cost();
    
    int bin_srch(double temp,int *part,int part_num);
    void in_bin_sort(int *part,int part_num);
    
    void find_max(int *part,int part_num);
    
    int FM_algo();
};

#endif /* Placement_Net_hpp */
