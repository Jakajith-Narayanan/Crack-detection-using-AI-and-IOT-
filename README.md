
# Construction Crack Monitoring System

## Overview

The **Construction Crack Monitoring System** is a real-time, IoT-based application that combines sensor hardware with machine learning to monitor and detect structural cracks in buildings. The system uses a variety of environmental sensors to gather live data and predict the structural integrity of a construction site. A cloud dashboard (via the Blynk IoT platform) provides remote access and real-time alerts.

## Features

- Real-time monitoring of temperature, humidity, moisture, vibration, and accelerometer data.
- ML-based prediction using Random Forest and Gradient Boosting classifiers.
- Live data integration from ESP32 + sensors via Blynk cloud API.
- Visual dashboards and graphs for insight.
- Automatic alerts for moisture, vibration, and crack detection.
- Scalable to various construction environments and sensor deployments.

## Project Structure

```plaintext
Construction-Crack-Monitoring-System/
│
├── Train.ipynb                        # Data preprocessing, visualization, ML model training
├── Prediction.ipynb                   # Real-time prediction using live sensor data from Blynk
├── arduino.c                          # ESP32 firmware code to collect sensor data and send it to Blynk
├── Construction Crack Monitoring System.docx   # Detailed project documentation
└── README.md                          # Project overview and usage instructions (this file)
```

## IoT Hardware & `arduino.c`

The file `arduino.c` contains the embedded firmware for the ESP32 microcontroller. It connects multiple sensors and sends real-time readings to the Blynk IoT cloud platform, which serves as the live data source for the machine learning models.

### Sensor Setup in `arduino.c`

- **DHT11**: Measures temperature and humidity.
- **HX711 Load Cell**: Captures pressure/load data.
- **Moisture Sensor**: Reads soil/concrete moisture level.
- **Vibration Sensor**: Detects physical vibration or impact.
- **MPU6050**: Captures accelerometer (x, y, z) readings.

### Blynk Integration

The ESP32 sends sensor values to Blynk virtual pins (V0–V11), which are then read via HTTP API in `Prediction.ipynb`. Some example endpoints used:

```
GET https://blynk.cloud/external/api/get?token=<AUTH_TOKEN>&V0  # Temperature
GET https://blynk.cloud/external/api/get?token=<AUTH_TOKEN>&V1  # Humidity
GET https://blynk.cloud/external/api/get?token=<AUTH_TOKEN>&V4  # Vibration
...
```

### Alerts from ESP32

The firmware also sends alerts via Blynk when:
- Moisture level is too low or too high.
- Vibration is detected.
- Abnormal acceleration suggests a fall or shift.

These are displayed in the Blynk app and can be used to trigger warnings in the machine learning interface.

## Running the Project

### 1. Flash Arduino Firmware
- Load `arduino.c` onto an ESP32.
- Connect the required sensors as defined in the code.
- Connect ESP32 to Wi-Fi (SSID and password in the code).
- Verify sensor readings are updating in the Blynk dashboard.

### 2. Train Machine Learning Models
- Run `Train.ipynb` to process sample datasets and train your models.
- This notebook saves the trained models (`.pkl` files) for use in prediction.

### 3. Real-Time Prediction
- Run `Prediction.ipynb`.
- This notebook fetches sensor data from Blynk, preprocesses it, and applies the trained model to detect if the site is "Normal" or has a "Crack".
- Output is logged in real-time, and additional alerts can be issued.

## System Modules

- **Data Acquisition** (via ESP32 and `arduino.c`)
- **Preprocessing & Cleaning** (`Train.ipynb`)
- **Feature Engineering & Model Training** (`Train.ipynb`)
- **Prediction & Evaluation** (`Prediction.ipynb`)
- **Remote Monitoring & Visualization** (via Blynk app)
- **Alert & Notification System** (via Blynk from ESP32)

## Installation Requirements

- Python 3.7+
- Jupyter Notebook
- Libraries: `numpy`, `pandas`, `scikit-learn`, `matplotlib`, `seaborn`, `requests`
- ESP32 with sensors
- Blynk IoT App and token

Install Python dependencies:
```bash
pip install -r requirements.txt
```

## Future Enhancements

- Add support for more types of sensors (e.g., strain gauges).
- Move predictions and alert handling to an edge device.
- Build a mobile app dashboard for field teams.
- Use cloud databases for long-term storage and trend analysis.

## License

MIT License

## Authors

- [Your Name]
- Contributors: [Others, if applicable]
