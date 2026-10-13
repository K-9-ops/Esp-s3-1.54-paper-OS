// Clock: the time, big, and nothing else.
// A or B = bring the time up to date (it can be minutes old after a sleep). Hold B = "stay on": while the app is
// open the device doesn't go to sleep and the time moves on by itself every minute. It uses the battery much
// faster, so it is off until you ask for it.
#include "../../core/app.h"
#include "../../core/clock.h"
#include "../../core/display.h"
#include "../../core/launcher.h"
#include "../../core/power.h"
#include "../../core/theme.h"

RTC_DATA_ATTR static bool stayOn;  // survives sleep; a power loss puts it back to off
static int shownMinute = -1;       // the minute on screen, -1 = none (RAM: rebuilt by the next draw)

static void onEnter() { shownMinute = -1; }

static void draw() {
  shownMinute = -1;
  if (!clockValid()) {
    display.setFont(FONT_LARGE);
    drawCentered("--:--", 76);
    display.setFont(FONT_SMALL);
    drawCentered("Clock not set", 112);
    drawCentered("Settings > Date & time", 134);
    drawHints("", "", "");
    return;
  }
  const struct tm now = clockLocal();
  shownMinute = now.tm_min;
  char text[8];
  snprintf(text, sizeof text, "%02d:%02d", now.tm_hour, now.tm_min);
  display.setFont(FONT_LARGE);
  display.setTextSize(2);  // twice the large font, so the time fills the screen...
  if (textWidth(text) > display.width() - 2 * MARGIN) display.setTextSize(1);  // ...unless that wouldn't fit
  drawCentered(text, 82);
  display.setTextSize(1);  // every other screen expects the normal size
  drawHints("", "refresh", stayOn ? "let it sleep" : "stay on");
}

static Redraw onButton(Event e) {
  if (e == Event::BLong) {
    stayOn = !stayOn;
    launcherToast(stayOn ? "Stays on" : "Sleeps as usual");
  }
  return Redraw::Partial;  // any press, including the one that woke it, shows the right time
}

static Redraw tick() {
  if (!clockValid()) return Redraw::None;
  const struct tm now = clockLocal();
  if (stayOn) {
    powerActivity();  // never idle, so no deep sleep while the clock is open
    powerWakeWithin((60 - now.tm_sec) * 1000UL + 50);  // light sleep ends just after the next minute starts
  }
  return now.tm_min != shownMinute ? Redraw::Tick : Redraw::None;
}

static void onExit() {}

static void status(char *out, size_t len) {
  snprintf(out, len, "%s", stayOn ? "Stays on" : "");
}

extern const App clockApp = {"Clock", ICON_CLOCK, onEnter, onButton, draw, onExit, nullptr, status, tick};
