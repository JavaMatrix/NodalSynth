#include <SFML/Graphics.hpp>
#include "RoundedRectangleShape.hpp"
#include <Windows.h>

void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(800, 600), "NodalSynth", sf::Style::Default, settings);
	sf::View view = window.getView();
	sf::RoundedRectangleShape shape(sf::Vector2f(200, 100), 10, 10);
	shape.setFillColor(sf::Color::Color(249, 188, 69));
	shape.setOutlineThickness(3.f);
	shape.setOutlineColor(sf::Color::Color(62, 52, 73));
	bool mouseDown = false;
	sf::Vector2f lastMouse;

	while (window.isOpen())
	{
		sf::Event sfe;
		while (window.pollEvent(sfe))
		{
			if (sfe.type == sf::Event::Closed)
			{
				window.close();
				OutputDebugString("CLOSED\n");
			}
			else if (sfe.type == sf::Event::Resized)
			{
				sf::Vector2f old_sz = view.getSize();
				sf::Vector2f new_sz = sf::Vector2f(sfe.size.width, sfe.size.height);
				sf::Vector2f old_cn = view.getCenter();
				view = window.getView();
				view.setSize(sfe.size.width, sfe.size.height);
				view.setCenter(old_cn + ((new_sz - old_sz) / 2.f));
				window.setView(view);
				OutputDebugString("RESIZED\n");
			}
			else if (sfe.type == sf::Event::MouseButtonPressed)
			{
				if (sfe.mouseButton.button == sf::Mouse::Button::Middle)
				{
					mouseDown = true;
				}
				OutputDebugString("MOUSEDOWN\n");
			}
			else if (sfe.type == sf::Event::MouseButtonReleased)
			{
				if (sfe.mouseButton.button == sf::Mouse::Button::Middle)
				{
					mouseDown = false;
				}
				OutputDebugString("MOUSEUP\n");
			}
			else if (sfe.type == sf::Event::MouseWheelScrolled)
			{
				float zoomAmount = 1.1f;

				if (sfe.mouseWheelScroll.delta > 0)
				{
					zoomViewAt({ sfe.mouseWheelScroll.x, sfe.mouseWheelScroll.y }, window, (1.f / zoomAmount));
				}
				else if (sfe.mouseWheelScroll.delta < 0)
				{
					zoomViewAt({ sfe.mouseWheelScroll.x, sfe.mouseWheelScroll.y }, window, zoomAmount);
				}

				OutputDebugString(std::to_string(sfe.mouseWheelScroll.delta).c_str());
				OutputDebugString("SCROLL\n");
			}
		}

		sf::Vector2i mpos = sf::Mouse::getPosition(window);
		sf::Vector2f wpos = window.mapPixelToCoords(mpos);
		window.setTitle(std::to_string(wpos.x) + " " + std::to_string(wpos.y) + " | " + (mouseDown ? "!" : "."));

		if (mouseDown)
		{
			view = window.getView();
			view.setCenter(view.getCenter() + ((lastMouse - wpos) * 2.f));
			window.setView(view);
		}

		mpos = sf::Mouse::getPosition(window);
		wpos = window.mapPixelToCoords(mpos);
		lastMouse = wpos;

		window.clear(sf::Color::Color(20, 20, 20));
		window.draw(shape);
		window.display();
	}

	return 0;
}

void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom)
{
	const sf::Vector2f beforeCoord{ window.mapPixelToCoords(pixel) };
	sf::View view{ window.getView() };
	view.zoom(zoom);
	window.setView(view);
	const sf::Vector2f afterCoord{ window.mapPixelToCoords(pixel) };
	const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
	view.move(offsetCoords);
	window.setView(view);
}