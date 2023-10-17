#include <FastLED.h>

const int NUM_LEDS = 148;
const int DATA_PIN = 9;  // must be PWM, 9 on Pro Mini/Micro, 7 on Mega
const int potPin = A0;
const int BRIGHTNESS = 50;
const int switch1 = 3;
const int switch2 = 4;

CRGB leds[NUM_LEDS];

int getDuration() {
  int s1State = digitalRead(switch1);
  int s2State = digitalRead(switch2);
  int duration = 45;

  if (!s1State && !s2State) {
    duration = 40;
  } else if (s1State && s2State) {
    duration = 70;
  }

  Serial.print("Running with ");
  Serial.print(duration);
  Serial.println(" minutes.");

  return duration;
}

// Use this in rebound()
void setReboundLeds(int i, byte hue) {
  CHSV color = CHSV(hue, 255, 255);
  FastLED.clear();
  leds[i] = color;
  leds[i + 1] = color;
  leds[i + 2] = color;
  FastLED.show();
}

void rebound() {
  int numDots = 3;
  byte hue = 0;
  while (true) {
    for (int i = 0; i < (NUM_LEDS - numDots); i++) {
      hue++;
      setReboundLeds(i, hue);
    }

    for (int i = (NUM_LEDS - numDots); i > 0; i--) {
      hue++;
      setReboundLeds(i, hue);
    }
  }
}

void rebound2() {
  int width = 1;

  while (true) {
    int hertz = 40;
    // go right
    width = map(analogRead(potPin), 0, 1023, 1, 100);
    for (int i = 0; i < (NUM_LEDS - width); i++) {
      for (int j = 0; j < width; j++) {
        leds[i + j] = CHSV(millis() / hertz % 255, 255, 255);
      }
      FastLED.show();
      for (int j = 0; j < width; j++) {
        leds[i + j] = CRGB::Black;
      }
      FastLED.show();
    }

    // go left
    width = map(analogRead(potPin), 0, 1023, 1, 100);
    for (int i = (NUM_LEDS - width); i > 0; i--) {
      for (int j = 0; j < width; j++) {
        leds[i + j] = CHSV(millis() / hertz % 255, 255, 255);
      }
      FastLED.show();
      for (int j = 0; j < width; j++) {
        leds[i + j] = CRGB::Black;
      }
      FastLED.show();
    }
  }
}

void randomBlinks() {
  randomSeed(analogRead(0));
  FastLED.showColor(CRGB::Black);
  while (true) {
    int i = random(NUM_LEDS);
    leds[i] = CRGB::Red;
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.show();
    delay(20);
    leds[i] = CRGB::Black;
    FastLED.show();
  }
}

void randomBlinks2() {
  randomSeed(analogRead(0));
  FastLED.showColor(CRGB::Black);
  while (true) {
    int i = random(NUM_LEDS);
    leds[i] = CHSV(millis() % 255, 255, 255);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.show();
    delay(100);
    leds[i] = CRGB::Black;
    FastLED.show();
  }
}

void potentiometer() {
  FastLED.showColor(CRGB::Black);
  randomSeed(analogRead(0));
  int randColor = random(255);
  while (true) {
    int bar = map(analogRead(potPin), 0, 1023, 0, 150);
    // Serial.println(bar);
    if (bar < 2) {
      randColor = random(255);
    }
    for (int i = 0; i < bar; i++) {
      // leds[i] = CRGB::Blue;
      leds[i] = CHSV(randColor, 255, 100);
    }
    for (int i = bar; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
    }
    FastLED.show();
    // delay(100);
  }
}

void potentiometer2() {
  FastLED.showColor(CRGB::Black);
  int barWidth = 4;
  while (true) {
    int bar = map(analogRead(potPin), 0, 1023, 0, NUM_LEDS - barWidth);
    for (int i = 0; i < NUM_LEDS; i++) {
      if (i >= bar && i < (bar + barWidth)) {
        leds[i] = CRGB::Blue;
      } else {
        leds[i] = CRGB::Black;
      }
    }
    FastLED.show();
  }
}

void potentiometer3() {
  FastLED.showColor(CRGB::Black);
  randomSeed(analogRead(0));
  int randColor = random(255);
  while (true) {
    int bar = map(analogRead(potPin), 0, 1023, 0, 150);
    bool switchTime = false;
    if ((millis() / 1000) % 3 == 0) {
      if (!switchTime) {
        randColor = random(255);
        switchTime = true;
      }
    } else {
      switchTime = false;
    }
    for (int i = 0; i < bar; i++) {
      leds[i] = CHSV(randColor, 255, 100);
    }
    for (int i = bar; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
    }
    FastLED.show();
  }
}

void sineWave() {
  long counter = 0;
  while (true) {
    int numSines = map(analogRead(potPin), 0, 1023, 1, 32);
    int degrees = counter % 360;
    float radians = degrees / 57.2958;
    int led[6];
    for (int i = 0; i < numSines; i++) {
      led[i] = (NUM_LEDS / 2) + (sin(radians) * NUM_LEDS / (i + 2));
      leds[led[i]] = CRGB::Blue;
    }
    FastLED.show();
    for (int i = 0; i < numSines; i++) {
      led[i] = (NUM_LEDS / 2) + (sin(radians) * NUM_LEDS / (i + 2));
      leds[led[i]] = CRGB::Black;
    }
    FastLED.show();
    counter++;
  }
}

void rainbow() {
  while (true) {
    // int amount = map(analogRead(potPin), 0, 1023, 0, 20);
    for (int x = 0; x < NUM_LEDS; x++) {
      leds[x] = CHSV(map(x, 0, 150, 0, 255), 255, 255);
    }
    FastLED.show();
    delay(5);
  }
}

