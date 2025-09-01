# EE 329 – Lab A5: SPI DAC with Keypad Input (STM32L4)

This project implements a **digital-to-analog converter (DAC) interface** on the STM32L4A6ZG Nucleo board, controlled by a **4×4 matrix keypad**.  
The system allows a user to enter a 3‑digit voltage value, which is converted into millivolts and transmitted to an **MCP4921 DAC** over SPI to generate the corresponding analog output.

---

## 🎯 Project Overview
- **Input:** 4×4 keypad connected to GPIOD  
- **Processing:** Convert 3‑digit keypad entry (e.g., `275`) into millivolts (`2750 mV`)  
- **Output:** 16‑bit DAC command word sent to MCP4921 over SPI1 (PA4–PA7), producing an analog voltage  
- **Controls:**  
  - Digits `0–9` → Voltage entry  
  - `*` → Reset current input  
- **Peripherals Used:**  
  - Keypad on **PD0–PD7**  
  - MCP4921 DAC on **SPI1** (CS=PA4, SCK=PA5, MOSI=PA7)  

---

## 🧩 Source Files
- `main.c / main.h` – Application entry, keypad polling, DAC command generation【111†source】【112†source】  
- `keypad.c / keypad.h` – GPIO configuration and debounced key scanning for 4×4 matrix keypad【109†source】【110†source】  
- `dac.c / dac.h` – SPI DAC control, millivolt‑to‑command conversion【107†source】【108†source】  
- `spi.c / spi.h` – SPI1 initialization routine【113†source】【114†source】  

---

## ⚡ Hardware Setup
- **STM32L4A6ZG Nucleo Board**  
- **Keypad Wiring (4×4 matrix):**
  - Rows → PD0–PD3 (inputs, pulldown)  
  - Columns → PD4–PD7 (outputs, push‑pull)  
- **DAC Wiring (MCP4921):**
  - CS → PA4  
  - SCK → PA5  
  - MOSI → PA7  
  - VREF, AVDD, AGND wired per MCP4921 datasheet  

---

## 🚀 How to Use
1. Import the sources into **STM32CubeIDE**.  
2. Build and flash to the **NUCLEO‑L4A6ZG** board.  
3. Connect the keypad and DAC as described.  
4. Enter a 3‑digit number on the keypad (e.g., `275`).  
   - System interprets as **2750 mV** and outputs the voltage on the DAC.  
5. Press `*` to clear/reset the input sequence.  

---

## 📂 Repository Structure
```
.
├── main.c / main.h          # Application logic
├── keypad.c / keypad.h      # 4×4 keypad driver
├── dac.c / dac.h            # MCP4921 DAC driver (SPI)
├── spi.c / spi.h            # SPI1 configuration
├── .gitignore               # Ignore build artifacts
└── README.md                # Project documentation
```

---

## ✅ Learning Outcomes
- Interfacing STM32 with external peripherals via **SPI**  
- Using a **4×4 matrix keypad** with GPIO polling and debounce logic  
- Converting **digital input (keypad)** into **analog output (DAC)**  
- Building a clean, modular embedded system project with documentation  

---

## 📜 License
This project is licensed under the MIT License – see [LICENSE](LICENSE).

---

👤 **Author:** Nathan Heil  
📅 **Course:** EE‑329 (Embedded Systems)  
