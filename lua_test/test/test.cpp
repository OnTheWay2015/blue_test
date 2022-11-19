
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <queue>

class cvPoint 
{
public :
cvPoint(int x,int y):m_x(x),m_y(y) {}
~cvPoint() {}

public :
    int m_x;
    int m_y;

};

class gm_path_node
{
public :
    gm_path_node():
        x(0), y(0), G(0), F(0), H(0), parent(NULL)
    {

    }
    gm_path_node(unsigned short _x, unsigned short _y, unsigned short _G, unsigned short _F, unsigned short _H, gm_path_node* _p):
        x(_x), y(_y), G(_G), F(_F), H(_H), parent(_p)
    {

    }
    ~gm_path_node() {}

public :
    unsigned short x;
    unsigned short y;
    unsigned short G;
    unsigned short F;
    unsigned short H;
    gm_path_node* parent;
};

struct less
{
    bool operator()(gm_path_node*& _Left, gm_path_node*& _Right) const
    {
        if (_Left->F < _Right->F)
        {
            return false;
        }
        else if (_Left->F == _Right->F)
        {
            if (_Left->H <= _Right->H)
            {
                return false;
            }
        }
        return true;
    }
};
typedef std::map<int,bool> BlockInfosType ;

    bool is_grid_walkable(int x, int y, BlockInfosType& BlockInfos)
    {
        //if(x < 0 || y<0 || x >= m_mapWidth || y >= m_mapHeight)
        //{
        //    return false;
        //}
        //
        //int idx = y*m_mapWidth + x;
        //if(idx >= grids.size())
        //{
        //    script_log(SLT_ERR, "map[%d] is_grid_walkable idx[%d] >= grids.size[%d]\n", pmap_desc->id, idx, grids.size());
        //    return false;
        //}

        return true;
    }

int gGetDirection(int fx, int fy, int tx, int ty)
{// [1,2，4，8，16,32,64,128,右下左上,右下，左下，左上，右上,]

    int xsub = tx - fx;
    int ysub = ty - fy;
    int d = 0;
    if(  ysub == 0 )
    {
        d = ( xsub > 0 ) ? 1 : 4;
    }
    else if( xsub == 0 )
    {
        d = ( ysub > 0 ) ? 2 : 8;
    }
    else if( xsub >0 && ysub >0 )
    {
        d = 16;
    }
    else if( xsub >0 && ysub < 0 )
    {
        d = 128;
    }
    else if( xsub <0 && ysub > 0 )
    {
        d = 32;
    }
    else if( xsub <0 && ysub < 0 )
    {
        d = 64;
    }
    return d;
}


