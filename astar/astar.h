/*
 *
 * A*
 * F = G + H;
 * F为总代价，G是起点到指定格子的移动代价，H是指定格子到达终点的估算代价
 */

#ifndef ASTAR_H
#define ASTAR_H

#define POT_HIGH  1e10

#include <vector>

//坐标点类，x为地图中x，y为地图中y
class  aPoint
{
public:
     aPoint(int a_x,int a_y) {
         x = a_x;
         y = a_y;
     };
     int x;
     int y;
     bool operator ==(const aPoint &other) const{
         return x==other.x && y==other.y;
     }
};

//代价类， index为点的索引值，f为总代价，g为起点到当前点的移动代价
class aCost
{
public:
    aCost(int a_i,float a_f,float a_g) {
        i = a_i;
        f = a_f;
        g = a_g;
    };
    int i;
    float f;
    float g;
    bool operator ==(const aCost &other) const{
        return i == other.i;
    }
};


class astar
{
public:
    astar();
    astar(int nx,int ny);
    void make_plan(unsigned char *map, aPoint start, aPoint end, std::vector<int> &plan);
    int get_index(aPoint &point);
    int get_index(int x,int y);
    ~astar();

private:
    int m_nx;
    int m_ny;
    int m_ns;

    float *m_f;                     //存储每个点的 f 值，总代价
    int   *m_parent;                //每个节点的父节点
    bool  *m_close_list;            //close表
    std::vector<aCost> m_open_list; //open表

    void add_node(aCost &now_node,aPoint relative_point,aPoint &end_point);
    void get_path(aPoint &start,aPoint &end,std::vector<int> &plan);
    void clear();
    int find_min();
};

#endif // ASTAR_H
