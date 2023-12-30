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
#include "ellipsecreator.h"
#include "Frontend/GItems/GItem.h"
#include "Frontend/GLayouts/GLinearLayout.h"
#include "Frontend/GUI/RUCheckbox.h"
#include "Frontend/GUI/RUDropdown.h"
#include "Frontend/GUI/RUImageComponent.h"
#include "Frontend/GUI/RUMsgBox.h"
#include "Frontend/GUI/RUTabContainer.h"
#include "Frontend/GUI/RUTable.h"
#include "Frontend/GUI/Text/RUButton.h"
#include "Frontend/GUI/Text/RULabel.h"
#include "Frontend/GUI/Text/RUTextbox.h"
#include "Frontend/Graphics/graphics.h"
#include "Frontend/RUGraph/RUGraph.h"
#include "Frontend/RUGraph/Graphable.h"
#include "Frontend/GFXUtilities/Circle.h"
#include "Frontend/GFXUtilities/Ellipse.h"
#include "crt0.h"
#include "main.h"
#include "services/gui_callback.h"

// using namespace shmea;
using namespace glades;

/*!
 * @brief EllipseCreatorPanel constructor
 * @details builds the EllipseCreator panel
 * @param name name of the panel
 * @param width width of the panel
 * @param height height of the panel
 */
EllipseCreatorPanel::EllipseCreatorPanel(const shmea::GString &name, int width,
                                         int height)
    : GPanel(name, width, height) 
{
  keepGraping = true;
  clickMode = MODE_CIRCLES;
  pointTypeFlag = TYPE_FOCAL_POINT;
  cFocalPoint = NULL;
  prevCircle = NULL;
  buildPanel();
}

EllipseCreatorPanel::EllipseCreatorPanel(GNet::GServer *newInstance,
                                         const shmea::GString &name, int width,
                                         int height)
    : GPanel(name, width, height) 
{
  keepGraping = true;
  clickMode = MODE_CIRCLES;
  pointTypeFlag = TYPE_FOCAL_POINT;
  cFocalPoint = NULL;
  prevCircle = NULL;
  buildPanel();
}

void EllipseCreatorPanel::buildPanel() 
{

	// Setup the gfx env
	int width=getWidth();
	int height=getHeight();

		int newTextColor=2;
		SDL_Color newBGColor={0xAA, 0xAA, 0xAA, 0xFF};
		

		GLinearLayout* mainLayout = new GLinearLayout("EllipseCreatorPanel");
		mainLayout->setOrientation(GLinearLayout::VERTICAL);
		mainLayout->setX(0);
		mainLayout->setY(0);
		addSubItem(mainLayout);

		dotGraph = new RUGraph(width/2.0f, height/2.0f);
		dotGraph->setVisible(true);
		dotGraph->setDotMatrixEnabled(true);
		dotGraph->setName("dotGraph");
		dotGraph->setMouseDownListener(GeneralListener(this, &EllipseCreatorPanel::onMouseDown));
		mainLayout->addSubItem(dotGraph);

		// Instructions
		lblmode = new RULabel();
		lblmode->setWidth(200);
		lblmode->setHeight(40);
		lblmode->setText("Mode: Circles");
		lblmode->setName("lblmode");
		lblmode->setFontColor(newTextColor);
		lblmode->setVisible(true);

		mainLayout->addSubItem(lblmode);
		// Instructions
		RULabel* lblhelp1 = new RULabel();
		lblhelp1->setWidth(800);
		lblhelp1->setHeight(40);
		lblhelp1->setBGColor(newBGColor);
		lblhelp1->setText("Click once to set a focal point, once more to set a radius.");
		lblhelp1->setName("lblhelp1");
		lblhelp1->setFontColor(newTextColor);
		lblhelp1->setVisible(true);
		mainLayout->addSubItem(lblhelp1);

		RULabel* lblhelp2 = new RULabel();
		lblhelp2->setWidth(width/2);
		lblhelp2->setHeight(40);
		lblhelp2->setText("For Ellipse mode, any subsequent clicks add foci.");
		lblhelp2->setName("lblhelp2");
		lblhelp2->setFontColor(newTextColor);
		lblhelp2->setVisible(true);
		mainLayout->addSubItem(lblhelp2);

		// Clear all circles button
		RUButton* btnclear = new RUButton();
		btnclear->setWidth(200);
		btnclear->setHeight(40);
		btnclear->setX(width - btnclear->getWidth() - 6);
		btnclear->setY(height - btnclear->getHeight() - 6);
		btnclear->setName("btnclear");
		btnclear->setText("  Clear Circles");
		btnclear->setFontColor(newTextColor);
		btnclear->setVisible(true);
		btnclear->setMouseDownListener(GeneralListener(this, &EllipseCreatorPanel::clearCircleHelper));
		addSubItem(btnclear);

		// Toggle graph mode
		RUButton* btnmode = new RUButton();
		btnmode->setWidth(200);
		btnmode->setHeight(40);
		btnmode->setX(width - btnmode->getWidth() - btnclear->getWidth() - 12);
		btnmode->setY(height - btnmode->getHeight() - 6);
		btnmode->setName("btnmode");
		btnmode->setText("  Toggle Mode");
		btnmode->setFontColor(newTextColor);
		btnmode->setVisible(true);
		btnmode->setMouseDownListener(GeneralListener(this, &EllipseCreatorPanel::toggleModeHelper));
		addSubItem(btnmode);
}


