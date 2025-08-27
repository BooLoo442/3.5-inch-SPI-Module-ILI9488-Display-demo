#include <PNGdec.h>
#include <SPI.h>
#include <TFT_eSPI.h>
// import library


// reference image data files...
#include "WeedLogo.h" 
#include "grinder_icon.h"

// display settings (image data and ui)
const uint16_t MAX_IMAGE_WIDTH = 240;
const uint8_t SCREEN_ROTATION = 1;
const uint16_t BACKGROUND_COLOR = TFT_BLACK;
const uint16_t TEXT_COLOR = TFT_WHITE;
const uint16_t BUTTON_COLOR = TFT_MAGENTA;
const uint16_t BUTTON_TEXT_COLOR = TFT_LIGHTGREY;
const uint16_t DELAY_MS = 4000; // time before menu -- 1000 - 1s, 2000 - 2s, 3000 - 3s, 4000 - 4s


PNG png;
TFT_eSPI tft = TFT_eSPI();

// Draw The image (dont touch)
int pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WIDTH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0x000000);
  tft.pushImage(pDraw->x, pDraw->y, pDraw->iWidth, 1, lineBuffer);
  return 1; // Success
}

// Drawing weight 0g
void drawWeightDisplay() {
  const int16_t weight_x = tft.width() / 2 - 60;
  const int16_t weight_y = tft.height() - 50;
  tft.setTextColor(BUTTON_TEXT_COLOR, BACKGROUND_COLOR);
  tft.setTextSize(2);
  tft.setCursor(weight_x + 1, weight_y + 10);
  tft.println("Weight: 0g");
}

// Draw "start grinder"
void drawStartButton() {
  const int16_t button_x = tft.width() / 2 - 70;
  const int16_t button_y = tft.height() / 2 + 60;
  tft.fillRoundRect(button_x - 15, button_y, 170, 40, 10, BUTTON_COLOR);
  tft.setTextColor(BUTTON_TEXT_COLOR, BUTTON_COLOR);
  tft.setTextSize(2);
  tft.setCursor(button_x - 9, button_y + 15);
  tft.println("Start Grinder");
}

// Draw GRINDER image
void drawGrinderIcon() {
  int16_t rc = png.openFLASH((uint8_t *)grinder_icon, sizeof(grinder_icon), pngDraw);
  if (rc == PNG_SUCCESS) {
    const int16_t icon_width = png.getWidth();
    const int16_t icon_height = png.getHeight();
    const int16_t icon_x = tft.width() / 2 - icon_width / 2;
    const int16_t icon_y = (tft.height() / 2 + 60) - icon_height - 40;
    tft.startWrite();
    pDraw->x = icon_x;
    pDraw->y = icon_y;
    png.decode(NULL, 0);
    png.close(); // Free PNG resources
    tft.endWrite();
  }
}

// Draw config info -- top left
void drawModeAndSpeed() {
  tft.setTextColor(TEXT_COLOR, BACKGROUND_COLOR);
  tft.setTextSize(1);
  tft.setCursor(20, 20);
  tft.println("Mode: Standard Mode");
  tft.setCursor(20, 40);
  tft.println("Speed: Medium");
}

// draw everything 
void displayWeedGrinderUI() {
  tft.fillScreen(BACKGROUND_COLOR);
  drawWeightDisplay();
  drawStartButton();
  drawGrinderIcon();
  drawModeAndSpeed();
}

// Display initial main logo image
void displayInitialImage() {
  int16_t rc = png.openFLASH((uint8_t *)bob, sizeof(bob), pngDraw);
  if (rc == PNG_SUCCESS) {
    const int16_t img_width = png.getWidth();
    const int16_t img_height = png.getHeight();
    const int16_t img_x = (tft.width() - img_width) / 2;
    const int16_t img_y = (tft.height() - img_height) / 2;
    //config
    tft.fillScreen(BACKGROUND_COLOR);
    tft.startWrite();
    pDraw->x = img_x;
    pDraw->y = img_y;
    uint32_t start_time = millis();
    png.decode(NULL, 0);
    png.close(); // Free resources
    tft.endWrite();
    tft.setTextColor(TEXT_COLOR, BACKGROUND_COLOR);
    tft.setTextSize(2);
    tft.setCursor(img_x, img_y + img_height + 10);
    tft.println("Powered by Bud...");
  } else {
    Serial.println("Failed loading logo, rc = " + String(rc));
  }
}

void setup() {
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(SCREEN_ROTATION);
  tft.fillScreen(BACKGROUND_COLOR);
  displayInitialImage();
  delay(DELAY_MS); //loading wait
  displayWeedGrinderUI();
}

void loop() {
  // Empty loop to keep UI static
}