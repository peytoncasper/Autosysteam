#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
int current_rev, neg_speed;
void user_1ms_isr_type2(void){ /* do nothing */ }
/*function to //display on brick*/
/*void disp(int row, char *str1, int val1, char *str2,int val2)
{
    //display_clear(0);
    //display_goto_xy(0, row);
    //display_string(str1);
    //display_int(val1, 0);
    //display_goto_xy(0, row+1);
    //display_string(str2);
    //display_int(val2, 0);
    //display_update();
}
/*void go_straight(int speed, int goal_rev)
{
    //nxt_motor_set_count(//nxt_PORT_A,0);
    //nxt_motor_set_count(//nxt_PORT_B,0);
    current_rev=//nxt_motor_get_count(//nxt_PORT_B);
    int revA=0,revB=0;
    int cntr=0;
    float diff;
    //int speedb=speed;
    //int speeda=speed;
    while(goal_rev+1!=current_rev)
        {

                    if(cntr==0)
                    {
                        //nxt_motor_set_speed(//nxt_PORT_A,speed,1);
                        cntr=1;
                        current_rev=//nxt_motor_get_count(//nxt_PORT_A);
                    }
                    else
                    {
                        //nxt_motor_set_speed(//nxt_PORT_B,speed,1);
                        cntr=0;
                        current_rev=//nxt_motor_get_count(//nxt_PORT_B);
                    }
                    revA=//nxt_motor_get_count(//nxt_PORT_A);
                    revB=//nxt_motor_get_count(//nxt_PORT_B);
                    //disp(4,"StraightA:",revA,"StraightB:",revB);


        }
    //disp(4,"StraightA:",revA,"StraightB:",revB);
    //nxt_motor_set_speed(//nxt_PORT_A,0, 1);
    //nxt_motor_set_speed(//nxt_PORT_B,0, 1);
}
/*function to turnn left by 'goal_rev' revolution count */
/*void turn_left(int speed, int goal_rev)
{
    //nxt_motor_set_count(//nxt_PORT_A,0);
    //nxt_motor_set_count(//nxt_PORT_B,0);
    neg_speed=speed*(-1);
    current_rev=//nxt_motor_get_count(//nxt_PORT_A);
        while(goal_rev!=current_rev)
        {
                    //nxt_motor_set_speed(//nxt_PORT_A,speed,1);
                    //nxt_motor_set_speed(//nxt_PORT_B,neg_speed,1);
                    current_rev=//nxt_motor_get_count(//nxt_PORT_A);
        }
    ////disp(4,"Left:",goal_rev);
    //nxt_motor_set_speed(//nxt_PORT_A,0, 1);
    //nxt_motor_set_speed(//nxt_PORT_B,0, 1);
}
void turn_right(int speed, int goal_rev)
{
    //nxt_motor_set_count(//nxt_PORT_A,0);
    //nxt_motor_set_count(//nxt_PORT_B,0);
    neg_speed=speed*(-1);
    current_rev=//nxt_motor_get_count(//nxt_PORT_B);
        while(goal_rev!=current_rev)
            {
            //nxt_motor_set_speed(//nxt_PORT_A,neg_speed,1);
            //nxt_motor_set_speed(//nxt_PORT_B,speed,1);
            current_rev= //nxt_motor_get_count(//nxt_PORT_B);
            }
    ////disp(4,"Right:",goal_rev);
    //nxt_motor_set_speed(//nxt_PORT_A,0, 1);
    //nxt_motor_set_speed(//nxt_PORT_B,0, 1);
}
void stop()
{
    //nxt_motor_set_speed(//nxt_PORT_A,0, 1);
    //nxt_motor_set_speed(//nxt_PORT_B,0, 1);
    TerminateTask();
}
*/
int manhattan_distance(int c_x, int c_y, int g_x, int g_y);
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
double start[2];// = {0.305, 1.219}; /* start location */
double goal[2];// = {3.658, 1.829}; /* goal location */
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
struct node {
    struct node* leafs;
    struct node* parent;
    int* coords;
    int distance;
    int depth;
    bool val;
    bool valid;
};

