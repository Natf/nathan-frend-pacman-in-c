#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stddef.h>

void initializeMap() {
    distanceLeftToMove = 0.0f;
    timeToEat = 0;
    int now = (int)time(NULL);
    redGhostAlive = true;
    blueGhostAlive = false;
    pinkGhostAlive = false;
    orangeGhostAlive = false;
    blueGhostSpawn = now + 10;
    pinkGhostSpawn = now + 20;
    orangeGhostSpawn = now + 25;

    pacmanXPos = pacmanStartX;
    pacmanYPos = pacmanStartY;
    redGhostXPos = ghostJailXPos;
    redGhostYPos = ghostJailYPos + tileWidth;
    blueGhostXPos = ghostJailXPos - tileWidth;
    blueGhostYPos = ghostJailYPos;
    pinkGhostXPos = ghostJailXPos;
    pinkGhostYPos = ghostJailYPos;
    orangeGhostXPos = ghostJailXPos + tileWidth;
    orangeGhostYPos = ghostJailYPos;

    redGhostDirection = PACMAN_DIRECTION_DOWN;
    blueGhostDirection = PACMAN_DIRECTION_NONE;
    pinkGhostDirection = PACMAN_DIRECTION_NONE;
    orangeGhostDirection = PACMAN_DIRECTION_NONE;
}

void keyboard(unsigned char key, int, int) {
    switch (key) {
        case 'q' :
            exit(1);
            break;
        case 32 :
            if (selectedMenu == 1) {
                loadMap();
                initializeMap();
                inGame = true;
            } else if (selectedMenu == 2) {
                selectedTexture = wallTexture;
                loadMap();
                inEditor = true;
            }
            break;
        case 'o':
            switch (selectedMapTile) {
                case PACMAN_TILE_EMPTY: selectedMapTile = PACMAN_TILE_POINTS; break;
                case PACMAN_TILE_POINTS: selectedMapTile = PACMAN_TILE_BIGPOINTS; break;
                case PACMAN_TILE_BIGPOINTS: selectedMapTile = PACMAN_TILE_WALL_LEFT; break;
                case PACMAN_TILE_WALL_LEFT: selectedMapTile = PACMAN_TILE_WALL_RIGHT; break;
                case PACMAN_TILE_WALL_RIGHT: selectedMapTile = PACMAN_TILE_WALL_TOP; break;
                case PACMAN_TILE_WALL_TOP: selectedMapTile = PACMAN_TILE_WALL_BOTTOM; break;
                case PACMAN_TILE_WALL_BOTTOM: selectedMapTile = PACMAN_TILE_WALL_TOPLEFT; break;
                case PACMAN_TILE_WALL_TOPLEFT: selectedMapTile = PACMAN_TILE_WALL_TOPRIGHT; break;
                case PACMAN_TILE_WALL_TOPRIGHT: selectedMapTile = PACMAN_TILE_WALL_BOTTOMRIGHT; break;
                case PACMAN_TILE_WALL_BOTTOMRIGHT: selectedMapTile = PACMAN_TILE_WALL_BOTTOMLEFT; break;
                case PACMAN_TILE_WALL_BOTTOMLEFT: selectedMapTile = PACMAN_TILE_MAPCORNER_TOPRIGHT; break;
                case PACMAN_TILE_MAPCORNER_TOPRIGHT: selectedMapTile = PACMAN_TILE_MAPCORNER_TOPLEFT; break;
                case PACMAN_TILE_MAPCORNER_TOPLEFT: selectedMapTile = PACMAN_TILE_MAPCORNER_BOTTOMRIGHT; break;
                case PACMAN_TILE_MAPCORNER_BOTTOMRIGHT: selectedMapTile = PACMAN_TILE_MAPCORNER_BOTTOMLEFT; break;
                case PACMAN_TILE_MAPCORNER_BOTTOMLEFT: selectedMapTile = PACMAN_TILE_MAPWALL_TOP; break;
                case PACMAN_TILE_MAPWALL_TOP: selectedMapTile = PACMAN_TILE_MAPWALL_BOTTOM; break;
                case PACMAN_TILE_MAPWALL_BOTTOM: selectedMapTile = PACMAN_TILE_MAPWALL_LEFT; break;
                case PACMAN_TILE_MAPWALL_LEFT: selectedMapTile = PACMAN_TILE_MAPWALL_RIGHT; break;
                case PACMAN_TILE_MAPWALL_RIGHT: selectedMapTile = PACMAN_TILE_GHOSTJAIL; break;
                case PACMAN_TILE_GHOSTJAIL: selectedMapTile = PACMAN_TILE_GHOSTJAIL_LEFT; break;
                case PACMAN_TILE_GHOSTJAIL_LEFT: selectedMapTile = PACMAN_TILE_GHOSTJAIL_RIGHT; break;
                case PACMAN_TILE_GHOSTJAIL_RIGHT: selectedMapTile = PACMAN_TILE_PACMAN; break;
                case PACMAN_TILE_PACMAN: selectedMapTile = PACMAN_TILE_EMPTY; break;
                default: break;
            }
        break;
        case 'p':
            switch (selectedMapTile) {
                case PACMAN_TILE_EMPTY: selectedMapTile = PACMAN_TILE_PACMAN; break;
                case PACMAN_TILE_POINTS: selectedMapTile = PACMAN_TILE_EMPTY; break;
                case PACMAN_TILE_BIGPOINTS: selectedMapTile = PACMAN_TILE_POINTS; break;
                case PACMAN_TILE_WALL_LEFT: selectedMapTile = PACMAN_TILE_BIGPOINTS; break;
                case PACMAN_TILE_WALL_RIGHT: selectedMapTile = PACMAN_TILE_WALL_LEFT; break;
                case PACMAN_TILE_WALL_TOP: selectedMapTile = PACMAN_TILE_WALL_RIGHT; break;
                case PACMAN_TILE_WALL_BOTTOM: selectedMapTile = PACMAN_TILE_WALL_TOP; break;
                case PACMAN_TILE_WALL_TOPLEFT: selectedMapTile = PACMAN_TILE_WALL_BOTTOM; break;
                case PACMAN_TILE_WALL_TOPRIGHT: selectedMapTile = PACMAN_TILE_WALL_TOPLEFT; break;
                case PACMAN_TILE_WALL_BOTTOMRIGHT: selectedMapTile = PACMAN_TILE_WALL_TOPRIGHT; break;
                case PACMAN_TILE_WALL_BOTTOMLEFT: selectedMapTile = PACMAN_TILE_WALL_BOTTOMRIGHT; break;
                case PACMAN_TILE_MAPCORNER_TOPRIGHT: selectedMapTile = PACMAN_TILE_WALL_BOTTOMLEFT; break;
                case PACMAN_TILE_MAPCORNER_TOPLEFT: selectedMapTile = PACMAN_TILE_MAPCORNER_TOPRIGHT; break;
                case PACMAN_TILE_MAPCORNER_BOTTOMRIGHT: selectedMapTile = PACMAN_TILE_MAPCORNER_TOPLEFT; break;
                case PACMAN_TILE_MAPCORNER_BOTTOMLEFT: selectedMapTile = PACMAN_TILE_MAPCORNER_BOTTOMRIGHT; break;
                case PACMAN_TILE_MAPWALL_TOP: selectedMapTile = PACMAN_TILE_MAPCORNER_BOTTOMLEFT; break;
                case PACMAN_TILE_MAPWALL_BOTTOM: selectedMapTile = PACMAN_TILE_MAPWALL_TOP; break;
                case PACMAN_TILE_MAPWALL_LEFT: selectedMapTile = PACMAN_TILE_MAPWALL_BOTTOM; break;
                case PACMAN_TILE_MAPWALL_RIGHT: selectedMapTile = PACMAN_TILE_MAPWALL_LEFT; break;
                case PACMAN_TILE_GHOSTJAIL: selectedMapTile = PACMAN_TILE_MAPWALL_RIGHT; break;
                case PACMAN_TILE_GHOSTJAIL_LEFT: selectedMapTile = PACMAN_TILE_GHOSTJAIL; break;
                case PACMAN_TILE_GHOSTJAIL_RIGHT: selectedMapTile = PACMAN_TILE_GHOSTJAIL_LEFT; break;
                case PACMAN_TILE_PACMAN: selectedMapTile = PACMAN_TILE_GHOSTJAIL_RIGHT; break;
                default: break;
            }
        break;
        case 's':
            if(inEditor) {
                saveMap();
                inEditor = false;
                glutPostRedisplay();
            }
        break;
    }
}

