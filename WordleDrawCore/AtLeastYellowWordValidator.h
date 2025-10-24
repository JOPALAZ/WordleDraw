#pragma once
#include "PresenceWordValidator.h"

class AtLeastYellowWordValidator : public IWordValidator
{
protected:
	std::unique_ptr<IWordValidator> PresenceWordValidatorInstance;
public:
	AtLeastYellowWordValidator()
	{
		PresenceWordValidatorInstance = std::make_unique<PresenceWordValidator>();
		ValidatorName = "AtLeastYellowWordValidator";
	}
	virtual bool Validate(std::string word, size_t position, std::vector<bool> mask, std::string answer) const override
	{
		bool thisPosMask = mask.at(position);
		char neededChar = word.at(position);

		if (thisPosMask)
		{
			return PresenceWordValidatorInstance.get()->Validate(word, position, mask, answer);
		}

		return PresenceWordValidatorInstance.get()->Validate(word, position, mask, answer) || answer.at(position) == neededChar;
	};
};