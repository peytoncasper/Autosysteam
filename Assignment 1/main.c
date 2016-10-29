#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define TILE_SIZE 0.305 /* Width and height of a single tile. Used to calculate intersections between tiles and quadcells*/

typedef struct {
    double x;
    double y;
} Point;

typedef struct PointNode PointNode;

struct PointNode{
    Point *point;
    PointNode *nextNode;

};

typedef struct QuadTreeNode QuadTreeNode;
struct QuadTreeNode {
    Point *centerPoint;
    double width;
    double height;
    bool occupied;
    int depth;

    struct QuadTreeNode *SWchild,
             *NWchild,
             *NEchild,
             *SEchild;

    PointNode *occupyingPoints;

};


bool subdivide_quad(QuadTreeNode *q);
void print_tabs(int tab_count);

Point * new_point(double x, double y){
    Point *p;
    if((p = malloc(sizeof *p)) != NULL){
        p->x = x;
        p->y = y;
    }
    return p;
}

PointNode * new_pointnode(Point *p){
    PointNode *pN;
    if((pN = malloc(sizeof *pN)) != NULL){
        pN->point = p;
        pN->nextNode = NULL;
    }
    return pN;
}

/**
 *
 * Creates a new point node from a point and inserts it at the head of a given list.
 *
 */
PointNode *insert_point_into_list(PointNode *rootNode, Point *newPoint){

    if (newPoint == NULL)
        return rootNode;

    PointNode *newPointNode;

    if((newPointNode = malloc(sizeof *newPointNode)) != NULL){
        newPointNode->point = newPoint;
        newPointNode->nextNode = rootNode;
    }
    return newPointNode;
    
}

/**
 *
 * Utility functino to print a single point coordinates.
 *
 */


void print_point(Point *p, int tab_count){
    print_tabs(tab_count);
    printf("(%lf, %lf)", p->x, p->y);
}

/**
 *
 * Utility funtion to print a list of point coordinates. Used for testing and should be safe to remove if needed.
 *
 */
void print_pointlist(PointNode *pN, int tab_count){

    PointNode *pNThis = pN;
    int i;

    if(pNThis == NULL){
        print_tabs(tab_count);
        printf("Point List Contains 0 Points\n");
    }

    while (pNThis != NULL)
    {
        print_tabs(tab_count);
        print_point(pNThis->point, 0);
        printf("\n");
        pNThis = pNThis->nextNode;
    }
}

/**
 *
 * Checks if a Quad and an obsticle intersect
 *
 */
bool point_intersects_quad(QuadTreeNode *q, Point *p){
    bool x_intersection = abs(q->centerPoint->x - p->x) <= (q->width + TILE_SIZE)/2 ;
    bool y_intersection = abs(q->centerPoint->y - p->y) <= (q->height + TILE_SIZE)/2 ;

/*  Prints results which should not be valid. Something is wrong here.
    if(
        (abs((q->centerPoint->x - 3.431250) < 0.01)  && abs((p->x - 2.743) < 0.01) && abs((q->width - 0.1525) < 0.01)) &&
        (abs((q->centerPoint->y - 0.905469) < 0.01)  && abs((p->y - 0.91500) < 0.01) && abs((q->height - 0.095312) < 0.01))
    ){
        printf("Intersection between ");
        print_point(p, 0);
        printf(" and ");
        print_quad(q,0);
    }
*/
    return (x_intersection && y_intersection);
}


/**
 *
 * Generates a new quad tree node
 *
 */
QuadTreeNode *new_quad(Point *centerPoint, double width, double height, bool occupied, int depth, PointNode *occupyingPoints){
    QuadTreeNode *q;

    if((q = malloc(sizeof *q)) != NULL){
        q->centerPoint = centerPoint;
        q->width = width;
        q->height = height;
        q->occupied = occupied;
        q->depth = depth;
        q->occupyingPoints = occupyingPoints;
    }

    return q;
}


/**
 *
 * Utility function which prints out a quad for debugging.
 *
 */
void print_quad(QuadTreeNode *q, int tab_count){
    print_tabs(tab_count);
    printf("QUAD:");
    printf("\n");
    
    print_tabs(tab_count+1);
    printf("Center: ");
    print_point(q->centerPoint, 1);
    printf("\n");
    
    print_tabs(tab_count+1);
    printf("Width: -\t- %lf\n", q->width);
    
    print_tabs(tab_count+1);
    printf("Height: -\t- %lf\n", q->height);
    
    print_tabs(tab_count+1);
    printf("Occupied: -\t- %d\n", q->occupied);
    
    print_tabs(tab_count+1);
    printf("Iteration Depth Remaining: %d\n", q->depth);
    
    print_tabs(tab_count+1);
    printf("occupyingPoints:\n");
    print_pointlist(q->occupyingPoints, tab_count+2);
    printf("\n\n");
}

/**
 *
 * Recursive print quad
 *
 */

