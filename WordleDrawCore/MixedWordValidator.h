#pragma once
#include "PresenceWordValidator.h"
#include "PerfectWordValidator.h"

class MixedWordValidator : public IWordValidator
{
protected:

	std::unique_ptr<IWordValidator> PresenceWordValidatorInstance;
	std::unique_ptr<IWordValidator> PerfectWordValidatorInstance;

public:
	MixedWordValidator()
	{
		PresenceWordValidatorInstance = std::make_unique<PresenceWordValidator>();
		PerfectWordValidatorInstance = std::make_unique<PerferctWordValidator>();
		ValidatorName = "MixedWordValidator";
	};

	virtual bool Validate(std::string word, size_t position, std::vector<bool> mask, std::string answer) const override
	{
		return	PresenceWordValidatorInstance.get()->Validate(word, position, mask, answer) || PerfectWordValidatorInstance.get()->Validate(word, position, mask, answer);
	}
};