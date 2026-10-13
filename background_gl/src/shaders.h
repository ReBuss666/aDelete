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

// 2. FRAGMENT SHADER (Прозрачный фон + видимый лазер поверх текста)
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

      // Делаем лазер чуть шире (0.1 вместо 0.05), чтобы его точно было видно
      float wave_pos = mod(u_time * 0.4, 2.4) - 0.7;
      float diag = (uv.x + (1.0 - uv.y)) * 0.5;
      float dist = abs(diag - wave_pos);
      float wave = smoothstep(0.1, 0.0, dist);

      float jitter = (random(vec2(u_time * 10.0, uv.y * 80.0)) - 0.5) * 0.005 * wave;
      vec2 uv_glitch = uv + vec2(jitter, 0.0);

      vec3 color_accent = vec3(0.486, 0.027, 0.105); // #7C071B

      // Изначально полностью прозрачный фон, чтобы просвечивал текст с text-canvas
      vec4 final_color = vec4(0.0, 0.0, 0.0, 0.0);

      // Рисуем свечение лазера поверх текста
      if (wave > 0.01) {
          float noise = random(uv_glitch + floor(u_time * 15.0));
          vec3 bright_white = vec3(0.95, 0.95, 0.95);
          vec3 wave_color = mix(color_accent * 2.0, bright_white, noise * 0.3);
          
          final_color.rgb = wave_color;
          final_color.a = wave * 0.8; // Полупрозрачный красный лазер
      }

      // Легкий VHS шум для атмосферы
      float grain = (random(uv + u_time) - 0.5) * 0.05;
      final_color.rgb += grain;
      final_color.a += 0.05;

      gl_FragColor = final_color;
    }
)";

#endif
