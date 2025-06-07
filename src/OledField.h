
#pragma once

#include "DisplayMode.h"
#include <Arduino.h>

class OledField {

private:
  int x, y;
  String content;
  DisplayMode mode;
  bool selected;
  bool visible; // Für Blinken

public:
  OledField(int x, int y, const String &content)
      : x(x), y(y), content(content), mode(MODE_NORMAL), selected(false),
        visible(true) {}

  int getX() const { return x; }
  int getY() const { return y; }
  bool getSelected();
  String getContent() const { return content; }
  DisplayMode getMode() const { return mode; }
  bool getVisible() const { return visible; }
  void setContent(const String &newText) { content = newText; }
  void setMode(DisplayMode newMode) { mode = newMode; }
  void setSelected(bool sel) { selected = sel; }
  void toggleVisibility() { visible = !visible; }
};