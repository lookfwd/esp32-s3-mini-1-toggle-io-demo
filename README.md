# ESP32-S3-MINI-1 GPIO Toggle Demo

A WiFi-enabled web interface for controlling GPIO pins on the ESP32-S3-MINI-1 development board.

## Features

- WiFi connectivity with automatic connection
- Web-based control panel accessible from any browser
- Toggle GPIO pins IO15 and IO33
- Real-time status updates
- Responsive design for mobile and desktop
- JSON API for programmatic control

## Hardware Requirements

- ESP32-S3-MINI-1 development board
- USB cable for programming and power
- WiFi network

## GPIO Pins

This project controls the following GPIO pins:
- **GPIO 15**: General purpose I/O pin
- **GPIO 33**: General purpose I/O pin

You can connect LEDs, relays, or other devices to these pins for control.

## Software Requirements

- [PlatformIO](https://platformio.org/) (recommended) or Arduino IDE
- Python 3.x (for PlatformIO)

## Installation

### Using PlatformIO (Recommended)

1. **Clone the repository**
   ```bash
   git clone <repository-url>
   cd esp32-s3-mini-1-toggle-io-demo
   ```

2. **Configure WiFi credentials**

   Edit `src/config.h` and update with your WiFi credentials:
   ```cpp
   const char* WIFI_SSID = "YourWiFiSSID";
   const char* WIFI_PASSWORD = "YourWiFiPassword";
   ```

3. **Build and upload**
   ```bash
   pio run --target upload
   ```

4. **Monitor serial output**
   ```bash
   pio device monitor
   ```

### Using Arduino IDE

1. **Install ESP32 board support**
   - Open Arduino IDE
   - Go to File → Preferences
   - Add to "Additional Board Manager URLs": `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   - Go to Tools → Board → Boards Manager
   - Search for "esp32" and install "esp32 by Espressif Systems"

2. **Open the project**
   - Rename `src/main.cpp` to `main.ino`
   - Open `main.ino` in Arduino IDE

3. **Configure WiFi credentials**

   Edit `config.h` with your WiFi credentials

4. **Select board**
   - Go to Tools → Board → ESP32 Arduino
   - Select "ESP32S3 Dev Module"

5. **Upload**
   - Connect your ESP32-S3 board
   - Select the correct port in Tools → Port
   - Click Upload

## Usage

1. **Power on the ESP32-S3**

   After uploading, open the Serial Monitor (115200 baud) to see the connection status.

2. **Find the IP address**

   The serial monitor will display:
   ```
   WiFi connected successfully!
   IP Address: 192.168.1.XXX
   Web server started!
   Access the control panel at: http://192.168.1.XXX
   ```

3. **Open the web interface**

   Navigate to the IP address in your web browser.

4. **Control GPIO pins**

   Use the ON/OFF buttons to toggle GPIO 15 and GPIO 33.

## API Endpoints

The web server provides the following endpoints:

- `GET /` - Main control panel (HTML)
- `GET /gpio?pin=15&state=1` - Set GPIO 15 to HIGH
- `GET /gpio?pin=15&state=0` - Set GPIO 15 to LOW
- `GET /gpio?pin=33&state=1` - Set GPIO 33 to HIGH
- `GET /gpio?pin=33&state=0` - Set GPIO 33 to LOW
- `GET /status` - Get current status of all GPIOs (JSON)
- `GET /ip` - Get device IP address

### Example API Usage

```bash
# Turn ON GPIO 15
curl "http://192.168.1.XXX/gpio?pin=15&state=1"

# Turn OFF GPIO 33
curl "http://192.168.1.XXX/gpio?pin=33&state=0"

# Get status
curl "http://192.168.1.XXX/status"
```

## Troubleshooting

### WiFi Connection Failed
- Check that SSID and password are correct in `config.h`
- Ensure your WiFi network is 2.4GHz (ESP32 doesn't support 5GHz)
- Check if your network has MAC filtering enabled

### Upload Failed
- Make sure the correct COM port is selected
- Try pressing and holding the BOOT button while uploading
- Check USB cable (some cables are power-only)

### GPIO Not Working
- Verify pin numbers are correct for your specific board
- Check if pins are already in use by other peripherals
- Ensure proper electrical connections

## Project Structure

```
esp32-s3-mini-1-toggle-io-demo/
├── src/
│   ├── main.cpp          # Main application code
│   └── config.h          # WiFi configuration
├── platformio.ini        # PlatformIO configuration
├── .gitignore           # Git ignore file
└── README.md            # This file
```

## Customization

### Adding More GPIO Pins

To control additional GPIO pins:

1. Define new pins in `main.cpp`:
   ```cpp
   #define GPIO_PIN_XX XX
   ```

2. Initialize them in `setup()`:
   ```cpp
   pinMode(GPIO_PIN_XX, OUTPUT);
   ```

3. Add control logic in `handleGPIO()`
4. Update the HTML interface

### Changing Web Interface

The HTML is embedded in `html_page` constant in `main.cpp`. Modify the HTML/CSS/JavaScript as needed.

## License

This project is open source and available for educational and commercial use.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
