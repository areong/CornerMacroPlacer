#ifndef VIEW_VIEW_H_
#define VIEW_VIEW_H_

class Window;

class View {
public:
    View(Window *window);
    virtual ~View();
    virtual void display() = 0;

protected:
    Window *window;
};

#endif