void special(int key, int, int) {
    if(!inGame) {
        switch (key) {
            case GLUT_KEY_UP:
                if(selectedMenu == 1) {
                    selectedMenu = 2;
                } else {
                    selectedMenu = 1;
                }
                break;
            case GLUT_KEY_DOWN:
                if(selectedMenu == 1) {
                    selectedMenu = 2;
                } else {
                    selectedMenu = 1;
                }
                break;
            default: break;
        }
        glutPostRedisplay();
        return;
    }

    switch (key) {
        case GLUT_KEY_LEFT: pacmanDirection = PACMAN_DIRECTION_LEFT; break;
        case GLUT_KEY_RIGHT: pacmanDirection = PACMAN_DIRECTION_RIGHT; break;
        case GLUT_KEY_UP: pacmanDirection = PACMAN_DIRECTION_UP; break;
        case GLUT_KEY_DOWN: pacmanDirection = PACMAN_DIRECTION_DOWN; break;
    }
}

void specialUp(int key, int, int) {
    switch (key) {
        case GLUT_KEY_LEFT: pacmanDirection = (pacmanDirection == PACMAN_DIRECTION_LEFT) ? PACMAN_DIRECTION_NONE : pacmanDirection; break;
        case GLUT_KEY_RIGHT: pacmanDirection = (pacmanDirection == PACMAN_DIRECTION_RIGHT) ? PACMAN_DIRECTION_NONE : pacmanDirection; break;
        case GLUT_KEY_UP: pacmanDirection = (pacmanDirection == PACMAN_DIRECTION_UP) ? PACMAN_DIRECTION_NONE : pacmanDirection; break;
        case GLUT_KEY_DOWN: pacmanDirection = (pacmanDirection == PACMAN_DIRECTION_DOWN) ? PACMAN_DIRECTION_NONE : pacmanDirection; break;
    }
}

void mouseMove(int x, int y) {
    if (inEditor) {
        mouseXPosition = x * (1400 / mapWidth);
        mouseYPosition = 1750 - (y * (1750 / mapHeight));
    }
}

void mouse(int button, int state, int x, int y)
{
    if (inEditor) {
        x = (x * (1400 / mapWidth)) / tileWidth;
        y = (1750 - (y * (1750 / mapHeight)) - (tileWidth * 4)) / tileWidth;
        if (x < 0) {
            x = 0;
        } else if (x > 27) {
            x = 27;
        }

        if (y < 0) {
            y = 0;
        } else if (y > 30) {
            y = 30;
        }
        if (button == GLUT_LEFT_BUTTON) {
            if (state == GLUT_DOWN) {
                map[y][x] = selectedMapTile;
            }
        }
    }
}