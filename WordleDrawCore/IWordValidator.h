#pragma once
#include "WordleDrawCore.h"


class IWordValidator
{
protected:
	IWordValidator() = default;
	IWordValidator(const IWordValidator&) = delete;
	IWordValidator(IWordValidator&&) = delete;
	IWordValidator& operator=(const IWordValidator&) = delete;
	IWordValidator& operator=(IWordValidator&&) = delete;
	std::string ValidatorName = "NOT IMPLIMENTED";
public:
	virtual std::string GetValidatorName() const { return ValidatorName; };
	virtual bool Validate(std::string word, size_t position, std::vector<bool> mask, std::string answer) const = 0;
};

