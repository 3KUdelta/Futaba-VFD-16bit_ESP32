/*
  FutabaVFD — SimpleFlipClock
  ============================
  The simplest possible "flip clock" example for this library.

  - The starting time comes from the compile time (__TIME__), so no RTC
    and no NTP connection is needed to try this out.
  - Every second, only the digit(s) that actually changed flip upward:
    the old digit slides down while the new one drops in from above.
    Digits that stay the same (and the ':' separators) are left alone.
  - Works on both the 8-digit and the 16-digit display — just uncomment
    the matching #define below.

  This is a beginner-friendly, stripped-down version of ClockDemo.ino:
  no struct, no queue, just one simple for-loop per tick.

  ── SELECT DISPLAY ───────────────────────────────────────────────────────
  Uncomment ONE line:
*/
#define DISPLAY_8
// #define DISPLAY_16
// ──────────────────────────────────────────────────────────────────────────

#include <FutabaVFD.h>

#ifdef DISPLAY_16
  FutabaVFD vfd(16, /*CS*/ 5, /*RESET*/ -1);
  #define VFD_MISO 19   // ESP32 core wants a valid MISO pin, even though the VFD never uses it
  #define TIME_COL 4    // put "hh:mm:ss" (8 chars) in the middle of 16 digits
#else
  FutabaVFD vfd(8, /*CS*/ 5, /*RESET*/ 19);
  #define VFD_MISO -1
  #define TIME_COL 0
#endif

char previousTime[9] = "";  // what is currently shown on the display
unsigned long startSeconds; // time of day (in seconds) when the sketch started
unsigned long startMillis;  // millis() at that same moment

// ── Step 1: read "hh:mm:ss" from the compiler and turn it into seconds ─────
unsigned long secondsSinceMidnight() {
  const char* t = __TIME__;  // e.g. "14:35:09", filled in automatically at compile time
  int h = (t[0] - '0') * 10 + (t[1] - '0');
  int m = (t[3] - '0') * 10 + (t[4] - '0');
  int s = (t[6] - '0') * 10 + (t[7] - '0');
  return (unsigned long)h * 3600 + m * 60 + s;
}

// ── Step 2: turn a "seconds since midnight" count into "hh:mm:ss" text ─────
void formatTime(unsigned long totalSeconds, char out[9]) {
  int h = (totalSeconds / 3600) % 24;
  int m = (totalSeconds / 60) % 60;
  int s = totalSeconds % 60;
  out[0] = '0' + h / 10;  out[1] = '0' + h % 10;
  out[2] = ':';
  out[3] = '0' + m / 10;  out[4] = '0' + m % 10;
  out[5] = ':';
  out[6] = '0' + s / 10;  out[7] = '0' + s % 10;
  out[8] = '\0';
}

void setup() {
  Serial.begin(115200);
  delay(200);

  vfd.begin(/*SCLK*/ 18, /*MISO*/ VFD_MISO, /*MOSI*/ 23, /*spiHz*/ 100000);
  vfd.setBrightness(120);
  vfd.clear();

  startSeconds = secondsSinceMidnight();
  startMillis  = millis();

  // Draw the very first frame directly - no flip needed for the first draw.
  formatTime(startSeconds, previousTime);
  for (uint8_t col = 0; col < 8; col++) {
    vfd.writeChar(TIME_COL + col, previousTime[col]);
  }
}

void loop() {

  // ── Step 3: has a new second arrived? ─────────────────────────────────────
  // We always compute "how many seconds have passed" from millis(), instead
  // of just counting up with delay(1000). That way the clock can never drift
  // or skip a second, even if the flip animation below takes a little time.
  static unsigned long lastSecond = 0;
  unsigned long elapsedSeconds = (millis() - startMillis) / 1000;
  if (elapsedSeconds == lastSecond) return;  // no new second yet - do nothing
  lastSecond = elapsedSeconds;

  // ── Step 4: work out the new time text ────────────────────────────────────
  char currentTime[9];
  formatTime(startSeconds + elapsedSeconds, currentTime);

  // ── Step 5: flip only the digit(s) that actually changed ──────────────────
  // Scanned right-to-left, one column at a time, because the VFD hardware
  // can only animate one column at once.
  for (int8_t col = 7; col >= 0; col--) {
    if (currentTime[col] != previousTime[col] && currentTime[col] != ':') {
      vfd.flip(TIME_COL + col, currentTime[col], 220);  // start the flip
      while (vfd.isAnimating()) vfd.update();           // wait until it's done
    }
  }

  // remember what is now on the display, for next second's comparison
  for (uint8_t col = 0; col < 9; col++) previousTime[col] = currentTime[col];

  Serial.println(currentTime);
}
