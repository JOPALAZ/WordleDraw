#include "pch.h"
#include "WordleDrawCore.h"
#include "WordValidators.h"

using std::string;
using std::vector;

int calculateOcurrences(const string& word, const char& ch, const size_t& toPos)
{
	int occurences = 0;
	for (size_t pos = 0; pos < word.size() && pos <= toPos;++pos)
	{
		if(word.at(pos) == ch)
		{
			occurences += 1;
		}
	}
	return occurences;
}

WordleDrawCore::WordleDrawCore(const std::filesystem::path& pathToRefences)
{
	ConstructClass(pathToRefences);
}
WordleDrawCore::WordleDrawCore(std::filesystem::path& pathToRefences)
{
	ConstructClass(pathToRefences);
}

void WordleDrawCore::ConstructClass(const std::filesystem::path& pathToRefences)
{
	LoadReferences(pathToRefences);
	if(WordsReferences.empty())
	{
		throw std::runtime_error("No reference words were loaded.");
	}
}

void WordleDrawCore::LoadReferences(const std::filesystem::path& pathToRefences)
{
	std::ifstream filestream;
	try
	{
		filestream.open(pathToRefences, std::ios_base::in);
	}
	catch(std::exception ex)
	{
		throw std::runtime_error("Couldn't open a file for refences. Path = [" + pathToRefences.string() + "]. Inner exception [" + ex.what() + "]");
	}
	string line = "";
	std::optional<size_t> lastLength;
	while (filestream >> line)
	{
		if(!lastLength.has_value() || lastLength.value() == line.size())
		{
			WordsReferences.push_back(line);
			lastLength.emplace(line.size());
		}
		else
		{
			throw std::runtime_error("Length of loaded words is inconsistent.");
		}

	}
}

vector<string> WordleDrawCore::GetWordsForBitmap(const WordleBitmap& bitmap, const std::string& answer, const ValidatorType& validatorType)
{
	vector<string> words;
	std::string validatorNames = "";
	words.reserve(6);

	std::vector<std::unique_ptr<IWordValidator>> validators;

	GenerateValidators(validators, validatorType);

	for(const std::unique_ptr<IWordValidator>& validator : validators)
	{
		try
		{
			words.clear();
			validatorNames += validator.get()->GetValidatorName() + ", ";
			for (const vector<bool>& line : bitmap)
			{
				words.push_back(WordleDrawCore::MatchWordByLine(line, validator, answer));
			}

			std::cout << "Found words using validator [" + validator.get()->GetValidatorName() + "] " << std::endl;
			return words;
		}
		catch(std::runtime_error ex)
		{
			std::cout << ex.what() << std::endl;
		}
	}

	validatorNames = validatorNames.length() > 2 ? validatorNames.substr(0, validatorNames.length() - 2) : validatorNames;

	throw std::runtime_error("Couldn't find words for bitmap that would satisfy given bitmap. Tried these validators : [" + validatorNames + "]");
}

vector<vector<WordleDrawCore::Color>> WordleDrawCore::GetColorsForWordAndAnswer(const string& answer, const vector<string>& words)
{

	std::unordered_map<char, int> answerCharHitcountMap;
	vector<vector<WordleDrawCore::Color>> result;

	for(const char& ch : answer)
	{
		if(answerCharHitcountMap.contains(ch))
		{
			answerCharHitcountMap[ch] += 1;
		}
		else
		{
			answerCharHitcountMap[ch] = 1;
		}
	}

	for(const std::string& word : words)
	{
		if(word.size() != answer.size())
		{
			throw std::invalid_argument("Words length is inconsistent with answer lenght");
		}
		vector<WordleDrawCore::Color> buffer;
		for(size_t pos = 0; pos < word.size();++pos)
		{
			const char& wordChar = word.at(pos);
			const char& answerChar = answer.at(pos);

			if(wordChar == answerChar)
			{
				buffer.push_back(Green);
			}
			else
			{
				if (calculateOcurrences(word, wordChar, pos) <= answerCharHitcountMap[wordChar])
				{
					buffer.push_back(Yellow);
				}
				else
				{
					buffer.push_back(Black);
				}
			}
		}
		result.push_back(buffer);
	}

	return result;
}



