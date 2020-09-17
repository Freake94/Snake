#ifndef TYPES_H
#define TYPES_H

#include <math.h>

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

using b32 = bool;

struct RndGen {
    u32    x;
    u32    y;
    u32    z;
};

inline u32 NextRandom(RndGen* g) {
    g->x  ^= g->x << 16;
    g->x  ^= g->x >> 5;
    g->x  ^= g->x << 1;
    
    u32 t = g->x;
    
    g->x  = g->y;
    g->y  = g->z;
    g->z  = t ^ g->x ^ g->y;
    
    return g->z;
}

thread_local RndGen default_rnd_gen = { 123456789u, 362436069u, 521288629u };

inline int iRand(int min, int max) {
    
    return min + NextRandom(&default_rnd_gen) % (max - min);
}

inline u32 uRand(u32 min, u32 max) {
    
    return min + NextRandom(&default_rnd_gen) % (max - min);
}

inline f32 fRand(f32 min, f32 max) {
    return min + ((f32)NextRandom(&default_rnd_gen) / (f32)0xFFFFFFFF) * (max - min);
}

struct v2 {
    f32 x;
    f32 y;
};

union v3 {
    struct {
        f32 x, y, z;
    };
    struct {
        f32 r, g, b;
    };
};

union v4 {
    struct{
        f32 x, y, z, w;
    };
    struct {
        f32 r, g, b, a;
    };
};

v2 operator+(v2 a, v2 b) { return v2 { a.x + b.x, a.y + b.y }; }
v2 operator-(v2 a, v2 b) { return v2 { a.x - b.x, a.y - b.y }; }

v2 operator+=(v2& a, v2 b) { a = a + b; return a; }
v2 operator-=(v2& a, v2 b) { a = a - b; return a; }

v2 operator*(v2 a, f32 f)  { return v2 { a.x * f, a.y * f }; }

v2 operator*(v2 a, v2 b)  { return v2 { a.x * b.x, a.y * b.y }; }

struct v2i {
    i32 x;
    i32 y;
};


v2i operator+(v2i a, v2i b) { return v2i { a.x + b.x, a.y + b.y }; }
v2i operator-(v2i a, v2i b) { return v2i { a.x - b.x, a.y - b.y }; }
v2i operator+=(v2i& a, v2i b) { a = a + b; return a; }
v2i operator-=(v2i& a, v2i b) { a = a - b; return a; }

v2i operator*(v2i a, i32 i)  { return v2i { a.x * i, a.y * i }; }
v2i operator/(v2i a, i32 i)  { return v2i { a.x / i, a.y / i }; }

v2i operator*(v2i a, v2i b)  { return v2i { a.x * b.x, a.y * b.y }; }

b32 operator==(v2i a, v2i b) { return (a.x == b.x && a.y == b.y); }
b32 operator!=(v2i a, v2i b) { return !(a.x == b.x && a.y == b.y); }



struct v2u {
    u32 x;
    u32 y;
};

v2u operator+(v2u a, v2i b) { return v2u { a.x + b.x, a.y + b.y }; }
v2u operator-(v2u a, v2i b) { return v2u { a.x - b.x, a.y - b.y }; }

v2u operator+(v2u a, v2u b) { return v2u { a.x + b.x, a.y + b.y }; }
v2u operator-(v2u a, v2u b) { return v2u { a.x - b.x, a.y - b.y }; }

v2u operator+=(v2u& a, v2u b) { a = a + b; return a; }
v2u operator-=(v2u& a, v2u b) { a = a - b; return a; }
b32 operator==(v2u a, v2u b) { return (a.x == b.x && a.y == b.y); }
b32 operator!=(v2u a, v2u b) { return !(a.x == b.x && a.y == b.y); }

v2u v2uRand(v2u min, v2u max) {
    return v2u { uRand(min.x, max.x), uRand(min.y, max.y) };
}

v2u V2u(v2 a)  { return { (u32)a.x, (u32)a.y}; }
v2  V2(v2u a)  { return { (f32)a.x, (f32)a.y}; }

v2u V2u(v2i a) { return { (u32)a.x, (u32)a.y}; }
v2i V2i(v2u a) { return { (i32)a.x, (i32)a.y}; }

v2i V2(v2 a)  { return { (i32)a.x, (i32)a.y}; }
v2  V2(v2i a) { return { (f32)a.x, (f32)a.y}; }

f32 len(v2 a)  { return sqrt(a.x * a.x + a.y * a.y); }
f32 len(v2i a) { return sqrt(a.x * a.x + a.y * a.y); }
f32 len(v2u a) { return sqrt(a.x * a.x + a.y * a.y); }



inline f32 Clamp(f32 n, f32 min, f32 max) {
    if (n < min) return min;
    if (n > max) return max;
    return n;
}

inline f32 ClampMin(f32 n, f32 min) {
    if (n < min) return min;
    return n;
}

inline f32 ClampMax(f32 n, f32 max) {
    if (n > max) return max;
    return n;
}

#endif //TYPES_H
