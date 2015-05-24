#include "Frame.hpp"


Frame::Frame():
m_vertices(sf::TrianglesStrip, 13)
{

}

void Frame::setColor(const sf::Color& color)
{
	for (unsigned int v{ 0 }; v < m_vertices.getVertexCount(); ++v)
		m_vertices[v].color = color;
}

void Frame::setInnerRect(const sf::FloatRect& innerRect)
{
	m_innerRect = innerRect;
	update();
}

void Frame::setOuterRect(const sf::FloatRect& outerRect)
{
	m_outerRect = outerRect;
	update();
}


// PRIVATE

void Frame::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = NULL;
	target.draw(m_vertices, states);
}

void Frame::update()
{
	m_vertices[0].position = { m_innerRect.left, m_outerRect.top };
	m_vertices[1].position = { m_innerRect.left, m_innerRect.top };
	m_vertices[2].position = { m_outerRect.left + m_outerRect.width, m_outerRect.top };
	m_vertices[3].position = { m_outerRect.left + m_outerRect.width, m_innerRect.top };
	m_vertices[4].position = { m_innerRect.left + m_innerRect.width, m_innerRect.top };
	m_vertices[5].position = { m_outerRect.left + m_outerRect.width, m_outerRect.top + m_outerRect.height};
	m_vertices[6].position = { m_innerRect.left + m_innerRect.width, m_outerRect.top + m_outerRect.height };
	m_vertices[7].position = { m_innerRect.left + m_innerRect.width, m_innerRect.top + m_innerRect.height };
	m_vertices[8].position = { m_outerRect.left, m_outerRect.top + m_outerRect.height };
	m_vertices[9].position = { m_outerRect.left, m_innerRect.top + m_innerRect.height };
	m_vertices[10].position = { m_innerRect.left, m_innerRect.top + m_innerRect.height };
	m_vertices[11].position = { m_outerRect.left, m_outerRect.top };
	m_vertices[12].position = m_vertices[0].position;
}