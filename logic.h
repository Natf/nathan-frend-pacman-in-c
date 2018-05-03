#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stddef.h>
#include <math.h>

float getDistanceToTravel(float moveSpeed) {
    struct timeval timeNow;
    gettimeofday(&timeNow, NULL);
    double elapsedTime = (((timeNow.tv_sec - lastLogicCall.tv_sec) * 1000.0) + ((timeNow.tv_usec - lastLogicCall.tv_usec) / 1000.0)) / 1000;
    float distance = elapsedTime * moveSpeed;
    return distance;
};

void eatMapTile(int y, int x) {
    switch(map[y][x]) {
        case PACMAN_TILE_POINTS: score += 100; break;
        case PACMAN_TILE_BIGPOINTS:
            score += 500;
            timeToEat = (int)time(NULL) + 5;
        break;
        default: break;
    }
    map[y][x] = PACMAN_TILE_EMPTY;
}

bool canMoveInDirection(float xPos, float yPos, PacmanDirection direction, bool eatTile) {
    float xDestination = xPos + (tileWidth/2);
    float yDestination = yPos + (tileWidth/2);

    switch(direction) {
        case PACMAN_DIRECTION_LEFT: xDestination -= tileWidth; break;
        case PACMAN_DIRECTION_RIGHT: xDestination += tileWidth; break;
        case PACMAN_DIRECTION_UP: yDestination += tileWidth; break;
        case PACMAN_DIRECTION_DOWN: yDestination -= tileWidth; break;
        default: return false;
    }

    PacmanTile destinationContents = map[(int)(yDestination/tileWidth)][(int)(xDestination/tileWidth)];

    switch (destinationContents) {
        case PACMAN_TILE_POINTS:
            if (eatTile) {
                eatMapTile((int)(yDestination/tileWidth), (int)(xDestination/tileWidth));
            }
            return true;
        case PACMAN_TILE_BIGPOINTS:
            if (eatTile) {
                eatMapTile((int)(yDestination/tileWidth), (int)(xDestination/tileWidth));
            }
            return true;
        case PACMAN_TILE_EMPTY: return true;
        default: return false;
    }
}

void doMovement() {
    if((distanceLeftToMove <= 0.0f) && (pacmanDirection != PACMAN_DIRECTION_NONE)) { // finished moving a tile and ready to accept new direction
        if(canMoveInDirection(pacmanXPos, pacmanYPos, pacmanDirection, true)) {
            pacmanCurrentDirection = pacmanDirection;
            distanceLeftToMove = tileWidth;
        }
    }

    float distance = getDistanceToTravel(pacmanMoveSpeed);

    if(distanceLeftToMove > 0.0f) {
        if (distance > distanceLeftToMove) {
            distance = distanceLeftToMove;
        }
        switch(pacmanCurrentDirection) {
            case PACMAN_DIRECTION_LEFT: pacmanXPos -= distance; break;
            case PACMAN_DIRECTION_RIGHT: pacmanXPos += distance; break;
            case PACMAN_DIRECTION_UP: pacmanYPos += distance; break;
            case PACMAN_DIRECTION_DOWN: pacmanYPos -= distance; break;
            default:break;
        }
        distanceLeftToMove -= distance;
    }
}