int main()
{
    start[0] = .305; start[1] = 1.219; goal[0] = 3.658; goal[1] = 1.829;
    // Mark map edges as not safe to navigate.
    /*for(int i = 0; i < ( GRID_WIDTH * GRID_SEG_PER_TILE ); i++) {
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
    }*/

    int i,j,r,c,x,y;
    int a[19][13] = {{0}};
    int yw;
    for(yw=0;yw<19;yw++)
    {
        a[yw][0]=99;
        a[yw][12]=99;
    }
    for(yw=0;yw<13;yw++)
    {
        a[0][yw]=99;
        a[18][yw]=99;
    }
    for(i=0;i<num_obstacles;i++)
    {
        x=ceil(obstacle[i][0]/0.305);
        y=ceil(obstacle[i][1]/0.305);
        r=x+1;
        c=y+1;
        a[r][c]=99;
    }
    int current_cords[2] = {float_point_to_grid_coords(start[0]), float_point_to_grid_coords(start[1])};
    int goal_cords[2] = {float_point_to_grid_coords(goal[0]), float_point_to_grid_coords(goal[1])};

    int queue[20][2];

    queue[0][0] = goal_cords[0];
    queue[0][1] = goal_cords[1];
    int len = 1;
    int count = 0;
    while(len > 0)
    {
        int head[2];
        head[0] = queue[0][0];
        head[1] = queue[0][1];

        for(i = 1; i < len; i++)
        {
            queue[i-1][0] = queue[i][0];
            queue[i-1][1] = queue[i][1];
        }
        len--;

        if(a[head[0] + 1][head[1]] == 0)
        {
            queue[len][0] = head[0] + 1;
            queue[len][1] = head[1];
            a[head[0] + 1][head[1]] = a[head[0]][head[1]] + 1;

            len++;
        }
        if(a[head[0] - 1][head[1]] == 0)
        {
            queue[len][0] = head[0] - 1;
            queue[len][1] = head[1];
            a[head[0] - 1][head[1]] = a[head[0]][head[1]] + 1;

            len++;
        }
        if(a[head[0]][head[1] + 1] == 0)
        {
            queue[len][0] = head[0];
            queue[len][1] = head[1] + 1;
            a[head[0]][head[1] + 1] = a[head[0]][head[1]] + 1;

            len++;
        }
        if(a[head[0]][head[1] - 1] == 0)
        {
            queue[len][0] = head[0];
            queue[len][1] = head[1] - 1;
            a[head[0]][head[1] - 1] = a[head[0]][head[1]] + 1;

            len++;
        }
    }
    a[goal_cords[0]][goal_cords[1]] = 0;



//    for(i = 0; i < 19; i++)
//    {
//        for(j = 0; j < 13; j++)
//        {
//            if(a[i][j] != 99)
//            {
//                a[i][j] = manhattan_distance(i, j, goal_cords[0], goal_cords[1]);
//            }
//        }
//    }

    /*for(i=0;i<19;i++)
{
    for(j=0;j<13;j++)
    {
        printf("%d   \t",a[i][j]);
    }
    printf("\n");
    }*/
    a[0][0] = -1;
    for(j=12;j>= 0;j--)
    {
        for(i=0;i<19;i++)
        {
            printf("%d   \t",a[i][j]);
        }
        printf("\n");
    }
    int w,e;
    current_cords[0] = 1;
    current_cords[1] = 5;
    w=current_cords[0];
    e=current_cords[1];
    //printf("%d  %d",w,e);
    while(a[current_cords[0]][current_cords[1]] != 0){
        int min_distance = 100;

        if(a[current_cords[0]+1][current_cords[1]]<min_distance && a[current_cords[0]+1][current_cords[1]] != 99)
        {
            min_distance=a[w+1][e];
            w=current_cords[0]+1;
        }
        if(a[current_cords[0]-1][current_cords[1]]<min_distance && a[current_cords[0]-1][current_cords[1]] != 99)
        {
            min_distance=a[w-1][e];
            w=current_cords[0]-1;
        }
        if(a[current_cords[0]][current_cords[1]-1]<min_distance && a[current_cords[0]][current_cords[1]-1] != 99)
        {
            min_distance=a[w][e-1];
            e=current_cords[1]-1;
        }
        if(a[current_cords[0]][current_cords[1]+1]<min_distance && a[current_cords[0]][current_cords[1]+1] != 99)
        {
            min_distance=a[w][e+1];
            e=current_cords[1]+1;
        }

        //go(current_cords[0], current_cords[1], w, e);
        current_cords[0] = w;
        current_cords[1] = e;
        printf("%d \t %d\n",w,e);

        min_distance = 100;
    }

    /*int length = 0;
    int current_orientation = 2;
    int previous[2] = {-1,-1};
    while(current_cords[0] != goal_cords[0] || current_cords[1] != goal_cords[1]){
    int distances[4] = {99999,99999,99999,99999};
    if(a[current_cords[0] -1][current_cords[1]] != 1
        && (current_cords[0] - 1 != previous[0]
        || current_cords[1] != previous[1]))
        distances[0] = manhatten_distance(current_cords[0] - 1, current_cords[0], goal_cords[0], goal_cords[1]);
        if(a[current_cords[0]][current_cords[1] - 1] != 1
        && (current_cords[0] != previous[0]
        || current_cords[1] - 1 != previous[1]))
        distances[1] = manhatten_distance(current_cords[0], current_cords[0] - 1, goal_cords[0], goal_cords[1]);
        if(a[current_cords[0] + 1][current_cords[1]] != 1
        && (current_cords[0] + 1 != previous[0]
        || current_cords[1] != previous[1]))
        distances[2] = manhatten_distance(current_cords[0] + 1, current_cords[0], goal_cords[0], goal_cords[1]);
        if(a[current_cords[0]][current_cords[1] + 1] != 1
        && (current_cords[0] != previous[0]
        || current_cords[1] + 1 != previous[1]))
        distances[3] = manhatten_distance(current_cords[0], current_cords[0] + 1, goal_cords[0], goal_cords[1]);
    int min = distances[0];
    int i, direction = 0;
    for(i = 0; i < 4; i++)
    {
        if(distances[i] < min)
        {
            min = distances[i];
            direction = i;
        }
    }
    int diff = current_orientation - direction;
    if(diff > 0)
    {
        turn_right(40,190 * abs(diff));
    }
    else if(diff < 0)
    {
        turn_left(40,163 * abs(diff));
    }
    current_orientation = direction;
    //go_straight
    go_straight(40,720);
    previous[0] = current_cords[0];
    previous[1] = current_cords[1];
    if(direction == 0)
        current_cords[0]--;
    else if(direction == 1)
        current_cords[1]--;
    else if(direction == 2)
        current_cords[0]++;
    else if(direction == 3)
        current_cords[1]++;
    //display_clear(0);
    //display_goto_xy(0, 0);
    //display_string("Dist1:");
    //display_int(distances[0], 0);
    //display_goto_xy(0, 1);
    //display_string("Dist2:");
    //display_int(distances[1], 0);
    //display_goto_xy(0, 2);
    //display_string("Dist3:");
    //display_int(distances[2], 0);
    //display_goto_xy(0, 3);
    //display_string("Dist4:");
    //display_int(distances[3], 0);
    //display_goto_xy(0, 4);
    //display_string("Diff:");
    //display_int(diff, 0);
    //display_update();
    systick_wait_ms(5000);
    }*/

    return 0;
}
int manhattan_distance(int c_x, int c_y, int g_x, int g_y)
{
    return abs(c_x - g_x) + abs(c_y - g_y);
}
int float_point_to_grid_coords(float point){
    return (int)roundf(point/TILE_SIZE);
}
