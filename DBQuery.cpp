//
//  DBQuery.cpp
//  
//
//  Created by David Bahena Moctezuma
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
