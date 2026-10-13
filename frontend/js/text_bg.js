const tCanvas = document.getElementById('text-canvas');
const ctx = tCanvas.getContext('2d');

let tWidth, tHeight, cols, rows;
const spacingX = 110;
const spacingY = 30;

function resizeTextCanvas() {
  tWidth = tCanvas.width = window.innerWidth;
  tHeight = tCanvas.height = window.innerHeight;
  cols = Math.ceil(tWidth / spacingX) + 2;
  rows = Math.ceil(tHeight / spacingY) + 2;
}
window.addEventListener('resize', resizeTextCanvas);
resizeTextCanvas();

function drawTextGrid() {
  ctx.clearRect(0, 0, tWidth, tHeight);
  ctx.font = '14px "JetBrains Mono", monospace';

  // Emscripten использует performance.now() для u_time
  const u_time = performance.now() / 1000.0;

  // Точная копия математики из C++ шейдера
  const wave_pos = (u_time * 0.5) % 2.4 - 0.7;

  for (let r = 0; r < rows; r++) {
    for (let c = 0; c < cols; c++) {
      // Сдвигаем каждый ряд, чтобы сетка была "косой"
      const x = c * spacingX - (r * 25);
      const y = r * spacingY;

      const uv_x = x / tWidth;
      const uv_y = y / tHeight;

      // Математика диагонали из GLSL
      const diag = (uv_x + (1.0 - uv_y)) * 0.5;
      const dist = Math.abs(diag - wave_pos);

      // Цепная реакция: чем ближе к центру лазера, тем сильнее эффект
      if (dist < 0.015) {
        // Эпицентр лазера: ASCII глитчи и яркий белый цвет
        ctx.fillStyle = '#abb5aa';
        ctx.fillText(Math.random() > 0.4 ? 'aDelete' : '@D#l%t&', x, y);
      } else if (dist < 0.045) {
        // Края лазера: красный цвет и стабильный aDelete
        ctx.fillStyle = '#7c071b';
        ctx.fillText('aDelete', x, y);
      } else {
        // Обычный фон: тусклый Delete
        ctx.fillStyle = 'rgba(156, 164, 155, 0.12)';
        ctx.fillText('Delete', x, y);
      }
    }
  }
  requestAnimationFrame(drawTextGrid);
}

drawTextGrid();
