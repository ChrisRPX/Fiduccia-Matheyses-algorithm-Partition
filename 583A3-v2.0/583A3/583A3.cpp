//
//  main.cpp
//  581A2
//
//  Created by Peixu Ren on 2018-02-06.
//  Copyright © 2018 Peixu Ren. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include "graphics.h"
#include "Grids.hpp"
#include "Net.hpp"
#include "Placement_Net.hpp"
#include "test.hpp"
using namespace std;

#define delay_time 500

void drawscreen(void);
void act_on_button_press(float x, float y);
void draw_init(int map_h,int map_w,int gridsize);
void draw_cells(PLC &plcnet,int gridsize);
void draw_netline(PLC &plcnet,int gridsize);
void delay();


bool read_file(char *filename,PLC &plcnet);
void rand_part(PLC &plcnet);

int FM_algo(PLC &plcnet,int round,int gridsize);

int main(int argc, const char * argv[])
{
    // insert code here...
    clock_t start;
    clock_t end;
    double duration;
    
    int i,j;
    //Grids **mygrids = NULL;
    PLC plcnet;
    
    char path[100] = "/Users/peixu/Documents/XCode/583A3/A3_Benchmarks/";
    char filename[10] = "paira";//"cm162a";
    //cout<<"Please input the benchmark name: "<<endl;
    //cin>>filename;
    strcat(path,filename);
    strcat(path,".txt");
    if(!read_file(path,plcnet))
    {
        cout<<"Please clarify the filename is correct, or please modify the path in cpp file"<<endl;
        exit(1); // terminate with error
    }
    
    /********** Start: draw all the grids and execute the algorithm **********/
    //init the graph
    int gridsize = 50;
    printf ("About to start graphics.\n\n");
    init_graphics("583 Demo", WHITE);
    init_world (0.,0.,(plcnet.grid_col+2)*gridsize,plcnet.grid_row*gridsize);
    event_loop(act_on_button_press,NULL,NULL,drawscreen);//act_on_button_press
    clearscreen();
    draw_init(plcnet.grid_row,plcnet.grid_col,gridsize);
    event_loop(act_on_button_press,NULL,NULL,NULL);//act_on_button_press
    /********** End: draw all the grids and execute the algorithm **********/

    /********** Start: simulated annealing **********/
    
    start = clock();
    rand_part(plcnet); //Initialize: place all the cells at random
    
    int init_cost = plcnet.cal_total_cost();
    cout<<"Initial plcnet.cost="<<init_cost<<endl;
    
    int min_cost=0;
    for(i=0;i<6;i++)
    {
        min_cost = FM_algo(plcnet,i+1,gridsize);
        cout<<"min cost is "<<min_cost<<endl;
    }
    
    init_cost = plcnet.cal_total_cost();
    cout<<"Final plcnet.cost="<<init_cost<<endl<<endl;
    end = clock();
    
    duration = (end-start)/(double)CLOCKS_PER_SEC;
    cout<<"dtime="<<duration<<endl<<endl;
    
    /********** End: simulated annealing **********/

    clearscreen();
    draw_init(plcnet.grid_row,plcnet.grid_col,gridsize);
    draw_cells(plcnet,gridsize);
    draw_netline(plcnet,gridsize);
    flushinput();
    event_loop(act_on_button_press,NULL,NULL,NULL);//act_on_button_press
    close_graphics();
    printf ("Graphics closed down.\n");
    
    return 0;
}

bool read_file(char *filename,PLC &plcnet)
{
    int i,j;
    ifstream inf;
    inf.open(filename);
    if(!inf)
    {
        cout<<"Unable to open file"<<endl;
        return false; // terminate with error
    }
    inf>>plcnet.cell_num>>plcnet.wire_num>>plcnet.grid_row>>plcnet.grid_col;
    
    if(plcnet.cell_num>plcnet.grid_row*plcnet.grid_col)
    {
        cout<<"Too many cells for this net!"<<endl;
        return false; // terminate with error
    }
    
    plcnet.cells = new Grids [plcnet.cell_num];
    plcnet.parta = new int [plcnet.cell_num/2]; //The first step is to move a cell from B to A, so A always needs an extra position
    plcnet.partb = new int [plcnet.cell_num - (plcnet.cell_num/2)];
    for(i=0;i<plcnet.cell_num;i++)
    {
        plcnet.cells[i].cell_No = i;
    }
    plcnet.nets = new Net [plcnet.wire_num];
    for(i=0;i<plcnet.wire_num;i++)
    {
        plcnet.nets[i].net_No = i;
        inf>>plcnet.nets[i].cell_num;
        inf>>plcnet.nets[i].src_No;
        plcnet.nets[i].create_edges();
        plcnet.nets[i].create_sinks();

        for(j=0;j<plcnet.nets[i].cell_num-1;j++)
        {
            plcnet.nets[i].edge[j].src_No = plcnet.nets[i].src_No;
            inf>>plcnet.nets[i].sink_No[j];
            plcnet.nets[i].edge[j].sink_No = plcnet.nets[i].sink_No[j];
        }
    }
    inf.close();
    
    return true;
}

