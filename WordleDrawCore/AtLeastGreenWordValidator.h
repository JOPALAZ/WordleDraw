#pragma once
#include "IWordValidator.h"

class AtLeastGreenWordValidator : public IWordValidator
{
public:
	AtLeastGreenWordValidator()
	{
		ValidatorName = "AtLeastGreenWordValidator";
	}

	virtual bool Validate(std::string word, size_t position, std::vector<bool> mask, std::string answer) const override
	{
		return (word.at(position) == answer.at(position)) == mask.at(position);
	};
};