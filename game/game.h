//
// Created by Phillip Teng on 2019-04-15.
//
#ifndef TETRIX_EC535_GAME_H
#define TETRIX_EC535_GAME_H

#include "block.h"
#include "grid.cpp"
#include <QApplication>
#include <iostream>
//#include <unistd.h>

using namespace std;


class game {
    // occupancy of each cell in grid
    int state[10][20];
    // color of each cell in grid
    int colorState[10][20];
    // number of filled blocks for each row
    int rowState[20];

    //for pausing game
    bool paused;

    bool blockInPlay;

    block activeBlock;
	block waitingBlock;
	bool hasStarted;

    bool rowToRemove;

    string Color[8];
    
    int rowsRemoved;

public:
    //checking if game has ended
    bool gameEnded;
	int mode;
    game(){
        //constructor
		mode = 0;
		hasStarted = false;
        blockInPlay = false;
        rowsRemoved = 0;
		rowToRemove = false;
        //game starts
        gameEnded = false;
        paused = false;
	
		setColor(mode); // set block colors

        //Grid occupancy and colors
        for (int i = 0; i < 10; i++){
            for(int j = 0; j < 20; j++){
                state[i][j] = 0;
                colorState[i][j] = 0;
            }
        }

        // No blocks in any rows at start of game
        for (int i=0; i<20; i++)
            rowState[i] = 0;

    };
    
    int getRowsRemoved(){
    	int newRowsRemoved = rowsRemoved;
    	rowsRemoved = 0;
    	return newRowsRemoved;
    }

    void rotate(block * input, Grid grid, int direction){
        /* direction = 0 : rotate right
         * direction = 1 : rotate left
         */
        // Get new orientation
        vector<pair<int, int> > rotatedCoord = input->getRotateCoord(direction);
        bool boundCheck = checkBoundary(rotatedCoord); // Check if can rotate coords
        if (!boundCheck){ // if pass
            int ori = input->rotateOrientation(input->orientation, direction); // get orientation after rotate
            grid.replaceBlock(input->coord, rotatedCoord, Color[input->color]); // rotate block in grid
            input->setBlock(ori, rotatedCoord); // save new orientation and rotated coords
        }
    };

    void installBlock(block * input, Grid grid){
        // Record block locations in state
        bool createNewBlock = false;
        int x, y;
        for (int i=0; i<input->coord.size(); i++){
            x = input->coord[i].first;
            y = input->coord[i].second;
            state[x][y] = 1;
            colorState[x][y] = input->color;

            // Update * blocks in rows
            rowState[y] += 1;

            if (rowState[y] == 10)
                rowToRemove = true;
        }
        
        //blockInPlay = false; // inactivate block
        generateNewBlock(grid);
    }

    bool checkBoundary(vector<pair<int, int> > coord){
        // Check if bordering boundary
        int x, y;
        for (int i=0; i<coord.size(); i++){
            x = coord[i].first;
            y = coord[i].second;
			if (y < 0)
				continue;
            if (state[x][y] == 1 || x >= 10 || x < 0 || y >= 20)
                return true;
        }
        return false;
    }

    void translateY(block * input, Grid grid){
        // translating speed slow or fast
        vector<pair<int, int> > newCoord = input->coord; // translated coords
        for (int i=0; i<newCoord.size(); i++)
            newCoord[i].second = newCoord[i].second + 1;
		
		bool boundCheck = checkBoundary(newCoord); // check overlap
        if (boundCheck){ // if bordering boundary, install block in state array
            installBlock(input, grid); // installs block in state array & makes new block
			return;
		}
        // Translate block down in grid
        grid.replaceBlock(input->coord, newCoord, Color[input->color]);
        input->coord = newCoord; // save new coords
        input->refPt.second = input->refPt.second+1; // update block's ref pt
    };

    void translateX(block * input, Grid grid, int direction){
        /*
         * direction = 0 : right
         * direction = 1 : left
         */
        // translating speed slow or fast
        vector<pair<int, int> > newCoord = input->coord; // translated coords

        // Set increment direction
        int increment;
        if (direction == 0)
            increment = 1;
        else
            increment = -1;
        // Calculate coords
        for (int i=0; i<newCoord.size(); i++){
            newCoord[i].first = newCoord[i].first + increment;
        }

        // Check if translated coords border boundary
        bool boundCheck = checkBoundary(newCoord); // check overlap
        if (boundCheck) // if bordering boundary, do nothing
            return;

        // Translate horizontally in grid
        grid.replaceBlock(input->coord, newCoord, Color[input->color]);
        input->coord = newCoord; // save new coords
        input->refPt.first = input->refPt.first+increment; // update block's ref pt

    };

