const tCanvas = document.getElementById('text-canvas');
const ctx = tCanvas.getContext('2d');

let tWidth, tHeight, cols, rows;
const spacingX = 110;
const spacingY = 32;
const rowShift = 35; // Сдвиг ряда влево

const GLITCH_CHARS = '@#$%&*!?aDeLtE01';
const ACTIVE_WORD = ['a', 'D', 'e', 'l', 'e', 't', 'e'];
const IDLE_WORD = ['D', 'e', 'l', 'e', 't', 'e', ' '];

const gridState = [];

function resizeTextCanvas() {
  tWidth = tCanvas.width = window.innerWidth;
  tHeight = tCanvas.height = window.innerHeight;

  rows = Math.ceil(tHeight / spacingY) + 2;

  // Рассчитываем запас колонок с учетом максимального сдвига всех рядов влево
  const maxShift = rows * rowShift;
  cols = Math.ceil((tWidth + maxShift) / spacingX) + 4;

  initGrid();
}

function initGrid() {
  gridState.length = 0;
  for (let r = 0; r < rows; r++) {
    gridState[r] = [];
    for (let c = 0; c < cols; c++) {
      const wordChars = [];
      for (let i = 0; i < 7; i++) {
        wordChars.push({
          state: 'IDLE',
          freezeProgress: 0,
          scrambleTimer: 0,
          char: IDLE_WORD[i],
          color: 'rgba(156, 164, 155, 0.12)'
        });
      }
      gridState[r][c] = wordChars;
    }
  }
}

window.addEventListener('resize', resizeTextCanvas);
resizeTextCanvas();

function drawTextGrid() {
  ctx.clearRect(0, 0, tWidth, tHeight);
  ctx.font = '14px "JetBrains Mono", monospace';

  const charWidth = ctx.measureText('M').width || 8.4;
  const u_time = performance.now() / 1000.0;
  const wave_pos = (u_time * 0.45) % 2.4 - 0.7;

  for (let r = 0; r < rows; r++) {
    for (let c = 0; c < cols; c++) {
      if (!gridState[r] || !gridState[r][c]) continue;
      const wordChars = gridState[r][c];

      const baseX = c * spacingX - (r * rowShift);
      const y = r * spacingY;

      for (let i = 0; i < 7; i++) {
        const charObj = wordChars[i];
        const x = baseX + i * charWidth;

        // Не рисуем буквы, если они ушли за левый край экрана
        if (x + charWidth < 0) continue;

        const uv_x = x / tWidth;
        const uv_y = y / tHeight;

        const diag = (uv_x + uv_y) * 0.5;
        const dist = Math.abs(diag - wave_pos);

        // 1. УДАР ЛАЗЕРА
        if (dist < 0.018 && charObj.state === 'IDLE') {
          charObj.state = 'FREEZE';
          charObj.char = ACTIVE_WORD[i];
          charObj.freezeProgress = 1.0;
        }

        // 2. ФАЗА FREEZE
        if (charObj.state === 'FREEZE') {
          const p = charObj.freezeProgress;
          const r_col = Math.floor(124 + (255 - 124) * p);
          const g_col = Math.floor(7 + (255 - 7) * p);
          const b_col = Math.floor(27 + (255 - 27) * p);

          charObj.color = `rgb(${r_col}, ${g_col}, ${b_col})`;
          charObj.freezeProgress -= 0.03;

          if (charObj.freezeProgress <= 0) {
            charObj.state = 'SCRAMBLE';
            charObj.scrambleTimer = 20 + Math.floor(Math.random() * 20);
          }
        }
        // 3. ФАЗА SCRAMBLE
        else if (charObj.state === 'SCRAMBLE') {
          charObj.scrambleTimer--;
          charObj.char = GLITCH_CHARS[Math.floor(Math.random() * GLITCH_CHARS.length)];

          const alpha = charObj.scrambleTimer / 35;
          charObj.color = `rgba(124, 7, 27, ${Math.max(alpha, 0.2)})`;

          if (charObj.scrambleTimer <= 0) {
            charObj.state = 'IDLE';
            charObj.char = IDLE_WORD[i];
            charObj.color = 'rgba(156, 164, 155, 0.12)';
          }
        }

        if (charObj.char !== ' ') {
          ctx.fillStyle = charObj.color;
          ctx.fillText(charObj.char, x, y);
        }
      }
    }
  }
  requestAnimationFrame(drawTextGrid);
}

drawTextGrid();
