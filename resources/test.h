// Confidential, unpublished property of Robert Carneiro

// The access and distribution of this material is limited solely to
// authorized personnel.  The use, disclosure, reproduction,
// modification, transfer, or transmittal of this work for any purpose
// in any form or by any means without the written permission of
// Robert Carneiro is strictly prohibited.
#ifndef _RUTESTPANEL
#define _RUTESTPANEL

#include "Frontend/GItems/GPanel.h"
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <vector>

class gfxpp;
class Point2;
class RUMsgBox;
class RUImageComponent;
class RUButton;
class RUListbox;
class RUTextbox;
class RUTable;
class RUDrawComponent;
class RULabel;
class RUTabContainer;

namespace GNet {
class GServer;
};

class TestPanel : public GPanel
{
protected:
	virtual void onStart();

	gfxpp* cGfx;
	GNet::GServer* serverInstance;

	void buildPanel();

public:
	RULabel* lblDrawGraph;
	RUDrawComponent* drawGraph;
	RUTextbox* tbRadius;
	RUButton* btnMode;
	// RUTabContainer* panelTabs;
	TestPanel(const std::string&, int, int);
	TestPanel(gfxpp*, GNet::GServer*, const std::string&, int, int);
	// get size / count

	virtual void GuiCommander1(const std::string&, int, int);
	virtual void GuiCommander2(const std::string&, int, int);
	virtual void GuiCommander3(const std::string&, int, int);
	virtual void GuiCommander4(const std::string&, int, int);
	virtual void GuiCommander5(const std::string&, int, int);
	virtual void GuiCommander6(const std::string&, int, int);
	virtual void GuiCommander7(const std::string&, int, int);
	virtual void GuiCommander8(const std::string&, int, int);
	virtual void GuiCommander9();
	virtual void GuiCommander10(const std::string&, int, int);
	virtual void GuiCommander11(char);
	virtual void GuiCommander12(const std::string&, int, int);
	virtual void GuiCommander13(const std::string&, int, int);
	virtual void GuiCommander14(const std::string&, int, int);
	virtual void GuiCommander15(int);
};

#endif
