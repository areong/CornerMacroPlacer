#include "quadtree/Quadtree.h"
#include <algorithm>
#include <iostream>
#include "quadtree/Point.h"
#include "quadtree/PointValue.h"
#include "utils/Utils.h"

Quadtree::Quadtree(int xStart, int yStart, int xEnd, int yEnd, PointValue *pointValue) {
    this->xStart = xStart;
    this->yStart = yStart;
    this->xEnd = xEnd;
    this->yEnd = yEnd;
    this->pointValue = pointValue;
    xCenter = (int) ((xStart + xEnd) / 2);
    yCenter = (int) ((yStart + yEnd) / 2);
    lHalfWidth = xCenter - xStart;
    rHalfWidth = xEnd - xCenter;
    bHalfHeight = yCenter - yStart;
    tHalfHeight = yEnd - yCenter;
    if (lHalfWidth == 0) lHalfWidth = 1;    // Avoid divide by zero.
    if (rHalfWidth == 0) rHalfWidth = 1;
    if (bHalfHeight == 0) bHalfHeight = 1;
    if (tHalfHeight == 0) tHalfHeight = 1;
    points = new std::vector<Point *>();
    bl = 0;
    br = 0;
    tl = 0;
    tr = 0;
    numAllPoints = 0;
    noMatchedPointFound = false;
}

Quadtree::~Quadtree() {
    delete pointValue;
    delete points;
    delete bl;
    delete br;
    delete tl;
    delete tr;
}

void Quadtree::insert(Point *point, int x, int y, bool useInputXY) {
    numAllPoints += 1;
    int x1;
    int y1;
    if (useInputXY) {
        x1 = x;
        y1 = y;
    } else {
        x1 = pointValue->getX(point);
        y1 = pointValue->getY(point);
    }
    if (bl == 0) {
        // The Quadtree does not split. Try to put point here.
        if (points->size() == 0) {
            points->push_back(point);
            return;
        } else {
            int x2 = pointValue->getX(points->at(0));
            int y2 = pointValue->getY(points->at(0));
            if (x1 == x2 && y1 == y2) {
                // X and y are same. Add point to this Quadtree.
                points->push_back(point);
                return;
            } else {
                // Create sub Quadtrees and insert points to the sub Quadtrees.
                // Insert point later.
                bl = new Quadtree(xStart, yStart, xCenter, yCenter, pointValue->copy());
                br = new Quadtree(xCenter, yStart, xEnd, yCenter, pointValue->copy());
                tl = new Quadtree(xStart, yCenter, xCenter, yEnd, pointValue->copy());
                tr = new Quadtree(xCenter, yCenter, xEnd, yEnd, pointValue->copy());
                getSubQuadtreePointIsIn(x2, y2)->insert(points, x2, y2, true);
                points->clear();
            }
        }
    }
    // Insert point to one of the sub Quadtrees.
    getSubQuadtreePointIsIn(x1, y1)->insert(point, x1, y1, true);
}

void Quadtree::insert(std::vector<Point *> *points, int x, int y, bool useInputXY) {
    numAllPoints += points->size();
    int x1;
    int y1;
    if (useInputXY) {
        x1 = x;
        y1 = y;
    } else {
        x1 = pointValue->getX(points->at(0));
        y1 = pointValue->getY(points->at(0));
    }
    if (bl == 0) {
        // The Quadtree does not split. Try to put points here.
        if (this->points->size() == 0) {
            this->points->reserve(points->size());
            this->points->insert(this->points->end(), points->begin(), points->end());
            return;
        } else {
            int x2 = pointValue->getX(this->points->at(0));
            int y2 = pointValue->getY(this->points->at(0));
            if (x1 == x2 && y1 == y2) {
                this->points->reserve(this->points->size() + points->size());
                this->points->insert(this->points->end(), points->begin(), points->end());
                return;
            } else {
                // Create sub Quadtrees and insert this->points to the sub Quadtrees.
                // Insert points later.
                bl = new Quadtree(xStart, yStart, xCenter, yCenter, pointValue->copy());
                br = new Quadtree(xCenter, yStart, xEnd, yCenter, pointValue->copy());
                tl = new Quadtree(xStart, yCenter, xCenter, yEnd, pointValue->copy());
                tr = new Quadtree(xCenter, yCenter, xEnd, yEnd, pointValue->copy());
                getSubQuadtreePointIsIn(x2, y2)->insert(this->points, x2, y2, true);
                this->points->clear();
            }
        }
    }
    // Insert points to one of the sub Quadtrees.
    getSubQuadtreePointIsIn(x1, y1)->insert(points, x1, y1, true);

}

