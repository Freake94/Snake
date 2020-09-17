#include <math.h>

b32 active_pathfinding;

i32 record_score;

static v2i direction[] = {
    { 1, 0 },
    {-1, 0 },
    { 0, 1 },
    { 0,-1 },
};

b32 PathFinding(GameState* gs, v2u target) {
    Entity* e = &gs->player;
    u32 map[MAP_HEIGHT][MAP_WIDTH] = {};
    v2i relations[MAP_HEIGHT][MAP_WIDTH];
    for(int i = 0; i < e->count; i++) {
        map[e->pos[i].y][e->pos[i].x] = e->count - i + 1;
        if(target == e->pos[i]) {
            return false;
        }
    }
    
    for(int y = 0; y < MAP_HEIGHT; y++) {
        for(int x = 0; x < MAP_WIDTH; x++) {
            relations[y][x] = {-1, -1};
        }
    }
    v2i pos[MAP_WIDTH * MAP_HEIGHT] = {};
    pos[0] = V2i(e->pos[0]);
    i32 count = 1;
    b32 target_accessible = false;
    b32 target_found = false;
    i32 iteration = 0;
    while(count > 0) {
        iteration++;
        i32 new_count = 0;
        v2i new_pos[MAP_WIDTH * MAP_HEIGHT] = {};
        for(int i = 0; i < count; i++) {
            for(int dir = 0; dir < 4; dir++) {
                v2i potential_pos = pos[i] + direction[dir];
                if(potential_pos.x >= MAP_WIDTH) potential_pos.x = 0;
                if(potential_pos.x < 0) potential_pos.x = MAP_WIDTH - 1;
                if(potential_pos.y >= MAP_HEIGHT) potential_pos.y = 0;
                if(potential_pos.y < 0) potential_pos.y = MAP_HEIGHT - 1;
                
                if(map[potential_pos.y][potential_pos.x] <= iteration) {
                    new_pos[new_count++] = potential_pos;
                    if(relations[potential_pos.y][potential_pos.x] == v2i{ -1, -1 }) {
                        relations[potential_pos.y][potential_pos.x] = pos[i];
                    }
                    map[potential_pos.y][potential_pos.x] = e->count + iteration;
                }
            }
        }
        count = new_count;
        
        for(int i = 0; i < count; i++) {
            pos[i] = new_pos[i];
            if(V2u(pos[i]) == target) {
                target_found = true;
                for(int dir = 0; dir < 4; dir++) {
                    v2i potential_pos = V2i(target) + direction[dir];
                    if(potential_pos.x >= MAP_WIDTH) potential_pos.x = 0;
                    if(potential_pos.x < 0) potential_pos.x = MAP_WIDTH - 1;
                    if(potential_pos.y >= MAP_HEIGHT) potential_pos.y = 0;
                    if(potential_pos.y < 0) potential_pos.y = MAP_HEIGHT - 1;
                    
                    if(map[potential_pos.y][potential_pos.x] <= iteration) {
                        target_accessible = true;
                        break;
                    }
                }
            }
        }
        if(target_found) break;
    }
    gs->pf.current = 0;
    if(target_accessible) {
        gs->pf.apple_search = true;
        gs->pf.target = target;
        v2i final_path[MAP_WIDTH * MAP_HEIGHT];
        final_path[0] = V2i(e->pos[0]);
        final_path[iteration] = V2i(target);
        for(int i = iteration - 1; i > 0; i--) {
            final_path[i] = relations[final_path[i+1].y][final_path[i+1].x];
        }
        for(int i = 0; i < iteration; i++) {
            gs->pf.dir[i] = final_path[i+1] - final_path[i];
            if(len(gs->pf.dir[i]) > 1.0f) {
                gs->pf.dir[i] = (gs->pf.dir[i] / (i32)(len(gs->pf.dir[i]))) * -1;
            }
        }
        gs->pf.count = iteration-1;
        return true;
    }
    return false;
}

