#include "Application/application.h"

#include <iostream>
#include <stdexcept>


int main()
{
	try
	{
		Application app;
		app.run();
	}
	catch (std::exception& e)
	{
		std::cout << "\nEXCEPTION: " << e.what() << std::endl;
		//char i;
		//std::cin >> i;
	}
}