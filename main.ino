#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_TouchScreen.h>     // Touch library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <TinyGPS.h>
#include <SPI.h>
#include <SD.h>

// Calibrates value
#define SENSIBILITY 300
#define MINPRESSURE 10
#define MAXPRESSURE 1000

//These are the pins for the shield!
#define YP A1
#define XM A2
#define YM 7
#define XP 6

#define TS_MINX 174
#define TS_MINY 81
#define TS_MAXX 971
#define TS_MAXY 853

// Init TouchScreen:

TouchScreen ts = TouchScreen(XP, YP, XM, YM, SENSIBILITY);

// LCD Pin
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4 // Optional : otherwise connect to Arduino's reset pin

// Assign human-readable names to some common 16-bit color values:
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

// Init LCD

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// Dimensions
uint16_t width = 0; //240
uint16_t height = 0; //320

//-- Setup
//main menu
#define MENUBUTTONS 5
#define STATS 0
#define DISTANCE 1
#define SPEED 2
#define ALT 3
#define ULT 4

Adafruit_GFX_Button buttons[MENUBUTTONS];

//connect RX to TX1
//connect TX to RX1
typedef struct
{
  float lat;
  float lon;
} coordinate;

typedef struct {
  int _year;
  byte _month, _day, _hour, _minute, _second, _hundredths;
} date_time;
TinyGPS gps; // create gps object
float lat, lon;


//SCK 52
//MOSI 51
//MISO 50
int CS_PIN = 53; //MEGA
File file;

const int echoPin = 44; // attach pin D2 Arduino to pin Echo of HC-SR04
const int trigPin = 45; //attach pin D3 Arduino to pin Trig of HC-SR04

bool gps_available = false;
bool sdcard_available = false;
void setup()
{
  Serial.begin(9600);
  Serial.println("GPS Altimeter,Odometer and Speedometer");
  Serial.println("Initializing Screen");
  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(2); // Need for the Mega, please changed for your choice or rotation initial
  width = tft.width() - 1;
  height = tft.height() - 1;
  Serial.print("width :");
  Serial.println(width);
  Serial.print("height :");
  Serial.println(height);
  tft.fillScreen(RED);
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.println("GPS\nAltimeter\nOdometer\nSpeedometer");
  Serial.println("Screen Initialized Successfully!");
  InitializeMenu();
  delay(3000);
  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  tft.println("Initializing\nSD card");
  Serial.println("Initializing SD card"); //arduion ide
  tft.setCursor(0, 0);
  tft.fillScreen(BLACK);
  if (!SD.begin(53))
  {
    Serial.println("SD card Initialization failed!");
    tft.println("SD card\nInitialization\nfailed!");
  }
  else
  {
    sdcard_available  = true;
    Serial.println("SD Card Initialized successfully!");
    tft.println("SD Card\nInitialized\nsuccessfully!");
  }
  delay(1000);
  tft.setCursor(0, 0);
  tft.fillScreen(BLACK);
  tft.println("Initializing\nUltrasonic\nsensor");
  Serial.println("Initializing Ultrasonic sensor");
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  tft.println("Initialized!");
  Serial.println("Initialized!");
  delay(1000);
  tft.setCursor(0, 0);
  tft.fillScreen(BLACK);
  tft.println("Waiting for\nGPS to\nconnect");
  Serial1.begin(9600); // connect gps sensor
  delay(3000);
  tft.setCursor(0, 0);
  tft.fillScreen(BLACK);
  Serial.println("Connecting...");
  tft.print("Connecting");
  delay(1000);
  tft.print(".");
  delay(1000);
  tft.print(".");
  delay(1000);
  tft.print(".");

  /*file = SD.open("sample.txt", FILE_WRITE);
    if (file)
    {
    Serial.println("File created successfully.");
    return 1;
    }
    else
    {
    Serial.println("Error while creating file.");
    return 0;
    }
  */
  // gps_available = true;
}