//bool find_path(int frm_x, int frm_y, int to_x, int to_y, dynamic_array<cvPoint>& path, bool keepDir)
bool find_path1(BlockInfosType& BlockInfos, int width,int height,
                int frm_x, int frm_y, int to_x, int to_y, std::vector<cvPoint>& path, bool keepDir)
{
    if (!is_grid_walkable(frm_x, frm_y, BlockInfos))
    {
        return false;
    }

    if (!is_grid_walkable(to_x, to_y, BlockInfos))
    {
        return false;
    }

    path.clear();

    if (frm_x == to_x && frm_y == to_y)
    {
        return true;
    }

    std::map<unsigned int, gm_path_node>    nodes;
    std::priority_queue<gm_path_node*, std::vector<gm_path_node*>, less>   open_grids;
    char grid_mark[256 * 256];
    memset(grid_mark, 0, sizeof(grid_mark));


    unsigned int tile_idx = (frm_y << 8) | (frm_x & 0x000000ff);
    gm_path_node& start_node = nodes[tile_idx];
    start_node = gm_path_node(frm_x, frm_y, 0, 0, 0, nullptr);
    open_grids.push(&start_node);

    gm_path_node* end_node = NULL;

    while (open_grids.size() > 0)
    {
        gm_path_node* cur_node = open_grids.top();
        open_grids.pop();

        unsigned int node_idx = (unsigned int)((cur_node->y << 8) | (cur_node->x & 0x000000ff));

        grid_mark[node_idx] = 1; // mark as closed

        int i = 0;
        int j = 0;
        for (i = 0; i < 3; ++i)
        {
            int edge_node_y = (cur_node->y + i - 1);

            if (edge_node_y < 0 || edge_node_y >= height)
            {
                continue;
            }

            unsigned int edge_node_idx_y = edge_node_y << 8;
            for (j = 0; j < 3; ++j)
            {
                int edge_node_x = (cur_node->x + j - 1);

                if (edge_node_x < 0 || edge_node_x >= width)
                {
                    continue;
                }

                int edge_node_idx = edge_node_idx_y | (edge_node_x & 0x000000ff);

                if (grid_mark[edge_node_idx] != 0)
                {
                    continue;
                }

                if (!is_grid_walkable(edge_node_y, edge_node_x, BlockInfos))
                {
                    grid_mark[edge_node_idx] = 1;
                    continue;
                }

                int G = cur_node->G + 1;
                if (cur_node->x != edge_node_x && cur_node->y != edge_node_y)
                {
                    G = cur_node->G + 2;
                }

                if (keepDir && (cur_node->parent != NULL))
                {
                    int dir1 = gGetDirection(cur_node->parent->x, cur_node->parent->y, cur_node->x, cur_node->y);
                    int dir2 = gGetDirection(cur_node->x, cur_node->y, edge_node_x, edge_node_y);

                    if (dir1 != dir2)
                    {
                        G += 1;
                    }
                }

                int H = abs(edge_node_y - to_y) + abs(edge_node_x - to_x);
                int F = G + H;

                std::map<unsigned int, gm_path_node>::iterator it = nodes.find(edge_node_idx);
                if (it != nodes.end())
                {
                    gm_path_node& edge_node = (it->second);
                    if (F < edge_node.F || (F == edge_node.F && H < edge_node.H))
                    {
                        edge_node.F = F;
                        edge_node.H = H;
                        edge_node.G = G;
                        edge_node.parent = cur_node;
                    }
                }
                else
                {


                    gm_path_node& new_node = nodes[edge_node_idx];
                    new_node = gm_path_node(edge_node_x, edge_node_y, G, F, H, cur_node);



                    std::stringstream ss;
                    ss << "idx" << edge_node_idx
                        << ",add x:" << new_node.x
                        << ",y:" << new_node.y
                        << ",G:" << new_node.G
                        << ",F:" << new_node.F
                        << ",H:" << new_node.H
                        << std::endl;
                    std::cout << ss.str();


                    open_grids.push(&new_node);


                    if (edge_node_x == to_x && edge_node_y == to_y)
                    {
                        end_node = &new_node;
                        break;
                    }
                }
            }

            if (end_node != NULL)
            {
                break;
            }
        }

        if (end_node != NULL)
        {
            break;
        }
    }

    if (end_node == NULL)
    {
        return false;
    }

    for (; end_node != NULL; )
    {
        path.push_back(cvPoint(end_node->x, end_node->y));
        end_node = end_node->parent;
    }
    return true;
}


// 使用friend其排序与sort排序刚好相反 
//struct node
//{
//	std::string name;
//	int price;
//	//friend bool operator<(const node &f1,const node &f2) 
//	//建议使用上面的引用来提高效率 
//	friend bool operator<(node f1,node f2)
//	{
//		return f1.price<f2.price;
//	}
//};
//priority_queue<node> q;
// 不使用 friend 友元将它写在外面 
struct node
{
	std::string name;
	int price;
};
struct cmp
{
	bool operator() (node f1,node f2)
	{
		return f1.price<f2.price;
	}
} ;

