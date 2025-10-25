#pragma once
#include "IWordValidator.h"

class PerferctWordValidator : public IWordValidator
{
public:
	PerferctWordValidator()
	{
		ValidatorName = "PerferctWordValidator";
	}

	virtual bool Validate(std::string word, size_t position, std::vector<bool> mask, std::string answer) const override 
	{
		if(mask.at(position))
		{
			return word.at(position) == answer.at(position);
		}
		else
		{
			return answer.find(word.at(position)) == std::string::npos;
		}
	};
};