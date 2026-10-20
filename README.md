# [aDELETE] // Portfolio System

> **// KYRGYZSTAN->BISHKEK**  
> Interactive portfolio website featuring a C++/WebGL retro-cyberpunk background shader and an asynchronous text-scrambling engine.

---

## ⚡ Tech Stack

- **Frontend & UI:** HTML5 Canvas, Vanilla JavaScript (ES6+), CSS3 (Custom Variables, Flexbox).
- **Background Engine:** C++, OpenGL ES, compiled to WebAssembly (WASM) via **Emscripten**.
- **Fonts & Aesthetics:** JetBrains Mono, CRT/VHS scanline overlay, dynamic matrix-style laser scanlines.

---

## 🚀 Features

- **Hybrid Layer Rendering:** Synchronized dual-canvas architecture (C++ WebGL background layer + 2D JS text grid layer).
- **Interactive Laser Scanline:** Dynamic diagonal laser wave that reacts with real-time text corruption (`Delete` $\to$ `aDelete` $\to$ random ASCII glitch).
- **FSM-based State Engine:** Per-character finite state machine handling freeze, color transitions, and asynchronous scrambling.
- **Responsive Layout:** Dynamic grid calculations ensuring full-screen coverage across any display resolution.

---

## 🛠️ Local Development

1. Clone the repository:
   ```bash
   git clone [https://github.com/your-username/aDelete.git](https://github.com/your-username/aDelete.git)
   cd aDelete

```

2. Run a local HTTP server:
```bash
python3 -m http.server 8080

```


3. Open your browser and navigate to:
```text
http://localhost:8080/frontend/

```



---

## 👤 Author

**Adilet Shamzaev**

Software Engineering Student @ [Manas University](https://manas.edu.kg) // Backend & Graphics

---

## 📄 License

This project is open-source and available under the [MIT License](https://www.google.com/search?q=LICENSE).
