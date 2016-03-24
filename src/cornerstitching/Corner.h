#ifndef CORNERSTITCHING_CORNER_H_
#define CORNERSTITCHING_CORNER_H_

class Tile;

class Corner {
public:
    // @param   isGapOnHorizontalSide
    //          For a type 0 Corner, this attribute is not used.
    Corner(int x, int y, int direction, bool isType1, bool isGapOnHorizontalSide);
    ~Corner();
    void setHorizontalTile(Tile *tile);
    void setVerticalTile(Tile *tile);
    Tile *getHorizontalTile();
    Tile *getVerticalTile();
    void setType(bool isType1);
    bool isType0();
    bool isType1();
    int getX();
    int getY();

    // test

    void print();

private:
    int x;
    int y;
    // Value 0, 1, 2, 3 represents bl, br, tl and tr respectively.
    int direction;
    bool type1;
    bool gapOnHorizontalSide;
    Tile *horizontalTile;
    Tile *verticalTile;
};

#endif