bool Quadtree::remove(Point *point, int x, int y, bool useInputXY) {
    numAllPoints -= 1;
    if (bl == 0) {
        // The Quadtree does not split. Point is in points.
        points->erase(std::find(points->begin(), points->end(), point));
        return points->empty();
    } else {
        // Remove from sub Quadtrees.
        int x1;
        int y1;
        if (useInputXY) {
            x1 = x;
            y1 = y;
        } else {
            x1 = pointValue->getX(point);
            y1 = pointValue->getY(point);
        }
        if (!getSubQuadtreePointIsIn(x1, y1)->remove(point, x1, y1, true)) {
            return false;
        }
        // The sub Quadtree becomes empty.
        // If the number of empty sub Quadtrees is one or zero,
        // and the non-empty sub Quadtree does not split,
        // merge sub Quadtrees.
        int numEmptySubQuadtrees = 4;
        Quadtree *nonEmptySubQuadtree;
        if (!bl->isEmpty()) {
            numEmptySubQuadtrees -= 1;
            nonEmptySubQuadtree = bl;
        }
        if (!br->isEmpty()) {
            numEmptySubQuadtrees -= 1;
            nonEmptySubQuadtree = br;
        }
        if (!tl->isEmpty()) {
            numEmptySubQuadtrees -= 1;
            nonEmptySubQuadtree = tl;
        }
        if (!tr->isEmpty()) {
            numEmptySubQuadtrees -= 1;
            nonEmptySubQuadtree = tr;
        }
        if (numEmptySubQuadtrees == 3 && nonEmptySubQuadtree->doesNotSplit()) {
            // Get all Points of nonEmptySubQuadtree and save them in this Quadtree.
            points = nonEmptySubQuadtree->getAllPoints();
            delete bl;
            delete br;
            delete tl;
            delete tr;
            bl = 0;
            br = 0;
            tl = 0;
            tr = 0;
            return false;
        } else if (numEmptySubQuadtrees == 4) {
            // Delete all sub Quadtrees.
            delete bl;
            delete br;
            delete tl;
            delete tr;
            bl = 0;
            br = 0;
            tl = 0;
            tr = 0;
            return true;
        } else {
            return false;
        }
    }
}

int Quadtree::getNumAllPoints() {
    return numAllPoints;
}

bool Quadtree::isEmpty() {
    return numAllPoints == 0;
}

bool Quadtree::doesNotSplit() {
    return bl == 0;
}

Point *Quadtree::getPointRandomly() {
    if (bl == 0) {
        // Get a Point randomly.
        return points->at(Utils::randint(0, points->size()));
    } else {
        // Select a sub Quadtree randomly with weights being their number of Points
        // and get a Point randomly from the sub Quadtree.
        double wBl = bl->getNumAllPoints();
        double wBr = br->getNumAllPoints();
        double wTl = tl->getNumAllPoints();
        double wTr = tr->getNumAllPoints();
        return getSubQuadtreeRandomlyByWeights(wBl, wBr, wTl, wTr)->getPointRandomly();
    }
}

