#include <math.h>
#include "gooball.h"
#include "util.h"

#undef NDEBUG
#define DEBUG
#include <debug.h>
#include <string.h>

gooball_t gooballs[MAX_NUM_GOOBALLS];
uint8_t num_gooballs = 0;

gooball_t* add_goo(int24_t x, int24_t y) {
    gooball_t *gooball = &gooballs[num_gooballs];
    gooball->position_x = x;
    gooball->position_y = y;
    gooball->velocity_x = 0;
    gooball->velocity_y = 0;
    gooball->num_connections = 0;

    for(uint8_t i = 0; i < num_gooballs; i++) {
        gooball_t *other = &gooballs[i];
        uint24_t distance = distance_squared(gooball->position_x, gooball->position_y,
                                             other->position_x, other->position_y);
        if(distance < (IDEAL_LENGTH / 256 * 1.2) * (IDEAL_LENGTH / 256 * 1.2)) {
            connect_distance(gooball, other, distance);
        }
        // todo: sort by distance
        if(gooball->num_connections == MAX_NEW_CONNECTIONS) break;
    }

    num_gooballs++;

    return gooball;
}

void remove_goo(gooball_t *gooball) {
    // Delete connections
    while(gooball->num_connections) {
        gooball_t *other = gooball->connections[gooball->num_connections - 1].gooball;
        disconnect(gooball, other);
    }
    gooball_t *last = &gooballs[num_gooballs - 1];
    // Replace the gooball being deleted with the one from the end of the gooball list
    if(gooball != last) {
        memcpy(gooball, last, sizeof *gooball);
        // Update all of the connections on gooballs connected to the last element
        for(uint8_t i = 0; i < last->num_connections; i++) {
            gooball_t *other = last->connections[i].gooball;
            for(uint8_t j = 0; j < other->num_connections; j++) {
                if(other->connections[j].gooball == last) {
                    other->connections[j].gooball = gooball;
                }
            }
        }
    }
    num_gooballs--;
}

bool connect(gooball_t *goo1, gooball_t *goo2) {
    if(goo1->num_connections == MAX_CONNECTIONS ||
       goo2->num_connections == MAX_CONNECTIONS) return false;
    uint24_t distance = distance_squared(goo1->position_x, goo1->position_y,
                                         goo2->position_x, goo2->position_y);
    return connect_distance(goo1, goo2, distance);
}

bool connect_distance(gooball_t *goo1, gooball_t *goo2, uint24_t dist_squared) {
    if(goo1->num_connections == MAX_CONNECTIONS ||
       goo2->num_connections == MAX_CONNECTIONS) return false;

    connection_t *conn1 = &goo1->connections[goo1->num_connections++];
    conn1->gooball = goo2;
    conn1->dist_squared = dist_squared;
    conn1->primary = true;

    connection_t *conn2 = &goo2->connections[goo2->num_connections++];
    conn2->gooball = goo1;
    conn2->dist_squared = dist_squared;
    conn2->primary = false;
    return true;
}

void disconnect(gooball_t *goo1, gooball_t *goo2) {
    for(uint8_t i = 0; i < goo1->num_connections; i++) {
        if(goo1->connections[i].gooball == goo2) {
            // Replace with the last connection
            memmove(&goo1->connections[i], &goo1->connections[goo1->num_connections - 1], sizeof goo1->connections[i]);
            goo1->num_connections--;
            break;
        }
    }
}

void process_goo(gooball_t *gooball) {
    // todo: optimize
    gooball->velocity_y += GRAVITY;

    for(uint8_t i = 0; i < gooball->num_connections; i++) {
        connection_t *connection = &gooball->connections[i];
        // Only do physics calculations one time per connection
        if(!connection->primary) continue;
        gooball_t *other = connection->gooball;
        uint24_t ideal_length = connection->dist_squared;
        int24_t dx = (gooball->position_x - other->position_x) / 256;
        int24_t dy = (gooball->position_y - other->position_y) / 256;
        uint24_t length_squared = dx*dx + dy*dy;
        int24_t force = (ideal_length - length_squared) * CONNECTION_FORCE;
        int24_t vx = dx * force / 256;
        int24_t vy = dy * force / 256;
        gooball->velocity_x += vx;
        gooball->velocity_y += vy;
        other->velocity_x -= vx;
        other->velocity_y -= vy;
    }
}

void apply_velocity(gooball_t *gooball) {
    // todo: optimize?
    // Add drag, so that stuff doesn't just bounce around forever
    gooball->velocity_x = gooball->velocity_x * 240 / 256;
    gooball->velocity_y = gooball->velocity_y * 240 / 256;

    gooball->position_x += gooball->velocity_x;
    gooball->position_y += gooball->velocity_y;

    if(gooball->position_y + GOOBALL_RADIUS >= GROUND_Y) {
        gooball->velocity_y = 0;
        gooball->position_y = GROUND_Y - GOOBALL_RADIUS;
    }
}