void rand_part(PLC &plcnet)
{
    int i;
    int random;
    srand((unsigned int)time(0));
    random = 0;
    for(i=0;i<plcnet.cell_num/2;i++) //i starts from 0
    {
        while(plcnet.cells[random].partA_B!=-1)
            random = rand()%plcnet.cell_num;
        plcnet.cells[random].partA_B=0;
        plcnet.parta_num += 1;  //However, parta_num doesn't need to add 1 because the maximum "unlocked" node in A is parta_num
                                //even it receives another node from B, because the moved node is locked!
        plcnet.parta[i] = random;
    }
    
    //plcnet.parta_num++; //for convinience
    for(;i<plcnet.cell_num;i++) //i starts from plcnet.cell_num/2
    {
        while(plcnet.cells[random].partA_B!=-1)
            random = rand()%plcnet.cell_num;
        plcnet.cells[random].partA_B=1;
        plcnet.partb_num += 1;
        plcnet.partb[i-plcnet.cell_num/2] = random;
    }
    
    plcnet.maxa_inx = plcnet.parta_num-1;
    plcnet.maxb_inx = plcnet.partb_num-1;
    //cout<<"maxa_inx="<<plcnet.maxa_inx<<" maxb_inx="<<plcnet.maxb_inx<<endl;
    plcnet.init_partnum(); //all the nets inits their cells' partition
    plcnet.init_crossing();
    plcnet.cal_all_gains();
    plcnet.set_inv_net();
    
    plcnet.in_bin_sort(plcnet.parta,plcnet.parta_num);
    plcnet.in_bin_sort(plcnet.partb,plcnet.partb_num);
}

void drawscreen(void)
{
    clearscreen();
    setcolor(BLACK);
    drawtext(500,200,"Please click \"procceed\"button",2000.);
    //    setcolor(BLACK);
    //    drawrect(500,500,500+100,500+100);
}

void act_on_button_press(float x, float y) {
    
    /* Called whenever event_loop gets a button press in the graphics *
     * area.  Allows the user to do whatever he/she wants with button *
     * clicks.                                                        */
    printf("User clicked a button at coordinates (%f, %f)\n", x, y);
}

void draw_init(int map_h,int map_w,int gridsize)
{
    //char nnnn[10];
    int gx=0,gy=0;
    int agap = gridsize/8;
    setcolor(BLACK);
    for(int i=0;i<map_h;i++)
    {
        gx=0;
        for(int j=0;j<map_w;j++)
        {
            if(j==map_w/2)
                gx += gridsize*2;
            drawrect(gx+agap,gy+agap,gx+gridsize-agap,gy+gridsize-agap);
            gx += gridsize;
        }
        gy += gridsize;
    }
}

void draw_cells(PLC &plcnet,int gridsize)
{
    char nnnn[10];
    int gx,gy;
    int agap = gridsize/8;
    
    int i;
    for(i=0;i<plcnet.parta_num+1;i++)
    {
        if(plcnet.parta[i]!=-1)
        {
            plcnet.cells[plcnet.parta[i]].x = i%(plcnet.grid_col/2);
            plcnet.cells[plcnet.parta[i]].y = i/(plcnet.grid_col/2);
            gx = plcnet.cells[plcnet.parta[i]].x*gridsize;
            gy = plcnet.cells[plcnet.parta[i]].y*gridsize;
            
            setcolor(LIGHTGREY);
            fillrect(gx+agap,gy+agap,gx+gridsize-agap,gy+gridsize-agap);
            sprintf(nnnn,"%d",plcnet.parta[i]);
            setcolor(WHITE);
            drawtext(gridsize/2+gx,gridsize/2+gy,nnnn,150.);
        }
    }
    
    for(i=0;i<plcnet.partb_num;i++)
    {
        if(plcnet.partb[i]!=-1)
        {
            plcnet.cells[plcnet.partb[i]].x = i%(plcnet.grid_col/2) + (plcnet.grid_col/2+2);
            plcnet.cells[plcnet.partb[i]].y = i/(plcnet.grid_col/2);
            gx = plcnet.cells[plcnet.partb[i]].x*gridsize;
            gy = plcnet.cells[plcnet.partb[i]].y*gridsize;
            
            setcolor(LIGHTGREY);
            fillrect(gx+agap,gy+agap,gx+gridsize-agap,gy+gridsize-agap);
            sprintf(nnnn,"%d",plcnet.partb[i]);
            setcolor(WHITE);
            drawtext(gridsize/2+gx,gridsize/2+gy,nnnn,150.);
        }
    }
}

