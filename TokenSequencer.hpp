//
//  TokenSequencer.hpp
//  assignment4
//
//  Created by David Bahena Moctezuma on 5/3/22.
//

#ifndef TokenSequencer_hpp
#define TokenSequencer_hpp

#include <stdio.h>
#include "Tokenizer.hpp"
#include "keywords.hpp"

namespace ECE141 {
	class TokenSequencer{
	public:
		TokenSequencer(Tokenizer& aTokenizer);
		//implement ocf
		TokenSequencer& is(Keywords aKeyword);
		TokenSequencer& then(Keywords aKeyword);
		TokenSequencer& hasId(const std::string);
		TokenSequencer& hasOp(Operators anOperator);
		bool hasNumber(const int aNumber);

		bool getStatus(){return status;}
		Tokenizer& theTokenizer;
		int index = 0;
	protected:
		bool status;
	};
}
#endif /* TokenSequencer_hpp */