void SpawnApple(GameState* gs) {
    v2 min = { 0, 0 };
    v2 max = { MAP_WIDTH, MAP_HEIGHT };
    b32 valid = false;
    Entity* e = &gs->player;
    while(!valid && e->count < ENTITY_MAX) {
        v2u new_pos = v2uRand(V2u(min), V2u(max));
        valid = true;
        for(int i = 0; i < e->count; i++) {
            if(e->pos[i] == new_pos) valid = false;
        }
        if(valid) gs->apple = new_pos;
    }
}

b32 CollisionCheck(Entity* e) {
    for(int i = 1; i < e->count; i++) {
        if(e->pos[i] == e->pos[0]) {
            if(record_score < e->count) {
                record_score = e->count;
                printf("NEW RECORD ");
            }
            printf("--- %d ---\n", record_score);
            printf("Collide with part %d of %d\n", i, e->count);
            return true;
        }
    }
    return false;
}

b32 self_steering = false;
static v2i current_dir;

void HeadTailUpdate(GameState* gs) {
    Entity* e = &gs->player;
    for(int i = e->count-1; i > 0; i--) {
        e->pos[i] = e->pos[i-1];
    }
    e->pos[0].x = u32(e->pos[0].x + e->dir.x) % MAP_WIDTH;
    e->pos[0].y = u32(e->pos[0].y + e->dir.y) % MAP_HEIGHT;
    if(CollisionCheck(&gs->player)) {
        gs->game_running = false;
        gs->game_over    = true;
    }
    if(e->pos[0] == gs->apple) {
        e->pos[e->count++] = e->pos[0];
        printf("%d\n", e->count);
        SpawnApple(gs);
    }
    current_dir = e->dir;
}