void loop()
{
  if (gps_available == true) {
    menu();
  } else {
    while (Serial1.available())
    {
      if (gps.encode(Serial1.read())) // encode gps data
      {
        gps.f_get_position(&lat, &lon); // get latitude and longitude

        Serial.print("Position: ");

        //Latitude
        Serial.print("Latitude: ");
        Serial.print(lat, 6);

        Serial.print(",");

        //Longitude
        Serial.print("Longitude: ");
        Serial.println(lon, 6);
        tft.setCursor(0, 0);
        tft.fillScreen(BLACK);
        tft.println("GPS\nconnected!");
        Serial.println("GPS connected!");
        gps_available = true;
        delay(500);
        break;
        /*tft.fillScreen(BLACK);
          tft.setCursor(0, 0);
          tft.setTextColor(WHITE);
          tft.setTextSize(3);
          tft.print("Lat: ");
          tft.print(lat, 6);
          tft.print("\n");
          tft.print("Lon: ");
          tft.print(lon, 6);
          file.print(lat);
          file.print(" ");
          file.println(lon);
          delay(1000);
          if (file)
          {
          file.close();
          Serial.println("File closed");
          }
          }*/
      }
    }
  }
  /*if (!sdcard_available) {
    if (!SD.begin(53))
    {
      //Serial.println("SD card Initialization failed!");
      //tft.println("SD card]nInitialization\nfailed!");
    }
    else
    {
      tft.setCursor(0, 0);
      tft.fillScreen(BLACK);
      sdcard_available  = true;
      Serial.println("SD Card Initialized successfully!");
      tft.println("SD Card\nInitialized\nsuccessfully!");
      delay(500);
    }
    }*/
}

TSPoint waitOneTouch()
{

  TSPoint p;

  do
  {
    p = ts.getPoint();

    pinMode(XM, OUTPUT); //Pins configures again for TFT control
    pinMode(YP, OUTPUT);

  } while ((p.z < MINPRESSURE) || (p.z > MAXPRESSURE));

  return p;
}

void InitializeMenu()
{
  char buttonlabel[MENUBUTTONS][10] = {"Stats", "Distance", "Speed", "Altitude", "Ultrasonic"};//2d array of string
  for (int i = 0; i < MENUBUTTONS; i++)
  {
    buttons[i].initButton(&tft, 120, 50 + i * 55, 200, 50, WHITE, RED, WHITE, buttonlabel[i], 2);
  }
}


void getLocation(coordinate& c)
{
  while (1)
  {
    if (Serial1.available() && gps.encode(Serial1.read())) // encode gps data
    {
      gps.f_get_position(&c.lat, &c.lon); // get latitude and longitude

      //Serial.print("Position: ");

      //Latitude
      //Serial.print("Latitude: ");
      //Serial.print(c.lat, 6);

      // Serial.print(",");

      //Longitude
      //Serial.print("Longitude: ");
      //Serial.println(c.lon, 6);

      break;
    }
  }
}

String get_time() {
  int _year;
  byte _month, _day, _hour, _minute, _second, _hundredths;
  unsigned long _fix_age;
  gps.crack_datetime(&_year, &_month, &_day, &_hour, &_minute, &_second, &_hundredths, &_fix_age);
  _hour += 5;
  _minute   += 30;
  if (_minute >= 60) {
    _hour += _minute / 60;
    _minute = _minute % 60;
  }
  if (_hour >= 24) {
    _day += _hour / 24;
    _hour = _hour % 24;
  }
  String dt;
  if (_hour < 10) {
    dt.concat("0");
  }
  dt.concat(_hour);
  dt.concat(":");
  if (_minute < 10) {
    dt.concat("0");
  }
  dt.concat(_minute);
  dt.concat(":");
  if (_second < 10) {
    dt.concat("0");
  }
  dt.concat(_second);
  return dt;
  /*  switch (d._month) {
      case 1:
      case 3:
      case 5:
      case 7:
      case 8:
      case 10:
      case 12:
        if (d._day > 31) {
          d._day = d._day % 31;
          if (d._month == 12) {
            d._year += 1;
            d._month  = 1;
          } else {
            d._month += 1;
          }
        }
        break;
      case 4:
      case 6:
      case 9:
      case 11:
        if (d._day > 30) {
          d._day = d._day % 30;
          d._month += 1;
        }
        break;
      case 2:
        if (d._year % 4 == 0 || d._year % 100 == 0 || d._year % 400 == 0) {
          d._day = d._day % 29;
        } else {
          d._day = d._day % 28;
        }
        d._month += 1;
        break;
    }*/
}

