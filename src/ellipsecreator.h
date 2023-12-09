// Copyright 2020 Robert Carneiro, Derek Meer, Matthew Tabak, Eric Lujan
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and
// associated documentation files (the "Software"), to deal in the Software
// without restriction,
// including without limitation the rights to use, copy, modify, merge, publish,
// distribute,
// sublicense, and/or sell copies of the Software, and to permit persons to whom
// the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#ifndef _RUELLIPSECREATORPANEL
#define _RUELLIPSECREATORPANEL

#include "Backend/Machine Learning/DataObjects/ImageInput.h"
#include "Backend/Machine Learning/glades.h"
#include "Frontend/GItems/GPanel.h"
#include <map>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <time.h>
#include <vector>

class Circle;
class Point2;
class GLinearLayout;
class RUImageComponent;
class RULabel;
class RUTextbox;
class RUCheckbox;
class RUButton;
class RUDropdown;
class RUGraph;
class RUTable;
class RUProgressBar;
class RUTabContainer;
class PlotType;

namespace shmea {
class GTable;
class GList;
}; // namespace shmea

namespace GNet {
class GServer;
};

namespace glades {
class NNInfo;
};

class EllipseCreatorPanel : public GPanel {
protected:
  virtual void updateFromQ(const shmea::ServiceData*);
  virtual void onStart();

  GNet::GServer *serverInstance;
  bool keepGraping;

  int64_t parsePct(const shmea::GType&);

  void buildPanel();
  RUGraph* dotGraph;
  RULabel* lblmode;
  bool clickMode;
  bool pointTypeFlag;
  Point2* cFocalPoint;
  Circle* prevCircle;

public:
  static const int MODE_CIRCLES = 0;
  static const int MODE_ELLIPSES = 1;

  static const int TYPE_FOCAL_POINT  = 0;
  static const int TYPE_RADIUS = 1;

  EllipseCreatorPanel(const shmea::GString&, int, int);
  EllipseCreatorPanel(GNet::GServer*, const shmea::GString&, int, int);
  virtual ~EllipseCreatorPanel();

  void clearCircleHelper(const shmea::GString&, int, int);
  void toggleModeHelper(const shmea::GString&, int, int);
  void addCircle(const Point2*, double);

  void onMouseDown(const shmea::GString&, int, int);
};

#endif
