#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "build_world.h"

int main(int argc, char *argv[])
{
    Universe_t *universe;
    
    if(argc != 2)
    {
        printf("enter filename or - for output");
        exit(1);
    }
    srandom(time(NULL)); 
    universe = build_universe();
    
#ifdef DEBUG 
    print_universe(universe);
#endif
    dump_json(argv[1],universe);
    return SUCCESS;
}
    
Universe_t* build_universe(void)
{
    Universe_t *universe;
    Node_t *new_node;
    char *name = calloc(1,15);
    strncpy(name,"hillsverse",15);
    
    universe = calloc(1, sizeof(Universe_t));
    universe->name = name;
    while(universe->size < UNIVERSE_SIZE)
    {
        new_node = build_node(universe->size);
        add_node(universe,new_node);
    }
    //build_links(universe);

   return universe;
}

void dump_json(const char *filename, Universe_t *universe)
{
    FILE *f;
    Node_t *node;
    Planet_t *planet;
    int i,j;

    if(!strcmp(filename,"-"))
        f = stdout;
    else
        f = fopen(filename,"w");
    
    fprintf(f,"\{");
    fprintf(f,"\n");
    fprintf(f,"\"version\":%d,",1);
    fprintf(f,"\n");
    fprintf(f,"\"size\":%d,",universe->size);
    fprintf(f,"\n");
    fprintf(f,"\"name\":\"%s\",",universe->name);
    fprintf(f,"\n");
    
    fprintf(f,"\"nodes\":\[");
    for(i=0;i<universe->size;i++)
    {
        node = universe->nodes[i];
        fprintf(f,"\{");
        fprintf(f,"\n");
        fprintf(f," \"id\":%d,",node->id);
        fprintf(f,"\n");
        fprintf(f," \"n_planets\":%d,",node->n_planets);
        fprintf(f,"\n");
        fprintf(f," \"radius\":%lf,",node->radius);
        fprintf(f,"\n");
        fprintf(f," \"angle\":%lf,",node->angle);
        fprintf(f,"\n");
        fprintf(f," \"azimuth\":%lf,",node->azimuth);
        fprintf(f,"\n");

        fprintf(f," \"star\":\{");
        fprintf(f,"\n");
        fprintf(f,"  \"temperature\":%d,",node->star->temperature);
        fprintf(f,"\n");
        fprintf(f,"  \"color\":%d,",node->star->color);
        fprintf(f,"\n");
        fprintf(f,"  \"mass\":%d,",node->star->mass);
        fprintf(f,"\n");
        fprintf(f,"  \"radius\":%ld",node->star->radius);
        fprintf(f,"\n");
        fprintf(f," \},");
        fprintf(f,"\n");

        fprintf(f," \"planets\":\[");
        for(j=0;j<node->n_planets;j++)
        {
            planet = node->planets[j];
            fprintf(f," \{");
            fprintf(f,"\n");
            fprintf(f,"   \"mass\":%d,",planet->mass);
            fprintf(f,"\n");
            fprintf(f,"   \"orbit_radius\":%d,",planet->orbit_radius);
            fprintf(f,"\n");
            fprintf(f,"   \"period\":%ld,",planet->period);
            fprintf(f,"\n");
            fprintf(f,"   \"temperature\":%lf,",planet->temperature);
            fprintf(f,"\n");
            fprintf(f,"   \"albedo\":%d,",planet->albedo);
            fprintf(f,"\n");
            fprintf(f,"   \"absorption\":%d,",planet->absorption);
            fprintf(f,"\n");
            fprintf(f,"   \"greenhouse\":%d,",planet->greenhouse);
            fprintf(f,"\n");
            fprintf(f,"   \"atmosphere\":%d,",planet->atmosphere);
            fprintf(f,"\n");
            fprintf(f,"   \"seed\":%d",planet->seed);
            fprintf(f,"\n");
            if( !((j+1) == node->n_planets) )
                fprintf(f,"  \},");
        }
        if(j>0)
            fprintf(f,"  \}");
        fprintf(f,"\n");
        fprintf(f,"  \]");
        fprintf(f,"\n");
        if( (i+1) != universe->size )
            fprintf(f,"\},");
    }
    if(i>0)
        fprintf(f,"\}");
    fprintf(f,"\n");
    fprintf(f,"\]");
    fprintf(f,"\n");
    fprintf(f,"\}");
}


    
void print_universe(Universe_t *universe)
{
    Planet_t *planet;
    int i,j;
    for(i=0; i<universe->size; i++)
    {
        printf("\n===NODE %d===\n",universe->nodes[i]->id);
        printf("Radius=%lf,Angle=%lf,Azimuth=%lf\n",universe->nodes[i]->radius,
                                                    universe->nodes[i]->angle,
                                                    universe->nodes[i]->azimuth);
        printf("Star:\n-temp: %d (K)\n-color: %d (RED-BLUE)\n-mass: %d(SUNS*100)\n-radius: %ld (SUNS*100)\n",
                universe->nodes[i]->star->temperature,
                universe->nodes[i]->star->color,
                universe->nodes[i]->star->mass,
                universe->nodes[i]->star->radius);
        printf("n_planets= %d\n",universe->nodes[i]->n_planets);
        for(j=0; j<universe->nodes[i]->n_planets; j++)
        {
            planet = universe->nodes[i]->planets[j];
            printf("Planet %d\n",j);
            printf("-mass: %d (Yg)\n",planet->mass);
            printf("-orbit_radius: %d (Mm)\n",planet->orbit_radius);
            printf("-period: %ld (s)\n",planet->period);
            printf("-temperature: %lf (K)\n",planet->temperature);
            printf("-albedo: %d \n",planet->albedo);
            printf("-absorption: %d \n",planet->absorption);
            printf("-greenhouse: %d (Constant)\n",planet->greenhouse);
            printf("-atmosphere: %d (NONE,NORMAL)\n",planet->atmosphere);
            printf("-seed: %d\n",planet->seed);
        }
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
    new_node->radius = random() / (double)(RAND_MAX/MAX_RADIUS);
    new_node->angle =  random() / (RAND_MAX/(2*M_PI));
    new_node->azimuth = random() / (RAND_MAX/(MAX_AZIMUTH));
    new_node->n_planets = random() % MAX_PLANETS;
    if(random() % 2)
        new_node->azimuth = (2*M_PI) - new_node->azimuth;
    
    build_star(new_node);
    build_planets(new_node);

    return new_node;
}

int build_star(Node_t* new_node)
{
    Star_t *new_star;
    new_star = calloc(1, sizeof(Star_t));
    new_star->color = random() % 7;
    switch(new_star->color)
    {
        case RED:
            new_star->temperature = (random() % 1300) + 2400;
            new_star->mass = (random() % 37)  + 8;
            new_star->radius = (((random() % 37)  + 8)/100) * 10^9;
            break;
        case ORANGE:
            new_star->temperature = (random() % 2500) + 3700;
            new_star->mass = (random() % 35)  + 45;
            new_star->radius = (((random() % 35)  + 45)/100) * 10^9;
            break;
        case YELLOW:
            new_star->temperature = (random() % 800) + 5200;
            new_star->mass = (random() % 24)  + 80;
            new_star->radius = (((random() % 24)  + 80)/100) * 10^9;
            break;
        case YELLOW_WHITE:
            new_star->temperature = (random() % 1500) + 6000;
            new_star->mass = (random() % 36)  + 104;
            new_star->radius = (((random() % 36)  + 104)/100) * 10^9;
            break;
        case WHITE:
            new_star->temperature = (random() % 2500) + 7500;
            new_star->mass = (random() % 70)  + 140;
            new_star->radius = (((random() % 70)  + 140)/100) * 10^9;
            break;
        case BLUE_WHITE:
            new_star->temperature = (random() % 20000) + 10000;
            new_star->mass = (random() % 1390)  + 210;
            new_star->radius = (((random() % 1390)  + 210)/100) * 10^9;
            break;
        case BLUE:
            new_star->temperature = (random() % 30000) + 30000;
            new_star->mass = (random() % 2000)  + 1390;
            new_star->radius = (((random() % 2000)  + 1390)/100) * 10^9;
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
       new_planet = calloc(1, sizeof(Planet_t));
       new_planet->mass = (random() % 1000) + 10;
       new_planet->orbit_radius = prev_radius + (random() % 1000000) + 100000;
       prev_radius = new_planet->orbit_radius;
       if((random() % 100) >= 40)
           new_planet->atmosphere = ATM_STANDARD;
       else
           new_planet->atmosphere = ATM_NONE;
       
       //Calculate orbital speed
       // G = 6.67 x 10^19 Mm^2N/Yg^2
       // T^2 = (R^3 * 4 * R_PI^2) / (G * new_star->mass)
       // T = SQRT((R^3 * 4 * R_PI^2) / (G * new_star->mass))
       // T = SQRT(R^3 * 4 * R_PI^2) / SQRT(G) * SQRT(MASS)
       // T = ( SQRT(R^3) * SQRT(4) * SQRT(R_PI^2) ) / ( SQRT(G) * SQRT(MASS) )
       // T = ( SQRT(R^3) * 2 * R_PI ) / (8.167*10^9) * SQRT(MASS) )
       
       long double mass_s = new_node->star->mass/100.0 * 1.989*pow(10,30);
       long double radius_e = new_planet->orbit_radius * pow(10,6);
       long double r = sqrt(pow(radius_e,3));
       long double mass = sqrt(mass_s);
       new_planet->period = ( r*2*M_PI )/( 8.167*pow(10,-6) * mass );

       if(new_planet->atmosphere == ATM_STANDARD)
       {
           new_planet->albedo = random() % 50;
           new_planet->absorption = random() % 50;
           new_planet->greenhouse = (random() % 25) + 20;
       }
       else
       {
           new_planet->albedo = 0; 
           new_planet->absorption = 0;
           new_planet->greenhouse = 0;
       }
       //Calculate surface temp
       //T = TG + Tsun SQRT( rsun SQRT(1 - ALPHA - BETA / 2) / 2d )
#if 0
       new_planet->albedo = 31;
       new_planet->absorption = 26;
       new_planet->greenhouse = 36;
       new_planet->orbit_radius = 149600;
       new_node->star->radius = 100;
       new_node->star->temperature=6000;
#endif  
       double ab = (1.0 - new_planet->albedo/100.0 - new_planet->absorption/100.0 );
       double d = ( new_planet->orbit_radius * pow(10,6) );
       double rsun = ( (double)new_node->star->radius/100.0 * 6.963 * powl(10,8) );

       new_planet->temperature = new_planet->greenhouse + new_node->star->temperature *
                                 sqrt( rsun * sqrt(ab/2.0) / (2.0*d));
       //new_planet->temperature = new_planet->greenhouse + new_node->star->temperature * 
       //                          sqrt((new_node->star->radius/100*pow(10.0,8.0)) *
       //                          sqrt(1 - new_planet->albedo - new_planet->absorption / 2) / 
       //                          (1.4*sqrt(new_planet->orbit_radius)*pow(10.0,3.0)));
   
       new_planet->seed = random();
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

//Find randomom unfilled node
Node_t* choose_randomom(Universe_t* universe)
{
    int randomom;
    int i = 0;
    int j;
    
    if(universe->size == 0)
        return NULL;

    randomom = random() % universe->size;
    for(randomom; i < universe->size; i++)
    {
        if( (randomom+i) >= universe->size)
            randomom = 0;
        
        for(j=0; j < universe->nodes[randomom+i]->n_exits; j++)
        {
            if(universe->nodes[randomom+i]->exits[j] == NULL)
            {
                return universe->nodes[randomom+i];
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