Point *Quadtree::getPointRandomlyByX(int x, bool notLessThanX) {
    if (bl == 0) {
        if (pointValue->getX(points->at(0)) < x ^ notLessThanX) {
            // Get a Point randomly.
            return points->at(Utils::randint(0, points->size()));
        } else {
            // There exists Point but it does not satisfy the condition.
            noMatchedPointFound = true;
            return 0;
        }
    } else {
        // Select a sub Quadtree randomly with weights being their number of Points
        // multiplied by their rlap area
        // and get a Point randomly from the sub Quadtree.
        double ratioL;
        double ratioR;
        int situationX;
        if (notLessThanX) {
            if (x < xCenter) {
                situationX = 0;
                ratioL = xCenter - x;
                ratioR = rHalfWidth;
            } else {
                situationX = 1;
                ratioL = 0;
                ratioR = xEnd - x;
            }
        } else {
            if (x < xCenter) {
                situationX = 2;
                ratioL = x - xStart;
                ratioR = 0;
            } else {
                situationX = 3;
                ratioL = lHalfWidth;
                ratioR = x - xCenter;
            }
        }
        ratioL /= lHalfWidth;   // Never divide by zero. See constructor.
        ratioR /= rHalfWidth;
        double wBl = bl->getNumAllPoints() * ratioL;
        double wBr = br->getNumAllPoints() * ratioR;
        double wTl = tl->getNumAllPoints() * ratioL;
        double wTr = tr->getNumAllPoints() * ratioR;
        if (wBl == 0 && wBr == 0 && wTl == 0 && wTr == 0) {
            // Failed to get a Point.
            resetAllSubQuadtreesNoMatchedPointFound();
            noMatchedPointFound = true;
            return 0;
        }
        while (true) {
            int ithSubQuadtree = getWhichSubQuadtreeRandomlyByWeights(wBl, wBr, wTl, wTr);
            Point *point;
            switch (situationX) {
            case 0:
                switch (ithSubQuadtree) {
                case 0: point = bl->getPointRandomlyByX(x, notLessThanX); break;
                case 1: point = br->getPointRandomly(); break;
                case 2: point = tl->getPointRandomlyByX(x, notLessThanX); break;
                case 3: point = tr->getPointRandomly(); break;
                }
                break;
            case 1:
                switch (ithSubQuadtree) {
                case 1: point = br->getPointRandomlyByX(x, notLessThanX); break;
                case 3: point = tr->getPointRandomlyByX(x, notLessThanX); break;
                }
                break;
            case 2:
                switch (ithSubQuadtree) {
                case 0: point = bl->getPointRandomlyByX(x, notLessThanX); break;
                case 2: point = tl->getPointRandomlyByX(x, notLessThanX); break;
                }
                break;
            case 3:
                switch (ithSubQuadtree) {
                case 0: point = bl->getPointRandomly(); break;
                case 1: point = br->getPointRandomlyByX(x, notLessThanX); break;
                case 2: point = tl->getPointRandomly(); break;
                case 3: point = tr->getPointRandomlyByX(x, notLessThanX); break;
                }
                break;
            }
            if (point != 0) {
                resetAllSubQuadtreesNoMatchedPointFound();
                return point;
            } else {
                // Failed to find a Point in the sub Quadtree.
                // Modify the weights.
                switch (ithSubQuadtree) {
                case 0: if (bl->foundNoMatchedPoint()) wBl = 0; break;
                case 1: if (br->foundNoMatchedPoint()) wBr = 0; break;
                case 2: if (tl->foundNoMatchedPoint()) wTl = 0; break;
                case 3: if (tr->foundNoMatchedPoint()) wTr = 0; break;
                }
                if (wBl == 0 && wBr == 0 && wTl == 0 && wTr == 0) {
                    // All sub Quadtrees failed.
                    resetAllSubQuadtreesNoMatchedPointFound();
                    noMatchedPointFound = true;
                    return 0;
                }
            }
        }
    }
}

