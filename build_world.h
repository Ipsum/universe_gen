#ifndef __BUILD_WORLD_H__
#define __BUILD_WORLD_H__


#define UNIVERSE_SIZE 10
#define MAX_EXITS 5

#define SUCCESS 0
#define ERROR 1

struct Node {
    int id;
    int n_exits;
    int filled;
    struct Node *exits[MAX_EXITS];
};

typedef struct Node Node_t;

typedef struct {
    int size;
    char *name;
    Node_t *nodes[UNIVERSE_SIZE];
} Universe_t;

Universe_t* build_universe(void);
void print_universe(Universe_t *universe);
Node_t* choose_random(Universe_t* universe);
int add_node(Universe_t* universe, Node_t* new_node);
int pair_nodes(Node_t* source, Node_t* destination);
Node_t* build_node(int id);

#endif
