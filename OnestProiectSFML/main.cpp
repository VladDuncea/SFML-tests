#include <SFML/Graphics.hpp>
#include <vector>

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH  900

#define  DRAG_AREA 20

bool insideShape(sf::Vector2f mousePos, sf::RectangleShape shape)
{
	sf::Vector2f shapePos = shape.getPosition();
	sf::Vector2f shapeSize = shape.getSize();
	//Check if x-axis is outside
	if (mousePos.x < shapePos.x || mousePos.x >(shapePos.x + shapeSize.x))
		return false;

	//Check if y-axis is outside
	if (mousePos.y<shapePos.y || mousePos.y >shapePos.y + shapeSize.y)
		return false;

	return true;
}

bool insideDragArea(sf::Vector2f mousePos, sf::RectangleShape shape)
{
	if (!insideShape(mousePos, shape))
		return false;
	sf::Vector2f shapePos = shape.getPosition();
	sf::Vector2f shapeSize = shape.getSize();
	//Check if its outside the drag area
	if (mousePos.x < shapePos.x+shapeSize.x - DRAG_AREA || mousePos.y < shapePos.y + shapeSize.y - DRAG_AREA)
		return false;

	return true;
}

int main()
{
	//Create window
	sf::RenderWindow window(sf::VideoMode( WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Onest");

	//Create cursors
	sf::Cursor cursorDefault;
	sf::Cursor cursorResize;
	cursorDefault.loadFromSystem(sf::Cursor::Arrow);
	cursorResize.loadFromSystem(sf::Cursor::SizeTopLeftBottomRight);

	//Create a square and color it red
	sf::RectangleShape shape2({ 100,100 });
	shape2.setFillColor(sf::Color::Red);

	//Local variables
	bool isMovingShape = false;
	bool isResizingShape = false;

	//Offset from corner
	sf::Vector2f offset;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			//Handle closing
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}	
			//Handle click
			if (event.type == sf::Event::MouseButtonPressed)
			{
				//Get mouse pos
				sf::Vector2f mousePos = (sf::Vector2f) sf::Mouse::getPosition(window);
				if (insideDragArea(mousePos, shape2))
				{
					isResizingShape = true;
					offset = mousePos;
				}else if (insideShape(mousePos,shape2))
				{
					isMovingShape=true;
					offset = shape2.getPosition() - mousePos;
				}
			}
			//Handle mouse depress
			if (event.type == sf::Event::MouseButtonReleased)
			{
				printf("Mouse depressed!");
				isMovingShape = false;
				isResizingShape = false;
			}
			if (event.type == sf::Event::MouseMoved)
			{
				//Get mouse pos
				sf::Vector2f mousePos = (sf::Vector2f) sf::Mouse::getPosition(window);
		

				//Check if mouse needs to be changed
				if (insideDragArea(mousePos, shape2))
				{
					window.setMouseCursor(cursorResize);
				}
				else if (!isResizingShape)
				{
					window.setMouseCursor(cursorDefault);
				}

				//Resize shape
				if (isResizingShape)
				{
					sf::Vector2f newSize = mousePos -shape2.getPosition();
					if (newSize.x < 1)
						newSize.x = 1;
					if (newSize.y < 1)
						newSize.y = 1;
					if (mousePos.x > WINDOW_WIDTH)
						newSize.x = shape2.getSize().x;
					if (mousePos.y > WINDOW_WIDTH)
						newSize.y = shape2.getSize().y;
					shape2.setSize(newSize);
				}
				//Move shape if it is clicked
				if (isMovingShape)
				{
					//Move shape taking offset in consideration
					sf::Vector2f updatedPos = mousePos + offset;

					//Bound to window
						//X bounding
					if (updatedPos.x < 0)
						updatedPos.x = 0;
					if (updatedPos.x + shape2.getSize().x > WINDOW_WIDTH)
						updatedPos.x = WINDOW_WIDTH - shape2.getSize().x;
						//Y bouding
					if (updatedPos.y < 0)
						updatedPos.y = 0;
					if (updatedPos.y + shape2.getSize().y > WINDOW_HEIGHT)
						updatedPos.y = WINDOW_HEIGHT - shape2.getSize().y;
					shape2.setPosition(updatedPos);
				}
			}
		}

		window.clear();
		window.draw(shape2);
		window.display();
	}

	return 0;
}