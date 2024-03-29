#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <iostream>
//#include <unistd.h>

//#include "grid.cpp"
#include "score.cpp"

#include "game.h"
#include "block.h"
#include <ctime>
#include "nextblock.cpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

#define THRESHOLD 1500

int trigger = 0;
game Machine;
bool menu_opened = false;

void sighandler(int);

int main(int argc, char **argv)
{
    QApplication * app = new QApplication(argc, argv);

    /////////// Create Grid ////////////
    Grid newGrid;
    QFrame * gridWidget = newGrid.getWidget();

    ///////////// Add score box /////////////
    Score score;
    QLabel * scoreLabel = score.getLabel();

    ////////// Create main widget (widget that is displayed) /////////////////
    QWidget * main = new QWidget();
    // Layout

    QHBoxLayout* centralLayout = new QHBoxLayout();

    // next block stuff -------------------
    nextBlock nb;
    QFrame * nbObj = nb.getWidget();

    // Add widgets to layout
    centralLayout->addWidget(nbObj);
    centralLayout->addWidget(gridWidget);
    centralLayout->addWidget(scoreLabel);
	
    // Add layout to main widget
    main->setLayout(centralLayout);
	main->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    main->showFullScreen();

    
	//Button stuff
	int pFile, oflags;
	int rd;

	struct sigaction action;
	char buffer[10] = "";

	// Opens to device file
	pFile = open("/dev/tetrix", O_RDWR);
	if (pFile < 0) {
		fprintf (stderr, "tetrix module isn't loaded\n");
		return 1;
	}

	// Setup signal handler
	memset(&action, 0, sizeof(action));
	action.sa_handler = sighandler;
	action.sa_flags = SA_SIGINFO;
	sigemptyset(&action.sa_mask);
	sigaction(SIGIO, &action, NULL);
	fcntl(pFile, F_SETOWN, getpid());
	oflags = fcntl(pFile, F_GETFL);
	fcntl(pFile, F_SETFL, oflags | FASYNC);
    

    /////////////////////////////////////////////////////////////////////
    ///////////////////////////// Gameplay //////////////////////////////
    /////////////////////////////////////////////////////////////////////

    std::cout << "Hello, World!" << std::endl;
    //generate a random block
    block thisNewblock = block();

    cout << "The type of block is " << thisNewblock.type << endl;

    int gameEnd = 1;
    srand(time(NULL));

    //intialize a game object
    Machine = game();

	char parsedButton[10] = "";
	char parsedTick[10] = "";
	int scoreVal = 0;
	int rowsRemoved = 0;
	write(pFile, "reset", 5);
	int thislvl = score.getScore() / THRESHOLD;
	while (true){
		// next block
		if (Machine.mode == 2)
			nb.placeBlock(Machine.getNextCoord(), "red");
		else
			nb.placeBlock(Machine.getNextCoord(), Machine.getNextColor());

		//Speeding up game based on intervals of the threshold
		if(thislvl != (score.getScore() / THRESHOLD)){
			thislvl = score.getScore() / THRESHOLD;
			write(pFile, "speed", 5);
		}

		if(trigger){ //On receiving signal
			trigger = 0;
			rd = read(pFile, buffer, 10);
			buffer[rd] = '\0';
			//printf("%s\n", buffer);
			sscanf(buffer, "%s %s",parsedButton, parsedTick);
			if(!strcmp(parsedButton, "four")){
				//rotate left
				Machine.update(app, newGrid, true, LEFT);			
			}
			else if(!strcmp(parsedButton, "one")){
				//translate left
				Machine.update(app, newGrid, false, RIGHT, true, LEFT);
			}
			else if(!strcmp(parsedButton, "three")){
				//translate right
				Machine.update(app, newGrid, false, RIGHT, true, RIGHT);
			}
			else if(!strcmp(parsedButton, "six")){
				//rotate right
				Machine.update(app, newGrid, true, RIGHT);
			}
			else if(!strcmp(parsedButton, "seven")){
				//restart
				Machine.restart(newGrid);				
				write(pFile, "reset", 5);
			}
			else if (!strcmp(parsedButton, "zero")){
				// hard drop
				Machine.update(app, newGrid, false, RIGHT, false, RIGHT, true);			
			}
			else if (!strcmp(parsedButton, "five")){
				//Cycle through colors/modes
				if (Machine.mode == 2)
					Machine.mode = 0;
				else
					Machine.mode++;
							
			}
			else if(!strcmp(parsedTick,"tick")){
				//Translate block down
				Machine.update(app, newGrid);
				rowsRemoved = Machine.getRowsRemoved();
				if (rowsRemoved > 0)
					score.setNewScore(rowsRemoved);
			}
			
			close(pFile);
			pFile = open("/dev/tetrix", O_RDWR);
			fcntl(pFile, F_SETOWN, getpid());
			oflags = fcntl(pFile, F_GETFL);
			fcntl(pFile, F_SETFL, oflags | FASYNC);
		}
        
   	}
//	close(pFile);
	return app->exec();
}

void sighandler(int signo)
{
	trigger = 1;
}

void printSomething(){
	cout << "HELLO WORLD" << endl;
}
