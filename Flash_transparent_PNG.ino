#include <PNGdec.h>
#include "SpongeBob.h" // Your resized image header file
#include "grinder_icon.h" // New image header file for the UI

PNG png; // PNG decoder instance

#define MAX_IMAGE_WIDTH 240 // Adjust to match or exceed your resized images' width

#include <SPI.h>
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

int16_t xpos = 0;
int16_t ypos = 0;

// PNG draw callback
int pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WIDTH];
  // Use PNG_RGB565_BIG_ENDIAN (based on previous success)
  // Set transparent pixels to black (0x000000) to match background
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0x000000);
  tft.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
  return 1; // Indicate success
}

// Function for the modern weed grinder UI
void displayWeedGrinderUI() {
  // Clear the screen for the UI
  tft.fillScreen(TFT_BLACK);
  
  // Weight display as a button-like element at bottom center
  int weightX = tft.width() / 2 - 60; // Center horizontally, wider for text
  int weightY = tft.height() - 50; // Near bottom
  tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK); // White text on grey
  tft.setTextSize(2);
  tft.setCursor(weightX + 1, weightY + 10);
  tft.println("Weight: 0g");
  
  // Start Grinder button (green, rounded) between arc and weight
  int buttonX = tft.width() / 2 - 70; // Center horizontally, wider
  int buttonY = tft.height() / 2 + 60; // Below image, above weight
  tft.fillRoundRect(buttonX - 15, buttonY, 170, 40, 10, TFT_MAGENTA); // Wider and rounded
  tft.setTextColor(TFT_LIGHTGREY, TFT_MAGENTA); // Light grey text on magenta
  tft.setTextSize(2);
  tft.setCursor(buttonX - 9, buttonY + 15);
  tft.println("Start Grinder");
  
  // Load and display the grinder icon image centered above the button
  int iconX, iconY;
  uint16_t iconW = 0, iconH = 0;
  int16_t rc = png.openFLASH((uint8_t *)grinder_icon, sizeof(grinder_icon), pngDraw);
  if (rc == PNG_SUCCESS) {
    iconW = png.getWidth();
    iconH = png.getHeight();
    iconX = tft.width() / 2 - iconW / 2; // Center horizontally
    iconY = buttonY - iconH - 40; // 60 pixels above button
    xpos = iconX;
    ypos = iconY;
    tft.startWrite();
    rc = png.decode(NULL, 0);
    tft.endWrite();
  }
  
  // Current mode and speed (top left)
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(20, 20);
  tft.println("Mode: Standard Mode");
  tft.setCursor(20, 40);
  tft.println("Speed: Medium");
  
  // Note: Buttons and image are visual placeholders and not interactive
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n\nUsing the PNGdec library");

  tft.begin();
  tft.setRotation(1); // Rotate 90° clockwise for sideways (landscape) orientation
  tft.fillScreen(TFT_BLACK); // Set initial background to black

  // Draw the PNG image once
  uint16_t pngw = 0, pngh = 0;
  int16_t rc = png.openFLASH((uint8_t *)bob, sizeof(bob), pngDraw);

  if (rc == PNG_SUCCESS) {
    Serial.println("Successfully opened PNG file");
    pngw = png.getWidth();
    pngh = png.getHeight();
    Serial.printf("Image metrics: (%d x %d), %d bpp, pixel type: %d\n", pngw, pngh, png.getBpp(), png.getPixelType());

    // Center the image
    xpos = (tft.width() - pngw) / 2;
    ypos = (tft.height() - pngh) / 2;

    // Ensure black background
    tft.fillScreen(TFT_BLACK);

    tft.startWrite();
    uint32_t dt = millis();
    rc = png.decode(NULL, 0);
    tft.endWrite();
    Serial.print(millis() - dt);
    Serial.println("ms");

    // Draw text below the image
    tft.setTextColor(TFT_WHITE, TFT_BLACK); // White text on black background
    tft.setTextSize(2); // Text size
    tft.setCursor(xpos, ypos + pngh + 10); // Position below image with 10px gap
    tft.println("Powered by Bud...");
  } else {
    Serial.println("Failed to open PNG file, rc = " + String(rc));
  }

  // Wait 4 seconds
  delay(4000);

  // Transition to weed grinder UI
  displayWeedGrinderUI();

  Serial.println("\r\nInitialisation done.");
}

void loop() {
  // Leave empty to keep UI static
  // Add interactive UI logic here if needed (e.g., touch or button input)
}