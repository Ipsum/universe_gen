#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "universe.h"

static void dump_json(const char *filename, Universe_t *universe)
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
        fprintf(f," },");
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
                fprintf(f,"  },");
        }
        if(j>0)
            fprintf(f,"  }");
        fprintf(f,"\n");
        fprintf(f,"  ]");
        fprintf(f,"\n");
        if( (i+1) != universe->size )
            fprintf(f,"},");
    }
    if(i>0)
        fprintf(f,"}");
    fprintf(f,"\n");
    fprintf(f,"]");
    fprintf(f,"\n");
    fprintf(f,"}");
}

int main(int argc, char *argv[])
{
    Universe_t *universe;
    
    if(argc != 2)
    {
        printf("enter filename or - for output");
        exit(1);
    }
    UniverseConfig_t config = UNIVERSE_CONFIG_DEFAULT_INITIALIZER_T;
    universe = build_universe("json_universe", time(NULL), &config);
    
#ifdef DEBUG 
    print_universe(universe);
#endif
    dump_json(argv[1],universe);
    return SUCCESS;
}