PacmanDirection ghostMoveNormal(float xPos, float yPos, PacmanDirection direction, float distance) {
    PacmanDirection origDirection = direction;
    while (true) {
        float xPosMod = xPos - (tileWidth/2) + distance;
        float yPosMod = yPos - (tileWidth/2) + distance;

        switch(direction) {
            case PACMAN_DIRECTION_LEFT: xPosMod += tileWidth * 0.75; break;
            case PACMAN_DIRECTION_DOWN: yPosMod += tileWidth * 0.75; break;
            default: break;
        }

        if (canMoveInDirection(xPosMod, yPosMod, direction, false)) {
            return direction;
        } else {
            int random = rand() % 12; // 4 4 2 2
            switch (origDirection) {
                case PACMAN_DIRECTION_LEFT:
                    if (random >= 8) {
                        direction = PACMAN_DIRECTION_UP;
                    } else if (random >= 4) {
                        direction = PACMAN_DIRECTION_DOWN;
                    } else if (random >= 2) {
                        direction = PACMAN_DIRECTION_LEFT;
                    } else if (random >= 0) {
                        direction = PACMAN_DIRECTION_RIGHT;
                    }
                    break;
                case PACMAN_DIRECTION_RIGHT:
                    if (random >= 8) {
                        direction = PACMAN_DIRECTION_UP;
                    } else if (random >= 4) {
                        direction = PACMAN_DIRECTION_DOWN;
                    } else if (random >= 2) {
                        direction = PACMAN_DIRECTION_LEFT;
                    } else if (random >= 0) {
                        direction = PACMAN_DIRECTION_RIGHT;
                    }
                    break;
                case PACMAN_DIRECTION_UP:
                    if (random >= 8) {
                        direction = PACMAN_DIRECTION_LEFT;
                    } else if (random >= 4) {
                        direction = PACMAN_DIRECTION_RIGHT;
                    } else if (random >= 2) {
                        direction = PACMAN_DIRECTION_DOWN;
                    } else if (random >= 0) {
                        direction = PACMAN_DIRECTION_UP;
                    }
                    break;
                case PACMAN_DIRECTION_DOWN:
                    if (random >= 8) {
                        direction = PACMAN_DIRECTION_LEFT;
                    } else if (random >= 4) {
                        direction = PACMAN_DIRECTION_RIGHT;
                    } else if (random >= 2) {
                        direction = PACMAN_DIRECTION_DOWN;
                    } else if (random >= 0) {
                        direction = PACMAN_DIRECTION_UP;
                    }
                    break;
                default: return PACMAN_DIRECTION_NONE;
            }
        }
    }
}

void ghostsMovement() {
    int now = (int)time(NULL);
    float distance = (now > timeToEat) ? getDistanceToTravel(ghostSpeed) : getDistanceToTravel(ghostSpeed*0.3);
    redGhostDirection = ghostMoveNormal(redGhostXPos, redGhostYPos, redGhostDirection, distance);
    switch (redGhostDirection) {
        case PACMAN_DIRECTION_LEFT: redGhostXPos -= distance; break;
        case PACMAN_DIRECTION_UP: redGhostYPos += distance; break;
        case PACMAN_DIRECTION_RIGHT: redGhostXPos += distance; break;
        case PACMAN_DIRECTION_DOWN: redGhostYPos -= distance; break;
        default: break;
    }

    blueGhostDirection = ghostMoveNormal(blueGhostXPos, blueGhostYPos, blueGhostDirection, distance);
    switch (blueGhostDirection) {
        case PACMAN_DIRECTION_LEFT: blueGhostXPos -= distance; break;
        case PACMAN_DIRECTION_UP: blueGhostYPos += distance; break;
        case PACMAN_DIRECTION_RIGHT: blueGhostXPos += distance; break;
        case PACMAN_DIRECTION_DOWN: blueGhostYPos -= distance; break;
        default: break;
    }

    pinkGhostDirection = ghostMoveNormal(pinkGhostXPos, pinkGhostYPos, pinkGhostDirection, distance);
    switch (pinkGhostDirection) {
        case PACMAN_DIRECTION_LEFT: pinkGhostXPos -= distance; break;
        case PACMAN_DIRECTION_UP: pinkGhostYPos += distance; break;
        case PACMAN_DIRECTION_RIGHT: pinkGhostXPos += distance; break;
        case PACMAN_DIRECTION_DOWN: pinkGhostYPos -= distance; break;
        default: break;
    }

    orangeGhostDirection = ghostMoveNormal(orangeGhostXPos, orangeGhostYPos, orangeGhostDirection, distance);
    switch (orangeGhostDirection) {
        case PACMAN_DIRECTION_LEFT: orangeGhostXPos -= distance; break;
        case PACMAN_DIRECTION_UP: orangeGhostYPos += distance; break;
        case PACMAN_DIRECTION_RIGHT: orangeGhostXPos += distance; break;
        case PACMAN_DIRECTION_DOWN: orangeGhostYPos -= distance; break;
        default: break;
    }
}