Point *Quadtree::getPointRandomlyByY(int y, bool notLessThanY) {
    if (bl == 0) {
        if (pointValue->getY(points->at(0)) < y ^ notLessThanY) {
            // Get a Point randomly.
            return points->at(Utils::randint(0, points->size()));
        } else {
            // There exists Point but it does not satisfy the condition.
            noMatchedPointFound = true;
            return 0;
        }
    } else {
        // Select a sub Quadtree randomly with weights being their number of Points
        // multiplied by their overlap area
        // and get a Point randomly from the sub Quadtree.
        double ratioB;
        double ratioT;
        int situationY;
        if (notLessThanY) {
            if (y < yCenter) {
                situationY = 0;
                ratioB = yCenter - y;
                ratioT = tHalfHeight;
            } else {
                situationY = 1;
                ratioB = 0;
                ratioT = yEnd - y;
            }
        } else {
            if (y < yCenter) {
                situationY = 2;
                ratioB = y - yStart;
                ratioT = 0;
            } else {
                situationY = 3;
                ratioB = bHalfHeight;
                ratioT = y - yCenter;
            }
        }
        ratioB /= bHalfHeight;  // Never divide by zero. See constructor.
        ratioT /= tHalfHeight;
        double wBl = bl->getNumAllPoints() * ratioB;
        double wBr = br->getNumAllPoints() * ratioB;
        double wTl = tl->getNumAllPoints() * ratioT;
        double wTr = tr->getNumAllPoints() * ratioT;
        if (wBl == 0 && wBr == 0 && wTl == 0 && wTr == 0) {
            // Failed to get a Point.
            resetAllSubQuadtreesNoMatchedPointFound();
            noMatchedPointFound = true;
            return 0;
        }
        while (true) {
            int ithSubQuadtree = getWhichSubQuadtreeRandomlyByWeights(wBl, wBr, wTl, wTr);
            Point *point;
            switch (situationY) {
            case 0:
                switch (ithSubQuadtree) {
                case 0: point = bl->getPointRandomlyByY(y, notLessThanY); break;
                case 1: point = br->getPointRandomlyByY(y, notLessThanY); break;
                case 2: point = tl->getPointRandomly(); break;
                case 3: point = tr->getPointRandomly(); break;
                }
                break;
            case 1:
                switch (ithSubQuadtree) {
                case 2: point = tl->getPointRandomlyByY(y, notLessThanY); break;
                case 3: point = tr->getPointRandomlyByY(y, notLessThanY); break;
                }
                break;
            case 2:
                switch (ithSubQuadtree) {
                case 0: point = bl->getPointRandomlyByY(y, notLessThanY); break;
                case 1: point = br->getPointRandomlyByY(y, notLessThanY); break;
                }
                break;
            case 3:
                switch (ithSubQuadtree) {
                case 0: point = bl->getPointRandomly(); break;
                case 1: point = br->getPointRandomly(); break;
                case 2: point = tl->getPointRandomlyByY(y, notLessThanY); break;
                case 3: point = tr->getPointRandomlyByY(y, notLessThanY); break;
                }
                break;
            }
            if (point != 0) {
                resetAllSubQuadtreesNoMatchedPointFound();
                return point;
            } else {
                // Failed to find a Point in the sub Quadtree.
                // Modify the weights.
                switch (ithSubQuadtree) {
                case 0: if (bl->foundNoMatchedPoint()) wBl = 0; break;
                case 1: if (br->foundNoMatchedPoint()) wBr = 0; break;
                case 2: if (tl->foundNoMatchedPoint()) wTl = 0; break;
                case 3: if (tr->foundNoMatchedPoint()) wTr = 0; break;
                }
                if (wBl == 0 && wBr == 0 && wTl == 0 && wTr == 0) {
                    // All sub Quadtrees failed.
                    resetAllSubQuadtreesNoMatchedPointFound();
                    noMatchedPointFound = true;
                    return 0;
                }
            }
        }
    }
}

