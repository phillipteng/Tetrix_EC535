//
// Created by Phillip Teng on 2019-04-15.
//
#include <utility> // for pair
#include <iostream> //everything else
#include <cstdlib> //for rand
#include <ctime>
#include <vector>
//#include <random>

#ifndef TETRIX_EC535_BLOCK_H
#define TETRIX_EC535_BLOCK_H

#define RIGHT 1
#define LEFT 0

using namespace std;

class block {
    enum Orientation{
        left, down, right, up
    };

public:
    enum Type {
        //The explanations to the type of the block is found here
        //https://tetris.fandom.com/wiki/Tetromino
        I, J, L, O, S, T, Z
    };

    //Variables
    Type type;
    int orientation;
    //Orientation orientation;
    vector<pair<int, int> > coord;
    pair<int,int> refPt;

    int color;

    vector<pair<int, int> > blockICoords(Orientation ori){
        vector<pair<int, int> > coord;
        switch(ori)
        {
            case left:
                coord.push_back(make_pair(0,2));
                coord.push_back(make_pair(1,2));
                coord.push_back(make_pair(2,2));
                coord.push_back(make_pair(3,2));
                break;
            case right:
                coord.push_back(make_pair(0,1));
                coord.push_back(make_pair(1,1));
                coord.push_back(make_pair(2,1));
                coord.push_back(make_pair(3,1));
                break;
            case up:
                coord.push_back(make_pair(1,0));
                coord.push_back(make_pair(1,1));
                coord.push_back(make_pair(1,2));
                coord.push_back(make_pair(1,3));
                break;
            default: // down
                coord.push_back(make_pair(2,0));
                coord.push_back(make_pair(2,1));
                coord.push_back(make_pair(2,2));
                coord.push_back(make_pair(2,3));
        }
        return coord;
    };

    vector<pair<int, int> > blockJCoords(Orientation ori){
        vector<pair<int, int> > coord;
        switch(ori)
        {
            case left:
                coord.push_back(make_pair(0,1));
                coord.push_back(make_pair(1,1));
                coord.push_back(make_pair(2,1));
                coord.push_back(make_pair(2,2));
                break;
            case right:
                coord.push_back(make_pair(0,0));
                coord.push_back(make_pair(0,1));
                coord.push_back(make_pair(1,1));
                coord.push_back(make_pair(2,1));
                break;
            case up:
                coord.push_back(make_pair(0,2));
                coord.push_back(make_pair(1,2));
                coord.push_back(make_pair(1,1));
                coord.push_back(make_pair(1,0));
                break;
            default: // down
                coord.push_back(make_pair(1,2));
                coord.push_back(make_pair(1,1));
                coord.push_back(make_pair(1,0));
                coord.push_back(make_pair(2,0));
        }
        return coord;
    };

    vector<pair<int, int> > blockLCoords(Orientation ori){
        vector<pair<int, int> > coord;
        switch(ori)
        {
            case left:
                coord.push_back(make_pair(0,1));
                coord.push_back(make_pair(1,1));
                coord.push_back(make_pair(2,1));
                coord.push_back(make_pair(0,2));
                break;
            case right:
                coord.push_back(make_pair(0,1));
                coord.push_back(make_pair(1,1));
                coord.push_back(make_pair(2,1));
                coord.push_back(make_pair(2,0));
                break;
            case up:
                coord.push_back(make_pair(0,0));
                coord.push_back(make_pair(1,0));
                coord.push_back(make_pair(1,1));
                coord.push_back(make_pair(1,2));
                break;
            default: // down
                coord.push_back(make_pair(2,2));
                coord.push_back(make_pair(1,2));
                coord.push_back(make_pair(1,1));
                coord.push_back(make_pair(1,0));
        }
        return coord;
    };

    vector<pair<int, int> > blockOCoords(Orientation ori){
        vector<pair<int, int> > coord;
        switch(ori)
        {
            case left:
                coord.push_back(make_pair(0,1));
                coord.push_back(make_pair(1,1));
                coord.push_back(make_pair(0,2));
                coord.push_back(make_pair(1,2));
                break;
            case right:
                coord.push_back(make_pair(0,1));
                coord.push_back(make_pair(1,1));
                coord.push_back(make_pair(0,2));
                coord.push_back(make_pair(1,2));
                break;
            case up:
                coord.push_back(make_pair(0,1));
                coord.push_back(make_pair(1,1));
                coord.push_back(make_pair(0,2));
                coord.push_back(make_pair(1,2));
                break;
            default: // down
                coord.push_back(make_pair(0,1));
                coord.push_back(make_pair(1,1));
                coord.push_back(make_pair(0,2));
                coord.push_back(make_pair(1,2));
        }
        return coord;
    };

