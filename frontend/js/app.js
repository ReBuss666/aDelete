let distortion = 0;
window.scrollDistortion = 0;

function updateScrollDistortion() {
  const maxScroll =
    document.documentElement.scrollHeight - window.innerHeight;

  const scrollProgress =
    maxScroll > 0 ? window.scrollY / maxScroll : 0;

  // Вверху 0, внизу мягкая выпуклость.
  const targetDistortion = scrollProgress * 0.16;

  distortion += (targetDistortion - distortion) * 0.12;
  window.scrollDistortion = distortion;

  const vhsOverlay = document.querySelector('.vhs-overlay');

  if (vhsOverlay) {
    vhsOverlay.style.transform = 'none';
  }

  if (typeof window.setDistortion === 'function') {
    window.setDistortion(distortion);
  }

  requestAnimationFrame(updateScrollDistortion);
}

requestAnimationFrame(updateScrollDistortion);

document.addEventListener('DOMContentLoaded', () => {
  // Находим все элементы с атрибутом data-value для scramble эффекта
  const scrambleElements = document.querySelectorAll('.scramble-title, .scramble-sub');

  scrambleElements.forEach((el) => {
    const fx = new TextScramble(el);
    const targetText = el.getAttribute('data-value') || el.innerText;

    // Запускаем расшифровку с небольшой задержкой
    setTimeout(() => {
      fx.setText(targetText);
    }, 300);

    // Эффект дешифровки при ховере
    el.addEventListener('mouseenter', () => {
      fx.setText(targetText);
    });
  });
});
