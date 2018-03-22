//
//  Net.cpp
//  583A2
//
//  Created by Peixu Ren on 2018-02-06.
//  Copyright © 2018 Peixu Ren. All rights reserved.
//

#include "Net.hpp"

Net::Net()
{
    net_No = -1;
    cell_num = 0;
    src_No = -1;
    parta_num = -1;
    partb_num = -1;
    
    cutsize=-1;
    
    sink_No = NULL;
    edge = NULL;
    involved=0;
}

Net::~Net()
{
    if(sink_No)
    {
        delete [] sink_No;
        //cout<<"The sink_No in net"<<this->net_No<<" has been deleted"<<endl;
    }
    if(edge)
    {
        delete [] edge;
    }
}

void Net::create_sinks()
{
    sink_No = new int [cell_num-1];
}

void Net::create_edges()
{
    cutsize = 1/((double)cell_num-1);
    //cout<<net_No<<".cutsize="<<cutsize<<endl;
    edge = new Edge [cell_num-1];
}

int Net::cal_net_cost()
{
    int i;
    if(parta_num==0 && partb_num==0)
        i = -1;
    if(parta_num==0 || partb_num==0)
        i = 0;
    else i = 1;
    return i;
}
