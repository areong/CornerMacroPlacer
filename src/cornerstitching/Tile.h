#ifndef CORNERSTITCHING_TILE_H_
#define CORNERSTITCHING_TILE_H_

class Corner;

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
    int getWidth() const;
    int getHeight() const;
    double getArea();

    // The two methods are used as sorting keys.
    int getPreviousWidth() const;
    int getPreviousHeight() const;
    /*
    Please call this method after width or height changed and
    before the Tile is inserted to a sorted container.
    It prevents the sorting keys, previousWidth and previousHeight, being changed
    when the Tile is in the sorted container.
    This method assigns width and height to previousWidth and previousHeight respectively.
    */
    void updateWidthAndHeightForSorting();

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
    void setTemporarilySolid();
    bool isTemporarilySolid();
    void setGroupId(int groupId);
    int getGroupId();
    bool hasGroupId();
    bool hasNoGroupId();

    // The following methods are for Corners.

    /*
    Set four blockage attributes.
    If fullyBlocked, set openWidths of the side zero.
    */
    void setBottomBlocked(bool blocked, bool fullyBlocked = false);
    void setTopBlocked(bool blocked, bool fullyBlocked = false);
    void setLeftBlocked(bool blocked, bool fullyBlocked = false);
    void setRightBlocked(bool blocked, bool fullyBlocked = false);
    /*
    Copy the corresponding attributes from tile.
    It is used in splitting Tiles.
    */
    void setBottomBlocked(Tile *tile);
    void setTopBlocked(Tile *tile);
    void setLeftBlocked(Tile *tile);
    void setRightBlocked(Tile *tile);
    bool checkBottomBlocked();
    bool checkTopBlocked();
    bool checkLeftBlocked();
    bool checkRightBlocked();
    bool isBottomBlocked();
    bool isTopBlocked();
    bool isLeftBlocked();
    bool isRightBlocked();
    int getBlOpenWidth();
    int getBrOpenWidth();
    int getTlOpenWidth();
    int getTrOpenWidth();
    int getLbOpenHeight();
    int getLtOpenHeight();
    int getRbOpenHeight();
    int getRtOpenHeight();

    /*
    Only assign corner. Do nothing else.
    */
    void setBlCorner(Corner *corner);
    void setBrCorner(Corner *corner);
    void setTlCorner(Corner *corner);
    void setTrCorner(Corner *corner);
    Corner *getBlCorner();
    Corner *getBrCorner();
    Corner *getTlCorner();
    Corner *getTrCorner();
    /*
    Assume both sides are blocked.
    Assume at most one side has positive open width/height.
    When creating a Corner, the Tile checks
    whether the corresponding Corner exists.
    If true, return 0. Else, return the created Corner.
    The type of created Corners are determined by openWidths,
    thus the openWidths are assumed correct.
    */
    Corner *createBlCorner();
    Corner *createBrCorner();
    Corner *createTlCorner();
    Corner *createTrCorner();
    /*
    Return the Corner and set the attribute to zero.
    If the Corner does not exist, return zero.
    Removing a Corner does not delete the Corner.
    */
    Corner *removeBlCorner();
    Corner *removeBrCorner();
    Corner *removeTlCorner();
    Corner *removeTrCorner();

    // Testing
    void print();
    void printFourNeighbors();

private:
    int xStart;
    int yStart;
    int xEnd;
    int yEnd;
    int width;
    int height;
    int previousWidth;  // These two attributes are used for sorting
    int previousHeight; // to avoid sorting keys being changed. See Trello card 21.
    Tile *tr;
    Tile *rt;
    Tile *bl;
    Tile *lb;
    bool solid;
    bool temporarilySolid;  // Represent wasted region.
    int groupId;    // Used in calculating empty space areas. It is -1 by default.

    // The following attributes are for Corners.
    // These are used only if the Tile is empty.

    bool bottomBlocked;
    bool topBlocked;
    bool leftBlocked;
    bool rightBlocked;
    int blOpenWidth;
    int brOpenWidth;
    int tlOpenWidth;
    int trOpenWidth;
    int lbOpenHeight;
    int ltOpenHeight;
    int rbOpenHeight;
    int rtOpenHeight;
    Corner *blCorner;
    Corner *brCorner;
    Corner *tlCorner;
    Corner *trCorner;
};

struct CompareTileWidth {
    bool operator() (const Tile *tile1, const Tile *tile2) const {
        return (tile1->getPreviousWidth() < tile2->getPreviousWidth());
    }
};

struct CompareTileHeight {
    bool operator() (const Tile *tile1, const Tile *tile2) const {
        return tile1->getPreviousHeight() < tile2->getPreviousHeight();
    }
};

#endif