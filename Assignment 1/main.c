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

int** enqueue(int** queue, int element[2], int* length);
int** dequeue(int** queue, int* length);
bool BFS(int start_pos[2], int goal_pos[2], int** coordinates_visited);
bool coordinates_check(int** coordinate_list, int* len, int coordinate[2]);
bool is_goal(int c_x, int c_y, int g_x, int g_y);
int** get_moves(int current_pos[2], int* num_moves);
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
    int i, j = 0;
    // Mark map edges as not safe to navigate.
    for(i = 0; i < ( GRID_WIDTH * GRID_SEG_PER_TILE ); i++) {
        for(j = 0; j < ( GRID_HEIGHT * GRID_SEG_PER_TILE ); j++) {

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
    for(i = 0; i < num_obstacles; i++)
    {
        int k,l = 0;
        int aprox_x = float_point_to_grid_coords(obstacle[i][0]);
        int aprox_y = float_point_to_grid_coords(obstacle[i][1]);

        obstacleArray[aprox_x][aprox_y] = true;

        for(k = aprox_x - tiles_to_cover; k <= aprox_x + tiles_to_cover; k++){
            for(l = aprox_y - tiles_to_cover; l <= aprox_y + tiles_to_cover; l++){
                if ((k >= 0) && (k < ( GRID_WIDTH * GRID_SEG_PER_TILE)) && (l >= 0) && (l < ( GRID_HEIGHT * GRID_SEG_PER_TILE)) )
                    obstacleArray[k][l] = true;
            }
        }

        //printf("Actual Location(%lf, %lf)\n", obstacle[i][0], obstacle[i][1] );
        //printf("Approximate Grid Coordinate: (%d, %d)\n", aprox_x, aprox_y );

    }

    int starting_cords[2] = {float_point_to_grid_coords(start[0]), float_point_to_grid_coords(start[1])};
    int goal_cords[2] = {float_point_to_grid_coords(goal[0]), float_point_to_grid_coords(goal[1])};
    int coordinates_visited[GRID_WIDTH * GRID_SEG_PER_TILE * GRID_HEIGHT][2];
    int length = 0;

    if(BFS(starting_cords, goal_cords, coordinates_visited))
        printf("Solution found");
    else
        printf("No Solution");

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

int** enqueue(int** queue, int element[2], int* length){
    (*length)++;
    int** temp = realloc(queue, sizeof(int*) * (*length));
    if (temp==NULL) {
        printf("Error allocating memory");
    }
    else
    {
        queue = temp;
    }

    queue[(*length) - 1] = element;
    return queue;
}
int** dequeue(int** queue, int* length){
    int i = 0;
    for(i = 1; i < (*length); i++)
    {
        queue[i - 1] = queue[i];
    }

    (*length)--;

    queue = realloc(queue, sizeof(int*) * (*length));
    return queue;
}
bool BFS(int start_pos[2], int goal_pos[2], int** coordinates_visited){
    // If starting position is the goal, return true;
    if(is_goal(start_pos[0], start_pos[1], goal_pos[0], goal_pos[1]))
        return true;
    // Initialize queue, queue_length, and visted_length
    int q_length = 1, v_length = 0;
    int** queue = malloc(sizeof(int*) * q_length);

    // Set the initial queue element equal to the start nodes
    queue[q_length - 1] = start_pos;
    int num_moves, i = 0;


    // TODO
    // If this runs/works on the NXT then we have to keep track of the paths
    // Essentially this means enqueuing int* arrays that contain the next possible move
    // Currently this will simply enqueue the next node, but you will need to malloc a suitable array and load the path
    // Reference: http://stackoverflow.com/questions/8922060/how-to-trace-the-path-in-a-breadth-first-search


    // Loop till the queue is empty
    while(q_length != 0){
        // Dequeue the first element
        int* head = queue[0];
        queue = dequeue(queue, &q_length);
        // Add the element to the visited list
        coordinates_visited[v_length] = head;

        // Get all the possible moves from the tile currently being looked at
        int** moves = get_moves(head, &num_moves);
        for(i = 0; i < num_moves; i++)
        {
            // If the tile is free
            if(!coordinates_check(coordinates_visited,&v_length, moves[i]))
            {
                // If the tile is the goal node then return true
                if(is_goal(moves[i][0], moves[i][1], goal[0], goal[1]))
                {
                    return true;
                }
                // If the tile is free then enqueue that move
                queue = enqueue(queue, moves[i], &q_length);
            }

        }
        num_moves = 0;
    }
    // All tiles traversed return false, no path
    return false;
}
int** get_moves(int current_pos[2], int* num_moves)
{
    int** moves = malloc(sizeof(int*) * 4);

    if(obstacleArray[current_pos[0]-1][current_pos[1]] == false) {
        moves[(*num_moves)] = malloc(sizeof(int) * 2);
        moves[(*num_moves)][0] = current_pos[0] - 1;
        moves[(*num_moves)][1] = current_pos[1];
        (*num_moves)++;
    }
    if(obstacleArray[current_pos[0]][current_pos[1] - 1] == false) {
        moves[(*num_moves)] = malloc(sizeof(int) * 2);
        moves[(*num_moves)][0] = current_pos[0];
        moves[(*num_moves)][1] = current_pos[1] - 1;
        (*num_moves)++;
    }
    if(obstacleArray[current_pos[0] + 1][current_pos[1]] == false) {
        moves[(*num_moves)] = malloc(sizeof(int) * 2);
        moves[(*num_moves)][0] = current_pos[0] + 1;
        moves[(*num_moves)][1] = current_pos[1];
        (*num_moves)++;
    }
    if(obstacleArray[current_pos[0]][current_pos[1] + 1] == false) {
        moves[(*num_moves)] = malloc(sizeof(int) * 2);
        moves[(*num_moves)][0] = current_pos[0];
        moves[(*num_moves)][1] = current_pos[1] + 1;
        (*num_moves)++;
    }

    return moves;
}
bool is_goal(int c_x, int c_y, int g_x, int g_y){
    return c_x == g_x && c_y == g_y;
}
bool coordinates_check(int** coordinate_list, int* len, int coordinate[2]){
    for(int i = 0; i < *len; i++)
    {
        if(coordinate_list[i][0] == coordinate[0] && coordinate_list[i][1] == coordinate[1]){
            return true;
        }
    }
    return false;
}


