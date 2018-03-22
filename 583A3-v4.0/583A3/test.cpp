//
//  test.cpp
//  583A3
//
//  Created by Peixu Ren on 2018-02-22.
//  Copyright © 2018 Peixu Ren. All rights reserved.
//
#include <iostream>
#include "test.hpp"

void test_node_status(PLC &plcnet,int cell_No)
{
    cout<<"the test node number is "<<cell_No<<endl;
    cout<<"part = "<<plcnet.cells[cell_No].partA_B<<endl;
    cout<<"gain = "<<plcnet.cells[cell_No].gain<<endl;
    cout<<"lock = "<<plcnet.cells[cell_No].locked<<endl;
}

void test_crossing(PLC &plcnet)
{
    int i,j;
    for(i=0;i<plcnet.wire_num;i++)
    {
        for(j=0;j<plcnet.nets[i].cell_num-1;j++)
        {
            cout<<"nets["<<i<<"].edge["<<j<<"].crossing="<<plcnet.nets[i].edge[j].crossing<<" ";
        }
         cout<<endl;
    }
}

void test_node_gain(PLC &plcnet)
{
    int p;
    cout<<"Nodes' gains are:"<<endl;
    for(p=0;p<plcnet.cell_num;p++)
    {
        cout<<"n"<<p<<"="<<plcnet.cells[p].gain<<" ";
        if(p && p%10==0)
            cout<<endl;
    }
    cout<<endl<<endl;
}

void test_sort(PLC &plcnet)
{
    int p;
    int parta_temp = plcnet.parta_num;
    cout<<"parta="<<endl;
    if(plcnet.maxa_inx<plcnet.parta_num-1) parta_temp++;
    for(p=0;p<parta_temp;p++)
    {
        cout<<"n"<<plcnet.parta[p]<<"="<<plcnet.cells[plcnet.parta[p]].gain<<" ";
    }
    cout<<endl<<"partb="<<endl;
    for(p=0;p<plcnet.partb_num;p++)
    {
        //cout<<plcnet.partb[p]<<" ";
        cout<<"n"<<plcnet.partb[p]<<"="<<plcnet.cells[plcnet.partb[p]].gain<<" ";
    }
    cout<<endl<<endl;
}

void test_locked_num(PLC &plcnet)
{
    int palocked=0,pblocked=0;
    for(int i=0;i<plcnet.parta_num;i++)
    {
        if(plcnet.cells[plcnet.parta[i]].locked)
            palocked++;
    }
    for(int i=0;i<plcnet.partb_num;i++)
    {
        if(plcnet.cells[plcnet.partb[i]].locked)
            pblocked++;
    }
    cout<<"Part A has "<<plcnet.parta_num<<" nodes,"<<endl;
    cout<<"and it has "<<palocked<<"locked nodes."<<endl;
    cout<<"Part B has "<<plcnet.partb_num<<" nodes."<<endl;
    cout<<"and it has "<<pblocked<<"locked nodes."<<endl;
}

void test_finalpart(PLC &plcnet)
{
    int parta=0,partb=0;
    for(int p=0;p<plcnet.cell_num;p++)
    {
        if(plcnet.cells[p].partA_B==0)
        {
            parta++;
        }
        else partb++;
    }
    cout<<"parta="<<parta<<" partb="<<partb<<endl;
}
