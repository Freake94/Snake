#ifndef GAME_H
#define GAME_H

#define MAP_WIDTH  16
#define MAP_HEIGHT 16
#define ENTITY_MAX (MAP_HEIGHT * MAP_WIDTH)

#define TILE_RAD {0.5f, 0.5f}

struct Entity {
    v2u pos[ENTITY_MAX];
    i32 count;
    v2i dir;
};

struct PathFindingQueue {
    b32 apple_search;
    i32 current;
    i32 count;
    v2u target;
    v2i dir[MAP_WIDTH * MAP_HEIGHT];
};

enum VisualMode : i32 {
    VISUAL_MODE_PARTICLES,
    VISUAL_MODE_REGULAR,
    VISUAL_MODE_COUNT
};

struct GameState {
    b32 game_running;
    b32 game_over;
    f32 dt;
    f32 dt_turn;
    f32 move_dt;
    i32 visual_mode;
    v2u apple;
    Entity player;
    ParticleSystem ps;
    PathFindingQueue pf;
};

#endif //GAME_H
