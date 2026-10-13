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