void testMapPath1()
{
    std::priority_queue<node, std::vector<node>, cmp> q;


    for (int i = 0; i < 50; i++)
    {
        q.push({ "124",1 });
    }

    //q.push({ "124",-1 });
    //q.push({ "124",100 });

    //q.push({ "124",-1 });
    //q.push({ "124",100 });

    //q.push({ "124",-1 });
    //q.push({ "124",100 });



    //std::map<unsigned int ,gm_path_node>    nodes;
    //std::priority_queue<gm_path_node*, std::vector<gm_path_node*>, gm_path_node::less>   open_grids;
    //gm_path_node& start_node = nodes[0];
    //start_node = gm_path_node(1, 1, 0, 0, 0, NULL);
    //int i =1;
    //for ( ; i < 50; i++)
    //{
    //    if (i % 5 == 0)
    //    {
    //        open_grids.pop();
    //    }
    //    else
    //    {
    //        gm_path_node& new_node = nodes[i];
    //        new_node = gm_path_node(i, i, i, i, i, nullptr);
    //        open_grids.push(&new_node);

    //    }
    //}
  
    //{
    //i = 80; 
    //gm_path_node& new_node = nodes[i];
    //new_node = gm_path_node(i, i, i, i, i, nullptr);
    //open_grids.push(&new_node);

    //} 
    //{
    //i = 60; 
    //gm_path_node& new_node = nodes[i];
    //new_node = gm_path_node(i, i, i, i, i, nullptr);
    //open_grids.push(&new_node);

    //} 
    //{

    //i = 81; 
    //gm_path_node& new_node = nodes[i];
    //new_node = gm_path_node(i, i, i, i, i, nullptr);
    //open_grids.push(&new_node);

    //} 
    //{

    //i = 61; 
    //gm_path_node& new_node = nodes[i];
    //new_node = gm_path_node(i, i, i, i, i, nullptr);
    //open_grids.push(&new_node);

    //} 
    //{

    //} 

    
}


bool find_path(BlockInfosType& BlockInfos, int width,int height,
                int frm_x, int frm_y, int to_x, int to_y, std::vector<cvPoint>& path, bool keepDir)

{


    std::vector<gm_path_node*> nodes;
    //std::map<unsigned int ,gm_path_node> nodes;
    std::priority_queue<gm_path_node*, std::vector<gm_path_node*>, less>   open_grids;


    gm_path_node* start_node = new gm_path_node (frm_x, frm_y, 0, 0, 0, nullptr);
    nodes.push_back(start_node);
    open_grids.push(start_node);

    int vvv = 1;
    while(open_grids.size() > 0)
    {
        gm_path_node* cur_node = open_grids.top();
        open_grids.pop();

        unsigned int node_idx = (unsigned int)((cur_node->y << 8) | (cur_node->x & 0x000000ff));

        int i = 0;
        int j = 0;
        for (i = 0; i < 3; ++i)
        {
            int edge_node_y = (cur_node->y + i - 1);


            unsigned int edge_node_idx_y = edge_node_y << 8;
            for (j = 0; j < 3; ++j)
            {
                int edge_node_x = (cur_node->x + j - 1);
                int edge_node_idx = edge_node_idx_y | (edge_node_x & 0x000000ff);

                int G = cur_node->G + 1;
                if (cur_node->x != edge_node_x && cur_node->y != edge_node_y)
                {
                    G = cur_node->G + 2;
                }

                int H = abs(edge_node_y - to_y) + abs(edge_node_x - to_x);
                int F = G + H;

                {
                    vvv ++;
                    //gm_path_node new_node(edge_node_x, edge_node_y, G, F, H, nullptr);
                    gm_path_node* new_node =  new gm_path_node(1, 1, vvv, vvv, vvv, nullptr);
                    //nodes.push_back(new_node);
                    open_grids.push(new_node);

                    if (edge_node_x == to_x && edge_node_y == to_y)
                    {
                        break;
                    }
                }
            }

        }

    }

    return true;
}
void testMapPath()
{
    BlockInfosType  infos;
    std::vector<cvPoint> path;
    find_path1(infos,100,100,10,10,30,30,path,true);
    std::stringstream ss;
    for (auto&&v : path)
    {
        ss << "x:" << v.m_x <<  "y:" << v.m_y << std::endl;
    }
   
    std::cout << ss.str(); 
}


