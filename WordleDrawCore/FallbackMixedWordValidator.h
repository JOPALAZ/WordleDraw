#pragma once
#include "FallbackPresenceWordValidator.h"
#include "PerfectWordValidator.h"

class FallbackMixedWordValidator : public IWordValidator
{
protected:

	std::unique_ptr<IWordValidator> FallbackPresenceWordValidatorInstance;
	std::unique_ptr<IWordValidator> PerfectWordValidatorInstance;

public:
	FallbackMixedWordValidator()
	{
		FallbackPresenceWordValidatorInstance = std::make_unique<FallbackPresenceWordValidator>();
		PerfectWordValidatorInstance = std::make_unique<PerferctWordValidator>();
		ValidatorName = "FallbackMixedWordValidator";
	};

	virtual bool Validate(std::string word, size_t position, std::vector<bool> mask, std::string answer) const override
	{
		return	FallbackPresenceWordValidatorInstance.get()->Validate(word, position, mask, answer) || PerfectWordValidatorInstance.get()->Validate(word, position, mask, answer);
	}
};