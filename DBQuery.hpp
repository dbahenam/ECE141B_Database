//
//  DBQuery.hpp
//  assignment5
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

/* Adapter/Bridge that is an interface on a bunch of other smaller interfaces that all work together*/
namespace ECE141{
  class DBQuery{
  public:
	DBQuery(const std::string &aName);
	//ocf
	DBQuery();
	DBQuery(const std::string aName, Entity anEntity);
	DBQuery(const DBQuery &aCopy);
	DBQuery& operator=(const DBQuery &aCopy);
	StatusResult selectAll(Tokenizer &aTokenizer);
	Entity getEntity(){return theEntity;};
	void setEntity(Entity anEntity){theEntity = anEntity;}
	std::string getTableName(){return EntityName;}
	void setTableName(const std::string &aName){EntityName = aName;}
//	select (fieldlist)
//	FROM (tablelist) JOINS ????
//	WHERE (ExpressionList)
//	GROUP BY (AttrList) -- partioning of our data
//	ORDER BY (view ordering)
	
  protected:
	std::string EntityName;
	Entity theEntity;
	StringList theAttributes;
	ValueList theValues;
	RowList theRows;
	Filters theFilter;
  };
}
#endif /* DBQuery_hpp */
