#ifndef POSITION_H
#define POSITION_H

class Position{
private:
    int _x;
    int _y;
public:
    Position(){_x=0; _y=0;}
    Position(int x,int y){_x=x;_y=y;}
    int x(){return _x;}
    int y(){return _y;}

    int setX(int x){_x=x;}
    int setY(int y){_y=y;}

    bool operator==(Position autre){return _x==autre.x() && _y==autre.y();}
};

#endif

