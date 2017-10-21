#include "astar.h"
#include <iostream>
#include <memory.h>
#include <algorithm>
#include <fstream>

using namespace std;

astar::astar():m_nx(0),m_ny(0),m_ns(0),m_f(NULL),m_close_list(NULL),m_parent(NULL)
{

}

astar::astar(int nx, int ny):m_nx(nx),m_ny(ny),m_ns(nx*ny),m_f(NULL),m_close_list(NULL),m_parent(NULL)
{

}

void astar::make_plan(unsigned char *map, aPoint start, aPoint end, std::vector<int> &plan)
{
    this->clear();
    m_close_list = new bool[m_ns];
    m_f = new float[m_ns];
    m_parent = new int[m_ns];
    memset(m_close_list,false,m_ns);
    for(int i=0;i<m_ns;i++)m_parent[i] = -1;
    for(int i=0;i<m_ns;i++)m_f[i] = POT_HIGH;

    for(int i=0;i<m_ns;i++){
        if(map[i] != 0)m_close_list[i] = true;
    }

    aCost first(this->get_index(start),0,0);
    m_f[first.i] = (abs(start.x-end.x)+abs(start.y-end.y))*10;
    m_parent[first.i] = first.i;
    m_open_list.push_back(first);

    while (m_open_list.size() > 0) {
        // find  min   ==>
        int index=this->find_min();
        aCost now_node = m_open_list[index];
        std::vector<aCost>::iterator it = m_open_list.begin()+index;
        m_open_list.erase(it);
        m_close_list[now_node.i] = true;
        // end find    <==

        if(now_node.i == this->get_index(end))break;

        this->add_node(now_node,aPoint(-1,-1),end);
        this->add_node(now_node,aPoint(-1,0),end);
        this->add_node(now_node,aPoint(-1,1),end);
        this->add_node(now_node,aPoint(0,-1),end);
        this->add_node(now_node,aPoint(0,1),end);
        this->add_node(now_node,aPoint(1,-1),end);
        this->add_node(now_node,aPoint(1,0),end);
        this->add_node(now_node,aPoint(1,1),end);
    }

    this->get_path(start,end,plan);

/*
    fstream file("b.txt",ios::out);
    for(int i=0;i<m_ns;i++){
        if((i%m_nx == 0) && (i!=0))file<<endl;
        if(m_f[i]==POT_HIGH)file<<"█";
        else file<<m_f[i]<<",";
    }
    file<< endl;
    file.close();
*/
    this->clear();
}

int astar::get_index(aPoint &point)
{
    return point.x+point.y*m_nx;
}

int astar::get_index(int x, int y)
{
    return x+y*m_nx;
}

int astar::find_min()
{
    int index=0,temp=POT_HIGH;
    int size = m_open_list.size();
    for(int i=0;i<size;i++){
        if(m_open_list[i].f<temp){
            temp = m_open_list[i].f;
            index = i;
        }
    }
    return index;
}

astar::~astar()
{
    this->clear();
}

//把节点加入open表
void astar::add_node(aCost &now_node, aPoint relative_point, aPoint &end_point)
{
    if(relative_point.x > 1 || relative_point.x < -1)return;
    if(relative_point.y > 1 || relative_point.y < -1)return;
    if((relative_point.x==0) && (relative_point.y==0))return;

    float relative_g = 0;
    if((relative_point.x == 0) || (relative_point.y == 0))relative_g = 10.0;
    else relative_g = 14.0;

    int x = now_node.i%m_nx;
    int y = now_node.i/m_nx;

    int new_x = x+relative_point.x;
    if(new_x < 0 || new_x > m_nx-1) return;

    int new_y = y+relative_point.y;
    if(new_y < 0 || new_y > m_ny-1) return;
//    if(new_x == end_point.x && new_y == end_point.y){
//        m_open_list.clear();
//        return;
//    }

    int new_index = this->get_index(new_x,new_y);
    if(m_close_list[new_index])return;

    float new_g = now_node.g+relative_g;
    float new_h = (abs(end_point.x-new_x)+abs(end_point.y-new_y))*10;
    float new_f = new_g + new_h;

    int length = m_open_list.size();
    bool flag = false;
    for(int i=0;i<length;i++){
        if(m_open_list[i].i==new_index){
            flag = true;
            if(m_open_list[i].g > new_g){
                m_open_list[i].g = new_g;
                m_open_list[i].f = new_f;
                m_f[new_index] = new_f;
                m_parent[new_index] = now_node.i;
            }
        }
    }

    if(!flag){
        m_parent[new_index] = now_node.i;
        aCost new_node(new_index,new_f,new_g);
        m_f[new_index] = new_f;
        m_open_list.push_back(new_node);
    }
}

//生成路径
void astar::get_path(aPoint &start,aPoint &end,std::vector<int> &plan)
{
//    bool *over = new bool[m_ns];
//    memset(over,false,m_ns);
    plan.clear();
    aPoint now_point = end;

    while(!(now_point == start)) {
        int index = this->get_index(now_point);
        plan.push_back(index);

        int parent = m_parent[index];
        if(parent < 0)break;
        int x = parent%m_nx;
        int y = parent/m_nx;
        now_point.x = x;
        now_point.y = y;
    }

//    while(!(now_point == start)) {
//        int index = this->get_index(now_point);
//        over[index] = true;
//        plan.push_back(index);

//        float min=POT_HIGH;
//        aPoint temp(-1,-1);
//        for(int i=-1;i<2;i++){
//            for(int j=-1;j<2;j++){
//                if(i==0 && j==0)continue;
//                int new_x = now_point.x+i;
//                int new_y = now_point.y+j;
//                if(new_x < 0 || new_x > m_nx-1)continue;
//                if(new_y < 0 || new_y > m_ny-1)continue;
//                if(over[this->get_index(new_x,new_y)])continue;
//                float f = m_f[this->get_index(new_x,new_y)];
//                over[this->get_index(new_x,new_y)] = true;
//                if(f<min){
//                    min = f;
//                    temp.x=new_x;
//                    temp.y=new_y;
//                }
//            }
//        }
//        if(temp.x>=0 && temp.y>=0){
//            now_point=temp;
//        }else {
//            break;
//        }
//    }
//    delete []over;
    return;
}

void astar::clear()
{
    m_open_list.clear();

    if(m_close_list){
        delete []m_close_list;
        m_close_list = NULL;
    }

    if(m_parent){
        delete []m_parent;
        m_parent = NULL;
    }

    if(m_f){
        delete []m_f;
        m_f = NULL;
    }
}




