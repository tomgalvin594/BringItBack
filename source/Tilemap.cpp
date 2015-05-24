#include "Tilemap.hpp"


Tilemap::Tilemap() :
m_tiles(gridSize.x * gridSize.y),
m_vertices(sf::Quads, 4 * gridSize.x * gridSize.y),
m_offset(0.f, 0.f),
m_isEntering(true)
{
	hx::Random random;
	for (unsigned int gridY{ 0 }; gridY < gridSize.y; ++gridY)
	{
		for (unsigned int gridX{ 0 }; gridX < gridSize.x; ++gridX)
			m_tiles[gridY * gridSize.x + gridX] = random.value(-1, 12);
	}
	update();
}

void Tilemap::update()
{
	for (unsigned int gridY{ 0 }; gridY < gridSize.y; ++gridY)
	{
		for (unsigned int gridX{ 0 }; gridX < gridSize.x; ++gridX)
		{
			const unsigned int vertexOffset{ (gridY * gridSize.x + gridX) * 4 };
			const sf::Vector2f topLeft{ static_cast<float>(gridX * tileSize.x), static_cast<float>(gridY * tileSize.y) };
			const sf::Vector2f bottomRight{ topLeft.x + tileSize.x, topLeft.y + tileSize.y };
			m_vertices[vertexOffset + 0].position = topLeft;
			m_vertices[vertexOffset + 1].position = { bottomRight.x, topLeft.y };
			m_vertices[vertexOffset + 2].position = bottomRight;
			m_vertices[vertexOffset + 3].position = { topLeft.x, bottomRight.y };

			sf::Color color{ sf::Colors::Black };
			const int currentTile{ m_tiles[gridY * gridSize.y + gridX] };

			if (currentTile == 0)
				color = sf::Colors::Brown;
			else if (currentTile == 1)
				color = sf::Colors::Red;
			else if (currentTile == 2)
				color = sf::Colors::Blue;
			else if (currentTile == 3)
				color = sf::Colors::Green;
			else if (currentTile == 4)
				color = sf::Colors::Cyan;
			else if (currentTile == 5)
				color = sf::Colors::Yellow;
			else if (currentTile == 6)
				color = sf::Colors::Orange;
			else if (currentTile == 7)
				color = sf::Colors::Purple;
			else if (currentTile == 8)
				color = sf::Colors::White;
			else if (currentTile == 9)
				color = sf::Colors::Lime;
			else if (currentTile == 10)
				color = sf::Colors::Grey;
			else if (currentTile == 11)
				color = sf::Colors::HotPink;
			else if (currentTile > 11)
				color = hx::Sfml::colorFromRgb(hx::Color::standardRandomColor());

			if (!m_isEntering && isDoorwayTile(currentTile))
				color = sf::Colors::DarkRed;

			for (unsigned int v{ 0 }; v < 4; ++v)
				m_vertices[vertexOffset + v].color = color;
		}
	}
	this->setOrigin(tileSize.x * m_offset.x, tileSize.y * m_offset.y);
}

void Tilemap::setTiles(std::vector<int>& tiles)
{
	const unsigned int leastNumberOfTiles{ static_cast<unsigned int>(hx::min(m_tiles.size(), tiles.size())) };
	for (unsigned int t{ 0 }; t < leastNumberOfTiles; ++t)
		m_tiles[t] = tiles[t];
}

void Tilemap::setTile(unsigned int tileNumber, int tile)
{
	if (tileNumber >= 0 && tileNumber < m_tiles.size())
		m_tiles[tileNumber] = tile;
}

void Tilemap::setOffset(hx::Vector2d offset)
{
	m_offset = sf::Vector2f{ static_cast<float>(offset.x), static_cast<float>(offset.y) };
}

void Tilemap::setEntering(bool isEntering)
{
	m_isEntering = isEntering;
}

bool Tilemap::isEntering()
{
	return m_isEntering;
}





// PRIVATE

void Tilemap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = NULL;
	target.draw(m_vertices, states);
}

bool Tilemap::isDoorwayTile(int tile)
{
	switch (tile)
	{
	case 2:	// blue door (the route)
	case 3:	// green door (open but not the route)
	case 4:	// cyan door (locked)
		return true;
	default:
		return false;
	}
}