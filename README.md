# vernacular-iot-voice-alert-node

# Vernacular-Language IoT Voice Alert Node

An IoT edge device that listens for MQTT/HTTP triggers and broadcasts voice alerts in local/vernacular languages (e.g., Hindi, Tamil, Spanish, Swahili). Ideal for agricultural warnings, industrial safety, and rural healthcare systems.

## 🔧 How It Works
1. **Trigger**: A sensor or cloud service publishes an alert to an MQTT topic (e.g., `alerts/farm/zone1`).
2. **Payload**: The message contains a JSON payload: `{"alert_id": "flood_hi", "lang": "hi"}`.
3. **Playback**: The ESP32 receives the payload, fetches the pre-loaded vernacular audio file from its local LittleFS storage (or streams it via HTTP), and plays it through an I2S amplifier and speaker.

## 📦 Bill of Materials (BOM)
| Component | Purpose | Quantity | Est. Cost (USD) |
|---|---|---|---|
| [ESP32 DevKit V1](https://robocraze.com/products/esp32-development-board) | Main Wi-Fi/MQTT controller | 1 | $6.00 |
| MAX98357A I2S Audio Amplifier | Drives the speaker from ESP32 I2S pins | 1 | $2.50 |
| 3W 4Ω Mini Speaker | Audio output | 1 | $1.50 |
| 18650 Battery + Holder | Portable power supply (optional) | 1 | $3.00 |
| LM2596 Buck Converter | Steps down 12V to 5V (if using external power) | 1 | $1.20 |

## 🔌 Circuit Setup
| MAX98357A Pin | ESP32 Pin |
|---|---|
| VIN | 3.3V (or 5V depending on module) |
| GND | GND |
| BCLK (Bit Clock) | GPIO 27 |
| LRC (Left/Right Clock) | GPIO 26 |
| DIN (Data In) | GPIO 25 |

## 💻 Code Structure
- `src/main.cpp`: ESP32 firmware handling Wi-Fi, MQTT subscription, and I2S audio playback.
- `scripts/tts_generator.py`: Helper script to generate `.mp3` files from text using Google TTS for offline caching.
- `data/alerts.json`: Dictionary mapping alert triggers to local file paths.

## 🚀 Getting Started
1. Install [PlatformIO](https://platformio.org/) in VS Code.
2. Generate your vernacular audio files: `python3 scripts/tts_generator.py --text "खेत में पानी आ गया है" --lang hi --output data/flood_hi.mp3`
3. Upload the `data` folder to ESP32 LittleFS using the "PlatformIO: Upload Filesystem Image" task.
4. Update Wi-Fi and MQTT credentials in `src/main.cpp` and click **Upload**.