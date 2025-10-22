#pragma once
#include "pch.h"
#ifdef WORDLEDRAWCORE_EXPORTS
#define WORDLEDRAW_API __declspec(dllexport)
#else
#define WORDLEDRAW_API __declspec(dllimport)
#endif

using std::string;
using std::vector;

//TODO RULE OF 5 (AT LEAST OF 3)

class WORDLEDRAW_API WordleBitmap
{
public:
	WordleBitmap(const vector<vector<bool>>& field);
	WordleBitmap(vector<vector<bool>>& field);
	class WORDLEDRAW_API Iterator
	{
	public:
		explicit  Iterator(size_t value, std::shared_ptr<vector<bool>> field) : itteratorShift(value), _field(field) {};
		vector<bool> operator*();
		Iterator& operator++();
		bool operator!=(const Iterator& other) const;
	protected:
		vector<bool> GetThisBitLine();
		size_t itteratorShift = 0;
		std::shared_ptr<vector<bool>> _field;
	};

	Iterator begin() {
		return Iterator(0, _field);
	}

	Iterator end() {
		return Iterator(30, _field); 
	}
protected:
	void TransformOptimizeVectors(const vector<vector<bool>>& field);
	std::shared_ptr<vector<bool>> _field;
	static const size_t step = 5;
};

class WORDLEDRAW_API WordleDrawCore
{
public:
	WordleDrawCore(const std::filesystem::path& pathToRefences, const string& answerWord);
	WordleDrawCore(std::filesystem::path& pathToRefences, string& answerWord);
	vector<string> GetWordsForBitmap(WordleBitmap bitmap);

protected:
	void ConstructClass(const std::filesystem::path& pathToRefences, const string& answerWord);
	vector<string> GetWordsWithMatchOnPosition(vector<string> words, size_t position, vector<bool> mask);
	vector<string> WordsReferences;
	bool CanUseSelectedChar(string word, size_t position, vector<bool> mask);
	void LoadReferences(const std::filesystem::path& pathToRefences, const string& answerWord);
	string MatchWordByLine(vector<bool> line);
	string AnswerWord;
};

