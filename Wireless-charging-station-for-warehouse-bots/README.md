# 🚗⚡ EV Wireless Charging Station for Warehouse Bots 🤖

![Project](https://img.shields.io/badge/Project-Electrical%20&%20Electronics-blue)
![Arduino](https://img.shields.io/badge/Arduino-ESP32-green)
![Wireless Charging](https://img.shields.io/badge/Technology-WPT-orange)
![License](https://img.shields.io/badge/License-Academic-yellow)

---

## 📌 Project Overview
This project implements a **Wireless EV Charging Station** for **Autonomous Warehouse Robots** using **Inductive Power Transfer** technology.  
It allows bots to **automatically align, charge, and resume operations** without any manual intervention, making warehouse operations **faster, smarter, and safer**.

**Full Project Report:** [`wireless_charging_station.pdf`](wireless_charging_station.pdf)

---

## ✨ Features
- 🔋 **Wireless Charging** – Inductive coils for contactless energy transfer.
- 🤖 **ESP32-Based Bot Control** – Line following, navigation & Bluetooth.
- ⚡ **SOC Calculation** – Battery State of Charge monitoring.
- 📶 **Automatic Alignment** – IR sensors help docking on the pad.
- 🛠️ **Autonomous Workflow** – No human intervention while charging.

---

## 🧩 System Architecture

### 🟢 Block Diagram of Bot
![Bot Block Diagram](images/block_diagram_bot.png)

### 🟢 Block Diagram of Charging Station
![Charging Station Block Diagram](images/block_diagram_station.png)

---

## ⚡ Circuit Diagrams

**Bot Circuit**  
![Bot Circuit](images/circuit_diagram_bot.png)

**Charging Station Circuit**  
![Charging Station Circuit](images/charging_station_diagram.png)

---

## 📊 Simulation & Hardware Results

**Simulation Result**  
![Simulation Result](images/simulation_result.png)

**Hardware Test Result**  
![Hardware Test Result](images/hardware_test_result.png)

---

## 👨‍💻 Code Files
- **Bot Control Code** → [`code/bot_code.ino`](code/bot_code.ino)  
- **SOC Estimation Code** → [`code/soc_estimation.ino`](code/soc_estimation.ino)

> ℹ️ The Arduino code files are included **as-is** from the academic report for evaluation purposes.

---

## 🧰 Components Used
- ESP32 Development Board
- Inductive Coils (TX & RX)
- IR Sensors
- Motor Driver (L298N / L293D)
- Li-ion Battery (3 × 3.7V)
- Voltage & Current Sensors
- LCD Display
- MOSFETs, Resistors, Capacitors, Diodes

---

## 🏗️ How It Works (High Level)
1. The bot monitors **SOC** continuously.
2. Below a safe threshold, it **navigates to the charging station**.
3. Uses **IR sensors** to align over the charging pad.
4. **Wireless power** transfers via inductive coupling.
5. Charging stops automatically when the battery is full.
6. Bot resumes its task queue.

---

## 🏆 Authors
- **Thejas D R** — 1DS22EE096  
- **Sachin Gowda P** — 1DS23EE420  
- **Sunil** — 1DS23EE426  
- **Thejas Gowda MR** — 1DS23EE427  

---

## 📜 License
This project is submitted as part of **Dayananda Sagar College of Engineering (DSCE)** academic work and is intended for educational purposes only.
