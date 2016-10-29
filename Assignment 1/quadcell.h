typedef struct {
    float x;
    float y;
} Point;

typedef struct {
    Point *value;
    PointNode *nextNode;
} PointNode;

typedef struct {
    Point *centerPoint;
    float width;
    float height;
    bool occupied;

    QuadTree *SWchild,
             *NWchild,
             *NEchild,
             *SEchild;

    PointNode occupyingPoint;

} QuadTree;

Point * new_point(float x, float y){
    Point *p;
    if((p = malloc(sizeof *p)) != NULL){
        p->x = x;
        p->y = y;
    }
}

QuadTree * new_quad(){
    QuadTree *q;

    if((q = malloc(sizeof *q)) != NULL){
        
    }
}


void free_point (point *p){
    free(p);
}