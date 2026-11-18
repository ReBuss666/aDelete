#ifndef SHADERS_H
#define SHADERS_H

// 1. VERTEX SHADER
const char *VERTEX_SHADER_SRC = R"glsl(
    attribute vec2 a_position;
    varying vec2 v_uv;

    void main() {
        v_uv = a_position * 0.5 + 0.5;
        gl_Position = vec4(a_position, 0.0, 1.0);
    }
)glsl";

// 2 FRAGMEnT SHADRE
const char *FRAGMENT_SHADER_SRC = R"glsl(
    precision mediump float;

    varying vec2 v_uv;
    uniform float u_time;
    uniform vec2 u_resolution;

    float random(vec2 st) {
        return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
    }

    uniform float u_distortion;

    void main() {
    vec2 screenUv = gl_FragCoord.xy / u_resolution.xy;

    float aspect = u_resolution.x / u_resolution.y;
    vec2 centered = screenUv - 0.5;

    centered.x *= aspect;

    // Same radial mapping as text_bg.js, solved backwards for this pixel.
    float distortion = u_distortion * 0.35;
    vec2 source = centered;
    for (int i = 0; i < 3; i++) {
        float radius = dot(source, source);
        source = centered / (1.0 + distortion * radius);
    }
    source.x /= aspect;

    vec2 uv = source + 0.5;

    float wave_pos = mod(u_time * 0.45, 2.4) - 0.7;

    float diag = (uv.x + (1.0 - uv.y)) * 0.5;
    float dist = abs(diag - wave_pos);

    float core = smoothstep(0.015, 0.0, dist);
    float glow = smoothstep(0.055, 0.0, dist);

    float jitter =
        (random(vec2(u_time * 12.0, uv.y * 100.0)) - 0.5)
        * 0.006
        * glow;

    vec2 uv_glitch = uv + vec2(jitter, 0.0);

    vec3 color_accent = vec3(0.486, 0.027, 0.105);
    vec3 color_white = vec3(0.95, 0.95, 0.95);

    vec4 final_color = vec4(0.0);

    if (glow > 0.01) {
        float noise = random(uv_glitch + floor(u_time * 20.0));

        vec3 laser_color = mix(
            color_accent * 2.5,
            color_white,
            core + noise * 0.2
        );

        final_color.rgb = laser_color;
        final_color.a = glow * 0.85;
    }

    float edge = min(
        min(screenUv.x, 1.0 - screenUv.x),
        min(screenUv.y, 1.0 - screenUv.y)
    );
    float screenVignette = smoothstep(0.0, 0.2, edge);
    float scanline = 0.94 + 0.06 * sin(gl_FragCoord.y * 3.14159);

    float grain = (random(uv + u_time) - 0.5) * 0.03;
    final_color.rgb += grain;
    final_color.a += 0.02;
    final_color.rgb *= (0.78 + screenVignette * 0.22) * scanline;

    gl_FragColor = final_color;
}
)glsl";

#endif
