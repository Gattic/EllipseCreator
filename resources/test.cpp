// Confidential, unpublished property of Robert Carneiro

// The access and distribution of this material is limited solely to
// authorized personnel.  The use, disclosure, reproduction,
// modification, transfer, or transmittal of this work for any purpose
// in any form or by any means without the written permission of
// Robert Carneiro is strictly prohibited.
#include "test.h"
#include "../crt0.h"
#include "../main.h"
#include "Backend/Database/GList.h"
#include "Backend/Networking/connection.h"
#include "Backend/Networking/main.h"
#include "Backend/Networking/service.h"
#include "Frontend/GFXUtilities/point2.h"
#include "Frontend/GItems/GItem.h"
#include "Frontend/GItems/GLayout.h"
#include "Frontend/GItems/RUColors.h"
#include "Frontend/GLayouts/GLinearLayout.h"
#include "Frontend/GUI/RUCheckbox.h"
#include "Frontend/GUI/RUDropdown.h"
#include "Frontend/GUI/RUImageComponent.h"
#include "Frontend/GUI/RUListbox.h"
#include "Frontend/GUI/RUMsgBox.h"
#include "Frontend/GUI/RUTabContainer.h"
#include "Frontend/GUI/RUTable.h"
#include "Frontend/GUI/Text/RUButton.h"
#include "Frontend/GUI/Text/RULabel.h"
#include "Frontend/GUI/Text/RUTextbox.h"
#include "Frontend/Graphics/graphics.h"
#include "Frontend/RUGraph/RUDrawComponent.h"
#include "simulation.h"

TestPanel::TestPanel(const std::string& name, int width, int height) : GPanel(name, width, height)
{
	serverInstance = NULL;
	buildPanel();
}

TestPanel::TestPanel(gfxpp* newGfx, GNet::GServer* newInstance, const std::string& name, int width,
					 int height)
	: GPanel(name, width, height)
{
	cGfx = newGfx;
	serverInstance = newInstance;
	buildPanel();
}

