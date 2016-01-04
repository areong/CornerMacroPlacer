#ifndef VIEW_VIEW_H_
#define VIEW_VIEW_H_

class Window;

class View {
public:
    View(Window *window);
    ~View();
    virtual void display() = 0;

protected:
    Window *window;
};

#endif