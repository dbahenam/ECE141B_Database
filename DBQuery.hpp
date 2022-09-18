//
//  DBQuery.hpp
//  
//
//  Created by David Bahena Moctezuma on 5/25/22.
//

#ifndef DBQuery_hpp
#define DBQuery_hpp

#include <stdio.h>
#include <string>
#include "Entity.hpp"
#include "Row.hpp"
#include "Filters.hpp"
#include "BasicTypes.hpp"

/* Adapter/Bridge that is an interface on a bunch of other smaller interfaces that all work together*/
namespace ECE141{
  class DBQuery{
  public:
	DBQuery(const std::string &aName);
	DBQuery();
	//ocf
	
	Entity getEntity(){return theEntity;}
	// Select * from TableName, Select first_name, last_name from TableName
	DBQuery Select(StringList& aList);
	DBQuery FROM(StringList& aList); // JOINS ????
	DBQuery WHERE(Expressions& expressionList);
	DBQuery GROUP_BY(ValueList& aList); // -- partioning of our data
	//DBQuery ORDER_BY (view ordering)
	std::string EntityName;
	Entity theEntity;
  protected:
	StringList theAttributes;
	ValueList theValues;
	RowList theRows;
	Filters theFilter;
  };
}
#endif /* DBQuery_hpp */
