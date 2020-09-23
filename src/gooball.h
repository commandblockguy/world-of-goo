#ifndef GOO_GOOBALL_H
#define GOO_GOOBALL_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_NUM_GOOBALLS 100

#define MAX_CONNECTIONS 8
#define MAX_NEW_CONNECTIONS 2
#define GOOBALL_RADIUS (5 * 256)
#define IDEAL_LENGTH (40 * 256)
#define CONNECTION_FORCE (7)

#define GRAVITY (256 / 4)
#define GROUND_Y (220 * 256)

typedef struct gooball gooball_t;
typedef struct connection connection_t;

struct connection {
    bool primary;
    gooball_t *gooball;
    uint24_t dist_squared;
};

struct gooball {
    int24_t position_x;
    int24_t position_y;
    int24_t velocity_x;
    int24_t velocity_y;
    uint8_t num_connections;
    connection_t connections[MAX_CONNECTIONS];
};

// todo: crawley bois

gooball_t* add_goo(int24_t x, int24_t y);
void remove_goo(gooball_t* gooball);

bool connect(gooball_t *goo1, gooball_t *goo2);
bool connect_distance(gooball_t *goo1, gooball_t *goo2, uint24_t dist_squared);
void disconnect(gooball_t *goo1, gooball_t *goo2);

void process_goo(gooball_t *gooball);
void apply_velocity(gooball_t *gooball);

extern gooball_t gooballs[MAX_NUM_GOOBALLS];
extern uint8_t num_gooballs;

#endif //GOO_GOOBALL_H
