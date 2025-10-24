#pragma once
#include "pch.h"

#ifdef WORDLEDRAWCORE_EXPORTS
#define WORDLEDRAW_API __declspec(dllexport)
#else
#define WORDLEDRAW_API __declspec(dllimport)
#endif


//TODO RULE OF 5 (AT LEAST OF 3)

class IWordValidator;

class WORDLEDRAW_API WordleBitmap
{
public:
	WordleBitmap(const std::vector<std::vector<bool>>& field);
	WordleBitmap(std::vector<std::vector<bool>>& field);
	class WORDLEDRAW_API Iterator
	{
	public:
		explicit  Iterator(size_t value, std::shared_ptr<std::vector<bool>> field) : itteratorShift(value), _field(field) {};
		std::vector<bool> operator*();
		Iterator& operator++();
		bool operator!=(const Iterator& other) const;
	protected:
		std::vector<bool> GetThisBitLine();
		size_t itteratorShift = 0;
		std::shared_ptr<std::vector<bool>> _field;
	};

	Iterator begin() {
		return Iterator(0, _field);
	}

	Iterator end() {
		return Iterator(30, _field); 
	}

	Iterator begin() const {
		return Iterator(0, _field);
	}

	Iterator end() const {
		return Iterator(30, _field);
	}

protected:
	void TransformOptimizeVectors(const std::vector<std::vector<bool>>& field);
	std::shared_ptr<std::vector<bool>> _field;
	static const size_t step = 5;
};

class WORDLEDRAW_API WordleDrawCore
{
public:
	enum WORDLEDRAW_API ValidatorType
	{
		PerferctValidator,
		PresenceValidator,
		MixedValidator,
		FallbackPresenceValidator,
		FallbackMixedValidator,
		AtLeastGreenValidator,
		AtLeastYellowValidator,
		NotSpecified
	};
	WordleDrawCore(const std::filesystem::path& pathToRefences, const std::string& answerWord);
	WordleDrawCore(std::filesystem::path& pathToRefences, std::string& answerWord);
	std::vector<std::string> GetWordsForBitmap(const WordleBitmap& bitmap, const ValidatorType& validatorType = NotSpecified);

protected:
	void ConstructClass(const std::filesystem::path& pathToRefences, const std::string& answerWord);
	std::vector<std::string> GetWordsWithMatchOnPosition(const std::vector<std::string>& words, size_t position, const std::vector<bool>& mask, const std::unique_ptr<IWordValidator>& validator);
	std::vector<std::string> WordsReferences;
	void LoadReferences(const std::filesystem::path& pathToRefences, const std::string& answerWord);
	std::string MatchWordByLine(const std::vector<bool>& line, const std::unique_ptr<IWordValidator>& validator);
	std::string AnswerWord;
	void GenerateValidators(std::vector<std::unique_ptr<IWordValidator>>& validators, const ValidatorType& validatorType);
};