Point *Quadtree::getPointRandomlyByXY(int x, int y, bool notLessThanX, bool notLessThanY) {
    if (bl == 0) {
        if (pointValue->getX(points->at(0)) < x ^ notLessThanX &&
            pointValue->getY(points->at(0)) < y ^ notLessThanY) {
            // Get a Point randomly.
            return points->at(Utils::randint(0, points->size()));
        } else {
            // There exists Point but it does not satisfy the condition.
            noMatchedPointFound = true;
            return 0;
        }
    } else {
        // Select a sub Quadtree randomly with weights being their number of Points
        // multiplied by their rlap area
        // and get a Point randomly from the sub Quadtree.
        double ratioL;
        double ratioR;
        double ratioB;
        double ratioT;
        int situationX;
        int situationY;
        if (notLessThanX) {
            if (x < xCenter) {
                situationX = 0;
                ratioL = xCenter - x;
                ratioR = rHalfWidth;
            } else {
                situationX = 1;
                ratioL = 0;
                ratioR = xEnd - x;
            }
        } else {
            if (x < xCenter) {
                situationX = 2;
                ratioL = x - xStart;
                ratioR = 0;
            } else {
                situationX = 3;
                ratioL = lHalfWidth;
                ratioR = x - xCenter;
            }
        }
        if (notLessThanY) {
            if (y < yCenter) {
                situationY = 0;
                ratioB = yCenter - y;
                ratioT = tHalfHeight;
            } else {
                situationY = 1;
                ratioB = 0;
                ratioT = yEnd - y;
            }
        } else {
            if (y < yCenter) {
                situationY = 2;
                ratioB = y - yStart;
                ratioT = 0;
            } else {
                situationY = 3;
                ratioB = bHalfHeight;
                ratioT = y - yCenter;
            }
        }
        ratioL /= lHalfWidth;   // Never divide by zero. See constructor.
        ratioR /= rHalfWidth;
        ratioB /= bHalfHeight;
        ratioT /= tHalfHeight;
        double wBl = bl->getNumAllPoints() * ratioL * ratioB;
        double wBr = br->getNumAllPoints() * ratioR * ratioB;
        double wTl = tl->getNumAllPoints() * ratioL * ratioT;
        double wTr = tr->getNumAllPoints() * ratioR * ratioT;
        if (wBl == 0 && wBr == 0 && wTl == 0 && wTr == 0) {
            // Failed to get a Point.
            resetAllSubQuadtreesNoMatchedPointFound();
            noMatchedPointFound = true;
            return 0;
        }
        while (true) {
            int ithSubQuadtree = getWhichSubQuadtreeRandomlyByWeights(wBl, wBr, wTl, wTr);
            Point *point;
            switch (situationX) {
            case 0:
                switch (situationY) {
                case 0:
                    switch (ithSubQuadtree) {
                    case 0: point = bl->getPointRandomlyByXY(x, y, notLessThanX, notLessThanY); break;
                    case 1: point = br->getPointRandomlyByY(y, notLessThanY); break;
                    case 2: point = tl->getPointRandomlyByX(x, notLessThanX); break;
                    case 3: point = tr->getPointRandomly(); break;
                    }
                    break;    
                case 1:
                    switch (ithSubQuadtree) {
                    case 2: point = tl->getPointRandomlyByXY(x, y, notLessThanX, notLessThanY); break;
                    case 3: point = tr->getPointRandomlyByY(y, notLessThanY); break;
                    }
                    break;
                case 2:
                    switch (ithSubQuadtree) {
                    case 0: point = bl->getPointRandomlyByXY(x, y, notLessThanX, notLessThanY); break;
                    case 1: point = br->getPointRandomlyByY(y, notLessThanY); break;
                    }
                    break;
                case 3:
                    switch (ithSubQuadtree) {
                    case 0: point = bl->getPointRandomlyByX(x, notLessThanX); break;
                    case 1: point = br->getPointRandomly(); break;
                    case 2: point = tl->getPointRandomlyByXY(x, y, notLessThanX, notLessThanY); break;
                    case 3: point = tr->getPointRandomlyByY(y, notLessThanY); break;
                    }
                    break;
                }
                break;    
            case 1:
                switch (situationY) {
                case 0:
                    switch (ithSubQuadtree) {
                    case 1: point = br->getPointRandomlyByXY(x, y, notLessThanX, notLessThanY); break;
                    case 3: point = tr->getPointRandomlyByX(x, notLessThanX); break;
                    }
                    break;    
                case 1:
                    // Only one case
                            point = tr->getPointRandomlyByXY(x, y, notLessThanX, notLessThanY);
                    break;
                case 2:
                    // Only one case
                            point = br->getPointRandomlyByXY(x, y, notLessThanX, notLessThanY);
                    break;
                case 3:
                    switch (ithSubQuadtree) {
                    case 1: point = br->getPointRandomlyByX(x, notLessThanX); break;
                    case 3: point = tr->getPointRandomlyByXY(x, y, notLessThanX, notLessThanY); break;
                    }
                    break;
                }
                break;
            case 2:
                switch (situationY) {
                case 0:
                    switch (ithSubQuadtree) {
                    case 0: point = bl->getPointRandomlyByXY(x, y, notLessThanX ,notLessThanY); break;
                    case 2: point = tl->getPointRandomlyByX(x, notLessThanX); break;
                    }
                    break;    
                case 1:
                    // Only one case
                            point = tl->getPointRandomlyByXY(x, y, notLessThanX, notLessThanY);
                    break;
                case 2:
                    // Only one case
                            point = bl->getPointRandomlyByXY(x, y, notLessThanX, notLessThanY);
                    break;
                case 3:
                    switch (ithSubQuadtree) {
                    case 0: point = bl->getPointRandomlyByX(x, notLessThanX); break;
                    case 2: point = tl->getPointRandomlyByXY(x, y, notLessThanX, notLessThanY); break;
                    }
                    break;
                }
                break;
            case 3:
                switch (situationY) {
                case 0:
                    switch (ithSubQuadtree) {
                    case 0: point = bl->getPointRandomlyByY(y, notLessThanY); break;
                    case 1: point = br->getPointRandomlyByXY(x, y, notLessThanX, notLessThanY); break;
                    case 2: point = tl->getPointRandomly(); break;
                    case 3: point = tr->getPointRandomlyByX(x, notLessThanX); break;
                    }
                    break;    
                case 1:
                    switch (ithSubQuadtree) {
                    case 2: point = tl->getPointRandomlyByY(y, notLessThanY); break;
                    case 3: point = tr->getPointRandomlyByXY(x, y, notLessThanX, notLessThanY); break;
                    }
                    break;
                case 2:
                    switch (ithSubQuadtree) {
                    case 0: point = bl->getPointRandomlyByY(y, notLessThanY); break;
                    case 1: point = br->getPointRandomlyByXY(x, y, notLessThanX, notLessThanY); break;
                    }
                    break;
                case 3:
                    switch (ithSubQuadtree) {
                    case 0: point = bl->getPointRandomly(); break;
                    case 1: point = br->getPointRandomlyByX(x, notLessThanX); break;
                    case 2: point = tl->getPointRandomlyByY(y, notLessThanY); break;
                    case 3: point = tr->getPointRandomlyByXY(x, y, notLessThanX, notLessThanY); break;
                    }
                    break;
                }
                break;
            }
            if (point != 0) {
                resetAllSubQuadtreesNoMatchedPointFound();
                return point;
            } else {
                // Failed to find a Point in the sub Quadtree.
                // Modify the weights.
                switch (ithSubQuadtree) {
                case 0: if (bl->foundNoMatchedPoint()) wBl = 0; break;
                case 1: if (br->foundNoMatchedPoint()) wBr = 0; break;
                case 2: if (tl->foundNoMatchedPoint()) wTl = 0; break;
                case 3: if (tr->foundNoMatchedPoint()) wTr = 0; break;
                }
                if (wBl == 0 && wBr == 0 && wTl == 0 && wTr == 0) {
                    // All sub Quadtrees failed.
                    resetAllSubQuadtreesNoMatchedPointFound();
                    noMatchedPointFound = true;
                    return 0;
                }
            }
        }
    }
}

