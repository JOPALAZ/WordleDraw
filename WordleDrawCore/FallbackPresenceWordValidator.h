#pragma once
#include "FallbackLoosePresenceWordValidator.h"

class FallbackPresenceWordValidator : public IWordValidator
{
protected:
	std::unique_ptr<IWordValidator> FallbackLoosePresenceWordValidatorInstance;
public:
	FallbackPresenceWordValidator()
	{
		FallbackLoosePresenceWordValidatorInstance = std::make_unique<FallbackLoosePresenceWordValidator>();
		ValidatorName = "FallbackPresenceWordValidator";
	}
	virtual bool Validate(std::string word, size_t position, std::vector<bool> mask, std::string answer) const override
	{
		bool thisPosMask = mask.at(position);
		char neededChar = word.at(position);

		if(thisPosMask)
		{
			return FallbackLoosePresenceWordValidatorInstance.get()->Validate(word, position, mask, answer) && answer.at(position) != neededChar;
		}
		
		return FallbackLoosePresenceWordValidatorInstance.get()->Validate(word, position, mask, answer);
	};
};