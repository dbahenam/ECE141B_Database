//
//  DBQuery.cpp
//  
//
//  Created by David Bahena Moctezuma on 5/25/22.
//

#include "DBQuery.hpp"

namespace ECE141{
  DBQuery::DBQuery(){
	EntityName = "";
  }
  DBQuery DBQuery::Select(StringList &aList){
	return *this;
  }
}
