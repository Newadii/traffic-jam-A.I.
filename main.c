#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <memory.h>

#define MAP_SIZE 6

int c_count;
unsigned char special_index;

typedef struct CAR {
    unsigned char special; // 1 = special
    unsigned char size; // 2 or 3
    unsigned char x_pos;
    unsigned char y_pos;
    unsigned char orientation; // 0 = horizontal, 1 = vertical
} car;

typedef struct STEP
{
    car* car_;
    char direction; // -1 or +1
} step;

typedef struct STATE {
    car *cars;
    step step_;
} state;

car *load_cars(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "no config file!\n");
        exit(1);
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp)
        exit(1);
    fscanf(fp, "%d", &c_count);
    car *cars = (car *) malloc(c_count * sizeof(struct CAR));
    int in[5];
    for (int i=0; i < c_count; i++)
    {
        fscanf(fp, "%d %d %d %d %d", &in[0], &in[1], &in[2], &in[3], &in[4]);

        cars[i].special = in[0];
        cars[i].size = in[1];
        cars[i].x_pos = in[2];
        cars[i].y_pos = in[3];
        cars[i].orientation = in[4];

        if(cars[i].special)
            special_index = i;
    }

    fclose(fp);
    return cars;
}

int **make_map(car *cars)
{
    int **map = malloc(MAP_SIZE * sizeof(int **));
    for (int i=0; i<MAP_SIZE; i++)
    {
        map[i] = malloc(MAP_SIZE * sizeof(int));
        for (int k=0; k<MAP_SIZE; k++)
            map[i][k] = -1;
    }

    for (int i=0; i < c_count; i++)
    {
        int x = cars[i].x_pos;
        int y = cars[i].y_pos;
        for (int k=0; k < cars[i].size; k++)
        {
            map[x][y] = i;
            if (cars[i].orientation)
                y++;
            else
                x++;
        }
    }
    return map;
}

void print_map(int **map)
{
    printf("map:\n");
    for (int y=0; y<MAP_SIZE; y++)
    {
        for (int x=0; x<MAP_SIZE; x++)
        {
            if (map[x][y] == -1)
                printf("- ");
            else
                printf("%c ", map[x][y] + 'a');
        }
        printf("\n");
    }
    printf("*****\n");
}

void print_cars(car *cars)
{
    printf("count: %d\n", c_count);
    for (int i=0; i<c_count; i++)
    {
        printf("%d: ", i);
        printf("%d ", cars[i].special);
        printf("%d ", cars[i].size);
        printf("%d ", cars[i].x_pos);
        printf("%d ", cars[i].y_pos);
        printf("%d\n", cars[i].orientation);
    }
    printf("*****\n");
}

int solve(car *old_cars)
{
    car *new_cars = malloc(c_count * sizeof(car));
    memcpy(new_cars, old_cars, c_count * sizeof(car));
    if (new_cars[special_index].x_pos == (MAP_SIZE-1) - new_cars[special_index].size)
        return 0;

}

int main(int argc, char **argv)
{
    car *cars = load_cars(argc, argv);
    print_cars(cars);

    int **map = make_map(cars);
    print_map(map);


    return 0;
}
