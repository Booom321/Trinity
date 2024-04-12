
#include <crtdbg.h>
#include <Trinity/Core/Utilities/CommandLine.h>
#include <vector>
std::vector<std::string> SplitString(const std::string& string, const std::string& delimiters)
{
	size_t start = 0;
	size_t end = string.find_first_of(delimiters);

	std::vector<std::string> result;

	while (end <= std::string::npos)
	{
		std::string token = string.substr(start, end - start);
		if (!token.empty())
			result.push_back(token);

		if (end == std::string::npos)
			break;

		start = end + 1;
		end = string.find_first_of(delimiters, start);
	}

	return result;
}
static inline TDynamicArray<std::string> Split(const std::string& text, char sep)
{
	TDynamicArray<std::string> tokens;

	std::string working_string;
	working_string.reserve(text.length());

	for (char ch : text) {
		if (ch == sep) {
			tokens.EmplaceBack(working_string);
			working_string.clear();
			continue;
		}

		working_string += ch;
	}

	if (!working_string.empty()) {
		tokens.EmplaceBack(working_string);
	}

	return tokens;
}
int main(int argc, char** argv)
{
	{
		TCommandLineParser* Parser = TCommandLineParser::GetInstance();

		Parser->AddCommandLineOption<TString>("-filepath", "");
		Parser->AddCommandLineOption<TInt32>("-number", 0);
		Parser->AddCommandLineOption<TBool>("-is_ok", false);
		Parser->AddCommandLineOption<TFloat>("-constant", 0.0f);
		Parser->AddCommandLineOption<TString>("-unknown", "adsfasd");

		for (int i = 0; i < Parser->GetCommanndLineOptionCount(); ++i)
		{
			Printf("{} = {}\n", Parser->CommandLineOptionAt(i).OptName.GetData(), Parser->CommandLineOptionAt(i).Value.GetData());
		}
		printf("========================\n");
		Parser->SetCommandLine(argc, argv);

		for (int i = 0; i < Parser->GetCommanndLineArgCount(); ++i)
		{
			Printf("{}\n", Parser->CommandLineArgAt(i).GetData());
		}

		if (!Parser->Parse())
		{
			Printf("Failed to parse!");
		}
		printf("========================\n");
		for (int i = 0; i < Parser->GetCommanndLineOptionCount(); ++i)
		{
			Printf("{} = {}\n", Parser->CommandLineOptionAt(i).OptName.GetData(), Parser->CommandLineOptionAt(i).Value.GetData());
		}
		printf("========================\n");
		TString val = Parser->GetStringOption("-filepath", TStringSearchCase::EIgnoreCase);
		Printf("{}\n", val);
		TCommandLineParser::DeleteInstance();
	}
	_CrtDumpMemoryLeaks();
	return 0;
}