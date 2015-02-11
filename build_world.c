#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "build_world.h"

int main(int argc, char *argv[])
{
    Universe_t *universe;
    
    srand(time(NULL)); 
    universe = build_universe();
    
    print_universe(universe);

    return SUCCESS;
}
    
Universe_t* build_universe(void)
{
    Universe_t *universe;
    Node_t *new_node;

    universe = calloc(1, sizeof(Universe_t));
    while(universe->size < UNIVERSE_SIZE)
    {
        new_node = build_node(universe->size);
        add_node(universe,new_node);
    }

   return universe;
}

void print_universe(Universe_t *universe)
{
    int i,j;
    for(i=0; i<universe->size; i++)
    {
        printf("\n===NODE %d===\nExits(%d):\n",universe->nodes[i]->id,universe->nodes[i]->n_exits);
        if(!universe->nodes[i]->exits[0])
        {
            printf("WARNING: ORPHAN\n");
            continue;
        }
        for(j=0; j<universe->nodes[i]->filled; j++)
        {
            printf("--ID: %d\n", universe->nodes[i]->exits[j]->id);
        }
        printf("======\n\n");
    }
}

Node_t* build_node(int id)
{
    int i;

    Node_t *new_node;
    new_node = calloc(1, sizeof(Node_t));
    new_node->id = id;
    new_node->n_exits = (rand() % 4) + 1; //between 1 and 5
    new_node->filled = 0;

    //Set all exits to NULL
    for(i=0; i<MAX_EXITS; i++)
    {
        new_node->exits[i]=NULL;
    }

    return new_node;
}

int add_node(Universe_t* universe, Node_t* new_node)
{
    Node_t *existing_node;

    existing_node = choose_random(universe);
    
    universe->nodes[universe->size] = new_node;
    universe->size++;

    if(!existing_node)
    {
        printf("WARNING: No empty nodes found, skipping\n");
        return SUCCESS;
    }
    pair_nodes(existing_node, new_node);
    return SUCCESS;
}

//Find random unfilled node
Node_t* choose_random(Universe_t* universe)
{
    int random;
    int i = 0;
    int j;
    
    if(universe->size == 0)
        return NULL;
    random = rand() % universe->size;
    for(random; i < universe->size; i++)
    {
        if( (random+i) >= universe->size)
            random = 0;
        
        for(j=0; j < universe->nodes[random+i]->n_exits; j++)
        {
            if(universe->nodes[random+i]->exits[j] == NULL)
            {
                return universe->nodes[random+i];
            }
        }
    }
    return NULL;
}

int pair_nodes(Node_t* source, Node_t* destination)
{
    int i, j;
    //Find open slot in source
    for(i=0;i<MAX_EXITS;i++)
    {
        //Error if full
        if(i==source->n_exits)
            return ERROR;

        if(source->exits[i] == NULL)
        {
            //Find open slot in destination
            for(j=0;j<MAX_EXITS;j++)
            {
                //Error if full
                if(j==destination->n_exits)
                    return ERROR;

                if(destination->exits[j] == NULL)
                {
                    //printf("Pairing ID %d [%d] with ID %d [%d]\n",source->id,i,destination->id,j); 
                    destination->exits[j] = source;
                    source->exits[i] = destination;
                    source->filled++;
                    destination->filled++;
                    return SUCCESS;
                }
            }
        }
    }

    return ERROR;
}