void EntityUpdate(GameState* gs) {
    gs->dt_turn += gs->dt;
    Entity* e = &gs->player;
    
    static v2i dir = {};
    if(key_pressed[GLFW_KEY_W] || key_pressed[GLFW_KEY_UP])    dir = { 0, 1 };
    if(key_pressed[GLFW_KEY_S] || key_pressed[GLFW_KEY_DOWN])  dir = { 0,-1 };
    if(key_pressed[GLFW_KEY_A] || key_pressed[GLFW_KEY_LEFT])  dir = {-1, 0 };
    if(key_pressed[GLFW_KEY_D] || key_pressed[GLFW_KEY_RIGHT]) dir = { 1, 0 };
    
    if(len(dir) > 0.0f) {
        if((dir * current_dir) == v2i{ 0, 0 }) {
            if(gs->dt_turn > 0.5f * gs->move_dt) {
                HeadTailUpdate(gs);
                gs->dt_turn = 0.0f;
            }
            e->dir = dir;
            dir = {};
        }
    }
    if(gs->dt_turn >= gs->move_dt) {
        gs->dt_turn -= gs->move_dt;
        if(active_pathfinding) {
            if(gs->pf.current >= gs->pf.count || !gs->pf.apple_search) {
                if(!PathFinding(gs, gs->apple) && gs->pf.apple_search) {
                    gs->pf.apple_search = false;
                    int i;
                    for(i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++) {
                        if(PathFinding(gs, v2uRand({ 0, 0 }, { MAP_WIDTH, MAP_HEIGHT }))) {
                            //printf("pos: {%d %d}\ntarget: {%d %d}", e->pos[0].x, e->pos[0].y, gs->pf.target.x, gs->pf.target.y);
                            break;
                        }
                    }
                }
            }
            e->dir = gs->pf.dir[gs->pf.current++];
        }
        HeadTailUpdate(gs);
    }
    if(gs->visual_mode == VISUAL_MODE_PARTICLES) {
        f32 float_val = ClampMin(gs->dt_turn / gs->move_dt, 0.0f);
        if(!gs->game_running) float_val = 0.0f;
        for(int i = 1; i < e->count; i++) {
            v2 dir_forward =  V2(V2i(e->pos[i-1]) - V2i(e->pos[i]));
            if(len(dir_forward) > 1.0f) dir_forward = {0,0};
            Particle p;
            p.life =  0.3f;
            p.pos = V2(e->pos[i]) + dir_forward * float_val + v2{ fRand(-0.5f, 0.5f), fRand(-0.5f, 0.5f) };
            v2 dir = v2{ (f32)e->dir.y, (f32)e->dir.x };
            //p.vel = (v2{ fRand(dir.x, -dir.x) + fRand(-0.3f, 0.3f), fRand(dir.y, -dir.y)  + fRand(-0.3f, 0.3f)}) * 5.0f;
            p.vel = {0,0};
            f32 col = fRand(0.3f, 0.9f);
            p.col = { col, 1.0f - (f32)i / e->count, 1.0f - (f32)i / e->count, 0.5f};
            AddParticle(&gs->ps, p);
        }
    }
    else if(gs->visual_mode == VISUAL_MODE_REGULAR) {
        for(int i = 0; i < e->count; i++) {
            Particle p;
            p.life =  0.3f;
            p.pos = V2(e->pos[i]) + v2{ fRand(-0.5f, 0.5f), fRand(-0.5f, 0.5f) };
            v2 dir = v2{ (f32)e->dir.y, (f32)e->dir.x };
            p.vel = (v2{ fRand(dir.x, -dir.x) + fRand(-0.3f, 0.3f), fRand(dir.y, -dir.y)  + fRand(-0.3f, 0.3f)}) * 5.0f;
            f32 col = fRand(0.3f, 0.9f);
            p.col = { col, col, col, 0.5f};
            AddParticle(&gs->ps, p);
        }
    }
}
void CameraUpdate() {
    i32 width;
    i32 height;
    glfwGetWindowSize(window, &width, &height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, width / f32(height), 1.0f, 80.0f);
    gluLookAt(MAP_WIDTH * 0.5f, MAP_HEIGHT * 0.5f, 32.0f,
              MAP_WIDTH * 0.5f, MAP_HEIGHT * 0.5f, 0.0f, 0.0f, 1.0f, 0.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glViewport(0, 0, width, height);
}

void GameInit(GameState* gs) {
    gs->pf = {};
    Entity* e = &gs->player;
    e->dir = { 1, 0 };
    e->count = 5;
    e->pos[0] = { 5, 5 };
    for(int i = 1; i < MAP_WIDTH * MAP_HEIGHT; i++) {
        gs->player.pos[i] = e->pos[0] - e->dir;
    }
    gs->dt_turn = 0.0f;
    SpawnApple(gs);
    glfwSetTime(0.0f);
    gs->game_running = true;
    gs->move_dt = 0.1f;
    gs->game_over = false;
}

void Run() {
    static GameState gs;
    active_pathfinding = false;
    record_score = 0;
    GameInit(&gs);
    gs.game_running = false;
    gs.visual_mode = VISUAL_MODE_REGULAR;
    while(!glfwWindowShouldClose(window)) {
        if(key_pressed[GLFW_KEY_F1]) {
            active_pathfinding = !active_pathfinding;
            if(active_pathfinding) gs.move_dt = 0.05f;
            else gs.move_dt = 0.2f;
        }
        if(key_pressed[GLFW_KEY_R]) {
            GameInit(&gs);
        }
        if(key_pressed[GLFW_KEY_M]) {
            if(++gs.visual_mode >= VISUAL_MODE_COUNT) gs.visual_mode = 0;
        }
        if(key_pressed[GLFW_KEY_SPACE]) {
            if(gs.game_over) GameInit(&gs);
            else gs.game_running = !gs.game_running;
            
        }
        gs.dt = glfwGetTime(); glfwSetTime(0.0f);
        
        if(gs.game_running){
            EntityUpdate(&gs);
        }
        CameraUpdate();
        glClear(GL_COLOR_BUFFER_BIT);
        Render(&gs);
        InputUpdate();
        UpdateParticles(&gs.ps, gs.dt);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}