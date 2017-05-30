
//http://www.sfml-dev.org/tutorials/2.1/graphics-vertex-array.php
//based on TileMap.hpp
//this TileMap does not place at equally spaced intervals in the y direction
//will place each instrument in its designated location from the composition file

#ifndef TILE_MAP_COMPOSITION
#define TILE_MAP_COMPOSITION

#include "TileMap.hpp"
#include <iostream>
#include <vector>

class TileMapComposition : public sf::Drawable, public sf::Transformable//NEEDS WORK
{
public:

	bool load(const std::string& tileset, sf::Vector2u tileSize, std::vector<char>(&composition)[384])
	{
		// load the tileset texture
		if (!m_tileset.loadFromFile(tileset))
			return false;

		// resize the vertex array to fit the level size
		m_vertices.setPrimitiveType(sf::Quads);
		m_vertices.resize(384 * 5 * 4 + 384 * 5 * 4 + 384 * 4);//5 + 5 + 1 = 11 sprites per measure index

		// populate the vertex array, with one quad per tile
		for (unsigned int i = 0; i < 384; ++i)
		{
			unsigned int k = 0;//let k be the note count
			//unsigned int k_FS = 0;//let k_FS be the flat/sharp count
			if (!composition[i].empty())
			{
				int tileNumber, tu, tv;
				for (unsigned int j = 0; j < composition[i].size() - 1; ++j)
				{
					if (composition[i].at(j) != '+')
					{
						// get the current tile number (i.e. instrument type)
						tileNumber = (int)composition[i].at(j) - 97;

						// find its position in the tileset texture
						tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
						tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

						// get a pointer to the current tile's quad
						sf::Vertex* quad = &m_vertices[(11 * i + k) * 4];
						// get a pointer to the current tile's flat/sharp quad
						sf::Vertex* quad_FS = &m_vertices[(11 * i + 5 + k) * 4];

						// define quad's 4 texture coordinates
						quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
						quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
						quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
						quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);

						// staff position data
						j++;

						// get where to put the tile, note: tileNumber is being reused
						tileNumber = (int)composition[i].at(j) - 97;

						// define quad's 4 corners
						quad[0].position = sf::Vector2f((i % 32) * 40 + 84, tileNumber * 10 + (i / 32) * 230 + 142);
						quad[1].position = sf::Vector2f((i % 32) * 40 + 84 + tileSize.x, tileNumber * 10 + (i / 32) * 230 + 142);
						quad[2].position = sf::Vector2f((i % 32) * 40 + 84 + tileSize.x, tileNumber * 10 + (i / 32) * 230 + 142 + tileSize.y);
						quad[3].position = sf::Vector2f((i % 32) * 40 + 84, tileNumber * 10 + (i / 32) * 230 + 142 + tileSize.y);

						// define quad_FS's 4 corners // quad_FS will be paired up with where quad is
						quad_FS[0].position = sf::Vector2f((i % 32) * 40 + 64, tileNumber * 10 + (i / 32) * 230 + 142);
						quad_FS[1].position = sf::Vector2f((i % 32) * 40 + 64 + tileSize.x, tileNumber * 10 + (i / 32) * 230 + 142);
						quad_FS[2].position = sf::Vector2f((i % 32) * 40 + 64 + tileSize.x, tileNumber * 10 + (i / 32) * 230 + 142 + tileSize.y);
						quad_FS[3].position = sf::Vector2f((i % 32) * 40 + 64, tileNumber * 10 + (i / 32) * 230 + 142 + tileSize.y);

						if (composition[i].at(j + 1) != '+')//check for natural, flat, or sharp
						{
							// natural, flat, or sharp data
							j++;

							// get the current tile number (i.e. flat or sharp), note: tileNumber is being reused
							tileNumber = (int)composition[i].at(j);

							if (tileNumber == 59)
								tileNumber = 20;
							else if (tileNumber == 35)
								tileNumber = 21;
							else
								tileNumber = 19;

							// find its position in the tileset texture
							tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
							tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

							// define quad_FS's 4 texture coordinates
							quad_FS[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
							quad_FS[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
							quad_FS[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
							quad_FS[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);

						}
						else//<-added this recently as the FS was just shifting position and not erasing... if any problems occur then this part may be the culprit
						{
							// define quad_FS's 4 texture coordinates
							quad_FS[0].texCoords = sf::Vector2f(19 * tileSize.x, 0);
							quad_FS[1].texCoords = sf::Vector2f(20 * tileSize.x, 0);
							quad_FS[2].texCoords = sf::Vector2f(20 * tileSize.x, 1);
							quad_FS[3].texCoords = sf::Vector2f(19 * tileSize.x, 1);
						}

						//increase note count
						k++;
					}
				}

				// get a pointer to the volume tile's quad
				sf::Vertex* quad_V = &m_vertices[(11 * i + 10) * 4];

				// get the current tile number (i.e. volume level), note: tileNumber is being reused
				tileNumber = (int)composition[i].back() - 97 + 22;

				// find its position in the tileset texture
				tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
				tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

				// define quad_V's 4 texture coordinates
				quad_V[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
				quad_V[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
				quad_V[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
				quad_V[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);

				// define quad_V's 4 corners
				quad_V[0].position = sf::Vector2f((i % 32) * 40 + 84, (i / 32) * 230 + 317);
				quad_V[1].position = sf::Vector2f((i % 32) * 40 + 84 + tileSize.x, (i / 32) * 230 + 317);
				quad_V[2].position = sf::Vector2f((i % 32) * 40 + 84 + tileSize.x, (i / 32) * 230 + 317 + tileSize.y);
				quad_V[3].position = sf::Vector2f((i % 32) * 40 + 84, (i / 32) * 230 + 317 + tileSize.y);
			}
			else
			{
				//this else statement is to erase the volume when it is "deleted" via undo

				// get a pointer to the volume tile's quad
				sf::Vertex* quad_V = &m_vertices[(11 * i + 10) * 4];

				// define quad_V's 4 texture coordinates
				quad_V[0].texCoords = sf::Vector2f(19 * tileSize.x, 0);
				quad_V[1].texCoords = sf::Vector2f(20 * tileSize.x, 0);
				quad_V[2].texCoords = sf::Vector2f(20 * tileSize.x, tileSize.y);
				quad_V[3].texCoords = sf::Vector2f(19 * tileSize.x, tileSize.y);
			}

			k = 5 - k;
			for (unsigned int j = 0; j < k; ++j)
			{
				// get a pointer to the current tile's quad
				sf::Vertex* quad = &m_vertices[(11 * i + (5 - k) + j) * 4];
				// get a pointer to the current tile's flat/sharp quad
				sf::Vertex* quad_FS = &m_vertices[(11 * i + 5 + (5 - k) + j) * 4];

				// define quad's 4 texture coordinates which should be the null tile
				quad[0].texCoords = sf::Vector2f(19 * tileSize.x, 0);
				quad[1].texCoords = sf::Vector2f(20 * tileSize.x, 0);
				quad[2].texCoords = sf::Vector2f(20 * tileSize.x, tileSize.y);
				quad[3].texCoords = sf::Vector2f(19 * tileSize.x, tileSize.y);

				// define quad's 4 corners
				quad[0].position = sf::Vector2f(0, 0);
				quad[1].position = sf::Vector2f(tileSize.x, 0);
				quad[2].position = sf::Vector2f(tileSize.x, tileSize.y);
				quad[3].position = sf::Vector2f(0, tileSize.y);

				// define quad_FS's 4 texture coordinates which should be the null tile
				quad_FS[0].texCoords = sf::Vector2f(19 * tileSize.x, 0);
				quad_FS[1].texCoords = sf::Vector2f(20 * tileSize.x, 0);
				quad_FS[2].texCoords = sf::Vector2f(20 * tileSize.x, tileSize.y);
				quad_FS[3].texCoords = sf::Vector2f(19 * tileSize.x, tileSize.y);

				// define quad_FS's 4 corners
				quad_FS[0].position = sf::Vector2f(0, 0);
				quad_FS[1].position = sf::Vector2f(tileSize.x, 0);
				quad_FS[2].position = sf::Vector2f(tileSize.x, tileSize.y);
				quad_FS[3].position = sf::Vector2f(0, tileSize.y);
			}

			/*k_FS = 5 - k_FS;
			for (unsigned int j = 0; j < k_FS; ++j)
			{
				// get a pointer to the current tile's quad
				sf::Vertex* quad_FS = &m_vertices[(11 * i + 5 + (5 - k_FS) + j) * 4];

				// define its 4 texture coordinates which should be the null tile
				quad_FS[0].texCoords = sf::Vector2f(19 * tileSize.x, 0);
				quad_FS[1].texCoords = sf::Vector2f(20 * tileSize.x, 0);
				quad_FS[2].texCoords = sf::Vector2f(20 * tileSize.x, tileSize.y);
				quad_FS[3].texCoords = sf::Vector2f(19 * tileSize.x, tileSize.y);

				// define its 4 corners
				quad_FS[0].position = sf::Vector2f(0, 0);
				quad_FS[1].position = sf::Vector2f(tileSize.x, 0);
				quad_FS[2].position = sf::Vector2f(tileSize.x, tileSize.y);
				quad_FS[3].position = sf::Vector2f(0, tileSize.y);
			}*/
		}
		return true;
	}

	//can only use load or loadflatsharp
	bool loadflatsharp(const std::string& tileset, sf::Vector2u tileSize, std::vector<char>(&composition)[384])
	{
		// load the tileset texture
		if (!m_tileset.loadFromFile(tileset))
			return false;

		// resize the vertex array to fit the level size
		m_vertices.setPrimitiveType(sf::Quads);
		m_vertices.resize(384 * 5 * 4);

		// populate the vertex array, with one quad per tile
		for (unsigned int i = 0; i < 384; ++i)
		{
			unsigned int k = 0;//let k be the note count
			if (!composition[i].empty())
			{
				for (unsigned int j = 0; j < composition[i].size() - 1; ++j)
				{
					if (composition[i].at(j) != '+')
					{
						// staff position data
						j++;

						// get a pointer to the current tile's quad
						sf::Vertex* quad = &m_vertices[(5 * i + k) * 4];

						// get where to put the tile, note: tileNumber is being reused
						int tileNumber = (int)composition[i].at(j) - 97;

						// define its 4 corners
						quad[0].position = sf::Vector2f((i % 32) * 40 + 64, tileNumber * 10 + (i / 32) * 230 + 142);
						quad[1].position = sf::Vector2f((i % 32) * 40 + 64 + tileSize.x, tileNumber * 10 + (i / 32) * 230 + 142);
						quad[2].position = sf::Vector2f((i % 32) * 40 + 64 + tileSize.x, tileNumber * 10 + (i / 32) * 230 + 142 + tileSize.y);
						quad[3].position = sf::Vector2f((i % 32) * 40 + 64, tileNumber * 10 + (i / 32) * 230 + 142 + tileSize.y);

						if (composition[i].at(j + 1) != '+')//check for natural, flat, or sharp
						{
							// natural, flat, or sharp data
							j++;

							// get the current tile number (i.e. flat or sharp)
							tileNumber = (int)composition[i].at(j);

							if (tileNumber == 59)
								tileNumber = 0;
							else if (tileNumber == 35)
								tileNumber = 1;
							else
								tileNumber = 2;

							// find its position in the tileset texture
							int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
							int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

							// define its 4 texture coordinates
							quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
							quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
							quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
							quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
						}
						else
						{
							// define its 4 texture coordinates which should be the null tile
							quad[0].texCoords = sf::Vector2f(2 * tileSize.x, 0);
							quad[1].texCoords = sf::Vector2f(3 * tileSize.x, 0);
							quad[2].texCoords = sf::Vector2f(3 * tileSize.x, tileSize.y);
							quad[3].texCoords = sf::Vector2f(2 * tileSize.x, tileSize.y);
						}

						//increase note count
						k++;
					}
				}
			}

			k = 5 - k;
			for (unsigned int j = 0; j < k; ++j)
			{
				// get a pointer to the current tile's quad
				sf::Vertex* quad = &m_vertices[(5 * i + (5 - k) + j) * 4];

				// define its 4 texture coordinates which should be the null tile
				quad[0].texCoords = sf::Vector2f(2 * tileSize.x, 0);
				quad[1].texCoords = sf::Vector2f(3 * tileSize.x, 0);
				quad[2].texCoords = sf::Vector2f(3 * tileSize.x, tileSize.y);
				quad[3].texCoords = sf::Vector2f(2 * tileSize.x, tileSize.y);

				// define its 4 corners
				quad[0].position = sf::Vector2f(0, 0);
				quad[1].position = sf::Vector2f(tileSize.x, 0);
				quad[2].position = sf::Vector2f(tileSize.x, tileSize.y);
				quad[3].position = sf::Vector2f(0, tileSize.y);
			}
		}
		return true;
	}

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the transform
		states.transform *= getTransform();

		// apply the tileset texture
		states.texture = &m_tileset;

		// draw the vertex array
		target.draw(m_vertices, states);
	}

	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
};

#endif