#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <memory.h>

#define MAP_SIZE (6 +2)
#define MAX_MOVE 4
#define MAX_DEPTH 256

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
    int car_index;
    car state;
    char direction; // -1 or +1
} step;
step *solution;

void delete_map(int **map)
{
    for (int i=0; i<MAP_SIZE; i++)
        free(map[i]);
    free(map);
}

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
        {
            if(i == 0 || i == MAP_SIZE-1 || k == 0 || k == MAP_SIZE-1)
                map[i][k] = -2;
            else
                map[i][k] = -1;
        }
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
            else if (map[x][y] == -2)
                printf("+ ");
            else
                printf("%c ", map[x][y] + 'a');
        }
        printf("\n");
    }
}

void print_state(car *cars)
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
}

int check_move_car(car *inspect_car, int direction, int **map)
{

    int to_x = 0, to_y = 0;
    int *to_ptr;

    if (inspect_car->orientation == 0)
    {
        to_ptr = &to_x;
    }
    else
        to_ptr = &to_y;

    if (direction > 0)
        *to_ptr = inspect_car->size + direction - 1;
    else
        *to_ptr = direction;

    if (map[inspect_car->x_pos + to_x][inspect_car->y_pos + to_y] == -1)
    {
        if (inspect_car->orientation == 0)
            inspect_car->x_pos += direction;
        else
            inspect_car->y_pos += direction;
        return 1;
    }
    return 0;
}

void move_car(car *inspect_car, int direction)
{
    if (inspect_car->orientation == 0)
        inspect_car->x_pos += direction;
    else
        inspect_car->y_pos += direction;
}

int solve(car *this_state, int depth)
{
    if (this_state[special_index].x_pos == (MAP_SIZE-1) - this_state[special_index].size)
        return 1;
    if (depth < 1)
        return 0;

    int **map = make_map(this_state);
    car *next_state = malloc(c_count * sizeof(car));
    memcpy(next_state, this_state, c_count * sizeof(car));

    for (int i=0; i < c_count; i++)
    {
        int direction;
        for (direction = 1; direction <= MAX_MOVE; direction++)
        {
            if (check_move_car(&next_state[i], direction, map))
            {
                if (solve(next_state, depth - 1))
                {
                    solution[depth -1].car_index = i;
                    solution[depth -1].state = this_state[i];
                    solution[depth -1].direction = direction;
                    free(next_state);
                    return 1;
                }
                move_car(&next_state[i], direction * (-1));
            } else
                break;
        }
        for (direction = -1; direction >= -(MAX_MOVE); direction--)
        {
            if (check_move_car(&next_state[i], direction, map))
            {
                if (solve(next_state, depth - 1))
                {
                    solution[depth -1].car_index = i;
                    solution[depth -1].state = this_state[i];
                    solution[depth -1].direction = direction;
                    free(next_state);
                    return 1;
                }
                move_car(&next_state[i], direction * (-1));
            } else
                break;
        }
    }
    delete_map(map);
    free(next_state);
    return 0;
}

car *prioritize(car *this_state)
{
    car *new_state = calloc(c_count, sizeof(car));




    free(this_state);
    return new_state;
}

int main(int argc, char **argv)
{
    car *init_state = load_cars(argc, argv);
    print_state(init_state);

    int **map = make_map(init_state);
    print_map(map);
    delete_map(map);

    solution = malloc(MAX_DEPTH * sizeof(step));
    int search_depth;
    for (search_depth = 0; search_depth < MAX_DEPTH; search_depth++)
    {
        printf("depth: %d\n", search_depth);
        if(solve(init_state, search_depth))
        {
            printf("found solution\n");
            break;
        }
    }
    int max_depth = --search_depth;
    for(; search_depth >= 0; search_depth--)
    {
        printf("%d: %c %s %d\n", max_depth - search_depth +1, solution[search_depth].car_index + 'a', solution[search_depth].state.orientation == 0 ? "horizontal" : "vertical", solution[search_depth].direction);
    }

    free(init_state);
    free(solution);
    return 0;
}
