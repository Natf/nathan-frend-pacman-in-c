#ifndef DRAW_H
#define DRAW_H
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stddef.h>

void drawText(const char* text) {
    size_t len = strlen(text);
    for (size_t i=0;i<len;i++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
}

void drawTile(int texture, float xFrame, float totalXFrames, float yFrame, float totalYFrames) {
    glEnable(GL_TEXTURE_2D);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin (GL_QUADS);
    glTexCoord2f ((xFrame-1)/totalXFrames, (yFrame-1)/totalYFrames); // lower left corner
    glVertex2i(0, 0);
    glTexCoord2f (xFrame/totalXFrames, (yFrame-1)/totalYFrames); // lower right corner
    glVertex2i(tileWidth, 0);
    glTexCoord2f (xFrame/totalXFrames, yFrame/totalYFrames); // upper right corner
    glVertex2i(tileWidth, tileWidth);
    glTexCoord2f ((xFrame-1)/totalXFrames, yFrame/totalYFrames); // upper left corner
    glVertex2i(0, tileWidth);
    glEnd ();
    glDisable(GL_TEXTURE_2D);
}

void drawSelection() {
    if (selectedMenu == 1) {
        glPushMatrix();
        glBegin(GL_LINE_LOOP);
        glVertex2i(450, 270);
        glVertex2i(880, 270);
        glVertex2i(880, 430);
        glVertex2i(450, 430);
        glEnd();
        glPopMatrix();
    } else {
        glPushMatrix();
        glBegin(GL_LINE_LOOP);
        glVertex2i(250, 70);
        glVertex2i(1100, 70);
        glVertex2i(1100, 230);
        glVertex2i(250, 230);
        glEnd();
        glPopMatrix();
    }
}

void drawTitleScreen() {
    glColor3f(1.0f, 1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(510.0f, 300.0f, 0.0f);
    glScalef(1.0f, 1.0f, 1.0f);
    drawText("PLAY");
    glPopMatrix();

    glPushMatrix();
    glTranslatef(280.0f, 100.0f, 0.0f);
    drawText("MAP EDITOR");
    glPopMatrix();

    glPushMatrix();
    glTranslatef(10.0f, 40.0f, 0.0f);
    glScalef(0.225f,0.225f,1.0f);
    drawText("Use the up/down arrows keys to make a selection and space bar to confirm selection.");
    glPopMatrix();

    drawSelection();
}

void drawMapTile(PacmanTile tile) {
    switch (tile) {
        case PACMAN_TILE_POINTS : drawTile(pointTexture,1,1,1,1); break;
        case PACMAN_TILE_BIGPOINTS : drawTile(bigPointTexture,1,1,1,1); break;
        case PACMAN_TILE_MAPCORNER_BOTTOMLEFT : drawTile(mapCornerTexture,1,2,2,2); break;
        case PACMAN_TILE_MAPCORNER_BOTTOMRIGHT : drawTile(mapCornerTexture,2,2,2,2); break;
        case PACMAN_TILE_MAPCORNER_TOPLEFT : drawTile(mapCornerTexture,1,2,1,2); break;
        case PACMAN_TILE_MAPCORNER_TOPRIGHT : drawTile(mapCornerTexture,2,2,1,2); break;
        case PACMAN_TILE_MAPWALL_BOTTOM :
            glTranslatef(tileWidth, tileWidth, 0.0f);
            glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
            drawTile(mapEdgeWallTexture, 1, 1, 1, 1);
            glTranslatef(-tileWidth, -tileWidth, 0.0f);
            break;
        case PACMAN_TILE_MAPWALL_TOP : drawTile(mapEdgeWallTexture, 1, 1, 1, 1); break;
        case PACMAN_TILE_MAPWALL_LEFT : drawTile(mapEdgeWallVerticalTexture, 1, 1, 1, 1); break;
        case PACMAN_TILE_MAPWALL_RIGHT :
            glTranslatef(tileWidth, tileWidth, 0.0f);
            glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
            drawTile(mapEdgeWallVerticalTexture, 1, 1, 1, 1);
            glTranslatef(-tileWidth, -tileWidth, 0.0f);
            break;
        case PACMAN_TILE_WALL_BOTTOM: drawTile(wallTexture, 2,3,3,3); break;
        case PACMAN_TILE_WALL_LEFT: drawTile(wallTexture, 1,3,2,3); break;
        case PACMAN_TILE_WALL_RIGHT: drawTile(wallTexture, 3,3,2,3); break;
        case PACMAN_TILE_WALL_TOP: drawTile(wallTexture, 2,3,1,3); break;
        case PACMAN_TILE_WALL_TOPRIGHT: drawTile(wallTexture, 3,3,1,3); break;
        case PACMAN_TILE_WALL_TOPLEFT: drawTile(wallTexture, 1,3,1,3); break;
        case PACMAN_TILE_WALL_BOTTOMLEFT: drawTile(wallTexture, 1,3,3,3); break;
        case PACMAN_TILE_WALL_BOTTOMRIGHT: drawTile(wallTexture, 3,3,3,3); break;
        case PACMAN_TILE_PACMAN: drawTile(pacmanTexture, 2, 2,1,1); break;
        case PACMAN_TILE_GHOSTJAIL: drawTile(ghostJailTexture, 2, 3,1,1); break;
        case PACMAN_TILE_GHOSTJAIL_LEFT: drawTile(ghostJailTexture, 1, 3,1,1); break;
        case PACMAN_TILE_GHOSTJAIL_RIGHT: drawTile(ghostJailTexture, 3, 3,1,1); break;
        default : drawTile(backgroundTexture,1,1,1,1); break;
    }
}

void drawMap() {
    for(int y = 30; y >= 0; y--) {
        for(int x = 27; x >= 0; x--) {
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();

            glTranslatef(0, tileWidth*4, 0.0f);
            glTranslatef(tileWidth*x, tileWidth*y, 0.0f);
            drawMapTile(map[y][x]);
            glPopMatrix();
        }
    }
}

void drawPacMan() {
    // select which texure to render
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(pacmanXPos + (tileWidth/2), pacmanYPos + (tileWidth/2) + tileWidth*4, 0.0f);
    switch(pacmanCurrentDirection) {
        case PACMAN_DIRECTION_LEFT: glRotatef(0.0f, 0.0f, 0.0f, 1.0f); break;
        case PACMAN_DIRECTION_RIGHT: glRotatef(180.0f, 0.0f, 0.0f, 1.0f); break;
        case PACMAN_DIRECTION_UP: glRotatef(270.0f, 0.0f, 0.0f, 1.0f); break;
        case PACMAN_DIRECTION_DOWN: glRotatef(90.0f, 0.0f, 0.0f, 1.0f); break;
        default: break;
    }
    glTranslatef(-(tileWidth/2) , -(tileWidth/2) , 0.0f);
    drawTile(pacmanTexture, (distanceLeftToMove > 0)? 1 : 2, 2,1,1);
    glPopMatrix();
}

void drawEye(float left, float xPos, float yPos, PacmanDirection direction) {
    glPushMatrix();
    glTranslatef((xPos + (tileWidth/2)) + (tileWidth * 0.175 * left), yPos + (tileWidth * 4.5), 0.0f);
    switch(direction) {
        case PACMAN_DIRECTION_UP: glRotatef(270.0f, 0.0f, 0.0f, 1.0f); break;
        case PACMAN_DIRECTION_RIGHT: glRotatef(180.0f, 0.0f, 0.0f, 1.0f); break;
        case PACMAN_DIRECTION_DOWN: glRotatef(90.0f, 0.0f, 0.0f, 1.0f); break;
        default: break;
    }
    glTranslatef(-(tileWidth/2) , -(tileWidth/2) , 0.0f);
    drawTile(ghostEyeTexture, 1, 1, 1, 1);
    glPopMatrix();
}

void drawGhost(float xPos, float yPos, int frame, PacmanDirection direction) {
    int now = (int)time(NULL);
    if (now < timeToEat) {
        frame = 5;
    }
    glPushMatrix();
    glTranslatef(xPos, yPos + (tileWidth * 4), 0.0f);
    drawTile(ghostsTexture, frame, 5, 1, 1);
    glPopMatrix();


    drawEye(1, xPos, yPos, direction);
    drawEye(-1, xPos, yPos, direction);
}

void drawGhosts() {
    glMatrixMode(GL_MODELVIEW);
    drawGhost(redGhostXPos, redGhostYPos, 1, redGhostDirection);
    drawGhost(blueGhostXPos, blueGhostYPos, 2, blueGhostDirection);
    drawGhost(pinkGhostXPos, pinkGhostYPos, 3, pinkGhostDirection);
    drawGhost(orangeGhostXPos, orangeGhostYPos, 4, orangeGhostDirection);
}

void drawGhostJail() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(0, tileWidth * 4, 0.0f);
    glTranslatef(ghostJailXPos - tileWidth, ghostJailYPos, 0.0f);
    drawTile(ghostJailTexture, 1, 3, 1, 1);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0, tileWidth * 4, 0.0f);
    glTranslatef(ghostJailXPos, ghostJailYPos, 0.0f);
    drawTile(ghostJailTexture, 2, 3, 1, 1);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0, tileWidth * 4, 0.0f);
    glTranslatef(ghostJailXPos + tileWidth, ghostJailYPos, 0.0f);
    drawTile(ghostJailTexture, 3, 3, 1, 1);
    glPopMatrix();
}

