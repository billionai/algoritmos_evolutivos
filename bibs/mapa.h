#ifndef MAPA_HEADER

#define MAPA_HEADER

#include "tile.h"
#include <queue>
#include <algorithm>

class map{
private:
    tile** t;
    int height, width;
public:
    map(int h=0,int w=-1):
        height(h),
        width((w>0)?w:h)
    {
        t=new tile*[height];
        t[0]=new tile[height*width];
        for(int i=1;i<width;i++){
            t[i]=t[i-1]+height;
        }
    }
    ~map(){
        delete[] t[0];
        delete[] t;
    }

    int h(){return height;}
    int w(){return width;}

    tile& operator [](coord P){ return t[P.x()][P.y()];}

    bool can_move(coord pos,int dir){
        return  ((dir == UP) && pos.x() > 0)        ||
                ((dir ==DOWN)&& pos.x() < height -1)||
                ((dir ==LEFT)&& pos.y() > 0)        ||
                ((dir==RIGHT)&& pos.y() < width - 1);
    }

    void connect(char dir, coord c){
        t[c.x()][c.y()].add_dir(dir);
        c=c.move(dir);
        t[c.x()][c.y()].add_dir((dir<=DOWN)?(dir^(DOWN|UP)):(dir^(LEFT|RIGHT)));
    }

    //return the smallest path from p1 to p2 EXCLUDING P1
    std::vector<coord> BFS(coord p1,coord p2){
        std::queue<coord> q;
        std::vector<coord> path;
        //matriz indicating which was the coordinate prior to his one
        coord parent[height][width];
        coord unvisited(-1,-1);

        q.push(p1);

        for(int i=0;i<height;i++)
            for(int j=0;j<width;j++) parent[i][j].set(-1,-1);
        //coordinate -1,-1 indicates that this tile was not visited yet

        //sets the first tile to be it's own parent as the end condition
        parent[p1.x()][p1.y()]=p1; 

        while(!q.empty()){
            p1=q.front();
            q.pop();

            //if the FINAL DESTINATION was reached, exit the loop
            if(p1==p2) break;

            //adds all possible directions to the queue
            if(((*this)[p1].up())&& parent[p1.x()-1][p1.y()]==unvisited){
                q.push(p1.move(UP));
                parent[p1.x()-1][p1.y()]=p1;
            }
            if(((*this)[p1].down())&& parent[p1.x()+1][p1.y()]==unvisited){
                q.push(p1.move(DOWN));
                parent[p1.x()+1][p1.y()]=p1;
            }
            if(((*this)[p1].left())&& parent[p1.x()][p1.y()-1]==unvisited){
                q.push(p1.move(LEFT));
                parent[p1.x()][p1.y()-1]=p1;
            }
            if(((*this)[p1].right())&& parent[p1.x()][p1.y()+1]==unvisited){
                q.push(p1.move(RIGHT));
                parent[p1.x()][p1.y()+1]=p1;
            }

        }
        if(parent[p2.x()][p2.y()]==unvisited) return path;//empty because no path could be found
        path.push_back(p2);
        p1=parent[p2.x()][p2.y()];
        while(p1!=parent[p1.x()][p1.y()]){
            path.push_back(p1);
            p1=parent[p1.x()][p1.y()];
        }
        //path is reversed, must change it back
        std::reverse(path.begin(),path.end());
        return path;
    }

    void unlock(coord pos){
        int dir;
        dir=(*this)[pos].get_lock_dir();
        (*this)[pos].unlock();
        (*this)[pos.move(dir)].add_dir(dir^((dir<LEFT)?(DOWN|UP):(LEFT|RIGHT)));
    }
};

#endif