void _stats()
{
  Serial.println("*****************************");
  Serial.println("stats module");
  bool save = false;
  coordinate c;
  bool _stop = false;
  TSPoint p;
  int count = 0;
  if (sdcard_available)
  {
    Adafruit_GFX_Button  yes_btn, no_btn;
    yes_btn.initButton(&tft, 62, 290, 105, 50, WHITE, RED, WHITE, "Yes", 2);
    no_btn.initButton(&tft,  177, 290, 105, 50, WHITE, RED, WHITE, "No", 2);
    tft.fillScreen(BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextSize(3);
    tft.println("Do you\nwant to\nsave the\nlive stats?");
    yes_btn.drawButton();
    no_btn.drawButton();
    p = waitOneTouch();
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
    if (yes_btn.contains(p.x, p.y)) {
      String dt("stats/");
      dt.concat(datetime());  //stats/ddhhmmss
      dt.concat(".csv");  //stats/ddhhmmss.csv
      char filename[dt.length() + 1];
      dt.toCharArray(filename, sizeof(filename));
      //tft.println("Saving data...");
      if (!SD.exists("stats/")) {
        SD.mkdir("stats/");
      }
      Serial.println(filename);
      file = SD.open(filename, FILE_WRITE);
      tft.fillScreen(BLACK);
      tft.setCursor(0, 0);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      if (!file)
      {
        tft.println("File error!");
      } else {
        save = true;
        file.println("time,lattitude,longitude,altitude(m),speed(m/s)"); //comma separated values
        tft.println("Started\nrecording\ndata\n");
      }
      tft.println("Touch to\ncontinue");
      waitOneTouch();
    }
  }
  String dt;
  char t[9];
  do
  {
    getLocation(c);
    tft.fillScreen(BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextSize(3);
    tft.println("Latitude:");
    tft.println(c.lat, 6);
    tft.println("Longitude:");
    tft.println(c.lon, 6);
    tft.println("Altitude: ");
    tft.println(gps.f_altitude());
    tft.println("meter from\nsea level");
    tft.println("Speed in m/s");
    tft.println(gps.f_speed_mps());
    tft.println("Touch to\nstop");
    Serial.println("Latitude:");
    Serial.println(c.lat, 6);
    Serial.println("Longitude:");
    Serial.println(c.lon, 6);
    Serial.println("Altitude: ");
    Serial.println(gps.f_altitude());
    Serial.println("meter from\nsea level");
    Serial.println("Speed in m/s");
    Serial.println(gps.f_speed_mps());
    if (save) {
      dt =  get_time();
      dt.toCharArray(t, sizeof(t));
      Serial.println(t);
      file.print(t);
      file.print(",");
      file.print(c.lat, 6);
      file.print(",");
      file.print(c.lon, 6);
      file.print(",");
      file.print(gps.f_altitude(), 6);
      file.print(",");
      file.println(gps.f_speed_mps(), 6);
    }
    count = 0;
    do
    {
      p = ts.getPoint();
      pinMode(XM, OUTPUT); //Pins configures again for TFT control
      pinMode(YP, OUTPUT);
      count++;
    } while (((p.z < MINPRESSURE) || (p.z > MAXPRESSURE)) && count < 100);
    if (count != 100) {
      _stop = true;
    }
  } while (!_stop);

  if (save) {
    file.close();
  }
  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.println("Exiting to\nmain menu\n");
  tft.println("Touch to\nconitnue\n");
  delay(1000);
}

void _speed() {
  Serial.println("*****************************");
  Serial.println("speed module");
  TSPoint p;
  float spd = 0.0f;
  float temp_spd =  0.0f;
  coordinate c;
  int count = 0;
  bool _stop = false;
  bool save = false;
  if (sdcard_available) {
    Adafruit_GFX_Button   yes_btn, no_btn ;
    yes_btn.initButton(&tft, 62, 290, 105, 50, WHITE, RED, WHITE, "Yes", 2);
    no_btn.initButton(&tft,  177, 290, 105, 50, WHITE, RED, WHITE, "No", 2);
    tft.fillScreen(BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextSize(3);
    tft.println("Do you\nwant to\nrecord the\nlive data");
    yes_btn.drawButton();
    no_btn.drawButton();
    p = waitOneTouch();
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
    if (yes_btn.contains(p.x, p.y)) {
      String dt("speed/");
      dt.concat(datetime());
      dt.concat(".csv");
      Serial.println(dt.length());
      char filename[dt.length() + 1];
      dt.toCharArray(filename, sizeof(filename));
      //tft.println("Saving data...");
      if (!SD.exists("speed/")) {
        SD.mkdir("speed/");
      }
      Serial.println(filename);
      file = SD.open(filename, FILE_WRITE);
      tft.fillScreen(BLACK);
      tft.setCursor(0, 0);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      if (!file)
      {
        tft.println("File error!");
      } else {
        save = true;
        tft.println("Started\nrecording\ndata");
        file.println("time,lattitude,longitude,speed(m/s),speed(km/hr)");
      }
      tft.println("Touch to\ncontinue!");
      waitOneTouch();
    }
  }
  String dt;
  char t[9];
  do
  {
    count = 0;
    getLocation(c);
    temp_spd = gps.f_speed_mps();
    if (temp_spd != spd) {
      spd = temp_spd;
      tft.fillScreen(BLACK);
      tft.setCursor(0, 0);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      tft.println("Speed");
      tft.print("m/s: ");
      tft.println(spd);
      tft.print("km/s: ");
      tft.println(gps.f_speed_kmph());
      tft.println("Touch to\nstop!");
      Serial.print("m/s ");
      Serial.println(spd);
      Serial.print("km/s ");
      Serial.println(gps.f_speed_kmph());
      if (save) {
        dt =  get_time();
        dt.toCharArray(t, sizeof(t));
        Serial.println(t);
        file.print(t);
        file.print(",");
        file.print(c.lat, 6);
        file.print(",");
        file.print(c.lon, 6);
        file.print(",");
        file.print(spd);
        file.print(",");
        file.println(gps.f_speed_kmph());
      }
    }

    do
    {
      p = ts.getPoint();
      pinMode(XM, OUTPUT); //Pins configures again for TFT control
      pinMode(YP, OUTPUT);
      count++;
    } while (((p.z < MINPRESSURE) || (p.z > MAXPRESSURE)) && count < 100);
    if (count != 100) {
      _stop = true;
    }
  } while (!_stop);
  if (save) {
    file.close();
  } 
  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.println("Exiting to\nmain menu\n");
  tft.println("Touch to\nconitnue\n");
  delay(1000);
}

void _altitude() {
  Serial.println("*****************************");
  Serial.println("altitude module");
  bool save = false;
  int count = 0;
  bool _stop = false;
  TSPoint p;
  coordinate c;
  if (sdcard_available)
  {
    Adafruit_GFX_Button  yes_btn, no_btn;
    yes_btn.initButton(&tft, 62, 290, 105, 50, WHITE, RED, WHITE, "Yes", 2);
    no_btn.initButton(&tft,  177, 290, 105, 50, WHITE, RED, WHITE, "No", 2);
    tft.fillScreen(BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextSize(3);
    tft.println("Do you\nwant to\nsave the\nlive altitude data?");
    yes_btn.drawButton();
    no_btn.drawButton();
    p = waitOneTouch();
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
    if (yes_btn.contains(p.x, p.y)) {
      String dt("altitude/");
      dt.concat(datetime());
      dt.concat(".csv");
      Serial.println(dt.length());
      char filename[dt.length() + 1];
      dt.toCharArray(filename, sizeof(filename));
      //tft.println("Saving data...");
      if (!SD.exists("altitude/")) {
        SD.mkdir("altitude/");
      }
      Serial.println(filename);
      file = SD.open(filename, FILE_WRITE);
      tft.fillScreen(BLACK);
      tft.setCursor(0, 0);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      if (!file)
      {
        tft.println("File error!");
      } else {
        save = true;
        file.println("time,latitude,longitude,altitude(m)");
        tft.println("Started\nrecording\ndata");
      }
      tft.println("Touch to\ncontinue!");
      waitOneTouch();
    }
  }
  delay(500);
  String dt;
  char t[9];
  do
  {
    getLocation(c);
    tft.fillScreen(BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextSize(3);
    tft.println("Altitude");
    tft.println(gps.f_altitude());
    tft.println("meters");
    Serial.println("Altitude");
    Serial.println(gps.f_altitude());
    Serial.println("meters");
    if (save) {
      dt =  get_time();
      dt.toCharArray(t, sizeof(t));
      Serial.println(t);
      file.print(t);
      file.print(",");
      file.print(c.lat, 6);
      file.print(",");
      file.print(c.lon, 6);
      file.print(",");
      file.println(gps.f_altitude(), 6);
    }
    count = 0;
    do
    {
      p = ts.getPoint();
      pinMode(XM, OUTPUT); //Pins configures again for TFT control
      pinMode(YP, OUTPUT);
      count++;
    } while (((p.z < MINPRESSURE) || (p.z > MAXPRESSURE)) && count < 100);
    if (count != 100) {
      _stop = true;
    }
  }
  while (!_stop);
  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.println("Exiting to\nmain menu\n");
  tft.println("Touch to\nconitnue\n");
  delay(1000);
}

String datetime() {
  int _year;
  byte _month, _day, _hour, _minute, _second, _hundredths;
  unsigned long _fix_age;
  coordinate temp;
  getLocation(temp);
  gps.crack_datetime(&_year, &_month, &_day, &_hour, &_minute, &_second, &_hundredths, &_fix_age);
  String dt;
  Serial.print("Year ");
  Serial.println(_year);
  Serial.print("Month ");
  Serial.println(_month);
  Serial.print("Day ");
  Serial.println(_day);
  Serial.print("Hour ");
  Serial.println(_hour);
  Serial.print("Minute ");
  Serial.println(_minute);
  Serial.print("Second ");
  Serial.println(_second);
  // dt.concat(_year);
  //dt.concat(_month);
  //conversion to IST
  _hour += 5;
  _minute   += 30;
  if (_minute >= 60) {
    _hour += _minute / 60;
    _minute = _minute % 60;
  }
  if (_hour >= 24) {
    _day += _hour / 24;
    _hour = _hour % 24;
  }
  if (_day < 10) {
    dt.concat("0");
  }
  dt.concat(_day);

  if (_hour < 10) {
    dt.concat("0");
  }
  dt.concat(_hour);


  if (_minute < 10) {
    dt.concat("0");
  }
  dt.concat(_minute);


  if (_second < 10) {
    dt.concat("0");
  }
  dt.concat(_second);
  Serial.println("Date and time");
  Serial.println(dt);
  return dt;
}
void _distance() {
  Serial.println("*******************************");
  Serial.println("distance module");
  Adafruit_GFX_Button  add_btn, exit_btn ;
  add_btn.initButton(&tft, 62, 200, 105, 50, WHITE, RED, WHITE, "Add", 2);
  exit_btn.initButton(&tft,  177, 200, 105, 50, WHITE, RED, WHITE, "Exit", 2);
  bool new_point = true;
  bool save = false;
  TSPoint p;
  float total_distance = 0.0f, distance = 0.0f;
  coordinate old, _new;
  old.lon = 0 ;
  old.lat = 0;
  int index = 0;
  if (sdcard_available)
  {
    Adafruit_GFX_Button  yes_btn, no_btn;
    yes_btn.initButton(&tft, 62, 290, 105, 50, WHITE, RED, WHITE, "Yes", 2);
    no_btn.initButton(&tft,  177, 290, 105, 50, WHITE, RED, WHITE, "No", 2);
    tft.fillScreen(BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextSize(3);
    tft.println("Do you\nwant to\nsave the\ndistance data?");
    Serial.println("Do you want to save the distance data?");
    yes_btn.drawButton();
    no_btn.drawButton();
    p = waitOneTouch();
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
    if (yes_btn.contains(p.x, p.y)) {
      String dt("distance/");
      dt.concat(datetime());
      dt.concat(".csv");
      Serial.println(dt.length());
      char filename[dt.length() + 1];
      dt.toCharArray(filename, sizeof(filename));
      //tft.println("Saving data...");
      if (!SD.exists("distance/")) {
        SD.mkdir("distance/");
      }
      Serial.println(filename);
      file = SD.open(filename, FILE_WRITE);
      tft.fillScreen(BLACK);
      tft.setCursor(0, 0);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      if (!file)
      {
        tft.println("File error!");
      } else {
        save = true;
        tft.println("Started\nrecording\ndata");
        file.println("time,lattitude1,longitude1,lattitude2,longitude2,distance(m)");
      }
      tft.println("Touch to\ncontinue!");
      waitOneTouch();
    }
    Serial.println("Yes");
  }
  String dt;
  char t[9];
  while (new_point) {
    tft.fillScreen(BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextSize(3);
    tft.print("Add point ");
    tft.println(index + 1);
    add_btn.drawButton();
    exit_btn.drawButton();
    p = waitOneTouch();
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
    if (add_btn.contains(p.x, p.y))
    {
      getLocation(_new);
      if(index == 0){                                                                                                      
      }else{
      distance = gps.distance_between(old.lat, old.lon, _new.lat, _new.lon);
      }
      old.lat = _new.lat;
      old.lon = _new.lon;
      total_distance +=  distance;
      if (save)
      { dt =  get_time();
        dt.toCharArray(t, sizeof(t));
        Serial.println(t);
        file.print(t);
        file.print(",");
        file.print(old.lat, 6);
        file.print(",");
        file.print(old.lon, 6);
        file.print(",");
        file.print(_new.lat, 6);
        file.print(",");
        file.print(_new.lon, 6);
        file.print(",");
        file.println(distance, 6);
      }
    }
    else if (exit_btn.contains(p.x, p.y)) {
      new_point = false;
    }
    index++;
  }

  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.println("The total\ndistance is");
  tft.println(total_distance);
  tft.println("in meters");
  Serial.print("distance ");
  Serial.println(total_distance);
  tft.println("Touch to exit\nto main menu");
  if (save) {
    file.close();
  }
  delay(1000);
}

void ultrasonic() {
  Serial.println("*******************************");
  Serial.println("ultrasonuc module");
  bool save = false;
  TSPoint p;
  bool _stop = false;
  if (sdcard_available)
  {
    Adafruit_GFX_Button  yes_btn, no_btn;
    yes_btn.initButton(&tft, 62, 290, 105, 50, WHITE, RED, WHITE, "Yes", 2);
    no_btn.initButton(&tft,  177, 290, 105, 50, WHITE, RED, WHITE, "No", 2);
    tft.fillScreen(BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextSize(3);
    tft.println("Do you want\nto save the\ndata?");
    Serial.println("Do you want to save the data?");
    yes_btn.drawButton();
    no_btn.drawButton();
    p = waitOneTouch();
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
    if (yes_btn.contains(p.x, p.y)) {
      String dt("us/");
      dt.concat(datetime());
      dt.concat(".csv");
      char filename[dt.length() + 1];
      dt.toCharArray(filename, sizeof(filename));
      //tft.println("Saving data...");
      if (!SD.exists("us/")) {
        SD.mkdir("us/");
        Serial.println("created directory ultrasonic");
      }
      Serial.println(filename);
      file = SD.open(filename, FILE_WRITE);
      tft.fillScreen(BLACK);
      tft.setCursor(0, 0);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      if (!file)
      {
        tft.println("File error!");
      } else {
        save = true;
        tft.println("Started\nrecording\ndata");
        file.println("time,distance(cm)");
      }
      tft.println("Touch to\ncontinue!");
      waitOneTouch();
    }
    Serial.println("Yes");
  }
  String dt;
  char t[9];
  int count = 0;
  long distance = 0;
  long duration;
  coordinate c;
  do {
    count = 0;
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2;
    // Prints the distance on the Serial Monitor
    tft.fillScreen(BLACK);
    tft.setCursor(0, 0);
    tft.setTextSize(3);
    tft.println("Distance\nfrom object\nin cm");
    tft.println(distance);
    Serial.print("distance in cm ");
    Serial.println(distance);
    if (save) {
      getLocation(c);
      dt =  get_time();
      dt.toCharArray(t, sizeof(t));
      file.print(t);
      file.print(",");
      file.println(distance);
    } delay(1000);
    do {
      p = ts.getPoint();
      pinMode(XM, OUTPUT); //Pins configures again for TFT control
      pinMode(YP, OUTPUT);
      count++;
    } while (((p.z < MINPRESSURE) || (p.z > MAXPRESSURE)) && count < 100);
    if (count != 100) {
      _stop = true;
    }
  }
  while (!_stop);
  if (save)
  {
    file.close();
  }
  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.println("Exiting to\nmain menu\n");
  tft.println("Touch to\nconitnue\n");
  delay(1000);
}

void menu()
{
  delay(500);
  tft.fillScreen(BLACK);
  tft.setCursor(30, 0);
  tft.print("Main Menu");
  for (int i = 0; i < MENUBUTTONS; i++)
  {
    buttons[i].drawButton();
  }
  TSPoint p;
  p = waitOneTouch();
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

  for (int i = 0; i < MENUBUTTONS; i++)
  {
    if (buttons[i].contains(p.x, p.y))
    {
      switch (i)
      {
        case STATS:
          _stats();
          break;
        case DISTANCE:
          _distance();
          break;
        case SPEED:
          _speed();
          break;
        case ALT:
          _altitude();
          break;
        case ULT:
          ultrasonic();
          break;
      }
    }
    waitOneTouch();
    //delay(1000);
  }
}


