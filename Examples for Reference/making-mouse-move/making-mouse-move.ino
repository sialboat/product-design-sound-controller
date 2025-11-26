#include <Mouse.h>

int buttonPin = 3;
const int xPin = A8;
const int yPin = A9;

const int threshold = 10;        // Dead zone to avoid drift
const float sensitivity = 0.05;  // Lower is slower

bool lastLeftClick = false;
bool leftClick = false;

void setup() {
  Mouse.begin();
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  lastLeftClick = leftClick;
  leftClick = !digitalRead(buttonPin);
  if((!lastLeftClick && leftClick) || (lastLeftClick && leftClick) ) {
    Mouse.press(MOUSE_LEFT);
  } else {
    Mouse.release(MOUSE_LEFT);
  }

  int xVal = analogRead(xPin);
  int yVal = analogRead(yPin);

  int xMove = xVal - 512;
  int yMove = yVal - 512;


  // Apply dead zone
  if (abs(xMove) < threshold) xMove = 0;
  if (abs(yMove) < threshold) yMove = 0;

  // Scale movement for smooth speed
  float moveX = xMove * sensitivity;
  float moveY = yMove * sensitivity;

  if (xMove != 0 || yMove != 0) {
    Mouse.move((int)moveX, (int)moveY, 0);
    delay(10);
  }
}