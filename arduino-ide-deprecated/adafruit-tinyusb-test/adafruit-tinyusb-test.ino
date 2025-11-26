// Adafruit TinyUSB Test
// Basic Serial and MIDI Communication .ino test file using Adafruit TinyUSB library
// for Teensy 4.1.

#include <Arduino.h>
#include <MIDI.h>
#include <Adafruit_TinyUSB.h>

Adafruit_USBD_CDC usb_serial;
Adafruit_USBD_MIDI usb_midi;

typedef struct midi_test {
  int channel = 1;
  int pitch = 48;
  int velocity = 25;
} midi_test;

midi_test midi_basics;

MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

void setup() {
  if(!TinyUSBDevice.isInitialized()) {
    TinyUSBDevice.begin(0);
  }

  usb_serial.begin(115200);
  usb_midi.setStringDescriptor("TinyUSB MIDI");
  MIDI.begin(MIDI_CHANNEL_OMNI);

  if(TinyUSBDevice.mounted()) {
    TinyUSBDevice.detatch();
    delay(10);
    TinyUSBDevice.attach();
  }
  // here is where you would set handlers
  // MIDI.setHandleNoteOn(handleNoteOn);
  // MIDI.setHandleNoteOff(handleNoteOff);
  while(!TinyUSBDevice.mounted()) delay(10);

  usb_serial.print("morning! (setup finished successfully)");
}

void loop() {
  #ifdef TINYUSB_NEED_POLLING_TASK
    TinyUSBDevice.task();
  #endif

  if(!TinyUSBDevice.mounted())
    return;

  MIDI.sendNoteOn(midi_basics.pitch, midi_basics.velocity, midi_basics.channel);
  MIDI.sendNoteOff(midi_basics.pitch, midi_basics.velocity, midi_basics.channel);

  midi_basics.pitch = (midi_basics.pitch + 1) & (63);
  midi_basics.velocity = (midi_basics.velocity + 1) & (127);
  midi_basics.channel = (midi_basics.channel + 1) & (8);
  MIDI.read();

}
