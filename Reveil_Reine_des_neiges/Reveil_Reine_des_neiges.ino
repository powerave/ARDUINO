#include <WiFi.h>
#include <time.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Retirer tous les delays et passer par interruption ISR

const char* ssid = "Bbox";
const char* pw = ""; // A Renseigner

const char* ntpServer = "pool.ntp.org"; // serveur gratuit pour choper l'heure précise
const long gmOffset_sec = 3600; // UTC +1 Paris
const long summerOffset_sec = 0; // Heure d'été a modifier en temps voulu -> 3600

const int pinBuzzer = 5;
const int button = 23;
//const int led1 = 18;
//const int led2 = 19;

// Fréquences pour les notes de la mélodie chantée
#define NOTE_DO5  523  // Li-
#define NOTE_RE5  587  // -bé-
#define NOTE_MI5  659  // -rée
#define NOTE_SI4  494  // Dé-
#define NOTE_DO5_2 523 // -li-
#define NOTE_SOL4 392  // -vrée
#define NOTE_LA4  440  // Je
#define NOTE_SI4_2 494 // ne
#define NOTE_DO5_3 523 // men-
#define NOTE_RE5_2 587 // -ti-
#define NOTE_SOL4_2 392 // -rai
#define NOTE_SOL4_3 392 // plus
#define NOTE_LA4_2 440 // ja-
#define NOTE_SI4_3 494 // -mais

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  pinMode(button, INPUT_PULLUP);
//  pinMode(led1, OUTPUT);
//  pinMode(led2, OUTPUT);
  Serial.begin(115200); // vitesse d'envoie en bits/s

  Serial.print("Connexion à ");
  Serial.println(ssid);
  WiFi.begin(ssid, pw);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("/nWifi is now connected to Bbox");
  configTime(gmOffset_sec, summerOffset_sec, ntpServer);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 not found");
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
}

void loop() {
  struct tm time_info;
//  digitalWrite(led1, LOW);
//  digitalWrite(led2, LOW);
 
  if (!getLocalTime(&time_info)) {
    Serial.println("Couldn't load local time");
    return;
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(20, 20);
  display.printf("%02d:%02d:%02d\n", time_info.tm_hour, time_info.tm_min, time_info.tm_sec);
  Serial.printf("%02d:%02d:%02d\n", time_info.tm_hour, time_info.tm_min, time_info.tm_sec);
  display.display();

  if (time_info.tm_hour == 7 && time_info.tm_min == 30) {
//    digitalWrite(led1, HIGH);
//    digitalWrite(led2, HIGH);
    int i = 0;
    while (i < 10) {
      Serial.println("C'est l'heure de se lever: Cocoricoooo!!");
      // "Li-bé-rée"
      if (stop_alarm(button))
        break;
      jouerNote(392, 200); // Sol4
      jouerNote(494, 200); // Si4
      jouerNote(523, 800); // Do5

      if (stop_alarm(button))
        break;
      delay(100);

      // "Dé-li-vrée"
      jouerNote(392, 200); // Sol4
      jouerNote(392, 200); // Sol4
      jouerNote(587, 800); // Re5

      if (stop_alarm(button))
        break;
      delay(200);

      // "Je ne mentirai plus jamais"
      jouerNote(523, 200); // Do5
      jouerNote(494, 200); // Si4
      jouerNote(440, 400); // La4
      jouerNote(440, 400); // La4
      if (stop_alarm(button))
        break;
      jouerNote(440, 200); // La4
      jouerNote(494, 400); // Si4
      jouerNote(523, 800); // Do5

      if (stop_alarm(button))
        break;
      delay(200);

      if (stop_alarm(button))
        break;
      jouerNote(392, 200); // Sol4
      jouerNote(494, 200); // Si4
      jouerNote(523, 800); // Do5

      if (stop_alarm(button))
        break;
      delay(100);

      jouerNote(392, 200); // Sol4
      jouerNote(659, 200); // Mi5
      jouerNote(659, 400); // Mi5
      jouerNote(587, 400); // Re5

      if (stop_alarm(button))
        break;
      delay(200);

      jouerNote(523, 400); // Do5
      jouerNote(659, 600);
      jouerNote(700, 400);
      jouerNote(659, 200);
      if (stop_alarm(button))
        break;
      jouerNote(587, 200); 
      jouerNote(523, 200);
      jouerNote(587, 200);
      jouerNote(523, 800); 
      
      if (stop_alarm(button))
        break;
      delay(3000);
      i++;
      }
  }

  delay(1000);
}

bool stop_alarm(int button) {
  if (digitalRead(button) == LOW) {
//    digitalWrite(led1, LOW);
//    digitalWrite(led2, LOW);
    Serial.println("bonne journée :D");
    delay (60000);
    return true;
  }
  return false;
}

void jouerNote(int freq, int ms) {
  tone(pinBuzzer, freq);
  delay(ms);
  noTone(pinBuzzer);
  delay(50); 
}
