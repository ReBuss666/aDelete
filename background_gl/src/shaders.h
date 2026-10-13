#ifndef SHADERS_H
#define SHADERS_H

// 1 VERTEX SHADER
const char *VERTEX_SHADER_SRC = R"(
  attribute vec2 a_position;
  varying vec2 v_uv;

  void main() {
    // [-1, 1] ->> UV coordinates [0, 1]
    v_uv = a_position * 0.5 + 0.5;
    gl_Position = vec4(a_position, 0.0, 1.0);
  }
)";

// 2 fragment shader

const char *FRAGMENT_SHADER_SRC = R"(
  precision mediump float;

  varying vec2 v_uv;
  uniform float u_time;
  uniform vec2 u_resolution;

  // random noise on gpu
  float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78,233))) * 43758.5453123);
  }

  void main() {
    // normal pixl coordinates
    vec2 uv = gl_FragCoord.xy / u_resolution.xy;
    // mod() зацикливает проход волны из левого верхнего угла по времени
    float wave_pos = mod(u_time * 0.45, 2.4) - 0.7;
    float diag = (uv.x + (1.0 - uv.y)) * 0.5;
    float dist = abs(diag - wave_pos);
    float wave = smoothstep(0.16, 0.0, dist);

    // --- VHS GLITCH (Сдвиг строк при сканировании) ---
    float jitter = (random(vec2(u_time * 10.0, uv.y * 80.0)) - 0.5) * 0.007 * wave;
    vec2 uv_glitch = uv + vec2(jitter, 0.0);

    // --- ЦВЕТОВАЯ ПАЛИТРА ---
    vec3 color_bg = vec3(0.039, 0.039, 0.039);     // #0A0A0A
    vec3 color_text = vec3(0.611, 0.643, 0.607);   // #9CA49B
    vec3 color_accent = vec3(0.486, 0.027, 0.105); // #7C071B

    vec3 color = mix(color_bg, color_text * 0.18, 0.5);

    // --- ВСПЫШКА СКАНИРОВАНИЯ ---
    if (wave > 0.01) {
        float noise = random(uv_glitch + floor(u_time * 15.0));
        vec3 wave_color = mix(color_accent * 2.0, vec3(0.95), noise * 0.3);
        color = mix(color, wave_color, wave * 0.85);
    }

    // --- VHS СТРУКТУРА (Scanlines, Noise, Vignette) ---
    // 1. CRT-полоски
    float scanline = sin(uv.y * u_resolution.y * 1.2) * 0.05;
    color -= scanline;

    // 2. Аналоговый шум кассеты
    float grain = (random(uv + u_time) - 0.5) * 0.04;
    color += grain;

    // 3. Виньетка (затенение по краям)
    vec2 vig_uv = uv * (1.0 - uv.yx);
    float vignette = vig_uv.x * vig_uv.y * 15.0;
    color *= clamp(pow(vignette, 0.25), 0.0, 1.0);

    gl_FragColor = vec4(color, 1.0);
  }
)";

#endif