bool Quadtree::foundNoMatchedPoint() {
    return noMatchedPointFound;
}

void Quadtree::resetNoMatchedPointFound() {
    noMatchedPointFound = false;
}

std::vector<Point *> *Quadtree::getPointsAtXY(int x, int y) {
    if (bl == 0) {
        if (points->size() > 0 && x == pointValue->getX(points->at(0)) && y == pointValue->getY(points->at(0))) {
            return new std::vector<Point *>(*points);
        } else {
            return 0;
        }
    } else {
        return getSubQuadtreePointIsIn(x, y)->getPointsAtXY(x, y);
    }
}

std::vector<Point *> *Quadtree::getAllPoints() {
    if (bl == 0) {
        return new std::vector<Point *>(*points);
    } else {
        std::vector<Point *> *blPoints = bl->getAllPoints();
        std::vector<Point *> *brPoints = br->getAllPoints();
        std::vector<Point *> *tlPoints = tl->getAllPoints();
        std::vector<Point *> *trPoints = tr->getAllPoints();
        std::vector<Point *> *allPoints = new std::vector<Point *>();
        allPoints->reserve(blPoints->size() + brPoints->size() + tlPoints->size() + trPoints->size());
        allPoints->insert(allPoints->end(), blPoints->begin(), blPoints->end());
        allPoints->insert(allPoints->end(), brPoints->begin(), brPoints->end());
        allPoints->insert(allPoints->end(), tlPoints->begin(), tlPoints->end());
        allPoints->insert(allPoints->end(), trPoints->begin(), trPoints->end());
        delete blPoints;
        delete brPoints;
        delete tlPoints;
        delete trPoints;
        return allPoints;
    }
}

