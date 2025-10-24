#pragma once
#include "IWordValidator.h"

class FallbackLoosePresenceWordValidator : public IWordValidator
{
public:
	FallbackLoosePresenceWordValidator()
	{
		ValidatorName = "FallbackLoosePresenceWordValidator";
	}
	virtual bool Validate(std::string word, size_t position, std::vector<bool> mask, std::string answer) const override
	{
		bool thisPosMask = mask.at(position);
		char neededChar = word.at(position);

		return thisPosMask ? (answer.find(neededChar) != std::string::npos) : answer.find(neededChar) == std::string::npos;
	};
};