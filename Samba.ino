#include "pitches.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "XXXXX";
const char* password = "XXXXX";

// API-Football credentials
const char* apiKey = "XXXXXXX";
const char* apiUrl = "https://v3.football.api-sports.io/fixtures";

//const int teamId = 10; // England ID (Won)
const int teamId = 1104; // Georgia Id (Lost last match)
//const int teamId = 1108; // Scotland ID (Draw on last match)


// Define the pin for the buzzer
const int buzzerPin = 25;

// Define the melody using notes and their durations for the victory song
int melody[] = {
  NOTE_C4, NOTE_DS4, NOTE_C4, NOTE_F4, NOTE_DS4, NOTE_F4, NOTE_G4, NOTE_DS4, NOTE_C4, NOTE_C4, 
  NOTE_DS4, NOTE_G4, NOTE_C5, NOTE_BB4, NOTE_C5, NOTE_D5, NOTE_BB4, NOTE_G4, NOTE_G4, NOTE_C5, 
  NOTE_G4, NOTE_F4, NOTE_DS4, NOTE_F4, NOTE_G4, NOTE_DS4, NOTE_C4, NOTE_C5, NOTE_DS5, NOTE_C5, 
  NOTE_C5, NOTE_BB4, NOTE_C5, NOTE_D5, NOTE_BB4, NOTE_C5
};

int noteDurations[] = {
  2, 2, 4, 2, 2, 4, 2, 2, 4, 4, 
  4, 4, 2, 2, 4, 4, 4, 4, 4, 2, 
  4, 4, 4, 4, 4, 4, 2, 2, 4, 4, 
  2, 2, 4, 4, 4, 1
};

// Define the melody using notes and their durations for the failure song
int failureMelody[] = {
  NOTE_C4, NOTE_B3, NOTE_A3, NOTE_G3, NOTE_F3, NOTE_E3, NOTE_D3
};

int failureNoteDurations[] = {
  4, 4, 4, 4, 4, 4, 4
};

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) { // Check the current connection status
    HTTPClient http;
    String url = String(apiUrl) + "?team=" + teamId + "&season=2024&last=1";
    http.begin(url);
    http.addHeader("x-apisports-key", apiKey);
    
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
      
      // Parse the JSON response
      DynamicJsonDocument doc(2048);
      DeserializationError error = deserializeJson(doc, payload);
      
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }

      JsonArray fixtures = doc["response"].as<JsonArray>();
      if (fixtures.size() > 0) {
        JsonObject fixture = fixtures[0];
        JsonObject teams = fixture["teams"];
        JsonObject goals = fixture["goals"];

        bool teamWon = (teams["home"]["id"] == teamId && goals["home"] > goals["away"]) ||
                       (teams["away"]["id"] == teamId && goals["away"] > goals["home"]);
        bool teamLost = (teams["home"]["id"] == teamId && goals["home"] < goals["away"]) ||
                        (teams["away"]["id"] == teamId && goals["away"] < goals["home"]);

        if (teamWon) {
          playGoalSong();
        } else if (teamLost) {
          playFailureSong();
        }
      }
    } else {
      Serial.print("Error on HTTP request: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
  
  delay(60000); // Wait a minute before checking again
}

void playGoalSong() {
  // Iterate over the notes of the melody
  for (int thisNote = 0; thisNote < sizeof(melody)/sizeof(melody[0]); thisNote++) {
    // Calculate the note duration
    int duration = 1000 / noteDurations[thisNote];

    // Play the note on the buzzer pin
    tone(buzzerPin, melody[thisNote], duration);

    // Pause for the note's duration plus 30% to distinguish notes
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);

    // Stop the tone playing
    noTone(buzzerPin);
  }
  Serial.println("Team won! Playing goal song...");
}

void playFailureSong() {
  // Iterate over the notes of the failure melody
  for (int thisNote = 0; thisNote < sizeof(failureMelody)/sizeof(failureMelody[0]); thisNote++) {
    // Calculate the note duration
    int duration = 1000 / failureNoteDurations[thisNote];

    // Play the note on the buzzer pin
    tone(buzzerPin, failureMelody[thisNote], duration);

    // Pause for the note's duration plus 30% to distinguish notes
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);

    // Stop the tone playing
    noTone(buzzerPin);
  }
  Serial.println("Team lost! Playing failure song...");
}
