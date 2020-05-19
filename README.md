# Emrine Arduino

Integration of [Emrine](https://emrine.com/) QR Code based access control solution on Arduino using a ST7735 1.44" TFT screen and a 4x4 membrane keypad.

### [checkout the video](https://www.youtube.com/watch?v=QKLvgKy5jTE)

Arduino Uno          |  Arduino Mini
:-------------------------:|:-------------------------:
![uno](https://user-images.githubusercontent.com/5183022/82349530-81656100-99fa-11ea-8de3-e4d7b5c0b9d6.jpg) | ![mini](https://user-images.githubusercontent.com/5183022/82349532-81fdf780-99fa-11ea-8c25-c37ec30ff510.jpg)

## What is Emrine ?

[Emrine](https://emrine.com/) is a QR Code based access control solution used to open electronic lockers, turn-off alarms, rent bicycles, ...

## Arduino UNO wiring

![uno_bb](https://user-images.githubusercontent.com/5183022/82336958-7d7e1280-99eb-11ea-97f3-2e560ec4afc9.png)

## Arduino Mini wiring

![mini_bb](https://user-images.githubusercontent.com/5183022/82336962-7e16a900-99eb-11ea-9f3e-329175b06af3.png)

## Hardware

For this demo project, the following harware is required :

* Adafruit 1.44" Color TFT LCD Display ST7735 ([link](https://www.adafruit.com/product/2088)) or any ST7735 lcd breakout compatible with [Adafruit-ST7735-Library](https://github.com/adafruit/Adafruit-ST7735-Library)
* 4x4 or 3x4 membrane keypad ([link](https://www.digikey.com/catalog/en/partgroup/matrix-membrane-keypad/68579))
* Arduino UNO or Arduino mini

If you are using the 1.8" breakout, you need to update the lcd initializing part to `tft.initR(INITR_BLACKTAB);`, check the st7735 library examples if using different LCD screens

If you are using a 3x4 keypad, update `COLUMN_NUM`,`keys` and `pin_column` accordingly.

## Run the sketch

Open `sketch.ino` in Arduino IDE

### Install the dependencies :

* in "Library Manager", type "keypad" in search and install "Keypad by Mark Stanley, Alexander Brevig"
* in "Library Manager", type "st7735" in search and install "Adafruit ST7735 and ST7789 library"
* in "Library Manager", type "QRCode" in search and install "QRCode by Richard Moore"

### Update Device ID and keys

On https://emrine.com, login and create a new device

* update `EMRINE_DEVICE_ID` according to your own `Device_ID` : 
* update `EMRINE_KEYA` and `EMRINE_KEYB` respectively to `keyA` and `keyB` values

Device id           |  Keys
:-------------------------:|:-------------------------:
![device_id](https://user-images.githubusercontent.com/5183022/82337199-c9c95280-99eb-11ea-8642-e6b2e3c0492e.png)  | ![keys](https://user-images.githubusercontent.com/5183022/82337201-ca61e900-99eb-11ea-9567-a64f4975731c.png)

The beginning of your sketch will look like this :
```c
#define EMRINE_DEVICE_ID "AIsPkdwm1weM9CSvQiDR"
#define EMRINE_KEYA 9746
#define EMRINE_KEYB 17869
```

You can now upload the sketch to your device

## Dependencies

* [Adafruit-ST7735-Library](https://github.com/adafruit/Adafruit-ST7735-Library): library for driving ST7735 display
* [QRCode](https://github.com/ricmoo/QRCode): library for generating QR code
* [Keypad](https://github.com/Chris--A/Keypad): library for getting keypad events