void recurs_print_quad(QuadTreeNode *q, int tab_count){
    print_tabs(tab_count);
    printf("QUAD:");
    printf("\n");
    
    print_tabs(tab_count+1);
    printf("Center: ");
    print_point(q->centerPoint, 1);
    printf("\n");
    
    print_tabs(tab_count+1);
    printf("Width: -\t- %lf\n", q->width);
    
    print_tabs(tab_count+1);
    printf("Height: -\t- %lf\n", q->height);
    
    print_tabs(tab_count+1);
    printf("Occupied: -\t- %d\n", q->occupied);
    
    print_tabs(tab_count+1);
    printf("Iteration Depth Remaining: %d\n", q->depth);
    
    print_tabs(tab_count+1);
    printf("occupyingPoints:\n");
    print_pointlist(q->occupyingPoints, tab_count+2);
    printf("\n\n");

    if((q->SWchild) != NULL){
        print_tabs(tab_count+1);
        printf("SWchild:");
        recurs_print_quad(q->SWchild, tab_count+1);
    }

    if((q->NWchild) != NULL){
        print_tabs(tab_count+1);
        printf("NWchild:");
        recurs_print_quad(q->NWchild, tab_count+1);
    }

    if((q->NEchild) != NULL){
        print_tabs(tab_count+1);
        printf("NEchild:");
        recurs_print_quad(q->NEchild, tab_count+1);
    }

    if((q->SEchild) != NULL){
        print_tabs(tab_count+1);
        printf("SEchild:");
        recurs_print_quad(q->SEchild, tab_count+1);
    }
}

void print_tabs(int tab_count){
    int i;

    for (i=0; i<tab_count; i++) {
        printf("\t");
    }
}

/**
 *
 * Checks through a list of points for intersection with the provided quad. If the quad is found to be occupied it will be marked and subdivided.
 *
 */
bool digest_point_list(QuadTreeNode *q, PointNode *pN){
    
    //printf("\n");
    //printf("Checking for intersection between:\n");
    //print_quad(q,0);
    //printf("\n");
    
    PointNode *checkList = pN;

    while (checkList != NULL){
        bool intersectsQuad = false;
        //printf("and point:");
        //print_point(checkList->point, 0);
        intersectsQuad = point_intersects_quad(q, checkList->point);
        //printf("  \tRESULT: \t%s", intersectsQuad ? "true" : "false");
        if(intersectsQuad){
            q->occupyingPoints = insert_point_into_list(q->occupyingPoints, checkList->point);
            q->occupied = true;
        }
        checkList = checkList->nextNode;
        //printf("\n");
    }

    if (q->occupied){
        //printf("QUAD IS OCCUPIED!\n");
        //printf("\n");
        //printf("\n");
        subdivide_quad(q);
        return true;
    }
    else {
        //printf("QUAD IS NOT OCCUPIED!\n");
        //printf("\n");
        //printf("\n");
        return false;
    }

}

/**
 *
 * Subdivides a quad cell into 4 sub cells. The points list of the origial quad is then digested by the children.
 *
 */
bool subdivide_quad(QuadTreeNode *q){
    int i=0;

    //for (i=0; i<q->depth; i++) {
    //    printf("\t");
    //}
    //printf("Subdividing quad:\n");
    //print_quad(q, q->depth);

    if ((q->depth > 0) && (q->occupied)){
        q->SWchild = new_quad(new_point(q->centerPoint->x - q->width/4, q->centerPoint->y - q->height/4), q->width/2, q->height/2, false, q->depth-1, NULL);
        //printf("SWchild quad:\n");
        //print_quad(q->SWchild, q->depth+1);

        q->NWchild = new_quad(new_point(q->centerPoint->x - q->width/4, q->centerPoint->y + q->height/4), q->width/2, q->height/2, false, q->depth-1, NULL);
        //printf("NWchild quad:\n");
        //print_quad(q->NWchild, q->depth+1);
    
        q->NEchild = new_quad(new_point(q->centerPoint->x + q->width/4, q->centerPoint->y + q->height/4), q->width/2, q->height/2, false, q->depth-1, NULL);
        //printf("NEchild quad:\n");
        //print_quad(q->NEchild, q->depth+1);

        q->SEchild = new_quad(new_point(q->centerPoint->x + q->width/4, q->centerPoint->y - q->height/4), q->width/2, q->height/2, false, q->depth-1, NULL);
        //printf("SEchild quad:\n");
        //print_quad(q->SEchild, q->depth+1);


        digest_point_list(q->SEchild, q->occupyingPoints);
        digest_point_list(q->NEchild, q->occupyingPoints);
        digest_point_list(q->NWchild, q->occupyingPoints);
        digest_point_list(q->SWchild, q->occupyingPoints);
    }
}


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

/* ------------------------ END OF PROVIDED CODE  -------------------------- */

double totalWidth = 4.88;
double totalHeight = 3.05;
int iterationDepth = 5;

int main(void){

    int i = 0;

    QuadTreeNode *rootQuad;
    PointNode *obstacleList = NULL;

    rootQuad = new_quad(new_point(totalWidth/2, totalHeight/2), totalWidth, totalHeight, false, iterationDepth, NULL);

    printf("Starting point: (%f, %f)\n", start[0], start[1]);
    printf("goal point: (%f, %f)\n", goal[0], goal[1]);

    Point *startPoint = new_point(start[0], start[1]);
    Point *goalPoint = new_point(goal[0], goal[1]);

    Point *obstaclePoint;

    for(i = 0; i < sizeof(obstacle) / sizeof(obstacle[0]); i++)
    {
        obstaclePoint = new_point(obstacle[i][0],obstacle[i][1]);
        obstacleList = insert_point_into_list(obstacleList, obstaclePoint);
    }

    //printf("Printing obstacle list:\n");
    //print_pointlist(obstacleList,0);

    digest_point_list(rootQuad, obstacleList);
    
    printf("AFTER POINT DIGESTION:\n");
    recurs_print_quad(rootQuad, 0);

    return 0;
}