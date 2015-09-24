
#ifdef TEST_CONFIGUREPARSER

#include <queue>
#include <string>

int main(int argc, char* argv[])
{
	std::cout << "\n  Testing ConfigureParser module\n "
		<< std::string(32, '=') << std::endl;

	for (int i = 1; i<argc; ++i)
	{
		std::cout << "\n  Processing file " << argv[i];
		std::cout << "\n  " << std::string(16 + strlen(argv[i]), '-');

		ConfigParseToConsole configure;
		Parser* pParser = configure.Build();
		try
		{
			if (pParser)
			{
				while (configure.Attach(argv[i]))
				{
					std::cout << "\n  could not open file " << argv[i] << std::endl;
					int j = 0;
					switch (j){
					case 0:
						std::cout << "\n case 0" << std::endl;
						break;
					default:
						break;
					}
					continue;
				}
			}
			else
			{
				std::cout << "\n\n  Parser not built\n\n";
				return 1;
			}

		}
		catch (std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
		}
		std::cout << "\n\n";
	}
}

#endif
