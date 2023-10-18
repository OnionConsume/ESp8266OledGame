#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int snakeX = SCREEN_WIDTH / 2;
int snakeY = SCREEN_HEIGHT / 2;
int fruitX, fruitY;
int score = 0;
int direction = 0; // 0 - up, 1 - right, 2 - down, 3 - left

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(10);
  }
  Serial.println("Starting OLED display setup...");

  Wire.begin(12, 14);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  display.clearDisplay();
  display.display();
  Serial.println("OLED display initialized.");

  randomSeed(analogRead(0));
  placeFruit();
}

void loop() {
  if (Serial.available() > 0) {
    char input = Serial.read();
    if (input == 'w' && direction != 2) {
      direction = 0; // up
    } else if (input == 'd' && direction != 3) {
      direction = 1; // right
    } else if (input == 's' && direction != 0) {
      direction = 2; // down
    } else if (input == 'a' && direction != 1) {
      direction = 3; // left
    }
  }

  updateSnake();
  checkCollision();
  displaySnake();
  delay(200);
}

void updateSnake() {
  switch (direction) {
    case 0:
      snakeY--;
      break;
    case 1:
      snakeX++;
      break;
    case 2:
      snakeY++;
      break;
    case 3:
      snakeX--;
      break;
  }
}

void checkCollision() {
  if (snakeX >= SCREEN_WIDTH || snakeX < 0 || snakeY >= SCREEN_HEIGHT || snakeY < 0) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.cp437(true);
    display.println("Game Over! Score: " + String(score));
    display.display();
    while (true) {
      // Game over
    }
  }

  if (snakeX == fruitX && snakeY == fruitY) {
    placeFruit();
    score++;
  }
}

void displaySnake() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(snakeX, snakeY);
  display.cp437(true);
  display.println("o");
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(fruitX, fruitY);
  display.cp437(true);
  display.println("*");
  display.display();
}

void placeFruit() {
  fruitX = random(0, SCREEN_WIDTH);
  fruitY = random(0, SCREEN_HEIGHT);
}
