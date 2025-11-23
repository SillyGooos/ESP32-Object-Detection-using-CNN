# ESP32-S3-CAM Object Detection using CNN

Real-time object detection system using ESP32-S3-CAM with COCO-SSD model running in the browser.

## Features

- Live video streaming from ESP32-S3-CAM
- Real-time object detection using TensorFlow.js COCO-SSD model
- Detects 80 common object classes
- Bounding boxes with confidence scores
- Browser-based AI processing (no server required)
- Configurable detection interval (default: 2 seconds)

## Hardware Requirements

- ESP32-S3-CAM board
- USB cable for programming
- WiFi network

## Software Requirements

- Arduino IDE (1.8.x or later)
- ESP32 board support for Arduino
- Web browser (Chrome recommended for best performance)

## Installation

### 1. Install ESP32 Board Support

1. Open Arduino IDE
2. Go to **File > Preferences**
3. Add this URL to "Additional Board Manager URLs":
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
4. Go to **Tools > Board > Boards Manager**
5. Search for "esp32" and install "esp32 by Espressif Systems"

### 2. Upload Code

1. Clone or download this repository
2. Open `esp32_modular.ino` in Arduino IDE
3. Configure your WiFi credentials in the code:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```
4. Select board: **Tools > Board > ESP32 Arduino > ESP32S3 Dev Module**
5. Select the correct COM port
6. Click **Upload**

## Usage

1. After uploading, open the Serial Monitor (115200 baud)
2. Wait for the ESP32 to connect to WiFi
3. Note the IP address displayed in Serial Monitor
4. Open your web browser and navigate to: `http://[ESP32_IP_ADDRESS]`
5. Wait for the COCO-SSD model to load (~25MB, may take 30-60 seconds)
6. Point the camera at objects to detect them

## Detectable Objects

The COCO-SSD model can detect 80 object classes including:

**People & Animals:**
- person, dog, cat, bird, horse, cow, sheep, bear, zebra, giraffe

**Vehicles:**
- bicycle, car, motorcycle, airplane, bus, train, truck, boat

**Indoor Objects:**
- chair, couch, bed, dining table, toilet, tv, laptop, mouse, keyboard, cell phone, book

**Kitchen Items:**
- bottle, wine glass, cup, fork, knife, spoon, bowl, banana, apple, sandwich, orange

**And many more...**

## Configuration

### Change Detection Interval

Edit the interval in `html_page.h`:
```javascript
setInterval(detectObjects, 2000); // Change 2000 to desired milliseconds
```

Examples:
- 1 second: `1000`
- 5 seconds: `5000`
- 500ms: `500`

### Change Camera Resolution

Edit in `esp32_modular.ino`:
```cpp
config.frame_size = FRAMESIZE_VGA; // Options: QVGA, VGA, SVGA, XGA, UXGA
```

## Project Structure

```
esp32_modular/
├── esp32_modular.ino    # Main ESP32 code (camera, WiFi, web server)
├── html_page.h          # Web interface (HTML, CSS, JavaScript)
└── README.md            # This file
```

## How It Works

1. **ESP32-S3-CAM** captures video frames and streams them via HTTP
2. **Web browser** receives the MJPEG stream
3. **TensorFlow.js** loads the COCO-SSD model in the browser
4. **JavaScript** periodically captures frames and runs object detection
5. **Canvas overlay** draws bounding boxes and labels on detected objects

## Troubleshooting

### Model stuck on "Initializing..."
- Check browser console (F12) for errors
- Ensure stable internet connection (model downloads from CDN)
- Try Chrome browser for best compatibility
- Wait longer (model is ~25MB and can take time to download)

### Camera not working
- Check Serial Monitor for error messages
- Verify camera pin configuration matches your board
- Try lowering frame size or JPEG quality

### WiFi connection fails
- Double-check SSID and password
- Ensure WiFi network is 2.4GHz (ESP32 doesn't support 5GHz)
- Check Serial Monitor for connection status

### Low frame rate
- Reduce camera resolution
- Increase detection interval
- Close other browser tabs

## Technical Details

**ESP32 Side:**
- Camera: OV2640/OV5640 sensor
- Resolution: VGA (640x480)
- JPEG compression: Quality 22
- Web server: ESP HTTP Server
- Stream format: MJPEG

**Browser Side:**
- AI Framework: TensorFlow.js 4.11.0
- Model: COCO-SSD 2.2.3
- Detection: Client-side processing
- No cloud/server required

## Performance

- Model load time: 30-60 seconds (one-time)
- Detection interval: Configurable (default 2 seconds)
- Inference time: ~100-300ms per frame (browser-dependent)
- Objects per frame: Up to 100 (practical limit ~10-20)

## Credits

- TensorFlow.js: https://www.tensorflow.org/js
- COCO-SSD Model: https://github.com/tensorflow/tfjs-models/tree/master/coco-ssd
- ESP32 Arduino Core: https://github.com/espressif/arduino-esp32

## License

This project is open source and available under the MIT License.

## Future Improvements

- Add support for custom trained models
- Implement person tracking
- Add snapshot/recording functionality
- Support for multiple camera streams
- Mobile-optimized interface
- Offline model hosting on SD card
