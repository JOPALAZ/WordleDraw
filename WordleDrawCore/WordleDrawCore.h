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
	WordleDrawCore(const std::filesystem::path& pathToRefences);
	WordleDrawCore(std::filesystem::path& pathToRefences);
	std::vector<std::string> GetWordsForBitmap(const WordleBitmap& bitmap, const std::string& answer, const ValidatorType& validatorType = NotSpecified);

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
}