void ghostSpawnCheck() {
    int timeNow = (int)time(NULL);
    if ((!redGhostAlive) && (timeNow >= redGhostSpawn)) {
        redGhostAlive = true;
        redGhostDirection = PACMAN_DIRECTION_DOWN;
        redGhostXPos = ghostJailXPos;
        redGhostYPos = ghostJailYPos + tileWidth;
    }
    if ((!blueGhostAlive) && (timeNow >= blueGhostSpawn)) {
        blueGhostAlive = true;
        blueGhostDirection = PACMAN_DIRECTION_DOWN;
        blueGhostXPos = ghostJailXPos;
        blueGhostYPos = ghostJailYPos + tileWidth;
    }
    if ((!pinkGhostAlive) && (timeNow >= pinkGhostSpawn)) {
        pinkGhostAlive = true;
        pinkGhostDirection = PACMAN_DIRECTION_DOWN;
        pinkGhostXPos = ghostJailXPos;
        pinkGhostYPos = ghostJailYPos + tileWidth;
    }
    if ((!orangeGhostAlive) && (timeNow >= orangeGhostSpawn)) {
        orangeGhostAlive = true;
        orangeGhostDirection = PACMAN_DIRECTION_DOWN;
        orangeGhostXPos = ghostJailXPos;
        orangeGhostYPos = ghostJailYPos + tileWidth;
    }
}

void looseLife() {
    pacmanLives --;
    if(pacmanLives >= 0) {
        score -= 1000;
        initializeMap();
    } else {
        pacmanLives = 2;
        distanceLeftToMove = 0.0f;
        inGame = 0;
        glutPostRedisplay();
    }
}

void ghostCheckCollisions() {
    int now = (int)time(NULL);
    if (pow(pow(redGhostXPos - pacmanXPos, 2) + pow(redGhostYPos - pacmanYPos, 2), 0.5) <= 30) {
        if (now > timeToEat) {
            looseLife();
        } else {
            redGhostXPos = ghostJailXPos - tileWidth;
            redGhostYPos = ghostJailYPos;
            redGhostDirection = PACMAN_DIRECTION_NONE;
            redGhostAlive = false;
            redGhostSpawn = now + 15;
            score += 1000;
        }
    } else if (pow(pow(blueGhostXPos - pacmanXPos, 2) + pow(blueGhostYPos - pacmanYPos, 2), 0.5) <= 30) {
        if (now > timeToEat) {
            looseLife();
        } else {
            blueGhostXPos = ghostJailXPos - tileWidth;
            blueGhostYPos = ghostJailYPos;
            blueGhostDirection = PACMAN_DIRECTION_NONE;
            blueGhostAlive = false;
            blueGhostSpawn = now + 15;
            score += 1000;
        }
    } else if (pow(pow(pinkGhostXPos - pacmanXPos, 2) + pow(pinkGhostYPos - pacmanYPos, 2), 0.5) <= 30) {
        if (now > timeToEat) {
            looseLife();
        } else {
            pinkGhostXPos = ghostJailXPos;
            pinkGhostYPos = ghostJailYPos;
            pinkGhostDirection = PACMAN_DIRECTION_NONE;
            pinkGhostAlive = false;
            pinkGhostSpawn = now + 15;
            score += 1000;
        }
    } else if (pow(pow(orangeGhostXPos - pacmanXPos, 2) + pow(orangeGhostYPos - pacmanYPos, 2), 0.5) <= 30) {
        if (now > timeToEat) {
            looseLife();
        } else {
            orangeGhostXPos = ghostJailXPos + tileWidth;
            orangeGhostYPos = ghostJailYPos;
            orangeGhostDirection = PACMAN_DIRECTION_NONE;
            orangeGhostAlive = false;
            orangeGhostSpawn = now + 15;
            score += 1000;
        }
    }
}