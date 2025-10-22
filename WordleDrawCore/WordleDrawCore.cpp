#include "pch.h"
#include "WordleDrawCore.h"
WordleDrawCore::WordleDrawCore(const std::filesystem::path& pathToRefences, const string& answerWord)
{
	ConstructClass(pathToRefences, answerWord);
}
WordleDrawCore::WordleDrawCore(std::filesystem::path& pathToRefences, string& answerWord)
{
	ConstructClass(pathToRefences, answerWord);
}

void WordleDrawCore::ConstructClass(const std::filesystem::path& pathToRefences, const string& answerWord)
{
	LoadReferences(pathToRefences, answerWord);
	if(WordsReferences.empty())
	{
		throw std::runtime_error("No reference words were loaded.");
	}
	if(answerWord.size() != WordsReferences.at(0).size())
	{
		throw std::runtime_error("Word length doesn't match for reference ["+ std::to_string(answerWord.size()) +"] and for answer ["+ std::to_string(WordsReferences.at(0).size()) + "].");
	}
	AnswerWord = answerWord;
}

void WordleDrawCore::LoadReferences(const std::filesystem::path& pathToRefences, const string& answerWord)
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
	size_t lastLength = 0xb16b00b5;
	while (filestream >> line)
	{
		if (line != answerWord)
		{
			if(lastLength == 0xb16b00b5 || lastLength == line.size())
			{
				WordsReferences.push_back(line);
				lastLength = line.size();
			}
			else
			{
				throw std::runtime_error("Length of loaded words is inconsistent.");
			}
		}
	}
}

vector<string> WordleDrawCore::GetWordsForBitmap(WordleBitmap bitmap)
{
	vector<string> words;
	words.reserve(6);
	for(auto line : bitmap)
	{
		words.push_back(WordleDrawCore::MatchWordByLine(line));
	}
	return words;
}

string WordleDrawCore::MatchWordByLine(vector<bool> line)
{
	vector<string> eligibleWords = WordsReferences;
	for(size_t i = 0; i<line.size(); ++i)
	{
		vector<string> bufWords = GetWordsWithMatchOnPosition(eligibleWords, i, line);
		eligibleWords = bufWords;
	}

	if(eligibleWords.empty())
	{
		string pattern;
		for(bool el : line)
		{
			pattern.push_back(el ? 'X' : '_');
		}
		throw std::runtime_error("Couldn not find a word with this pattern: " + pattern);
	}
	return eligibleWords.front();
}

vector<string> WordleDrawCore::GetWordsWithMatchOnPosition(vector<string> words, size_t position, vector<bool> mask)
{
	//TODO RETRY WITHOUT UNIQUENESS CHECK IF FAILED WITH IT
	vector<string> matches;
	bool thisPosMask = mask.at(position);
	for(string word : words)
	{
		char neededChar = word.at(position);
		if((AnswerWord.find(neededChar) != string::npos) == thisPosMask)
		{
			if(!thisPosMask || CanUseSelectedChar(word, position, mask))
			{
				matches.push_back(word);
			}
		}
	}
	return matches;
}

bool WordleDrawCore::CanUseSelectedChar(string word, size_t position, vector<bool> mask)
{
	char questionedChar = word.at(position);

	size_t referenceHits = 0;
	size_t thisWordHits = 0;

	for (size_t i = 0; i <= position; ++i)
	{
		if(mask.at(i)&&word.at(i)==questionedChar)
		{
			thisWordHits++;
		}
	}

	for(size_t i = 0; i < mask.size(); ++i)
	{
		if(AnswerWord.at(i) == questionedChar)
		{
			referenceHits++;
		}
	}

	return thisWordHits<=referenceHits;
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