    void rowRemoval(Grid grid){
		int newRowState[20];
        int newState[10][20];
        int newColorState[10][20];
		int newRowLoc[20];

        vector<int> rowsToDelete;
	
		// initialize new variables
		for (int i=0; i<10; i++){
			for (int j=0; j<20; j++){
				newState[i][j] = 0;
				newColorState[i][j] = 0;
				if (i == 0){
					newRowLoc[j] = j;
					newRowState[j] = 0;				
				}			
			}		
		}		
		
        // Find new row assignments
        for (int i=0; i < 20; i++){
            if (rowState[i] == 10){ // row filled
                // delete row
                rowsToDelete.push_back(i);
                newRowLoc[i] = -1;
                // Record new row locations of rows that will be shifted down
                for (int j=i-1; j>=0; j--){ // only shift rows above deleted row
                    if (newRowLoc[j] == -1){
                    	cout << "inside " << j << endl;
                        continue;
                        }
                    newRowLoc[j] += 1;
					cout << "shifting down row " << j << endl;
                }
            }
        }
        
		rowsRemoved = rowsToDelete.size();

        // Create new state and new color state based on new row assignments
        int newRow;
        for (int i=0; i<20; i++){
			if (newRowLoc[i] != -1){                
				newRow = newRowLoc[i];
                for (int j=0; j<10; j++){
                    newState[j][newRow] = state[j][i];
                    newColorState[j][newRow] = colorState[j][i];
                    cout << Color[colorState[j][i]] << endl; 
                }
				newRowState[newRow] = rowState[i];
            }
        }

        // Update state and grid
        for (int i=0; i<10; i++){
            for (int j=0; j<20; j++){
                state[i][j] = newState[i][j];
                colorState[i][j] = newColorState[i][j];
                // Update grid
                grid.setCellColor(i,j,Color[colorState[i][j]]);
				if (i == 0)
					rowState[j] = newRowState[j];
            }
        }

        rowToRemove = false;
    }

    int gameSpeed(int * gameEnd){

        int period = 1;

        if (gameEnded){
            *gameEnd = 1;
        }

        //hard coded to run every second
        return period;
    };

    void displayGrid(){
        for (int i = 0; i < 20; i++){
            for (int j = 0; j < 10; j++){
                cout << state[j][i] << " ";
            }
            cout << endl;
        }
    };

    void pause(){
        paused = true;
    }

    void unpause(){
        paused = false;
    }

    void generateNewBlock(Grid grid){
		if (hasStarted == false){
			activeBlock = block();
			waitingBlock = block();
			hasStarted = true;
		} else {
			activeBlock = waitingBlock;
			waitingBlock = block();
		}
        //activeBlock = block();
        activeBlock.color = (int) activeBlock.type + 1; // use type of block to index into  Color array
		waitingBlock.color = (int) waitingBlock.type + 1;        
		// check if space is already occupied and end game
        bool boundCheck = checkBoundary(activeBlock.coord); // check overlap
        if (boundCheck){
        	gameEnded = true;
        }
        
        // display block on grid
        grid.placeBlock(activeBlock.coord, Color[activeBlock.color]);
    }
	
	vector<pair<int, int> > getNextCoord(){
		return waitingBlock.zeroedCoord;	
	}

	string getNextColor(){
		return Color[waitingBlock.color];	
	}

	void hardDrop(block * input, Grid grid){
		int incr = 0;
		vector<pair<int, int > > tempCoord = input->coord;		
		while (!checkBoundary(tempCoord)){
			incr++;
			for (int i=0; i<tempCoord.size(); i++)
				tempCoord[i].second++;
		}

		for (int i=0; i < input->coord.size(); i++){
			tempCoord[i].second = input->coord[i].second + incr-1;		
		}

		grid.replaceBlock(input->coord, tempCoord, Color[input->color]);
        input->coord =  tempCoord; // save new coords
		installBlock(input, grid); 
	}

    void update(QApplication * app, Grid grid, bool isRotate = false, int rotateDir = RIGHT, bool isXmove = false, int xDir = RIGHT, bool isHardDrop = false){
        //run the game
        if (gameEnded)
        	return;
        if (!paused){
            //run the game

		    // Remove rows if row full
			if (rowToRemove){
				std::cout << "Removing a row" << std::endl;
				rowRemoval(grid);
			}
				
            if(!blockInPlay){ // no block in play
                generateNewBlock(grid);
                blockInPlay = 1;
            }
            else{
                //block already in play, translate the block down
				if (isRotate){
					rotate(&activeBlock, grid, rotateDir);
				 	app->processEvents();
				} else if (isXmove){
					translateX(&activeBlock, grid, xDir);
					app->processEvents();		
				} else if (isHardDrop) {
					hardDrop(&activeBlock, grid);
					app->processEvents();				
				} else {
                    translateY(&activeBlock, grid);
                    app->processEvents();
				}

	    	}
		}
    };
	
	void setColor(int modeInput){
			string colorLegend0[8] = {"lightgray", "cyan", "orange", "blue", "yellow", "red","magenta", "green"};
			string colorLegend1[8] = {"black", "black", "black", "black", "black", "black", "black", "black"};
			string colorLegend2[8] = {"lightgray", "lightgray", "lightgray", "lightgray", "lightgray", "lightgray", "lightgray", "lightgray"};
		for(int i = 0; i < 8; i++){
			if (modeInput == 0)
				Color[i] = colorLegend0[i];
			else if (modeInput == 1)
				Color[i] = colorLegend1[i];
			else
				Color[i] = colorLegend2[i];
		}
	}


	void restart(Grid grid){
		blockInPlay = false;
        rowsRemoved = 0;
		rowToRemove = false;
        //game starts
        gameEnded = false;
        paused = false;

        //Grid occupancy and colors
        for (int i = 0; i < 10; i++){
            for(int j = 0; j < 20; j++){
                state[i][j] = 0;
                colorState[i][j] = 0;
				grid.setCellColor(i,j, "lightgray");
            }
        }

        // No blocks in any rows at start of game
        for (int i=0; i<20; i++)
            rowState[i] = 0;

		setColor(mode);
	}	

};


#endif //TETRIX_EC535_GAME_H
