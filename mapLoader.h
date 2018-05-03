#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stddef.h>

void saveMap() {
    FILE *mapFile = fopen("map.txt", "w");
    if (mapFile == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    for (int y = 30; y >= 0; y--) {
        for (int x = 27; x >= 0; x--) {
            fprintf(mapFile, "%d\n", map[y][x]);
        }
    }
    const char *text = "Write this to the file";
    fclose(mapFile);
}

void loadEmptyMap() {
    for (int y = 30; y >= 0; y--) {
        for (int x = 27; x >= 0; x--) {
            map[y][x] = PACMAN_TILE_POINTS;
        }
    }
}

void loadMap() {
    FILE *mapFile = fopen("map-1.txt", "r");
    char *line = NULL;
    size_t len = 0;

    if (mapFile == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    for (int y = 30; y >= 0; y--) {
        for (int x = 27; x >= 0; x--) {
            getline(&line, &len, mapFile);
            int mapTile = atoi(line);
            map[y][x] = (PacmanTile) mapTile;

            switch(map[y][x]) {
                case PACMAN_TILE_GHOSTJAIL:
                    ghostJailXPos = tileWidth * x;
                    ghostJailYPos = tileWidth * y;
                    redGhostXPos = ghostJailXPos;
                    redGhostYPos = ghostJailYPos + tileWidth;
                    blueGhostXPos = ghostJailXPos - tileWidth;
                    blueGhostYPos = ghostJailYPos;
                    pinkGhostXPos = ghostJailXPos;
                    pinkGhostYPos = ghostJailYPos;
                    orangeGhostXPos = ghostJailXPos + tileWidth;
                    orangeGhostYPos = ghostJailYPos;
                    break;
                case PACMAN_TILE_PACMAN:
                    if (!inEditor) {
                        pacmanXPos = tileWidth * x;
                        pacmanStartX = pacmanXPos;
                        pacmanYPos = tileWidth * y;
                        pacmanStartY = pacmanYPos;
                        map[y][x] = PACMAN_TILE_EMPTY;
                    }
                default: break;
            }
        }
    }

    fclose(mapFile);
}