#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <utility>
#include <SFML/Graphics.hpp>

// std::string readFile (std::string const & path)
// {
// 	std::ifstream file (path.c_str ());
// 	if (!file.is_open ())
// 		throw std::runtime_error ("Failed to open file");
// 	std::stringstream read;
// 	read << file.rdbuf ();
// 	file.close ();
// 	return read.str ();
// }

time_t getCurrentTime ()
{
	time_t current_time = time (nullptr);
	if (current_time == -1)
		throw std::runtime_error ("Failed to get current time");
	return current_time;
}

time_t getPassedTime (time_t start)
{
	time_t current_time = getCurrentTime ();
	if (current_time == -1)
		throw std::runtime_error ("Failed to get passed time");
	return (difftime (current_time, start));
}

// std::vector<std::pair<int, int>> createEnemies ()
// {
// 	std::string read = readFile ("map");
// 	int x = 0;
// 	int y = 0;
// 	int index = 0;
// 	std::vector<std::pair<int, int>> enemies;
// 	while (index < read.size())
// 	{
// 		if (read[index] == '\n')
// 		{
// 			x = 0;
// 			y++;
// 		}
// 		else
// 		{
// 			if (read[index] == 'E')
// 				enemies.push_back (std::make_pair (x, y));
// 			x++;
// 		}
// 		index++;
// 	}
// 	return enemies;
// }

// int main()
// {
// 	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
// 	while (window.isOpen())
// 	{
// 		window.setFramerateLimit(60);
// 		while (window.pollEvent(event))
// 		{
// 			if (event.type == sf::Event::Closed)
// 				window.close();
// 			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
// 				window.close();
// 		}

// 		window.clear();
// 		// window.draw(...);
// 		window.display();
// 	}

// 	return 0;
// }

