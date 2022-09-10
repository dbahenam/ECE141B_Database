//
//  DBQuery.cpp
//  assignment5
//
//  Created by David Bahena Moctezuma on 5/25/22.
//

#include "DBQuery.hpp"

namespace ECE141{
  DBQuery::DBQuery() : EntityName{""}{}
  DBQuery::DBQuery(const std::string aName, Entity anEntity) : EntityName{aName}{}
  DBQuery::DBQuery(const DBQuery &aCopy){
	*this = aCopy;
  }
DBQuery& DBQuery::operator=(const DBQuery &aCopy){
  EntityName = aCopy.EntityName;
  theEntity = aCopy.theEntity;
  return *this;
}

//  StatusResult DBQuery::selectAll(Tokenizer &aTokenizer){
//	StatusResult theResult{noError};
//
//	return theResult;
//  }
}
