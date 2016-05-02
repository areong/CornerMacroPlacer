#ifndef VIEW_WINDOW_H_
#define VIEW_WINDOW_H_

#include <string>

class Window {
public:
    Window();
    virtual ~Window();
    void setWindowTitle(std::string title);
    void setWindowSize(int width, int height);
    /*
    Recalculate the given x y range according to the window size
    such that the shape on the window will display with x:y ratio 1:1.
    It is assumed that xStart < xEnd and yStart < yEnd.
    */
    void setXYRange(int xStart, int yStart, int xEnd, int yEnd);
    /*
    All stuff to be done in every main loop event,
    including glClear() and glFlush().
    */
    virtual void display() = 0;
    /*
    Call glutDisplayFunc() to register Window.display().
    */
    virtual void setDisplayFunction() = 0;
    /*
    Initialize Window including window size and x y range.
    */
    void initialize();
    /*
    Call glutMainLoop();
    */
    void runMainLoop();
    /*
    Call glutMainLoopEvent();
    */
    void runMainLoopEvent();

    // Tools

    /*
    @param noFill True than the rectangle will not be filled but stay hollow.
                  Only the outine will be drawn.
    */
    void drawRectangle(int xStart, int yStart, int xEnd, int yEnd,
        float fillR, float fillG, float fillB,
        float outlineR, float outlineG, float outlineB, bool noFill=false);
    void drawLine(float xStart, float yStart, float xEnd, float yEnd,
        float r, float g, float b);

protected:
    std::string windowTitle;
    int windowWidth;
    int windowHeight;
    int rangeXStart;
    int rangeXEnd;
    int rangeYStart;
    int rangeYEnd;
};

#endif