void TestPanel::buildPanel()
{
	lblDrawGraph = new RULabel();
	lblDrawGraph->setWidth(150);
	lblDrawGraph->setHeight(40);
	lblDrawGraph->setX(530);
	lblDrawGraph->setY(0);
	lblDrawGraph->setText("Draw Graph");
	lblDrawGraph->setName("lblDrawGraph");
	addSubItem(lblDrawGraph);

	// Draw graph test
	drawGraph = new RUDrawComponent(getWidth() / 2, getHeight() / 2, RUGraph::QUADRANTS_ONE);
	drawGraph->setX(300);
	drawGraph->setY(60);
	drawGraph->setName("drawGraph");
	addSubItem(drawGraph);

	// Radius label
	RULabel* lblRadius = new RULabel();
	lblRadius->setX(drawGraph->getX());
	lblRadius->setY(drawGraph->getY() + drawGraph->getHeight() + 6);
	lblRadius->setWidth(160);
	lblRadius->setHeight(30);
	lblRadius->setText("Radius");
	lblRadius->setName("lblRadius");
	addSubItem(lblRadius);

	// Radius textbox
	tbRadius = new RUTextbox();
	tbRadius->setX(lblRadius->getX() + lblRadius->getWidth() + 6);
	tbRadius->setY(lblRadius->getY());
	tbRadius->setWidth(160);
	tbRadius->setHeight(30);
	tbRadius->setText("8");
	tbRadius->setName("tbRadius");
	tbRadius->setKeyListener(&GPanel::GuiCommander11);
	addSubItem(tbRadius);

	// Clear DrawGraph Button
	btnMode = new RUButton();
	btnMode->setX(lblRadius->getX());
	btnMode->setY(lblRadius->getY() + lblRadius->getHeight() + 6);
	btnMode->setWidth(160);
	btnMode->setHeight(30);
	btnMode->setText("Toggle: Circles");
	btnMode->setMouseDownListener(&GPanel::GuiCommander8);
	btnMode->setName("btnMode");
	addSubItem(btnMode);

	// Clear DrawGraph Button
	RUButton* btnClear = new RUButton("red");
	btnClear->setX(btnMode->getX());
	btnClear->setY(btnMode->getY() + btnMode->getHeight() + 6);
	btnClear->setWidth(100);
	btnClear->setHeight(30);
	btnClear->setText("Clear");
	btnClear->setMouseDownListener(&GPanel::GuiCommander3);
	btnClear->setName("btnClear");
	addSubItem(btnClear);

	// Test Layout
	GLinearLayout* testLayout = new GLinearLayout("testLayout");
	testLayout->setX(6);
	testLayout->setY(6);
	addSubItem(testLayout);

	// Test Image
	// RUImageComponent* loadTest = new RUImageComponent("datasets/images/xorgate.bmp");
	/*Image* newImage = new Image();
	newImage->LoadPNG("datasets/images/Vox.png");
	RUImageComponent* loadTest = new RUImageComponent(newImage);
	loadTest->setWidth(100);
	loadTest->setHeight(100);
	loadTest->setX(getWidth() / 2 - loadTest->getWidth() / 2);
	loadTest->setY(getHeight() * (2.0f / 3.0f) - loadTest->getHeight() / 2);
	loadTest->setName("loadTest");
	loadTest->setBGImage(newImage);//this makes it work, fix size by making GImage the rep invariant
	(not the url, but keep the url still)
	addSubItem(loadTest);*/

	// Tabber Dropdown
	/*
	tabberDropdown = new RUDropdown();
	tabberDropdown->setHeight(30);
	tabberDropdown->setWidth(160);
	tabberDropdown->setOptionsShown(3);
	tabberDropdown->setName("tabberDropdown");
	tabberDropdown->setMouseDownListener(&GPanel::GuiCommander5);
	testLayout->addSubItem(tabberDropdown);

	tabberDropdown->addOption("Simulation");
	tabberDropdown->addOption("NNCreator");
	tabberDropdown->addOption("Test");
	*/

	// tab navigation
	/*panelTabs = new RUTabContainer();
	panelTabs->setWidth(160);
	panelTabs->setHeight(30);
	panelTabs->setX(20);
	panelTabs->setY(10);
	panelTabs->setOptionsShown(3);
	panelTabs->setPadding(10);
	panelTabs->addTab(" Simulation");
	panelTabs->addTab(" NNCreator");
	panelTabs->addTab(" Test");
	panelTabs->setMouseDownListener(&GPanel::GuiCommander5);

	panelTabs->setName("panelTabs");
	addSubItem(panelTabs);*/

	RULabel* panelTabsBackground = new RULabel();
	panelTabsBackground->setWidth(530);
	panelTabsBackground->setHeight(50);
	panelTabsBackground->setX(0);
	panelTabsBackground->setY(0);
	panelTabsBackground->setBGColor(RUColors::DEFAULT_COMPONENT_BACKGROUND);
	panelTabsBackground->setText("");
	panelTabsBackground->setName("panelTabsBackground");
	addSubItem(panelTabsBackground);

	GLinearLayout* testButtonsLayout = new GLinearLayout("testButtonsLayout");
	testButtonsLayout->setX(20);
	testButtonsLayout->setY(getHeight() / 2);
	testButtonsLayout->setPadding(20); // 20=padding
	addSubItem(testButtonsLayout);

	// Trader Test Button
	RUButton* traderButton = new RUButton();
	traderButton->setWidth(200);
	traderButton->setHeight(30);
	traderButton->setText("Trader Test");
	traderButton->setMouseDownListener(&GPanel::GuiCommander6);
	traderButton->setName("traderButton");
	testButtonsLayout->addSubItem(traderButton);

	// 3D Test Button
	RUButton* threeDimButton = new RUButton();
	threeDimButton->setWidth(100);
	threeDimButton->setHeight(30);
	threeDimButton->setText("3D Test");
	threeDimButton->setMouseDownListener(&GPanel::GuiCommander4);
	threeDimButton->setName("threeDimButton");
	testButtonsLayout->addSubItem(threeDimButton);

	// Networking Test Button
	RUButton* netButton = new RUButton();
	netButton->setWidth(100);
	netButton->setHeight(30);
	netButton->setText("GNet Test");
	netButton->setMouseDownListener(&GPanel::GuiCommander1);
	netButton->setName("netButton");
	testButtonsLayout->addSubItem(netButton);

	// Checkbox
	RUCheckbox* checkboxTest = new RUCheckbox("Checkbox");
	checkboxTest->setHeight(30);
	checkboxTest->setWidth(160);
	checkboxTest->setName("checkboxTest");
	testButtonsLayout->addSubItem(checkboxTest);

	// Easter Egg
	RUImageComponent* testImageComp =
		new RUImageComponent("resources/gui/Components/robotstatic.bmp");
	testImageComp->setX(getWidth() / 2 - 8);
	testImageComp->setY(getHeight() - 30);
	testImageComp->setWidth(16);
	testImageComp->setHeight(24);
	testImageComp->setName("testImageComp");
	testImageComp->setMouseDownListener(&GPanel::GuiCommander2);
	addSubItem(testImageComp);

	// Tab Container
	RUTabContainer* testTabContainer = new RUTabContainer();
	testTabContainer->setWidth(160);
	testTabContainer->setHeight(40);
	testTabContainer->setX(10);
	testTabContainer->setY(600);
	testTabContainer->setOptionsShown(4);
	testTabContainer->setPadding(20);
	testTabContainer->setBGColor(RUColors::DEFAULT_COLOR_BACKGROUND);
	testTabContainer->addTab("Checkbox");
	testTabContainer->addTab("Draw Graph");
	testTabContainer->addTab("Probiscus");
	testTabContainer->setSelectedTab("Draw Graph");
	testTabContainer->addItemToTab("Checkbox", checkboxTest);
	testTabContainer->addItemToTab("Draw Graph", lblDrawGraph);
	testTabContainer->addItemToTab("Draw Graph", drawGraph);
	testTabContainer->addItemToTab("Draw Graph", lblRadius);
	testTabContainer->addItemToTab("Draw Graph", tbRadius);
	testTabContainer->addItemToTab("Draw Graph", btnMode);
	testTabContainer->addItemToTab("Draw Graph", btnClear);

	testTabContainer->setName("testTabContainer");
	addSubItem(testTabContainer);
}

