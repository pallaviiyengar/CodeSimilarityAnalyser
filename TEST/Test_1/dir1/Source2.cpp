#include "Parser.h"
#include "SemiExpression.h"
#include "Tokenizer.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"


class ConfigParseToConsole2 : IBuilder
{
public:
	ConfigParseToConsole2() {
		int argc = 10;
		for (int i = 1; i < argc; ++i)
		{
			try
			{
				if (pParser)
				{

					std::cout << "\n  could not open file " << argv[i] << std::endl;
					continue;

				}
			}
			catch (std::exception& ex)
			{
				std::cout << "\n\n    " << ex.what() << "\n\n";
			}
		}
	}
	bool Attach(const std::string& name, bool isFile = true);

private:
	// Builder must hold onto all the pieces

	Toker* pToker;
	SemiExp* pSemi;
	Parser* pParser;
	Repository* pRepo;

	// add folding rules

	FoldingRules* pFR;

	// add Rules and Actions

	BeginningOfScope* pBeginningOfScope;
	HandlePush* pHandlePush;
	EndOfScope* pEndOfScope;
	HandlePop* pHandlePop;
	FunctionDefinition* pFunctionDefinition;
	PushFunction* pPushFunction;

	LoopKeyword* pLoopKeyword;
	TypeKeyword* pTypeKeyword;
	BracelessScope* pBracelessScope;

	PushLoopKeyword* pPushLoopKeyword;
	PushTypeKeyword* pPushTypeKeyword;
	PushBracelessScope* pPushBracelessScope;

	// prohibit copies and assignments

	ConfigParseToConsole(const ConfigParseToConsole&);
	ConfigParseToConsole& operator=(const ConfigParseToConsole&);
};
