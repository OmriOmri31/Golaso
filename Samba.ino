#include "pitches.h"

// Define the melody using notes and their durations
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



// Define the pin for the buzzer
const int buzzerPin = 25;

void setup() {
  // No setup needed
}

void loop() {
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
  // Add a delay to repeat the melody
  delay(1000);
}
