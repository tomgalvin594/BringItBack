#ifndef BRINGITBACK_TILEMAP_HPP
#define BRINGITBACK_TILEMAP_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <Hx/Plinth/Generic.hpp>
#include <Hx/Plinth/Random.hpp>
#include <Hx/Plinth/Color.hpp>
#include <Hx/Plinth/SFML/Generic.hpp>
#include <Hx/Plinth/SFML/ExtendedColorList.hpp>

//#include <Hx/Plinth.hpp>

class Tilemap : public sf::Drawable, public sf::Transformable // transformable is to offset entire grid (using setOrigin) rather than offset each vertex
{
public:
	const sf::Vector2u gridSize{ 17u, 17u }; // number of tiles visible on screen at once (16x16 with additional tiles in either direction for scroll/transition)
	const sf::Vector2u tileSize{ 32u, 32u };

	Tilemap();
	void update();
	void setTiles(std::vector<int>& tiles);
	void setTile(unsigned int tileNumber, int tile);
	void setOffset(hx::Vector2d offset);
	void setEntering(bool isEntering = true);
	bool isEntering();

private:
	sf::VertexArray m_vertices;
	std::vector<int> m_tiles;
	sf::Vector2f m_offset;
	bool m_isEntering; // when playing is entering, the doorways are marked. when leaving, the doorways all look alike

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	bool isDoorwayTile(int tile);
};

#endif // BRINGITBACK_TILEMAP_HPP