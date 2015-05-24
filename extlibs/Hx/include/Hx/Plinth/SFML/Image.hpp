//////////////////////////////////////////////////////////////////////////////
//
// Hx
// --
//
// Plinth
//
// Copyright(c) 2015 M.J.Silk
//
// This software is provided 'as-is', without any express or implied
// warranty.In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions :
//
// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software.If you use this software
// in a product, an acknowledgment in the product documentation would be
// appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not be
// misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
// M.J.Silk
// MJSilk2@gmail.com
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HX_PLINTH_SFML_IMAGE_HPP
#define HX_PLINTH_SFML_IMAGE_HPP

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

namespace hx
{
	namespace Sfml
	{
		namespace Image
		{

void clearWithColorButRetainTransparency(sf::Image& image, sf::Color color = sf::Color(255, 255, 255, 255));

		} // namspace Image
	} // namespace Sfml
} // namespace hx

#endif // HX_PLINTH_SFML_IMAGE_HPP