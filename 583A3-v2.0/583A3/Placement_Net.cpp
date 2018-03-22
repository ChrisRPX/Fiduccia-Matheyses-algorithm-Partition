//
//  Placement_Net.cpp
//  581A2
//
//  Created by Peixu Ren on 2018-02-06.
//  Copyright © 2018 Peixu Ren. All rights reserved.
//

#include "Placement_Net.hpp"

PLC::PLC()
{
    cell_num = 0;
    wire_num = 0;
    
    grid_col = 0;
    grid_row = 0;
    
    nets = NULL;
    cells = NULL;
    parta = NULL;
    partb = NULL;
    
    cost = 0;
    parta_num = 0;
    partb_num = 0;
    maxa_inx = -1;
    maxb_inx = -1;
}

PLC::~PLC()
{
    if(cells)
    {
        delete [] cells;
        cout<<"The cells in PLC has been deleted"<<endl;
    }
    if(nets)
    {
        delete [] nets;
        cout<<"The nets in PLC has been deleted"<<endl;
    }
    if(parta)
    {
        delete [] parta;
        cout<<"Set parta in PLC has been deleted"<<endl;
    }
    if(partb)
    {
        delete [] partb;
        cout<<"Set partb in PLC has been deleted"<<endl;
    }
}

void PLC::init_partnum()
{
    int i,j;
    for(i=0;i<wire_num;i++)
    {
        nets[i].parta_num=0;
        nets[i].partb_num=0;
        if(cells[nets[i].src_No].partA_B==0)
            nets[i].parta_num+=1;
        else nets[i].partb_num+=1;
        for(j=0;j<nets[i].cell_num-1;j++)
        {
            if(cells[nets[i].sink_No[j]].partA_B==0)
                nets[i].parta_num+=1;
            else nets[i].partb_num+=1;
        }
    }
}

void PLC::init_crossing()
{
    int i,j;
    for(i=0;i<wire_num;i++)
    {
        for(j=0;j<nets[i].cell_num-1;j++)
        {
            if(cells[nets[i].edge[j].src_No].partA_B!=cells[nets[i].edge[j].sink_No].partA_B)
                nets[i].edge[j].crossing = true;
            else nets[i].edge[j].crossing = false;
        }
    }
}

void PLC::re_init()
{
    int i,pa=0,pb=0;
    maxa_inx = parta_num-1;
    maxb_inx = partb_num-1;
    for(i=0;i<cell_num;i++)
    {
        cells[i].locked = false;
        if(cells[i].partA_B==0)
        {
            parta[pa] = cells[i].cell_No;
            pa++;
        }
        else
        {
            partb[pb] = cells[i].cell_No;
            pb++;
        }
    }
    cout<<"pa="<<pa<<" pb="<<pb<<endl;

    init_partnum();
    init_crossing();
    in_bin_sort(parta,parta_num);
    in_bin_sort(partb,partb_num);
}

void PLC::reset_inv()
{
    for(int i=0;i<wire_num;i++)
    {
        this->nets[i].involved=0;
    }
}

int PLC::find_inv_nets(int cell_No)
{
    this->reset_inv();
    int involved_num = 0;
    for(int i=0;i<wire_num;i++)
    {
        if(nets[i].src_No==cell_No)
        {
            nets[i].involved = 1;
            involved_num++;
            continue;
        }
        for(int j=0;j<this->nets[i].cell_num-1;j++)
        {
            if(nets[i].sink_No[j]==cell_No)
            {
                nets[i].involved = 2;
                involved_num++;
                break;
            }
        }
    }
    return involved_num;
}

void PLC::set_inv_net()
{
    int i,j,k;
    for(i=0;i<cell_num;i++)
    {
        cells[i].net_num = find_inv_nets(i);
        cells[i].net_No = new int [cells[i].net_num];
        k=0;
        for(j=0;j<wire_num;j++)
        {
            if(nets[j].involved)
            {
                cells[i].net_No[k] = j;
                k++;
            }
        }
    }
}

bool PLC::move_cell(int cell_No) //the moved node must have the max gain
{
    //cout<<cell_No<<endl;
    if(cells[cell_No].locked) return false;
    cells[cell_No].locked = true;
    if(cells[cell_No].partA_B==0) //was in part_A
    {
        maxa_inx--;
    }
    else
    {
        maxb_inx--;
    }
    cells[cell_No].partA_B = 1-cells[cell_No].partA_B;
    
    //find_inv_nets(cell_No);
    int i,j;
    int inv_net;
    for(i=0;i<cells[cell_No].net_num;i++)
    {
        inv_net = cells[cell_No].net_No[i];
        if(cells[cell_No].partA_B==0) //now is in part_A
        {
            nets[inv_net].parta_num += 1;
            nets[inv_net].partb_num -= 1;
        }
        else
        {
            nets[inv_net].parta_num -= 1;
            nets[inv_net].partb_num += 1;
        }
        for(j=0;j<nets[inv_net].cell_num-1;j++) //for all the edges in this involved net
        {
            if(cells[nets[inv_net].edge[j].src_No].partA_B!=cells[nets[inv_net].edge[j].sink_No].partA_B)
                nets[inv_net].edge[j].crossing = true;
            else nets[inv_net].edge[j].crossing = false;
        }
    }
    return true;
}

int PLC::cal_node_gain(int cell_No)
{
    int i,j;
    find_inv_nets(cell_No);
    cells[cell_No].gain = 0;
    for(i=0;i<wire_num;i++)
    {
        switch(nets[i].involved)
        {
            case 1:
                for(j=0;j<nets[i].cell_num-1;j++)
                {
                    if(nets[i].edge[j].crossing)
                    {
                        cells[cell_No].gain += nets[i].cutsize;
                    }
                    else cells[cell_No].gain -= nets[i].cutsize;
                }
                break;
            case 2:
                for(j=0;j<nets[i].cell_num-1;j++)
                {
                    if(nets[i].edge[j].sink_No==cell_No)
                    {
                        if(nets[i].edge[j].crossing)
                        {
                            cells[cell_No].gain += nets[i].cutsize;
                        }
                        else cells[cell_No].gain -= nets[i].cutsize;
                    }
                }
                break;
            default:
                break;
        }
    }
    return 0;
}

void PLC::cal_all_gains()
{
    int i;
    for(i=0;i<cell_num;i++)
        cal_node_gain(i);
}

int PLC::cal_total_cost()
{
    int acost = 0;
    int i;
    for(i=0;i<wire_num;i++)
    {
        //cout<<"nets"<<i<<"="<<nets[i].cal_net_cost()<<endl;
        acost += nets[i].cal_net_cost();
    }
    return acost;
}

int PLC::bin_srch(double temp,int *part,int key)
{
    int low = 0;
    int high = key-1;
    int i = (low+high)/2;
    //while(n != part[i])
    while(temp != cells[part[i]].gain)
    {
        if(high < low) return i;
        if(temp>cells[part[i]].gain) low = i+1;
        else high = i-1;
        if(high==-1)
        {
            i=-1;
            return i;
        }
        i = (low + high) / 2;
    }
    return i;
}

void PLC::in_bin_sort(int *part,int part_num)
{
    int key = 1;
    int i,inx;
    double temp;
    int r0;
    
    cal_all_gains();
    
    while(key<part_num)
    {
        r0 = part[key];
        temp = cells[r0].gain;
        i=bin_srch(temp,part,key);
        for(inx=key-1;inx>i;inx--) part[inx+1] = part[inx];
        part[i+1] = r0;
        key++;
    }
}
