LilyGO T5 4.7 inch e-paper display picture frame
=======================================

This project works with LilyGO T5 4.7 inch e-paper EPD display as available from [AliExpress](https://aliexpress.com/item/1005002272417292.html)

## Compiling and flashing

To compile you will need following libraries  
- https://github.com/Xinyuan-LilyGO/LilyGo-EPD47
- https://github.com/bblanchon/ArduinoJson  

In board manager choose ESP32 Dev Module with PSRAM Enabled.  
The project is configured as PlatformIO Project (Visual Studio Code AddIn) - to compile with arduino IDE rename src folder to OWM_EPD47.

A helper script exists to mass convert images. It will resize the images to 960x640 and brighten them somewhat. Put the source images in /images and the output will go to /data:
> chmod +x convert_image.sh

> ./convert_images.sh

Then upload to SPIFFS with (or use the gui)

> pio run --target uploadfs

# License

[GNU GENERAL PUBLIC LICENSE](./LICENSE)

## Remarks 
based off https://github.com/CybDis/Lilygo-T5-4.7-WeatherStation-with-HomeAssistant
