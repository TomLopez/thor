#include <SPI.h>
#include <epd2in9.h>
#include <epdpaint.h>
#include <avr/sleep.h>
#include <avr/power.h>

#define COLORED     0
#define UNCOLORED   1
#define pin2 2
#define pin3 3
#define trigPin 5
#define echoPin 6

/**
  * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
  * In this case, a smaller image buffer is allocated and you have to 
  * update a partial display several times.
  * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
  */
unsigned char image[1024];
Paint paint(image, 0, 0);    // width should be the multiple of 8 
Epd epd;

int bottlesCrushed;

void setup() {
  // put your setup code here, to run once:
  pinMode(pin2, INPUT);
  pinMode(pin3, INPUT_PULLUP);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  bottlesCrushed=0;
  
  Serial.begin(9600);
  if (epd.Init(lut_full_update) != 0) {
      Serial.print("e-Paper init failed");
      return;
  }
  clearScreen();
  writeScreen();
}

void loop() {
  delay(100);
  // put your main code here, to run repeatedly:
  enterSleep();

  bottlesCrushed+=1;
  delay(100);
  updateBottlesCrushed(bottlesCrushed);
  int distance = measureDistance();
}

void clearScreen() {
  /** 
   *  there are 2 memory areas embedded in the e-paper display
   *  and once the display is refreshed, the memory area will be auto-toggled,
   *  i.e. the next action of SetFrameMemory will set the other memory area
   *  therefore you have to clear the frame memory twice.
   */
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  delay(2000);

  if (epd.Init(lut_partial_update) != 0) {
      Serial.print("e-Paper init failed");
      return;
  }
}

void writeScreen() {
  paint.SetRotate(ROTATE_90);
  paint.SetWidth(24);
  paint.SetHeight(265);

  /* For simplicity, the arguments are explicit numerical coordinates */
  paint.Clear(COLORED);
  paint.DrawStringAt(0, 4, "Thor Trash Bin", &Font24, UNCOLORED);
  epd.SetFrameMemory(paint.GetImage(), 80, 10, paint.GetWidth(), paint.GetHeight());
  
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 4, "000 Bottles crushed", &Font20, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 20, 10, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
   paint.Clear(COLORED);
  paint.DrawStringAt(0, 4, "Thor Trash Bin", &Font24, UNCOLORED);
  epd.SetFrameMemory(paint.GetImage(), 80, 10, paint.GetWidth(), paint.GetHeight());
  
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 4, "000 Bottles crushed", &Font20, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 20, 10, paint.GetWidth(), paint.GetHeight());
}

void updateBottlesCrushed(int bottles) {
  paint.SetWidth(24);
  paint.SetHeight(50);
  paint.Clear(UNCOLORED);
  
  char buf [4];
  sprintf (buf, "%03i", bottles);
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 4, buf, &Font20, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 20, 10, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
}

void enterSleep(void)
{
  
  /* Setup pin2 as an interrupt and attach handler. */
  attachInterrupt(0, pin2Interrupt, LOW);
  delay(100);
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  
  sleep_enable();
  
  sleep_mode();
  
  /* The program will continue from here. */
  
  /* First thing to do is disable sleep. */
  sleep_disable(); 
}

void pin2Interrupt(void)
{
  /* This will bring us back from sleep. */
  
  /* We detach the interrupt to stop it from 
   * continuously firing while the interrupt pin
   * is low.
   */
  detachInterrupt(0);
  delay(100);
}

int measureDistance() 
{
  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
//  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  return distance;
}
}