void binaryCounter() {
  long c = 0;
  while (true) {
    char binary[NUM_LEDS] = { 0 };  // This is where the binary representation will be stored

    itoa(c, binary, 2);  // Convert c to a string using base 2 and save it in the array named binary
    Serial.print(binary);
    Serial.print(" | ");
    int buf_len = sizeof(binary) / sizeof(binary[0]);
    Serial.println(buf_len);

    c++;

    for (int i = buf_len; i > 0; i--) {
      if (binary[i] == '1') {
        leds[i] = CRGB::Blue;
      } else {
        leds[i] = CRGB::Black;
      }
    }
    FastLED.show();
  }
}

void binaryCounter2() {
  byte mask[NUM_LEDS] = { 0 };

  while (true) {
    byte cout = 1;
    for (int i = NUM_LEDS - 1; i > 0; i--) {
      if (cout == 0 && mask[i] == 0) {
        leds[i] = CRGB::Black;
        mask[i] = 0;
      } else if (cout + mask[i] == 1) {
        leds[i] = CRGB::Blue;
        mask[i] = 1;
        cout = 0;
      } else {
        cout = 1;
        leds[i] = CRGB::Black;
        mask[i] = 0;
      }
    }
    FastLED.show();
  }
}

void randomRainbow() {
  while (true) {
    for (int i = 0; i < 20; i++) {
      leds[random(NUM_LEDS - 1)] = CHSV(random(255), 255, 255);
    }
    for (int i = 0; i < 40; i++) {
      leds[random(149)] = CRGB::Black;
    }
    FastLED.show();
    delay(map(analogRead(A0), 0, 1023, 50, 500));
  }
}

void fadingWave() {
  int shift = 0;
  while (true) {
    byte color = random(255);
    for (int lap = 0; lap < NUM_LEDS; lap++) {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(color, 255, map(i + shift, 0, NUM_LEDS, 0, 255));
      }
      FastLED.show();
      delay(10);
      shift = (shift + 1) % NUM_LEDS;
    }
  }  // loop
}

void xmas() {
  FastLED.setBrightness(50);
  int phase = 0;
  while (true) {
    for (int i = 0; i < NUM_LEDS; i += 3) {
      if (phase == 0) {
        leds[i] = CRGB::Red;
        leds[i + 1] = CRGB::Green;
        leds[i + 2] = CRGB::Black;
      }
      if (phase == 1) {
        leds[i] = CRGB::Black;
        leds[i + 1] = CRGB::Red;
        leds[i + 2] = CRGB::Green;
      }
      if (phase == 2) {
        leds[i] = CRGB::Green;
        leds[i + 1] = CRGB::Black;
        leds[i + 2] = CRGB::Red;
      }
    }
    FastLED.show();
    delay(250);
    phase++;
    if (phase == 3) {
      phase = 0;
    }
  }
}

void brightnessTimer() {
  while (true) {
    int b = map(analogRead(potPin), 0, 1023, 0, 255);
    FastLED.setBrightness(b);
    FastLED.clear();
    int i = millis() / 10 % NUM_LEDS;
    leds[i] = CRGB::White;
    FastLED.show();
  }
}

void colorKnob() {
  while (true) {
    int h = map(analogRead(potPin), 0, 1023, 0, 255);
    FastLED.setBrightness(255);
    FastLED.clear();
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i].setHue(h);
    }
    FastLED.show();
  }
}

void fall() {
  while (true) {
    FastLED.setBrightness(100);
    FastLED.clear();
    for (int i = 0; i < NUM_LEDS; i++) {
      float r = random(1000);
      if (r > 667) {
        leds[i] = CHSV(0, 255, 255);
      } else if (r > 333) {
        leds[i] = CHSV(15, 255, 255);
      } else {
        leds[i] = CHSV(30, 255, 255);
      }
    }
    FastLED.show();
    delay(1000);
  }
}

void classroomTimer() {
  const long classMinutes = getDuration();
  const long classMillis = classMinutes * 60 * 1000;
  const long startTime = millis();
  const long endTime = startTime + classMillis;
  float ledsPer5Min = 5.0 * float(NUM_LEDS) / classMinutes;
  Serial.println(ledsPer5Min);

  while (millis() < endTime) {
    int h = map(analogRead(potPin), 0, 1023, 0, 255);
    FastLED.setBrightness(255);

    int pivot = (int)(NUM_LEDS * (float)(millis() - startTime) / classMillis);
    // set the blues
    for (int i = 0; i < pivot; i++) {
      leds[i] = CRGB::Blue;
    }

    // set the reds
    for (int i = pivot; i < NUM_LEDS; i++) {
      leds[i].setHue(h);
    }

    // set the 5 minute hand green?
    for (int m = 0; m < classMinutes; m += 5) {
      int led = int(round(m / 5 * ledsPer5Min));
      // Serial.print(m);
      // Serial.print(": ");
      // Serial.println(led);
      // delay(250);
      leds[led] = CHSV(0, 0, 60);
    }

    // set the white
    int secondHand = map(millis() % 1000, 0, 999, 0, NUM_LEDS - 1);
    leds[secondHand] = CRGB::White;

    FastLED.show();
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");
  pinMode(DATA_PIN, OUTPUT);

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);  // use a low brightness to avoid huge power consumption

  // brightnessTimer();
  // rebound();
  // rebound2();
  // randomBlinks();
  // randomBlinks2();
  // potentiometer();
  // potentiometer2();
  // potentiometer3();
  // sineWave();
  // rainbow();
  // binaryCounter();
  // binaryCounter2();
  // randomRainbow();
  // fadingWave();
  // xmas();
  // colorKnob();

  classroomTimer();
  sineWave();
  
}

void loop() {}