std::vector<Point *> *Quadtree::getPointsByX(int x, bool notLessThanX) {
    if (bl == 0) {
        if (points->size() > 0 && pointValue->getX(points->at(0)) < x ^ notLessThanX) {
            return new std::vector<Point *>(*points);
        } else {
            return new std::vector<Point *>();
        }
    } else {
        std::vector<Point *> *blPoints;
        std::vector<Point *> *brPoints;
        std::vector<Point *> *tlPoints;
        std::vector<Point *> *trPoints;
        if (x < xCenter) {
            if (notLessThanX) {
                blPoints = bl->getPointsByX(x, true);
                brPoints = br->getAllPoints();
                tlPoints = tl->getPointsByX(x, true);
                trPoints = tr->getAllPoints();
            } else {
                blPoints = bl->getPointsByX(x, false);
                brPoints = new std::vector<Point *>();
                tlPoints = tl->getPointsByX(x, false);
                trPoints = new std::vector<Point *>();
            }
        } else if (x == xCenter) {
            if (notLessThanX) {
                blPoints = new std::vector<Point *>();
                brPoints = br->getAllPoints();
                tlPoints = new std::vector<Point *>();
                trPoints = tr->getAllPoints();
            } else {
                blPoints = bl->getAllPoints();
                brPoints = new std::vector<Point *>();
                tlPoints = tl->getAllPoints();
                trPoints = new std::vector<Point *>();
            }
        } else {
            if (notLessThanX) {
                blPoints = new std::vector<Point *>();
                brPoints = br->getPointsByX(x, true);
                tlPoints = new std::vector<Point *>();
                trPoints = tr->getPointsByX(x, true);
            } else {
                blPoints = bl->getAllPoints();
                brPoints = br->getPointsByX(x, false);
                tlPoints = tl->getAllPoints();
                trPoints = tr->getPointsByX(x, false);
            }
        }
        std::vector<Point *> *allPoints = new std::vector<Point *>();
        allPoints->reserve(blPoints->size() + brPoints->size() + tlPoints->size() + trPoints->size());
        allPoints->insert(allPoints->end(), blPoints->begin(), blPoints->end());
        allPoints->insert(allPoints->end(), brPoints->begin(), brPoints->end());
        allPoints->insert(allPoints->end(), tlPoints->begin(), tlPoints->end());
        allPoints->insert(allPoints->end(), trPoints->begin(), trPoints->end());
        delete blPoints;
        delete brPoints;
        delete tlPoints;
        delete trPoints;
        return allPoints;
    }
}

std::vector<Point *> *Quadtree::getPointsByY(int y, bool notLessThanY) {
    if (bl == 0) {
        if (points->size() > 0 && pointValue->getY(points->at(0)) < y ^ notLessThanY) {
            return new std::vector<Point *>(*points);
        } else {
            return new std::vector<Point *>();
        }
    } else {
        std::vector<Point *> *blPoints;
        std::vector<Point *> *brPoints;
        std::vector<Point *> *tlPoints;
        std::vector<Point *> *trPoints;
        if (y < yCenter) {
            if (notLessThanY) {
                blPoints = bl->getPointsByY(y, true);
                brPoints = br->getPointsByY(y, true);
                tlPoints = tl->getAllPoints();
                trPoints = tr->getAllPoints();
            } else {
                blPoints = bl->getPointsByY(y, false);
                brPoints = br->getPointsByY(y, false);
                tlPoints = new std::vector<Point *>();
                trPoints = new std::vector<Point *>();
            }
        } else if (y == yCenter) {
            if (notLessThanY) {
                blPoints = new std::vector<Point *>();
                brPoints = new std::vector<Point *>();
                tlPoints = tl->getAllPoints();
                trPoints = tr->getAllPoints();
            } else {
                blPoints = bl->getAllPoints();
                brPoints = br->getAllPoints();
                tlPoints = new std::vector<Point *>();
                trPoints = new std::vector<Point *>();
            }
        } else {
            if (notLessThanY) {
                blPoints = new std::vector<Point *>();
                brPoints = new std::vector<Point *>();
                tlPoints = tl->getPointsByY(y, true);
                trPoints = tr->getPointsByY(y, true);
            } else {
                blPoints = bl->getAllPoints();
                brPoints = br->getAllPoints();
                tlPoints = tl->getPointsByY(y, false);
                trPoints = tr->getPointsByY(y, false);
            }
        }
        std::vector<Point *> *allPoints = new std::vector<Point *>();
        allPoints->reserve(blPoints->size() + brPoints->size() + tlPoints->size() + trPoints->size());
        allPoints->insert(allPoints->end(), blPoints->begin(), blPoints->end());
        allPoints->insert(allPoints->end(), brPoints->begin(), brPoints->end());
        allPoints->insert(allPoints->end(), tlPoints->begin(), tlPoints->end());
        allPoints->insert(allPoints->end(), trPoints->begin(), trPoints->end());
        delete blPoints;
        delete brPoints;
        delete tlPoints;
        delete trPoints;
        return allPoints;
    }
}

