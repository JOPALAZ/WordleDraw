#pragma once
#include "FallbackPresenceWordValidator.h"

class PresenceWordValidator : public IWordValidator
{
protected:

	//TODO CACHE EACH CHAR FOR ANSWER IN HASHTABLE
	bool CanUseSelectedChar(std::string word, size_t position, std::vector<bool> mask, std::string answer) const
	{
		char questionedChar = word.at(position);

		size_t referenceHits = 0;
		size_t thisWordHits = 0;

		for (size_t i = 0; i <= position; ++i)
		{
			if (mask.at(i) && word.at(i) == questionedChar)
			{
				thisWordHits++;
			}
		}

		for (size_t i = 0; i < mask.size(); ++i)
		{
			if (answer.at(i) == questionedChar)
			{
				referenceHits++;
			}
		}

		return thisWordHits <= referenceHits;
	}

	std::unique_ptr<IWordValidator> FallbackPresenceWordValidatorInstance;

public:
	PresenceWordValidator()
	{
		FallbackPresenceWordValidatorInstance = std::make_unique<FallbackPresenceWordValidator>();
		ValidatorName = "PresenceWordValidator";
	};
	
	virtual bool Validate(std::string word, size_t position, std::vector<bool> mask, std::string answer) const override
	{
		return	FallbackPresenceWordValidatorInstance.get()->Validate(word, position, mask, answer) && CanUseSelectedChar(word, position, mask, answer);
	}
};