void TestPanel::onStart()
{
	// printf("RUComponentContainer Size: %ld\n", ruComponentContainer.size());
}

void TestPanel::GuiCommander1(const std::string& cmpName, int x, int y)
{
	if (!serverInstance)
		return;

	std::string serverIP = "69.126.139.205";
	std::string clientName = "Mar";
	serverInstance->LaunchInstance(serverIP, clientName);
}

void TestPanel::GuiCommander2(const std::string& cmpName, int x, int y)
{
	// msgbox test
	MsgBox("Secret Title", "This is a spooky description for RUMsgBox. This is an extra sentence.",
		   RUMsgBox::INPUTBOX);
}

void TestPanel::GuiCommander3(const std::string& cmpName, int x, int y)
{
	drawGraph->clear(true);
}

void TestPanel::GuiCommander4(const std::string& cmpName, int x, int y)
{
	if (!cGfx)
		return;

	cGfx->changeRenderStatus(gfxpp::_3D);
}

void TestPanel::GuiCommander5(const std::string& cmpName, int x, int y)
{
	// Switch panels
	/*if (panelTabs->getTabSelected() == 0)
	{
		Frontend::simulationPanel->show();
		Frontend::nnCreatorPanel->hide();
		hide();
	}
	else if (panelTabs->getTabSelected() == 1)
	{
		Frontend::simulationPanel->hide();
		Frontend::nnCreatorPanel->show();
		hide();
	}
	else if (panelTabs->getTabSelected() == 2)
	{
		Frontend::simulationPanel->hide();
		Frontend::nnCreatorPanel->hide();
		show();
	}

	Frontend::simulationPanel->panelTabs->setSelectedTab(panelTabs->getTabSelected());
	Frontend::nnCreatorPanel->panelTabs->setSelectedTab(panelTabs->getTabSelected());*/
}

