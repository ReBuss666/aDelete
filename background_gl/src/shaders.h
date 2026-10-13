#ifndef SHADERS_H
#define SHADERS_H

// 1. VERTEX SHADER
const char *VERTEX_SHADER_SRC = R"(
    attribute vec2 a_position;
    varying vec2 v_uv;

    void main() {
        v_uv = a_position * 0.5 + 0.5;
        gl_Position = vec4(a_position, 0.0, 1.0);
    }
)";

// 2. FRAGMENT SHADER (Узкий, острый кибер-лазер)
const char *FRAGMENT_SHADER_SRC = R"(
    precision mediump float;

    varying vec2 v_uv;
    uniform float u_time;
    uniform vec2 u_resolution;

    float random(vec2 st) {
        return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
    }

    void main() {
      vec2 uv = gl_FragCoord.xy / u_resolution.xy;

      float wave_pos = mod(u_time * 0.45, 2.4) - 0.7;
      
      // Динамика из Левого-Верхнего в Правый-Нижний угол
      float diag = (uv.x + (1.0 - uv.y)) * 0.5;
      float dist = abs(diag - wave_pos);

      // 1. Острый белый центр луча (ширина ~0.015)
      float core = smoothstep(0.015, 0.0, dist);
      
      // 2. Узкий красный ореол свечения (ширина ~0.055)
      float glow = smoothstep(0.055, 0.0, dist);

      float jitter = (random(vec2(u_time * 12.0, uv.y * 100.0)) - 0.5) * 0.006 * glow;
      vec2 uv_glitch = uv + vec2(jitter, 0.0);

      vec3 color_accent = vec3(0.486, 0.027, 0.105); // #7C071B
      vec3 color_white  = vec3(0.95, 0.95, 0.95);

      vec4 final_color = vec4(0.0, 0.0, 0.0, 0.0);

      // Рисуем лазер только там, где проходит луч
      if (glow > 0.01) {
          float noise = random(uv_glitch + floor(u_time * 20.0));
          vec3 laser_color = mix(color_accent * 2.5, color_white, core + noise * 0.2);
          
          final_color.rgb = laser_color;
          final_color.a = glow * 0.85; // Прозрачный лазер поверх текста
      }

      // Микро-шум для эстетики
      float grain = (random(uv + u_time) - 0.5) * 0.03;
      final_color.rgb += grain;
      final_color.a += 0.02;

      gl_FragColor = final_color;
    }
)";

#endif
