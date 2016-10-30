#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


/* ------------------------ BEGINNING OF PROVIDED CODE  -------------------------- */

#define MAX_OBSTACLES 25 /* maximum number of obstacles */
int num_obstacles = 13; /* number of obstacles */
double obstacle[MAX_OBSTACLES][2] = /* obstacle locations */
{
    {0.61, 2.743},
    {0.915, 2.743},
    {1.219, 2.743},
    {1.829, 1.219},
    {1.829, 1.524},
    {1.829, 1.829},
    {1.829, 2.134},
    {2.743, 0.305},
    {2.743, 0.610},
    {2.743, 0.915},
    {2.743, 2.743},
    {3.048, 2.743},
    {3.353, 2.743},
    {-1,-1},
    {-1,-1},
    {-1,-1},
    {-1,-1},
    {-1,-1},
    {-1,-1},
    {-1,-1},
    {-1,-1},
    {-1,-1},
    {-1,-1},
    {-1,-1},
    {-1,-1}
};

double start[2] = {0.305, 1.219}; /* start location */
double goal[2] = {3.658, 1.829}; /* goal location */

// FUNCTION DECLARATIONS
void print_map();
int float_point_to_grid_coords(float point);

/* ------------------------ END OF PROVIDED CODE  -------------------------- */

#define GRID_WIDTH  16
#define GRID_HEIGHT 10
#define GRID_SEG_PER_TILE 2
#define TILE_SIZE 0.305 /* Width and height of a single tile.*/

bool obstacleArray[GRID_WIDTH * GRID_SEG_PER_TILE][GRID_HEIGHT * GRID_SEG_PER_TILE] = { false };


double totalWidth = 4.88;
double totalHeight = 3.05;
int iterationDepth = 5;

double robotDiameter = 0.305;

int main(void){
    
    // Mark map edges as not safe to navigate.
    for(int i = 0; i < ( GRID_WIDTH * GRID_SEG_PER_TILE ); i++) {
        for(int j = 0; j < ( GRID_HEIGHT * GRID_SEG_PER_TILE ); j++) {

            if (
                    ( j == 0 ) ||
                    ( i == 0 ) ||
                    ( i == ( GRID_WIDTH * GRID_SEG_PER_TILE ) - 1 ) ||
                    ( j == ( GRID_HEIGHT * GRID_SEG_PER_TILE ) - 1 )
                )
                {
                    obstacleArray[i][j] = true;
                }
        }
    } 
    

    int tiles_to_cover = (int)roundf((TILE_SIZE/2 + robotDiameter/2)/TILE_SIZE) * 2;
    //printf("Tiles to cover: %d \n", tiles_to_cover);
    // Mark Obsticles
    for(int i = 0; i < num_obstacles; i++)
    {
        int aprox_x = float_point_to_grid_coords(obstacle[i][0]);
        int aprox_y = float_point_to_grid_coords(obstacle[i][1]);

        obstacleArray[aprox_x][aprox_y] = true;

        for(int k = aprox_x - tiles_to_cover; k <= aprox_x + tiles_to_cover; k++){
            for(int l = aprox_y - tiles_to_cover; l <= aprox_y + tiles_to_cover; l++){
                if ((k >= 0) && (k < ( GRID_WIDTH * GRID_SEG_PER_TILE)) && (l >= 0) && (l < ( GRID_HEIGHT * GRID_SEG_PER_TILE)) )
                    obstacleArray[k][l] = true;
            }
        }

        //printf("Actual Location(%lf, %lf)\n", obstacle[i][0], obstacle[i][1] );
        //printf("Approximate Grid Coordinate: (%d, %d)\n", aprox_x, aprox_y );

    }
    print_map();
    return 0;
}

void print_map(){
    printf("\n X ->\tStart Coords: (%d, %d)", float_point_to_grid_coords(start[0]), float_point_to_grid_coords(start[1]) );
    printf("\n O ->\tGoal Coords: (%d, %d)", float_point_to_grid_coords(goal[0]), float_point_to_grid_coords(goal[1]) );
    printf("\n + ->\tOccupied Cell\n\n");

    for(int j = (GRID_HEIGHT * GRID_SEG_PER_TILE) - 1; j >= 0 ; j--) {
        for(int i = 0; i < (GRID_WIDTH * GRID_SEG_PER_TILE); i++) {
            if ( i == float_point_to_grid_coords(start[0]) && j == float_point_to_grid_coords(start[1])){
                printf("X ");
            }
            else if ( i == float_point_to_grid_coords(goal[0]) && j == float_point_to_grid_coords(goal[1])){
                printf("O ");
            }
            else
                printf("%c ", obstacleArray[i][j] ? '+' : ' ');
        }
        printf("\n");
    } 
}

int float_point_to_grid_coords(float point){
        return (int)roundf(point/TILE_SIZE) * GRID_SEG_PER_TILE;
}