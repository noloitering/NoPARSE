#include "../NoGUI/src/GUI.h"
#include "../NoMEM/src/NoMEM.h"

namespace NoGUI
{
	int savePage(std::shared_ptr< Page > pg);
	std::shared_ptr< Page > loadPage(std::string path);
}

namespace NoPARSE
{
	class Parser
	{
	private:
		std::unique_ptr< NoMEM::MEMManager > assets;
	public:
		Parser(std::unique_ptr< NoMEM::MEMManager > a)
			: assets(a) {}
	};
}