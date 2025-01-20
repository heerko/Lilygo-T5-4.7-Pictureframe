#include <SPIFFS.h>
#include "epd_driver.h" // Voor e-paper display functies
#include "libjpeg/libjpeg.h"

uint8_t *framebuffer;

#define DELAY_TIME 10 * 1000 // 10s

void setup()
{
  Serial.begin(115200);

  // Init SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("SPIFFS initialisation failed");
    while (true)
      ;
  }

  // SPIFFS.format();
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  if (file)
  {
    while (file)
    {
      Serial.printf("Found file: %s, Size: %u bytes\n", file.name(), file.size());
      file = root.openNextFile();
    }
  }
  else
  {
    Serial.println("No files found");
  }

  // Init EPD display
  epd_init();
  framebuffer = (uint8_t *)ps_calloc(EPD_WIDTH * EPD_HEIGHT / 2, sizeof(uint8_t));
  if (!framebuffer)
  {
    Serial.println("allocation of framebuffer failed");
    while (true)
      ;
  }
  memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2); // Vul framebuffer met wit

  if (psramFound())
  {
    Serial.println("PSRAM detected");
    Serial.printf("Free PSRAM: %u bytes\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
  }
  else
  {
    Serial.println("PSRAM NOT AVAILABLE");
  }

  libjpeg_init();

  Serial.println("Setup done");
}

void loop()
{
  File root = SPIFFS.open("/");
  while (root)
  {
    File file = root.openNextFile();
    while (file)
    {
      String fileName = file.name();
      if (fileName.endsWith(".jpg"))
      {
        Serial.println("Toont: " + fileName);
        DisplayJPEG(("/"+fileName).c_str());
        delay(DELAY_TIME); // wait x time for next image
      }
      file = root.openNextFile();
    }
    // reset loop
    root.close(); // close current directory
    root = SPIFFS.open("/");
  }
}

void DisplayJPEG(const char *filename)
{
  File file = SPIFFS.open(filename, "r");
  if (!file)
  {
    Serial.println("Kan bestand niet openen: " + String(filename));
    return;
  }

  size_t fileSize = file.size();
  Serial.printf("Bestandsnaam: %s, Grootte: %u bytes\n", filename, fileSize);

  if (fileSize == 0 || fileSize > 2 * 1024 * 1024)
  { // Limit filesize to 2MB
    Serial.println("File to big or empty");
    file.close();
    return;
  }

  uint8_t *fileBuffer = (uint8_t *)ps_malloc(fileSize); // Use PSRAM for buffer
  if (!fileBuffer)
  {
    Serial.println("Error allocating PSRAM buffer");
    file.close();
    return;
  }

  file.read(fileBuffer, fileSize);
  file.close();

  // display the image
  epd_poweron();
  epd_clear();
  show_jpg_from_buff(fileBuffer, fileSize, epd_full_screen());
  epd_poweroff_all();

  free(fileBuffer); // Release PSRAM buffer
}

/* Unused, this is the approach with raw files */

void DisplayAllRawFilesLoop() {
  File root = SPIFFS.open("/");
  if (!root || !root.isDirectory()) {
      Serial.println("SPIFFS-root not accessible.");
      return;
  }

  while (true) {
      File file = root.openNextFile();
      bool filesFound = false;

      while (file) {
          String fileName = file.name();
          if (fileName.endsWith(".raw")) {
              filesFound = true;
              Serial.println("Displaying: " + fileName);
              DisplayRaw(("/"+fileName).c_str());
              delay(DELAY_TIME); // wait x time for next image
          }
          file = root.openNextFile();
      }

      if (!filesFound) {
          Serial.println("No .raw files found.");
          delay(DELAY_TIME); 
      }

      root = SPIFFS.open("/"); // Reset directory to restart
  }
}


void DisplayRaw(const char* filename) {
  File file = SPIFFS.open(filename, "r");
  if (!file) {
      Serial.println("Kan bestand niet openen: " + String(filename));
      return;
  }

  size_t fileSize = file.size();
  if (fileSize != (EPD_WIDTH * EPD_HEIGHT / 2)) {
      Serial.println("Bestandsgrootte komt niet overeen met schermresolutie voor: " + String(filename));
      file.close();
      return;
  }

  file.read(framebuffer, fileSize);
  file.close();

  epd_poweron();
  epd_clear();
  epd_draw_grayscale_image(epd_full_screen(), framebuffer);
  epd_poweroff_all();

  Serial.println("Afbeelding weergegeven: " + String(filename));
}