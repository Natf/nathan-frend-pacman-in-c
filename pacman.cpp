#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/glut.h> 
#endif

#include <stddef.h>
#include <iostream>
#include <sys/time.h>
#include "load_and_bind_texture.h"

enum PacmanTile {
    PACMAN_TILE_EMPTY,
    PACMAN_TILE_POINTS,
    PACMAN_TILE_BIGPOINTS,
    PACMAN_TILE_WALL_LEFT,
    PACMAN_TILE_WALL_RIGHT,
    PACMAN_TILE_WALL_TOP,
    PACMAN_TILE_WALL_BOTTOM,
    PACMAN_TILE_WALL_TOPLEFT,
    PACMAN_TILE_WALL_TOPRIGHT,
    PACMAN_TILE_WALL_BOTTOMRIGHT,
    PACMAN_TILE_WALL_BOTTOMLEFT,
    PACMAN_TILE_MAPCORNER_TOPRIGHT,
    PACMAN_TILE_MAPCORNER_TOPLEFT,
    PACMAN_TILE_MAPCORNER_BOTTOMRIGHT,
    PACMAN_TILE_MAPCORNER_BOTTOMLEFT,
    PACMAN_TILE_MAPWALL_TOP,
    PACMAN_TILE_MAPWALL_BOTTOM,
    PACMAN_TILE_MAPWALL_LEFT,
    PACMAN_TILE_MAPWALL_RIGHT,
    PACMAN_TILE_GHOSTJAIL_LEFT,
    PACMAN_TILE_GHOSTJAIL,
    PACMAN_TILE_GHOSTJAIL_RIGHT,
    PACMAN_TILE_PACMAN
};

enum PacmanDirection {
    PACMAN_DIRECTION_NONE,
    PACMAN_DIRECTION_UP,
    PACMAN_DIRECTION_RIGHT,
    PACMAN_DIRECTION_DOWN,
    PACMAN_DIRECTION_LEFT
};

// textures
unsigned int pacmanTexture = 0;
unsigned int backgroundTexture = 0;
unsigned int pointTexture = 0;
unsigned int bigPointTexture = 0;
unsigned int mapCornerTexture = 0;
unsigned int mapEdgeWallTexture = 0;
unsigned int mapEdgeWallVerticalTexture = 0;
unsigned int wallTexture = 0;
unsigned int ghostsTexture = 0;
unsigned int ghostEyeTexture = 0;
unsigned int ghostJailTexture = 0;

// constants
const float tileWidth = 50.0f;
const float pacmanMoveSpeed = tileWidth * 11; // pacman's move speed in pixels per second

// game vars
float pacmanXPos = tileWidth;
float pacmanYPos = tileWidth;
float pacmanStartX;
float pacmanStartY;
int score = 0;
bool inGame = false;
bool inEditor = false;
int selectedMenu = 1;
struct timeval lastLogicCall;
int timeToEat = 0;
PacmanDirection pacmanDirection = PACMAN_DIRECTION_NONE;
PacmanDirection pacmanCurrentDirection = PACMAN_DIRECTION_NONE;
float distanceLeftToMove = 0.0f;
float mapWidth = 600;
float mapHeight = mapWidth * (1750/1400);
int pacmanLives = 2;

// editor vars //
int mouseXPosition;
int mouseYPosition;
unsigned int selectedTexture = wallTexture;
PacmanTile selectedMapTile = PACMAN_TILE_WALL_TOPLEFT;

// ghost vars
float ghostSpeed = tileWidth * 8;
float ghostJailXPos = 0;
float ghostJailYPos = 0;

PacmanDirection redGhostDirection = PACMAN_DIRECTION_DOWN;
PacmanDirection blueGhostDirection = PACMAN_DIRECTION_NONE;
PacmanDirection pinkGhostDirection = PACMAN_DIRECTION_NONE;
PacmanDirection orangeGhostDirection = PACMAN_DIRECTION_NONE;

bool redGhostAlive = true;
bool blueGhostAlive = false;
bool pinkGhostAlive = false;
bool orangeGhostAlive = false;
int redGhostSpawn;
int blueGhostSpawn;
int pinkGhostSpawn;
int orangeGhostSpawn;

float redGhostXPos = tileWidth * 10;
float redGhostYPos = tileWidth * 10;
float blueGhostXPos = tileWidth * 10;
float blueGhostYPos = tileWidth * 10;
float pinkGhostXPos = tileWidth * 10;
float pinkGhostYPos = tileWidth * 10;
float orangeGhostXPos = tileWidth * 10;
float orangeGhostYPos = tileWidth * 10;

PacmanTile map[31][28];

#include "draw.h"
#include "mapLoader.h"
#include "input.h"
#include "logic.h"

void logic() {
    if (inGame) {
        ghostSpawnCheck();
        ghostsMovement();
        doMovement();
        ghostCheckCollisions();
        glutPostRedisplay();
    } else if(inEditor) {
        glutPostRedisplay();
    }
    gettimeofday(&lastLogicCall, NULL);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    if(inGame) {
        drawMap();
        drawPacMan();
        drawScore();
        drawGhosts();
        drawGhostJail();
        drawLives();
    } else if (inEditor) {
        drawMap();
        drawToolTip();
        drawMousePiece();
    } else {
        drawTitleScreen();
    }
    glutSwapBuffers();
}

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1400, 0, 1750);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    gettimeofday(&lastLogicCall, NULL);
    pacmanTexture = load_and_bind_texture("./images/pacman.png");
    backgroundTexture = load_and_bind_texture("./images/black.png");
    pointTexture = load_and_bind_texture("./images/points.png");
    bigPointTexture = load_and_bind_texture("./images/big-points.png");
    mapCornerTexture = load_and_bind_texture("./images/map-corner.png");
    wallTexture = load_and_bind_texture("./images/wall.png");
    mapEdgeWallTexture = load_and_bind_texture("./images/map-wall.png");
    mapEdgeWallVerticalTexture = load_and_bind_texture("./images/map-wall-vertical.png");
    ghostsTexture = load_and_bind_texture("./images/ghosts.png");
    ghostEyeTexture = load_and_bind_texture("./images/ghost-eye.png");
    ghostJailTexture = load_and_bind_texture("./images/ghost-jail.png");
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    loadMap();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    glutInitWindowSize(mapWidth, mapHeight);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("PacMan");
    glutDisplayFunc(display);

    // handlers for keyboard input
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutSpecialUpFunc(specialUp);
    glutPassiveMotionFunc(mouseMove);
    glutMouseFunc(mouse);

    glutIdleFunc(logic);
    init();

    glutMainLoop();
    return 0;
}