// test the trader and adding orders
void TestPanel::GuiCommander6(const std::string& cmpName, int x, int y)
{
	if (!serverInstance)
		return;

	std::string serverIP = "127.0.0.1"; // 69.126.139.205
	GNet::Connection* cConnection = serverInstance->getConnection(serverIP);
	if (!cConnection)
		return;

	shmea::GList wData;
	// wData.addString("Trader_Run");
	wData.addString("Bad_Request");
	wData.addLong(1);
	wData.addString("AAPL");
	wData.addString("12-18-2018");
	wData.addString("12-19-2018");
	wData.addBoolean(true);
	wData.addFloat(1000.0);
	wData.addFloat(2.0);
	serverInstance->NewService(wData, cConnection);
	usleep(100000);

	wData.clear();
	// wData.addString("Add_Order");
	wData.addString("Bad_Request");
	wData.addLong(1);
	wData.addLong(1);
	wData.addLong(1545129180000);
	wData.addInt(2);
	wData.addString("AAPL");
	wData.addFloat(105.31);
	wData.addInt(500);
	serverInstance->NewService(wData, cConnection);
	usleep(100000);

	wData.clear();
	// wData.addString("Add_Order");
	wData.addString("Bad_Request");
	wData.addLong(1);
	wData.addLong(2);
	wData.addLong(1545129300000);
	wData.addInt(2);
	wData.addString("AAPL");
	wData.addFloat(110.31);
	wData.addInt(-500);
	serverInstance->NewService(wData, cConnection);
	usleep(100000);

	wData.clear();
	// wData.addString("Add_Order");
	wData.addString("Bad_Request");
	wData.addLong(1);
	wData.addLong(3);
	wData.addLong(1545129540000);
	wData.addInt(2);
	wData.addString("AAPL");
	wData.addFloat(115.31);
	wData.addInt(500);
	serverInstance->NewService(wData, cConnection);
	usleep(100000);

	wData.clear();
	// wData.addString("Add_Order");
	wData.addString("Bad_Request");
	wData.addLong(1);
	wData.addLong(3);
	wData.addLong(1545129660000);
	wData.addInt(0);
	wData.addString("AAPL");
	wData.addFloat(115.31);
	wData.addInt(500);
	serverInstance->NewService(wData, cConnection);
}

void TestPanel::GuiCommander7(const std::string& cmpName, int x, int y)
{
	//
}

void TestPanel::GuiCommander8(const std::string& cmpName, int x, int y)
{
	if (drawGraph && btnMode)
	{
		//
		drawGraph->toggleMode();
		if (drawGraph->getMode() == RUDrawComponent::MODE_CIRCLES)
			btnMode->setText("Toggle: Circles");
		else if (drawGraph->getMode() == RUDrawComponent::MODE_NELLIPSE)
			btnMode->setText("Toggle: n-Ellipse");
	}
}

/*!
 * @brief lose focus handler
 * @details handles a NNCreator lose focus event
 */
void TestPanel::GuiCommander9()
{
	//
}

void TestPanel::GuiCommander10(const std::string& cmpName, int x, int y)
{
	//
}

void TestPanel::GuiCommander11(char eventKeyPressed)
{
	drawGraph->setPenWidth(atof(tbRadius->getText().c_str()));
}

void TestPanel::GuiCommander12(const std::string& cmpName, int x, int y)
{
	//
}

void TestPanel::GuiCommander13(const std::string& cmpName, int x, int y)
{
	//
}

void TestPanel::GuiCommander14(const std::string& cmpName, int x, int y)
{
	//
}

void TestPanel::GuiCommander15(int newIndex)
{
	//
}