void EllipseCreatorPanel::clearCircleHelper(const shmea::GString& label, int x, int y) 
{
    if (dotGraph)
	dotGraph->clear(true);
    
    circles.clear();


    if(cFocalPoint)
	delete cFocalPoint;
    cFocalPoint = NULL;
    prevCircle = NULL;

}

void EllipseCreatorPanel::toggleModeHelper(const shmea::GString& label, int eventX, int eventY)
{
    clickMode = !clickMode;

    std::cout << "Changed click mode" << std::endl;
    if (dotGraph && lblmode) 
    {
	std::cout << clickMode << std::endl;
	if(clickMode == MODE_CIRCLES)
	{
	    lblmode->setText("Mode: Circles");
	} else if (clickMode == MODE_ELLIPSES)
	{
	    lblmode->setText("Mode: Ellipses");
	    std::cout << "Change text " <<std::endl;
	}
    }
}

void EllipseCreatorPanel::addCircle(const Point2* focalPoint, double radius) 
{
    printf("Circle(%f, %f, %f)\n", focalPoint->getX(), focalPoint->getY(), radius);

    SDL_Color newCircleColor = {0x00, 0x00, 0xFF, 0xFF};
    Circle* newCircle = new Circle();
    newCircle->setCenter(focalPoint);
    newCircle->setRadius(radius);
    prevCircle = newCircle;

    
    if(newCircle)
	dotGraph->add("circle", newCircle, newCircleColor);
    circles.insert(std::pair<int, Circle*>(dotGraph->getGraphSize()-1, newCircle));

}

void EllipseCreatorPanel::addEllipse(const Point2* focalPoint, double radius)
{
    printf("Ellipse(%f, %f, %f)\n", focalPoint->getX(), focalPoint->getY(), radius);

    SDL_Color newEllipseColor = {0x00, 0x00, 0xFF, 0xFF};
    Ellipse* newEllipse = new Ellipse();
    newEllipse->setRadius(radius);
    newEllipse->addFocalPoint(focalPoint);

    if(newEllipse)
        dotGraph->add("ellipse", newEllipse, newEllipseColor);
    
}

void EllipseCreatorPanel::onMouseDown(const shmea::GString& label, int eventX, int eventY) 
{
    if(clickMode == MODE_CIRCLES)
    {
	if (pointTypeFlag == TYPE_RADIUS) 
	{
	    if(!cFocalPoint)
		return;
	    //Radius
	    double radius = sqrt(pow(cFocalPoint->getX() - ((double)eventX), 2.0f) + pow(cFocalPoint->getY() - ((double)eventY), 2.0f));
	    EllipseCreatorPanel::addCircle(cFocalPoint, radius);

	    if(cFocalPoint)
		delete cFocalPoint;
	    cFocalPoint = NULL;

	}
	else if (pointTypeFlag == TYPE_FOCAL_POINT) 
	{
	    //Focal Point
	    cFocalPoint = new Point2(eventX, eventY);
	}

	pointTypeFlag = !pointTypeFlag;
    } else if(clickMode == MODE_ELLIPSES)
    {
	printf("%d\n", pointTypeFlag);
	if (pointTypeFlag == TYPE_RADIUS) 
	{
	    if(!cFocalPoint)
		return;
	    //Radius
	    double radius = sqrt(pow(cFocalPoint->getX() - ((double)eventX), 2.0f) + pow(cFocalPoint->getY() - ((double)eventY), 2.0f));
	    SDL_Color circleColor = RUColors::DEFAULT_COLOR_LINE;
	    EllipseCreatorPanel::addEllipse(new Point2(cFocalPoint->getX(), cFocalPoint->getY()), radius);
//	    dotGraph->add("circle", new Point2(cFocalPoint->getX(), cFocalPoint->getY()), circleColor);

	    if(cFocalPoint)
		delete cFocalPoint;
	    cFocalPoint = NULL;
	    //Switch the type of point we are waiting for
	    pointTypeFlag = !pointTypeFlag;

	} else if (pointTypeFlag == TYPE_FOCAL_POINT) 
	{
	    //Focal Point
	    cFocalPoint = new Point2(eventX, eventY);

	    if(dotGraph->getGraphSize() == 0)
	    {
		    pointTypeFlag = !pointTypeFlag;
	    } else 
	    {
		if(prevCircle)
		    prevCircle->setCenter(cFocalPoint);

		if(cFocalPoint)
		    delete cFocalPoint;
		cFocalPoint = NULL;

	    }

	}
    }   
}

EllipseCreatorPanel::~EllipseCreatorPanel() 
{

}

void EllipseCreatorPanel::updateFromQ(const shmea::ServiceData* data) 
{
	
}

void EllipseCreatorPanel::onStart()
{
}