string WordleDrawCore::MatchWordByLine(const vector<bool>& line, const std::unique_ptr<IWordValidator>& validator, const std::string& answer)
{
	vector<string> eligibleWords = WordsReferences;
	for(size_t i = 0; i<line.size(); ++i)
	{
		vector<string> bufWords = GetWordsWithMatchOnPosition(eligibleWords, i, line, validator, answer);
		eligibleWords = std::move(bufWords);
	}

	if (eligibleWords.empty())
	{
		string pattern;
		for (const bool& el : line)
		{
			pattern.push_back(el ? 'X' : '_');
		}
		throw std::runtime_error("Could not find a word with this pattern: [" + pattern + "] and validator [" + validator.get()->GetValidatorName() +"] ");
	}
	return eligibleWords.front();
}

void WordleDrawCore::GenerateValidators(std::vector<std::unique_ptr<IWordValidator>>& validators, const ValidatorType& validatorType)
{
	switch (validatorType)
	{
	case PerferctValidator:    
		validators.push_back(std::make_unique<PerferctWordValidator>());
		break;
	case PresenceValidator:
		validators.push_back(std::make_unique<PresenceWordValidator>());
		break;
	case MixedValidator:
		validators.push_back(std::make_unique<MixedWordValidator>());
		break;
	case FallbackPresenceValidator:
		validators.push_back(std::make_unique<FallbackPresenceWordValidator>());
		break;
	case FallbackMixedValidator:
		validators.push_back(std::make_unique<FallbackMixedWordValidator>());
		break;
	case AtLeastGreenValidator:
		validators.push_back(std::make_unique<AtLeastGreenWordValidator>());
		break;
	case AtLeastYellowValidator:
		validators.push_back(std::make_unique<AtLeastYellowWordValidator>());
		break;
	case NotSpecified:
		validators.push_back(std::make_unique<PerferctWordValidator>());
		validators.push_back(std::make_unique<PresenceWordValidator>());
		validators.push_back(std::make_unique<MixedWordValidator>());
		validators.push_back(std::make_unique<FallbackPresenceWordValidator>());
		validators.push_back(std::make_unique<FallbackMixedWordValidator>());
		validators.push_back(std::make_unique<AtLeastGreenWordValidator>());
		validators.push_back(std::make_unique<AtLeastYellowWordValidator>());
		break;
	default:
		throw std::runtime_error("There is no implemented validator with type [" + std::to_string(validatorType) + "] ");
		break;
	}
}

vector<string> WordleDrawCore::GetWordsWithMatchOnPosition(const vector<string>& words, size_t position, const vector<bool>& mask, const std::unique_ptr<IWordValidator>& validator, const std::string& answer)
{
	vector<string> matches;
	for(const string& word : words)
	{
		if(word != answer && validator.get()->Validate(word, position, mask, answer))
		{
			matches.push_back(word);
		}
	}
	return matches;
}

WordleBitmap::WordleBitmap(const vector<vector<bool>>& field)
{
	TransformOptimizeVectors(field);
}

WordleBitmap::WordleBitmap(vector<vector<bool>>& field)
{
	TransformOptimizeVectors(field);
}

void WordleBitmap::TransformOptimizeVectors(const vector<vector<bool>>& field)
{
	_field = std::make_shared<vector<bool>>();
	if(field.size()!=6)
	{
		throw std::invalid_argument("field must be the size of [6][5]");
	}
	_field->clear();
	_field->reserve(30);
	for(const vector<bool>& _2dBitmap : field)
	{
		if (_2dBitmap.size() != 5)
		{
			throw std::invalid_argument("field must be the size of [6][5]");
		}
		for(const bool bit : _2dBitmap)
		{
			_field->push_back(bit);
		}
	}
}

vector<bool> WordleBitmap::Iterator::GetThisBitLine()
{
	vector<bool> line;
	for(size_t i = itteratorShift; i<itteratorShift+WordleBitmap::step; ++i)
	{
		line.push_back(_field->at(i));
	}
	return line;
}

vector<bool> WordleBitmap::Iterator::operator*()
{
	return GetThisBitLine();
}

WordleBitmap::Iterator& WordleBitmap::Iterator::operator++()
{
	itteratorShift += WordleBitmap::step;
	return *this;
}

bool WordleBitmap::Iterator::operator!=(const WordleBitmap::Iterator& other) const
{
	return itteratorShift != other.itteratorShift;
}