//
//  TokenSequencer.cpp
//  assignment4
//
//  Created by David Bahena Moctezuma on 5/3/22.
//

#include "TokenSequencer.hpp"

namespace ECE141{

  TokenSequencer::TokenSequencer(Tokenizer& aTokenizer) : theTokenizer{aTokenizer}, status{false}{}
  //ocf

  TokenSequencer& TokenSequencer::is(Keywords aKeyword){
	  index=0;
	  Keywords theKeyword = theTokenizer.tokenAt(index).keyword;
	  if(theKeyword == aKeyword){status = true;}
	  return *this;
  }
  TokenSequencer& TokenSequencer::then(Keywords aKeyword){
	  index++;
	  Token nextToken = theTokenizer.peek(index); // get next keyword
	  Keywords theKeyword = nextToken.keyword;
	  if(status){
		  if(theKeyword != aKeyword){
			  status = false;
		  }
	  }
	  return *this;
  }
  TokenSequencer& TokenSequencer::hasId(const std::string){
	index++;
	if (index < theTokenizer.size()) {
		Token& nextToken = theTokenizer.peek(index);
		status = TokenType::identifier == nextToken.type;
		return *this;
	}
	else{
	  status = false;
	}
	return *this;
  }
  
  TokenSequencer& TokenSequencer::hasOp(Operators anOperator){
	return *this;
  }
  bool TokenSequencer::hasNumber(const int aNumber){
	return false;
  }


}
