#include "Parse.h"

int main(int argc, char ** argv)
{
	if (argc > 2)
	{
		rapidjson::Reader reader;
//		std::stringstream ss(argv[1]);
		std::ofstream out(argv[2]);
		if (!out)
		{
			std::cerr << "Could not open file for saving" << std::endl;
			
			return 1;
		}
		else
		{
			out << argv[1] << std::endl;
			
			return 0;
		}
	}
	
	return 0;
}