    vector<pair<int, int> > blockSCoords(Orientation ori){
        vector<pair<int, int> > coord;
        switch(ori)
        {
            case left:
                coord.push_back(make_pair(0,2));
                coord.push_back(make_pair(1,2));
                coord.push_back(make_pair(1,1));
                coord.push_back(make_pair(2,1));
                break;
            case right:
                coord.push_back(make_pair(0,1));
                coord.push_back(make_pair(1,1));
                coord.push_back(make_pair(1,0));
                coord.push_back(make_pair(2,0));
                break;
            case up:
                coord.push_back(make_pair(0,0));
                coord.push_back(make_pair(0,1));
                coord.push_back(make_pair(1,1));
                coord.push_back(make_pair(1,2));
                break;
            default: // down
                coord.push_back(make_pair(1,0));
                coord.push_back(make_pair(1,1));
                coord.push_back(make_pair(2,1));
                coord.push_back(make_pair(2,2));
        }
        return coord;
    };

    vector<pair<int, int> > blockTCoords(Orientation ori){
        vector<pair<int, int> > coord;
        switch(ori)
        {
            case left:
                     coord.push_back(make_pair(0,1));
                     coord.push_back(make_pair(1,1));
                     coord.push_back(make_pair(2,1));
                     coord.push_back(make_pair(1,2));
                break;
            case right:
                     coord.push_back(make_pair(0,1));
                     coord.push_back(make_pair(1,1));
                     coord.push_back(make_pair(2,1));
                     coord.push_back(make_pair(1,0));
                break;
            case up:
                     coord.push_back(make_pair(1,2));
                     coord.push_back(make_pair(1,1));
                     coord.push_back(make_pair(1,0));
                     coord.push_back(make_pair(0,1));
                break;
            default: // down
                     coord.push_back(make_pair(1,2));
                     coord.push_back(make_pair(1,1));
                     coord.push_back(make_pair(1,0));
                     coord.push_back(make_pair(2,1));
        }
        return coord;
    };

    vector<pair<int, int> > blockZCoords(Orientation ori){
        vector<pair<int, int> > coord;
        switch(ori)
        {
            case left:
                     coord.push_back(make_pair(0,1));
                     coord.push_back(make_pair(1,1));
                     coord.push_back(make_pair(1,2));
                     coord.push_back(make_pair(2,2));
                break;
            case right:
                     coord.push_back(make_pair(0,0));
                     coord.push_back(make_pair(1,0));
                     coord.push_back(make_pair(1,1));
                     coord.push_back(make_pair(2,1));
                break;
            case up:
                     coord.push_back(make_pair(0,2));
                     coord.push_back(make_pair(0,1));
                     coord.push_back(make_pair(1,1));
                     coord.push_back(make_pair(1,0));
                break;
            default: // down
                     coord.push_back(make_pair(1,2));
                     coord.push_back(make_pair(1,1));
                     coord.push_back(make_pair(2,1));
                     coord.push_back(make_pair(2,0));
        }
        return coord;
    };

    vector<pair<int, int> > getCoordinates(int ori){
        switch(type)
        {
            case I:
                return blockICoords(Orientation(ori));
            case L:
                return blockLCoords(Orientation(ori));
            case J:
                return blockJCoords(Orientation(ori));
            case O:
                return blockOCoords(Orientation(ori));
            case S:
                return blockSCoords(Orientation(ori));
            case Z:
                return blockZCoords(Orientation(ori));
            case T:
                return blockTCoords(Orientation(ori));
            default:
                break;
        }
    };

    int rotateOrientation(int oldOri, int direction){
        /* direction = 0 : rotate right
         * direction = 1 : rotate left
         */
        int newOri;
        if (direction){
            if (oldOri == 3)
                newOri = 0;
            else
                newOri = orientation + 1;
        } else {
            if (oldOri == 0)
                newOri = 3;
            else
                newOri = orientation - 1;
        }
        return newOri;
    }

    vector<pair<int, int> > getRotateCoord(int direction){
        /* direction = 0 : rotate right
         * direction = 1 : rotate left
         */
        // Get new orientation
        int newOri = rotateOrientation(orientation, direction);

        // Get base coordinates of rotated block
        vector<pair<int, int> > newCoord = getCoordinates(newOri);

        // add ref point to base coords to calculate true coords
        for (int i=0; i<newCoord.size(); i++){
            newCoord[i].first += refPt.first;
            newCoord[i].second += refPt.second;
        }
        return newCoord;
    }

    void setBlock(int ori, vector<pair<int, int> > newCoord){
        orientation = ori; // set new orientation
        coord = newCoord; // set new Coord
    }

    //generates a random block
    block(){
        color = 0;
        //randomly generate a number 0 to 6 for the seven types of blocks
        type = Type(rand()%7);
        //generate orientation
        orientation = rand()%4;

        coord = getCoordinates(orientation);

        // Default ref pt
        refPt = make_pair(4,-2);
        for (int i=0; i<coord.size(); i++){
            coord[i].first += refPt.first;
            coord[i].second += refPt.second;
        }


    };
};


#endif //TETRIX_EC535_BLOCK_H
