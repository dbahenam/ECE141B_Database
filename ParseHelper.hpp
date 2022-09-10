//
//  ParseHelper.hpp
//  RGAssignment4
//
//  Created by rick gessner on 4/18/21.
//

#ifndef ParseHelper_hpp
#define ParseHelper_hpp

#include <stdio.h>
#include "keywords.hpp"
#include "BasicTypes.hpp"
#include "Tokenizer.hpp"
#include "Attribute.hpp"
#include "TokenSequencer.hpp"
#include "Entity.hpp"

namespace ECE141 {

  //-------------------------------------------------
  
  class Entity;
  
  struct ParseHelper {
	
	using ClauseParser = StatusResult (ParseHelper::*)();

    ParseHelper(Tokenizer &aTokenizer) : tokenizer(aTokenizer) {}
                
    StatusResult parseAttributeOptions(Attribute &anAttribute);

    StatusResult parseAttribute(Attribute &anAttribute);

	StatusResult parseAttributeList(Entity& anEntity);
	
    StatusResult parseIdentifierList(StringList &aList);
	
    StatusResult parseKeyValues(KeyValues &aList,Entity &anEnity);
    
    StatusResult parseValueList(ValueList &aList);
	
	StatusResult parseKeyList(StringList &aList);
	
	bool parseStar();
	
	StatusResult parseClause(Keywords aKW);
	
	StatusResult parseWhere();
	
        
  protected:
    Tokenizer &tokenizer;
  };

}

#endif /* ParseHelper_hpp */

