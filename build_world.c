#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

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
    //build_links(universe);

   return universe;
}

void print_universe(Universe_t *universe)
{
    int i,j;
    for(i=0; i<universe->size; i++)
    {
        printf("\n===NODE %d===\n",universe->nodes[i]->id);
        printf("Radius=%lf,Angle=%lf,Azimuth=%lf\n",universe->nodes[i]->radius,
                                                    universe->nodes[i]->angle,
                                                    universe->nodes[i]->azimuth);
    }
}
#if 0
int build_links(Universe_t* universe)
{
    int i;
    Node_t* found_node;
    for(i=0;i<UNIVERSE_SIZE;i++)
    {
        found_node = choose_close(universe,universe->nodes[i]);
        if(!found_node)
        {
            printf("WARNING: No empty nodes found, skipping\n");
            continue;
        }
        if(pair_nodes(found_node, universe->nodes[i]))
        {
            printf("Couldn't pair nodes\n");
            return ERROR;
        }
    }

    return SUCCESS;
}
#endif
Node_t* build_node(int id)
{
    int i;

    Node_t *new_node;
    new_node = calloc(1, sizeof(Node_t));
    new_node->id = id;
    new_node->radius = rand() / (double)(RAND_MAX/MAX_RADIUS);
    new_node->angle =  rand() / (RAND_MAX/(2*M_PI));
    new_node->azimuth = rand() / (RAND_MAX/(MAX_AZIMUTH));

    if(rand() % 2)
        new_node->azimuth = (2*M_PI) - new_node->azimuth;
    
    build_star(new_node);
    build_planets(new_node);

    return new_node;
}

int build_star(Node_t* new_node)
{
    Star_t *new_star;
    new_star = calloc(1, sizeof(Star_t));
    new_star->color = rand() % 7;
    switch(new_star->color)
    {
        case RED:
            new_star->temperature = (rand() % 1300) + 2400;
            new_star->mass = (rand() % 37)  + 8;
            new_star->radius = (((rand() % 37)  + 8)/100) * 10^9;
            break;
        case ORANGE:
            new_star->temperature = (rand() % 2500) + 3700;
            new_star->mass = (rand() % 35)  + 45;
            new_star->radius = (((rand() % 35)  + 45)/100) * 10^9;
            break;
        case YELLOW:
            new_star->temperature = (rand() % 800) + 5200;
            new_star->mass = (rand() % 24)  + 80;
            new_star->radius = (((rand() % 24)  + 80)/100) * 10^9;
            break;
        case YELLOW_WHITE:
            new_star->temperature = (rand() % 1500) + 6000;
            new_star->mass = (rand() % 36)  + 104;
            new_star->radius = (((rand() % 36)  + 104)/100) * 10^9;
            break;
        case WHITE:
            new_star->temperature = (rand() % 2500) + 7500;
            new_star->mass = (rand() % 70)  + 140;
            new_star->radius = (((rand() % 70)  + 140)/100) * 10^9;
            break;
        case BLUE_WHITE:
            new_star->temperature = (rand() % 20000) + 10000;
            new_star->mass = (rand() % 1390)  + 210;
            new_star->radius = (((rand() % 1390)  + 210)/100) * 10^9;
            break;
        case BLUE:
            new_star->temperature = (rand() % 30000) + 30000;
            new_star->mass = (rand() % 2000)  + 1390;
            new_star->radius = (((rand() % 2000)  + 1390)/100) * 10^9;
    }

    new_node->star = new_star;
    return SUCCESS;
}

int build_planets(Node_t* new_node)
{
   int i;
   int prev_radius = 0;
   Planet_t *new_planet;

   for(i=0; i < new_node->n_planets; i++)
   {
       new_planet = NULL;
       new_planet->mass = (rand() % 1000) + 1;
       new_planet->orbit_radius = prev_radius + (rand() % 1000000) + 100000;
       prev_radius = new_planet->orbit_radius;
       new_planet->atmosphere = rand() % 2;
       //Calculate orbital speed
       // G = 6.67 x 10^19 Mm^2N/Yg^2
       // T^2 = (R^3 * 4 * R_PI^2) / (G * new_star->mass)
       // T = SQRT((R^3 * 4 * R_PI^2) / (G * new_star->mass))
       // T = SQRT(R^3 * 4 * R_PI^2) / SQRT(G) * SQRT(MASS)
       // T = ( SQRT(R^3) * SQRT(4) * SQRT(R_PI^2) ) / ( SQRT(G) * SQRT(MASS) )
       // T = ( SQRT(R^3) * 2 * R_PI ) / (8.167*10^9) * SQRT(MASS) )
       new_planet->period = (sqrt(new_planet->orbit_radius^3) * 2 * M_PI) / 
                            (8.167*pow(10.0,9.0) * sqrt(new_planet->mass));
       if(new_planet->atmosphere)
       {
           new_planet->albedo = rand() % 100;
           new_planet->absorption = rand() % 100;
           new_planet->greenhouse = (rand() % 25) + 20;
       }
       else
       {
           new_planet->albedo = 0; 
           new_planet->absorption = 0;
           new_planet->greenhouse = 0;
       }
       //Calculate surface temp
       //T = TG + Tsun SQRT( rsun SQRT(1 - ALPHA - BETA / 2) / 2d )
       new_planet->temperature = new_planet->greenhouse + new_node->star->temperature * 
                                 sqrt(new_node->star->radius *
                                 sqrt(1 - new_planet->albedo - new_planet->absorption / 2) / 
                                 (1.4*sqrt(new_planet->orbit_radius)*pow(10.0,3.0)));
   
       new_planet->seed = rand();
       new_node->planets[i] = new_planet;
   }
   return SUCCESS;
}
int add_node(Universe_t* universe, Node_t* new_node)
{
    universe->nodes[universe->size] = new_node;
    universe->size++;
    
    return SUCCESS;
}

double find_distance(Node_t* node1, Node_t* node2)
{
    if(node1->angle >= node2->angle)
        return tan(node1->angle - node2->angle) * node1->radius;
    return tan(node2->angle - node1->angle) * node1->radius;
}
#if 0
Node_t* choose_close(Universe_t* universe, Node_t* node)
{
    Node_t* closest = NULL;
    double shortest_distance = 0;
    double distance;
    int i;
    for(i=0; i< universe->size; i++)
    {
        if( !is_pair(universe->nodes[i],node) && !is_full(universe->nodes[i]))
        {
            distance = find_distance(node, universe->nodes[i]);
            if(distance < shortest_distance)
            {
                shortest_distance = distance;
                closest = universe->nodes[i];
            }
        }
    }

    return closest;
}
#endif
#if 0
int is_full(Node_t* node)
{
    if(node->filled >= node->n_exits)
        return 1;
    return 0;
}

int is_pair(Node_t* node1, Node_t* node2)
{
    int i;
    for(i=0;i<node1->filled;i++)
    {
        if(node1->exits[i]->id == node2->id)
            return 1;
    }

    return 0;
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
                    printf("Pairing ID %d [%d] with ID %d [%d]\n",source->id,i,destination->id,j); 
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
#endif
