#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

struct node {
    struct node* leafs;
    struct node* parent;

    int* coords;

    int distance;
    int depth;

    bool val;
    bool valid;
};
struct node generate_search_tree(int start_node[2], int current_node[2], int goal[2], int depth, int max_depth, bool map[32][20], int** coordinates_visted, int* len);bool coordinates_check(int** coordinate_list, int* len, int coordinate[2]);
int count_tree(struct node root, int count);
void print_memory_usage(struct node root);
void print_searched_map(int** coordinates_visted, int len);
struct node* UCS(struct node root);
void enqueue(struct node** queue, struct node* root, int* len);
struct node dequeue(struct node** queue, int* len);
void print_tree(struct node root, int starting_point[2]);
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

    int starting_cords[2] = {float_point_to_grid_coords(start[0]), float_point_to_grid_coords(start[1])};
    int goal_cords[2] = {float_point_to_grid_coords(goal[0]), float_point_to_grid_coords(goal[1])};
    int** coordinates_visited = malloc(sizeof(double*) * 760);
    int length = 0;
    //142
    struct node root = generate_search_tree(starting_cords, starting_cords, goal_cords, 0, 5001, obstacleArray, coordinates_visited, &length);
//    print_searched_map(coordinates_visited, length);
//    print_tree(root, goal_cords);
    UCS(root);
    print_searched_map(coordinates_visited, length);
    return 0;
}
void print_tree(struct node root, int goal[2]){
    root.distance = abs(root.coords[0] - goal[0]) + abs(root.coords[1] - goal[1]);
    printf("Distance: %d\n", root.distance);

    for(int i = 0; i < 4; i++){
        if(root.leafs[i].valid)
        {
            print_tree(root.leafs[i], goal);
        }
    }
}

void enqueue(struct node** queue, struct node* root, int* len){
    (*len)++;
    realloc(queue, sizeof(struct node) * (*len));
    queue[(*len) - 1] = root;
}
struct node dequeue(struct node** queue, int* len){
    (*len)--;
    struct node val = *queue[*len];
    realloc(queue, sizeof(struct node*) * (*len));

    return val;
}

void print_memory_usage(struct node root)
{
    int struct_size = sizeof(root.valid) + sizeof(root.val) + (sizeof(root.coords) * 2) + sizeof(root.depth) + (sizeof(root.leafs) * 4);
    printf("Search Tree Memory Usage: %d", struct_size * count_tree(root, 0));
}

int count_tree(struct node root, int count){
    count++;
    for(int i = 0; i < 4; i++)
    {
        if(root.leafs[i].valid)
        {
            count = count_tree(root.leafs[i], count);
        }
    }
    return count;

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
void print_searched_map(int** coordinates_visted, int len){
    printf("\n X ->\tStart Coords: (%d, %d)", float_point_to_grid_coords(start[0]), float_point_to_grid_coords(start[1]) );
    printf("\n O ->\tGoal Coords: (%d, %d)", float_point_to_grid_coords(goal[0]), float_point_to_grid_coords(goal[1]) );
    printf("\n + ->\tOccupied Cell\n\n");

    for(int j = (GRID_HEIGHT * GRID_SEG_PER_TILE) - 1; j >= 0 ; j--) {
        for(int i = 0; i < (GRID_WIDTH * GRID_SEG_PER_TILE); i++) {
            int node[2] = {i, j};
            if ( i == float_point_to_grid_coords(start[0]) && j == float_point_to_grid_coords(start[1])){
                printf("X ");
            }
            else if ( i == float_point_to_grid_coords(goal[0]) && j == float_point_to_grid_coords(goal[1])) {
                printf("O ");
            }
            else if (coordinates_check(coordinates_visted, &len, node)){
                printf("- ");
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

struct node** insert_priority_queue(struct node** queue, struct node* element, int length){
    realloc(queue, sizeof(struct node*) * length + 1);

    for(int i = 0; i < length; i++){
        if(queue[i]->distance >= element->distance){
            for(int j = i; j < length; j++)
            {
                queue[j+1] = queue[j];
            }
            queue[i] = element;
            return queue;
        }
    }
    queue[length] = element;
    return queue;
}
struct node* UCS(struct node root){
    int length = 1;
    struct node** queue = malloc(sizeof(struct node*) * length);

    queue[0] = &root;
    queue = insert_priority_queue(queue, &root.leafs[0], length);

    printf("%d\n", queue[0]->distance);
    printf("%d\n", queue[1]->distance);
}

struct node generate_search_tree(int start_node[2], int current_node[2], int goal[2], int depth, int max_depth, bool map[32][20], int** coordinates_visted, int* len){


    struct node root;
    root.val = map[current_node[0]][current_node[1]];
    root.leafs = malloc(sizeof(struct node) * 4);
    root.valid = true;

    root.coords = malloc(sizeof(int) * 2);
    root.coords[0] = current_node[0];
    root.coords[1] = current_node[1];




    root.distance = 99999;
    coordinates_visted[*len] = malloc(sizeof(int) * 2);
    coordinates_visted[*len][0] = current_node[0];
    coordinates_visted[*len][1] = current_node[1];
    (*len)++;


        if(current_node[0] - 1 >= 0 && map[current_node[0] - 1][current_node[1]] == 0)
        {
            int new_node[2] = {current_node[0] - 1, current_node[1]};

            if(!coordinates_check(coordinates_visted, len, new_node)) {
                struct node top = generate_search_tree(start_node, new_node, goal, depth + 1, max_depth, map, coordinates_visted, len);
                root.leafs[0] = top;
                top.parent = &root;
            }
            else
                root.leafs[0].valid = false;
        }
        else
            root.leafs[0].valid = false;
        if(current_node[0] + 1 <= 37 && map[current_node[0] + 1][current_node[1]] == 0)
        {
            int new_node[2] = {current_node[0] + 1, current_node[1]};
            if(!coordinates_check(coordinates_visted, len, new_node)) {

                struct node right = generate_search_tree(start_node, new_node, goal, depth + 1, max_depth, map, coordinates_visted, len);
                root.leafs[1] = right;
                right.parent = &root;

            }
            else
                root.leafs[1].valid = false;
        }
        else
            root.leafs[1].valid = false;
        if(current_node[1] - 1 >= 0 && map[current_node[0]][current_node[1] - 1] == 0)
        {
            int new_node[2] = {current_node[0], current_node[1] - 1};
            if(!coordinates_check(coordinates_visted, len, new_node)) {

                struct node left = generate_search_tree(start_node, new_node, goal, depth + 1, max_depth, map, coordinates_visted, len);
                root.leafs[2] = left;
                left.parent = &root;
            }
            else
                root.leafs[2].valid = false;
        }
        else
            root.leafs[2].valid = false;
        if(current_node[1] + 1 <= 37 && map[current_node[0]][current_node[1] + 1] == 0)
        {
            int new_node[2] = {current_node[0], current_node[1] + 1};
            if(!coordinates_check(coordinates_visted, len, new_node)) {

                struct node bottom = generate_search_tree(start_node, new_node, goal, depth + 1, max_depth, map, coordinates_visted, len);
                root.leafs[3] = bottom;
                bottom.parent = &root;

            }
            else
                root.leafs[3].valid = false;
        }
        else
            root.leafs[3].valid = false;

    return root;
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


