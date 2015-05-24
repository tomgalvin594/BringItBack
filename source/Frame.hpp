#ifndef BRINGITBACK_FRAME_HPP
#define BRINGITBACK_FRAME_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

//#include <Hx/Plinth.hpp>

class Frame : public sf::Drawable, public sf::Transformable
{
public:
	Frame();
	void setColor(const sf::Color& color);
	void setInnerRect(const sf::FloatRect& innerRect);
	void setOuterRect(const sf::FloatRect& outerRect);

private:
	sf::VertexArray m_vertices;
	sf::FloatRect m_innerRect;
	sf::FloatRect m_outerRect;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update();
};

#endif // BRINGITBACK_FRAME_HPP