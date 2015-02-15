#ifndef __BUILD_WORLD_H__
#define __BUILD_WORLD_H__


#define UNIVERSE_SIZE 1
#define MAX_PLANETS 2

#define MAX_RADIUS 10000
#define MAX_AZIMUTH M_PI/8

#define SUCCESS 0
#define ERROR 1

typedef enum {
    RED,
    ORANGE,
    YELLOW,
    YELLOW_WHITE,
    WHITE,
    BLUE_WHITE,
    BLUE
} Color_t;

typedef enum {
    ATM_NONE,
    ATM_STANDARD
} Atmosphere_t;

typedef struct {
    int temperature; //Kelvin
    Color_t color;
    int mass;
    long radius; //meters
} Star_t;

typedef struct {
    int mass; //Yottagrams
    int orbit_radius; //megameters
    int period; //megameters/hour
    int temperature; //C
    int albedo;
    int absorption;
    int greenhouse; 
    Atmosphere_t atmosphere;
    int seed;
} Planet_t;

typedef struct {
    int id;
    double radius;
    double angle;
    double azimuth;
    int n_planets;
    Planet_t *planets[MAX_PLANETS];
    Star_t *star;
} Node_t;

typedef struct {
    int size;
    char *name;
    Node_t *nodes[UNIVERSE_SIZE];
} Universe_t;


int build_links(Universe_t* universe);
Universe_t* build_universe(void);
void print_universe(Universe_t *universe);
Node_t* choose_random(Universe_t* universe);
int add_node(Universe_t* universe, Node_t* new_node);
int pair_nodes(Node_t* source, Node_t* destination);
Node_t* build_node(int id);
Node_t* choose_close(Universe_t* universe, Node_t* node);
int is_pair(Node_t* node1, Node_t* node2);
double find_distance(Node_t* node1, Node_t* node2);
int is_full(Node_t* node);
#endif