void drawScore() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(100.0f, 100.0f, 0.0f);
    glScalef(0.3f, 0.3f, 1.0f);
    drawText("SCORE: ");
    glPopMatrix();

    glPushMatrix();
    glTranslatef(400.0f, 100.0f, 0.0f);
    glScalef(0.3f, 0.3f, 1.0f);
    char str[15];
    sprintf(str, "%d", score);
    drawText(str);

    glPopMatrix();
}

void drawLives() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(700.0f, 100.0f, 0.0f);
    glScalef(0.3f, 0.3f, 1.0f);
    drawText("LIVES: ");
    glPopMatrix();

    for(int lives = pacmanLives; lives > 0; lives--) {
        glPushMatrix();
        glTranslatef(775.0f + (lives * 50), 90.0f, 0.0f);
        glScalef(1.0f, 1.0f, 1.0f);
        drawTile(pacmanTexture, 2, 2, 1, 1);
        glPopMatrix();
    }
}

void drawToolTip() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(100.0f, 100.0f, 0.0f);
    glScalef(0.3f, 0.3f, 1.0f);
    drawText("Currently selected tile: ");
    glPopMatrix();

    glPushMatrix();
    glTranslatef(550.0f, 100.0f, 0.0f);
    drawMapTile(selectedMapTile);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(100.0f, 50.0f, 0.0f);
    glScalef(0.3f, 0.3f, 1.0f);
    drawText("Press o/p to cycle through map pieces");
    glPopMatrix();
}

void drawMousePiece() {
    glPushMatrix();
    glTranslatef(mouseXPosition - (tileWidth/2), mouseYPosition - (tileWidth/2), 0.0f);
    drawMapTile(selectedMapTile);
    glPopMatrix();
}

#endif