
void RenderRectangle(v2 min, v2 max, v3 col) {
    glColor3f(col.r, col.g, col.b);
    glBegin(GL_QUADS);
    glVertex2f(min.x, min.y);
    glVertex2f(max.x, min.y);
    glVertex2f(max.x, max.y);
    glVertex2f(min.x, max.y);
    glEnd();
}

void RenderLine(v2 pos, v2 dir) {
    dir = dir * 0.5f;
    glColor3f(0,0,0);
    glBegin(GL_LINES);
    
    glVertex2f(pos.x + dir.x, pos.y);
    glVertex2f(pos.x, pos.y);
    
    glVertex2f(pos.x, pos.y + dir.y);
    glVertex2f(pos.x, pos.y);
    
    glEnd();
}

void RenderParticle(v2 min, v2 max, v4 col) {
    glColor4f(col.r, col.g, col.b, col.a);
    glBegin(GL_QUADS);
    glVertex2f(min.x, min.y);
    glVertex2f(max.x, min.y);
    glVertex2f(max.x, max.y);
    glVertex2f(min.x, max.y);
    glEnd();
}

void RenderRectCenter(v2 pos, v2 rad, v3 col) {
    RenderRectangle(pos - rad, pos + rad, col);
}

void RenderSnakePart(v2 pos, v2 rad, v2 dir, v2 dir_back, v3 col) {
    RenderRectCenter(pos, rad, { 1.0f, 1.0f, 1.0f });
    if(len(dir_back) <= 1.0f) {
        RenderLine(pos, dir);
        RenderLine(pos, dir_back * -1.0f);
    }
}

void RenderMap() {
    v3 col = { 1.0f, 1.0f, 1.0f };
    f32 width  = MAP_WIDTH  * 0.5f;
    f32 height = MAP_HEIGHT * 0.5f;
    RenderRectCenter({ -1.0f, height - 0.5f }, { 0.5f, height  + 1.0f },  col);
    RenderRectCenter({ MAP_WIDTH, height    }, { 0.5f, height  + 0.5f },  col);
    RenderRectCenter({ width, -1.0f         }, { width + 0.5f, + 0.5f },  col);
    RenderRectCenter({ width, MAP_HEIGHT    }, { width + 0.5f, + 0.5f },  col);
    static v3 cols[2] = {
        {0.5f, 0.5f, 0.5f},
        {0.7f, 0.7f, 0.7f}
    };
    for(int y = 0; y < MAP_HEIGHT; y++) {
        for(int x = 0; x < MAP_WIDTH; x++) {
            RenderRectangle({ x - 0.5f, y - 0.5f }, { x + 0.5f, y + 0.5f }, cols[(x + y) % 2]);
        }
    }
}

void RenderSnake(GameState* gs) {
    Entity* e = &gs->player;
    f32 float_val = ClampMin(gs->dt_turn / gs->move_dt, 0.0f);
    if(!gs->game_running) float_val = 0.0f;
    for(int i = 1; i < e->count; i++) {
        v2 dir =  V2(V2i(e->pos[i-1]) - V2i(e->pos[i]));
        v2 dir_back = dir * -1.0f;
        if(i < e->count-1) dir_back = V2(V2i(e->pos[i]) - V2i(e->pos[i+1]));
        
        if(len(dir) <= 1.0f) {
            RenderSnakePart(V2(e->pos[i]) + dir * float_val, TILE_RAD, dir, dir_back, { 1.0f, 1.0f, 1.0f });
        }
        else {
            RenderRectCenter(V2(e->pos[i]), TILE_RAD, { 1.0f, 1.0f, 1.0f });
            RenderRectCenter(V2(e->pos[i-1]), TILE_RAD, { 1.0f, 1.0f, 1.0f });
        }
    }
    RenderRectCenter(V2(e->pos[0]) + V2(e->dir) * float_val, TILE_RAD, { 1.0f, 0.0f, 0.0f });
}

void Render(GameState* gs) {
    RenderMap();
    if(gs->player.count > 0) {
        RenderParticles(&gs->ps);
        if(!gs->game_running || gs->visual_mode == VISUAL_MODE_REGULAR) RenderSnake(gs);
        RenderRectCenter(V2(gs->apple), TILE_RAD, { 0.0f, 1.0f, 0.0f });
    }
}

