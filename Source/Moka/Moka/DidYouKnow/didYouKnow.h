#ifndef DID_YOU_KNOW_H
#define DID_YOU_KNOW_H

#include <string>
#include <vector>


class DidYouKnow
{
public:
								DidYouKnow(std::size_t totalDidYouKnowMsgs);	// ALW - The messages can be found in Text.xml
								DidYouKnow(const DidYouKnow &) = delete;
	DidYouKnow &				operator=(const DidYouKnow &) = delete;

	const std::string			getDidYouKnow();


private:
	const std::string			mBaseName;
	const std::size_t			mMessageCount;
	std::vector<std::string>	mMessageNames;
};

#endif
