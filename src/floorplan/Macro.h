#ifndef FLOORPLAN_MACRO_H_
#define FLOORPLAN_MACRO_H_

class Macro {
public:
    Macro(int width, int height);
    ~Macro();
    int getWidth() const;
    int getHeight() const;
    void setXStart(int xStart);
    int getXStart();
    void setYStart(int yStart);
    int getYStart();
    void setXEnd(int xEnd);
    int getXEnd();
    void setYEnd(int yEnd);
    int getYEnd();

private:
    int width;
    int height;
    int xStart;
    int yStart;
    int xEnd;
    int yEnd;
};

#endif