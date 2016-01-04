#ifndef CORNERSTITCHING_TILE_H_
#define CORNERSTITCHING_TILE_H_

class Tile {
public:
    Tile(int xStart, int yStart, int xEnd, int yEnd, bool isSolid);
    ~Tile();
    void setXStart(int xStart);
    void setYStart(int yStart);
    void setXEnd(int xEnd);
    void setYEnd(int yEnd);
    int getXStart();
    int getYStart();
    int getXEnd();
    int getYEnd();
    void setTr(Tile *tile);
    void setRt(Tile *tile);
    void setBl(Tile *tile);
    void setLb(Tile *tile);
    Tile *getTr();
    Tile *getRt();
    Tile *getBl();
    Tile *getLb();
    bool isSolid();
    bool isEmpty();

    // Testing
    void print();
    void printFourNeighbors();

private:
    int xStart;
    int yStart;
    int xEnd;
    int yEnd;
    Tile *tr;
    Tile *rt;
    Tile *bl;
    Tile *lb;
    bool solid;
};

#endif