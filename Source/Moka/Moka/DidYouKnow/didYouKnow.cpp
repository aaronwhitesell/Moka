#include "didYouKNow.h"

#include "Trambo/Utilities/utility.h"


DidYouKnow::DidYouKnow(unsigned int totalDidYouKnowMsgs)
: mBaseName("didYouKnow")
, mMessageCount(totalDidYouKnowMsgs)
{
	for (size_t i = 0; i < mMessageCount; ++i)
	{
		mMessageNames.emplace_back(mBaseName + std::to_string(i));
	}
}

const std::string DidYouKnow::getDidYouKnow()
{
	const std::size_t index = trmb::randomInt(mMessageNames.size());
	const std::string messageName = mMessageNames.at(index);
	mMessageNames.erase(begin(mMessageNames) + index);

	return messageName;
}