/*
std::vector<Point *> *Quadtree::getPointsByXY(int x, int y, bool notLessThanX, bool notLessThanY) {
    if (bl == 0) {
        if (points->size() > 0 && pointValue->getX(points->at(0)) < x ^ notLessThanX
            && pointValue->getY(points->at(0)) < y ^ notLessThanY) {
            return new std::vector<Point *>(points);
        } else {
            return new std::vector<Point *>();
        }
    } else {
        std::vector<Point *> *blPoints;
        std::vector<Point *> *brPoints;
        std::vector<Point *> *tlPoints;
        std::vector<Point *> *trPoints;
        if (x < xCenter) {
            if (y < yCenter) {

            } else if (y == yCenter) {

            } else {

            }
        } else if (x == xCenter) {
            if (y < yCenter) {

            } else if (y == yCenter) {

            } else {

            }
        } else {
            if (y < yCenter) {

            } else if (y == yCenter) {

            } else {

            }
        }
        std::vector<Point *> *allPoints = new std::vector<Point *>();
        allPoints->reserve(blPoints->size() + brPoints->size() + tlPoints->size() + trPoints->size());
        allPoints->insert(allPoints->end(), blPoints->begin(), blPoints->end());
        allPoints->insert(allPoints->end(), brPoints->begin(), brPoints->end());
        allPoints->insert(allPoints->end(), tlPoints->begin(), tlPoints->end());
        allPoints->insert(allPoints->end(), trPoints->begin(), trPoints->end());
        delete blPoints
        delete brPoints
        delete tlPoints
        delete trPoints
        return allPoints;
    }
}
*/

void Quadtree::print() {
    std::cout << "(" << xStart << ",\t" << yStart << ",\t" << xEnd << ",\t" << yEnd << ")\n";
    std::cout << "numAllPoints: " << numAllPoints << "\n";
    if (bl == 0) {
        for (int i = 0; i < points->size(); ++i) {
            points->at(i)->print();
        }
    } else {
        bl->print();
        br->print();
        tl->print();
        tr->print();
    }
}

// Private

Quadtree *Quadtree::getSubQuadtreePointIsIn(Point *point) {
    if (pointValue->getX(point) < xCenter) {
        if (pointValue->getY(point) < yCenter) {
            return bl;
        } else {
            return tl;
        }
    } else {
        if (pointValue->getY(point) < yCenter) {
            return br;
        } else {
            return tr;
        }
    }
}

Quadtree *Quadtree::getSubQuadtreePointIsIn(int x, int y) {
    if (x < xCenter) {
        if (y < yCenter) {
            return bl;
        } else {
            return tl;
        }
    } else {
        if (y < yCenter) {
            return br;
        } else {
            return tr;
        }
    }
}

Quadtree *Quadtree::getSubQuadtreeRandomlyByWeights(double wBl, double wBr, double wTl, double wTr) {
    double sumToBr = wBl + wBr;
    double sumToTl = sumToBr + wTl;
    double sumToTr = sumToTl + wTr;
    double random = Utils::random() * sumToTr;
    if (random < wBl) {
        return bl;
    } else if (random < sumToBr) {
        return br;
    } else if (random < sumToTl) {
        return tl;
    } else {
        return tr;
    }
}

int Quadtree::getWhichSubQuadtreeRandomlyByWeights(double wBl, double wBr, double wTl, double wTr) {
    double sumToBr = wBl + wBr;
    double sumToTl = sumToBr + wTl;
    double sumToTr = sumToTl + wTr;
    double random = Utils::random() * sumToTr;
    if (random < wBl) {
        return 0;   // bl
    } else if (random < sumToBr) {
        return 1;   // br
    } else if (random < sumToTl) {
        return 2;   // tl
    } else {
        return 3;   // tr
    }
}

void Quadtree::resetAllSubQuadtreesNoMatchedPointFound() {
    bl->resetNoMatchedPointFound();
    br->resetNoMatchedPointFound();
    tl->resetNoMatchedPointFound();
    tr->resetNoMatchedPointFound();
}