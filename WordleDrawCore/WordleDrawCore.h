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
	enum WORDLEDRAW_API Color
	{
		Black,
		Yellow,
		Green
	};
	WordleDrawCore(const std::filesystem::path& pathToRefences);
	WordleDrawCore(std::filesystem::path& pathToRefences);
	std::vector<std::string> GetWordsForBitmap(const WordleBitmap& bitmap, const std::string& answer, const ValidatorType& validatorType = NotSpecified);
	std::vector<std::vector<Color>> GetColorsForWordAndAnswer(const std::string& answer, const std::vector<std::string>& words);

protected:
	void ConstructClass(const std::filesystem::path& pathToRefences);
	std::vector<std::string> GetWordsWithMatchOnPosition(const std::vector<std::string>& words, size_t position, const std::vector<bool>& mask, const std::unique_ptr<IWordValidator>& validator, const std::string& answer);
	std::vector<std::string> WordsReferences;
	void LoadReferences(const std::filesystem::path& pathToRefences);
	std::string MatchWordByLine(const std::vector<bool>& line, const std::unique_ptr<IWordValidator>& validator, const std::string& answer);
	void GenerateValidators(std::vector<std::unique_ptr<IWordValidator>>& validators, const ValidatorType& validatorType);
};

#include "WordleDrawCore.h" 
#include <vector>
#include <string>
#include <exception> 

extern "C" {

	__declspec(dllexport) void* CreateWordleDrawCore(const char* path)
	{
		try
		{
			return new WordleDrawCore(path);
		}
		catch (...)
		{
			return nullptr;
		}
	}

	__declspec(dllexport) void DeleteWordleDrawCore(void* instance)
	{
		if (!instance) return;
		delete static_cast<WordleDrawCore*>(instance);
	}

	__declspec(dllexport) int GetWordsForBitmap(void* instance,
		bool* bitmap,
		int validatorType,
		const char* answer,
		char* outputBuffer,
		int bufferSize)
	{
		if (!instance || !bitmap || !answer || !outputBuffer) return -10;

		try
		{
			std::vector<std::vector<bool>> array;
			for (size_t i = 0; i < 6; ++i)
			{
				array.push_back(std::vector<bool>{});
				for (size_t j = 0; j < 5; ++j)
				{
					array.at(i).push_back(bitmap[i * 5 + j]);
				}
			}
			WordleBitmap wordleBitmap(array);

			std::vector<std::string> result =
				static_cast<WordleDrawCore*>(instance)->GetWordsForBitmap(wordleBitmap,
					{ answer },
					static_cast<WordleDrawCore::ValidatorType>(validatorType));

			std::string output = "";
			for (size_t i = 0; i < result.size(); i++)
			{
				output += (result.at(i));
				if (i != result.size() - 1)
				{
					output += ",";
				}
			}

			if (output.length() + 1 > (size_t)bufferSize)
			{
				strncpy_s(outputBuffer, bufferSize, "BUFFER_TOO_SMALL", _TRUNCATE);
				return -2;
			}

			strncpy_s(outputBuffer, bufferSize, output.c_str(), _TRUNCATE);
			return (int)output.length();
		}
		catch (const std::exception& e)
		{
			if (strlen(e.what()) + 1 <= (size_t)bufferSize)
			{
				strncpy_s(outputBuffer, bufferSize, e.what(), _TRUNCATE);
			}
			else
			{
				strncpy_s(outputBuffer, bufferSize, "CPP_EXCEPTION_MSG_TOO_LONG", _TRUNCATE);
			}
			return -3;
		}
		catch (...)
		{
			strncpy_s(outputBuffer, bufferSize, "UNKNOWN_CPP_EXCEPTION", _TRUNCATE);
			return -99;
		}
	}

	__declspec(dllexport) int GetColorsForWordAndAnswer(void* instance,
		const char* answer,
		const char** words,
		int wordAmount,
		int* outputBuffer,
		int bufferSize,
		char* exceptionBuffer,
		int exceptionBufferSize)
	{
		if (!instance || !words || !answer || !outputBuffer || !exceptionBuffer) return -10;

		try
		{
			std::vector<std::string> wordsVector;
			std::string answerString(answer);
			for (size_t i = 0; i < wordAmount;++i)
			{
				wordsVector.push_back(words[i]);
			}
			auto colorVector = static_cast<WordleDrawCore*>(instance)->GetColorsForWordAndAnswer(answerString, wordsVector);
			int neededBufferSize = colorVector.size() * answerString.size();
			if(bufferSize < neededBufferSize)
			{
				throw std::runtime_error("The outputBuffer is too small for the result, needed [" + std::to_string(neededBufferSize) + "] positions.");
			}

			for(size_t i = 0; i < colorVector.size(); ++i)
			{
				const std::vector<WordleDrawCore::Color>& colorLine = colorVector.at(i);
				for (size_t j = 0; j < colorLine.size();++j)
				{
					outputBuffer[i * answerString.size() + j] = static_cast<int>(colorLine.at(j));
				}
			}
			
			return neededBufferSize;
		}
		catch (const std::exception& e)
		{
			if (strlen(e.what()) + 1 <= (size_t)exceptionBufferSize)
			{
				strncpy_s(exceptionBuffer, exceptionBufferSize, e.what(), _TRUNCATE);
			}
			else
			{
				strncpy_s(exceptionBuffer, exceptionBufferSize, "CPP_EXCEPTION_MSG_TOO_LONG", _TRUNCATE);
			}
			return -3;
		}
		catch (...)
		{
			strncpy_s(exceptionBuffer, exceptionBufferSize, "UNKNOWN_CPP_EXCEPTION", _TRUNCATE);
			return -99;
		}
	}
}