#include "view/Window.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

Window::Window() {
    windowWidth = 1;
    windowHeight = 1;
    rangeXStart = 0;
    rangeXEnd = 1;
    rangeYStart = 0;
    rangeYEnd = 1;
}

Window::~Window() {

}

void Window::setWindowSize(int width, int height) {
    windowWidth = width;
    windowHeight = height;
}

void Window::setXYRange(int xStart, int yStart, int xEnd, int yEnd) {
    double windowRatio = (double) windowHeight / (double) windowWidth;
    double rangeWidth = xEnd - xStart;
    double rangeHeight = yEnd - yStart;
    double rangeRatio = rangeHeight / rangeWidth;
    if (rangeRatio >= windowRatio) {
        int heightIncrease = (int) (rangeHeight / 100); // Leave some margin.
        rangeYStart = yStart - heightIncrease;
        rangeYEnd = yEnd + heightIncrease;
        int widthIncrease = (int) ((rangeHeight * 1.02 / windowRatio - rangeWidth) / 2);
        rangeXStart = xStart - widthIncrease;
        rangeXEnd = xEnd + widthIncrease;
    } else {
        int widthIncrease = (int) (rangeWidth / 100); // Leave some margin.
        rangeXStart = xStart - widthIncrease;
        rangeXEnd = xEnd + widthIncrease;
        int heightIncrease = (int) ((rangeWidth * 1.02 * windowRatio - rangeHeight) / 2);
        rangeYStart = yStart - heightIncrease;
        rangeYEnd = yEnd + heightIncrease;
    }
}

void Window::initialize() {
    // Dummy input
    int argc = 0;
    char **argv;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello");
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(rangeXStart, rangeXEnd, rangeYStart, rangeYEnd, -1.0, 1.0);

    setDisplayFunction();
}

void Window::runMainLoop() {
    glutMainLoop();
}

void Window::runMainLoopEvent() {
    glutMainLoopEvent();
}

void Window::drawRectangle(int xStart, int yStart, int xEnd, int yEnd,
    float fillR, float fillG, float fillB,
    float outlineR, float outlineG, float outlineB, bool noFill) {
    if (!noFill) {
        glColor3f(fillR, fillG, fillB);
        glRectf(xStart, yStart, xEnd, yEnd);
    }
    glColor3f(outlineR, outlineG, outlineB);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xStart, yStart);
    glVertex2f(xEnd, yStart);
    glVertex2f(xEnd, yEnd);
    glVertex2f(xStart, yEnd);
    glEnd();
}

void Window::drawLine(float xStart, float yStart, float xEnd, float yEnd,
    float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_LINES);
    glVertex2f(xStart, yStart);
    glVertex2f(xEnd, yEnd);
    glEnd();
}