void draw_netline(PLC &plcnet,int gridsize)
{
    int i,j;
    int gx1,gy1,gx2,gy2;
    setcolor(BLACK);
    for(i=0;i<plcnet.wire_num;i++)
    {
        gx1 = plcnet.cells[plcnet.nets[i].src_No].x * gridsize;
        gy1 = plcnet.cells[plcnet.nets[i].src_No].y * gridsize;
        for(j=0;j<plcnet.nets[i].cell_num-1;j++)
        {
            gx2 = plcnet.cells[plcnet.nets[i].sink_No[j]].x * gridsize;
            gy2 = plcnet.cells[plcnet.nets[i].sink_No[j]].y * gridsize;
            //cout<<"gx2="<<gx2<<"\tgy2="<<gy2<<endl;
            drawline(gridsize/2+gx1,gridsize/2+gy1,gridsize/2+gx2,gridsize/2+gy2);
        }
    }
}

void delay()
{
    int a=0;
    for(int i=0;i<delay_time;i++)
        for(int j=1;j<1000;j++)
            for(int k=0;k<1000;k++)
                a++;
}

int FM_algo(PLC &plcnet,int round,int gridsize)
{
    int i=0;
    int temp_cost=0;
    int roll_back=0;
    int min_cost = plcnet.cal_total_cost();
    
    int *temp_AB = new int [plcnet.cell_num];
    
    int move_cell=-1;
    /*
    int random;
    srand((unsigned int)time(0));
    for(i=0;round<3 && i<plcnet.cell_num/(round*4);i++)
    {
        random = rand()%plcnet.parta_num;
        while(plcnet.cells[plcnet.parta[random]].partA_B==1)
            random = rand()%plcnet.parta_num;
        move_cell = plcnet.parta[random];
        plcnet.cells[move_cell].partA_B = 1;
        
        random = rand()%plcnet.partb_num;
        while(plcnet.cells[plcnet.partb[random]].partA_B==0)
            random = rand()%plcnet.partb_num;
        move_cell = plcnet.partb[random];
        plcnet.cells[move_cell].partA_B = 0;
    }
     
    plcnet.re_init();
    
    for(i=0;i<plcnet.parta_num;i++)
    {
        if(plcnet.cells[plcnet.parta[i]].partA_B==1)
            cout<<plcnet.parta[i]<<" nima in a??"<<endl;
    }
    for(i=0;i<plcnet.partb_num;i++)
    {
        if(plcnet.cells[plcnet.partb[i]].partA_B==0)
            cout<<plcnet.partb[i]<<" nima in b??"<<endl;
    }
    */
    i=0;
    while(plcnet.maxa_inx>=0 || plcnet.maxb_inx>=0)
    {
        cout<<i+1<<"**"<<"maxa_inx="<<plcnet.maxa_inx<<" maxb_inx="<<plcnet.maxb_inx<<"\t";
        if(plcnet.maxa_inx>plcnet.maxb_inx)
        {
            move_cell = plcnet.parta[plcnet.maxa_inx]; //move A to B
        }
        if(plcnet.maxa_inx<plcnet.maxb_inx)
        {
            move_cell = plcnet.partb[plcnet.maxb_inx]; //move B to A
        }
        if(plcnet.maxa_inx==plcnet.maxb_inx)
        {
            if(plcnet.cells[plcnet.parta[plcnet.maxa_inx]].gain>plcnet.cells[plcnet.partb[plcnet.maxb_inx]].gain)
            {
                move_cell = plcnet.parta[plcnet.maxa_inx]; //move A to B
            }
            else
            {
                move_cell = plcnet.partb[plcnet.maxb_inx]; //move B to A
            }
        }
        
        cout<<plcnet.cells[plcnet.parta[plcnet.maxa_inx]].gain<<" vs. "<<plcnet.cells[plcnet.partb[plcnet.maxb_inx]].gain<<"   ";
        
        cout<<move_cell<<"."<<plcnet.cells[move_cell].locked<<"\t"<<plcnet.cells[move_cell].partA_B;
        plcnet.move_cell(move_cell);

        round++;
        plcnet.cal_all_gains();
        plcnet.in_bin_sort(plcnet.parta,plcnet.maxa_inx);
        plcnet.in_bin_sort(plcnet.partb,plcnet.maxb_inx);
        cout<<" after "<<plcnet.cells[move_cell].partA_B<<endl;
        
        clearscreen();
        draw_init(plcnet.grid_row,plcnet.grid_col,gridsize);
        draw_cells(plcnet,gridsize);
        draw_netline(plcnet,gridsize);
        flushinput();
        
        temp_cost = plcnet.cal_total_cost();
        if(min_cost>temp_cost && plcnet.maxb_inx-plcnet.maxa_inx==plcnet.partb_num-plcnet.parta_num)
        {
            min_cost = temp_cost;
            roll_back = move_cell;
            for(int p=0;p<plcnet.cell_num;p++)
            {
                temp_AB[p] = plcnet.cells[p].partA_B;
            }
        }
        i++;
    }
    
    
    for(i=0;i<plcnet.cell_num;i++)
    {
        plcnet.cells[i].partA_B = temp_AB[i];
    }
    plcnet.re_init();
    
    if(temp_AB)
        delete [] temp_AB;
    return min_cost;
}

