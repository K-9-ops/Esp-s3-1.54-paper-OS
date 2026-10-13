#include "apps.h"

// Launcher order. Adding an app: one new file in src/apps/<name>/ plus one line here.
extern const App timetableApp, clockApp, badgeApp, dexApp, chooserApp, notesApp, gamesApp, petApp, settingsApp;

const App *const APPS[] = {&timetableApp, &clockApp,   &notesApp, &petApp,  &badgeApp,
                           &dexApp,       &chooserApp, &gamesApp, &settingsApp};
const int APP_COUNT = sizeof(APPS) / sizeof(APPS[0]);
