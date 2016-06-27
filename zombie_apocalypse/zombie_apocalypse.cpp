// 
// a somewhat crazy implementation of conway's life game
//

#include "stdafx.h"
#include <iostream>
#include <utility>
#include <map>
#include <random>
#include <string>
#include <cmath>
#include <algorithm>

namespace zombies
{
	typedef std::pair<bool, bool> cell_t;

	// gaming field incapsulation; rectangular, width x height cells.
	template <typename D = uint8_t> class field
	{
		typedef std::pair<D, D> coords_t;
		typedef std::map<coords_t, cell_t> field_t;

		field_t _body;
		D _width, _height;
	public:
		// initialises gaming field, filling with random boolean values
		field(D width, D height)
			: _width(width), _height(height)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(0, 1);

			for (int y = 0; y < _height; y++)
				for (int x = 0; x < _width; x++)
				{
					auto current_coords = (coords_t)std::make_pair(x, y);

					// generate random number 
					auto val = std::lround(dis(gen));
					auto current_cell = (cell_t)std::make_pair((val % 2 == 1), false);
					_body.insert({current_coords, current_cell});
				}
		}

		void iterate()
		{
			// stage 1: iterate through cells, setting secondary attribute according to 
			// neighbours
			std::for_each(_body.begin(), _body.end(), [this](std::pair<const coords_t, cell_t>& cell0) 
			{
				int aliveNeighbors = 0 /*, totalNeighbors = 0*/;
				aliveNeighbors = std::count_if(_body.cbegin(), _body.cend(), [&](const std::pair<coords_t, cell_t>& cell) 
				{
					D x = cell.first.first;
					D y = cell.first.second;

					D x0 = cell0.first.first;
					D y0 = cell0.first.second;

					// do not count itself
					if ((x == x0) && (y == y0)) return false;

					// a neighbor does have coordinates different not more than 1 compared to original cell
					// and should be alive
					if ((abs(x0 - x) <= 1) && (abs(y0 - y) <= 1))
					{
						//std::cout << "a neighbour! alive=" << cell.second.first << std::endl;
						//totalNeighbors++;
						if (cell.second.first)
						{
							return true;
						}
					}
					return false;
				});
				//std::cout << "Cell: x=" << (int)cell0.first.first << " y=" << (int)cell0.first.second 
				//	<< " neighbors=" << totalNeighbors
				//	<< " alive=" << aliveNeighbors << std::endl;
				if (cell0.second.first == true) 
				{
					if (aliveNeighbors < 2) 
					{
						cell0.second.second = false;
						return;
					}
					if ((aliveNeighbors == 2) || (aliveNeighbors == 3))
					{
						cell0.second.second = true;
						return;
					}
					if (aliveNeighbors > 3) 
					{
						cell0.second.second = false; // overpopulation
						return;
					}
				}
				else
				{
					if (aliveNeighbors >= 3) cell0.second.second = true;
				}
			});

			// stage 2: copy secondary attribute to primary attribute
			std::for_each(_body.begin(), _body.end(), [](std::pair<const coords_t, cell_t>& cell0) {
				cell0.second.first = cell0.second.second;
			});
		}

		void display()
		{
			for (int y = _height - 1; y >= 0; y--)
			{
				for (int x = 0; x < _width; x++)
				{
					cell_t& val = _body.at(std::make_pair(x, y));
					std::cout << (val.first ? "X" : " ");
				}
				std::cout << std::endl;
			}
		}
	};
}

// entry point
int main()
{
	zombies::field<uint8_t> f(10, 10);

	// cycle
	do 
	{
		f.iterate();
		std::system("cls");
		f.display();
		std::system("pause");
	} 
	while (true);
	return 0;
}

