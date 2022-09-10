//
//  Row.cpp
//  PA3
//
//  Created by rick gessner on 4/2/22.
//


#include <stdio.h>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <memory>
#include "Row.hpp"
#include <iostream>

namespace ECE141 {

  //Row::Row(uint32_t entityId) {}
  Row::Row(const Row &aRow) {*this=aRow;}

  Row::~Row() {}

  Row& Row::operator=(const Row &aRow) {
	entityName = aRow.entityName;
	for(const auto & thePair : aRow.data){
	  data[thePair.first] = thePair.second;
	}
	return *this;
  }
  bool Row::operator==(Row &aCopy) const {return false;}

  //STUDENT: What other methods do you require?
                      
  Row& Row::set(const std::string &aKey,const Value &aValue) {
    return *this;
  }
  
  StatusResult Row::mapKeyValue(StringList &aStrList, ValueList &aValueList){
	StatusResult theResult{noError};
	if ((aStrList.size() != aValueList.size()) || aValueList.empty()) { return unknownError; }
			transform(aStrList.begin(), aStrList.end(), aValueList.begin(), std::inserter(data, data.end()), [](std::string key, Value value) {
				return std::make_pair(key, value);
				});
	return theResult;
  }

  void Row::encode(std::ostream& aStream){
	std::string space = " ";
	aStream << entityName << space << data.size() << space;
	for(auto thePair : data){
	  std::string second = "";
	  aStream << thePair.first.data() << space;
	  std::visit([&](auto&& value) {
		  // use value, which may be int&, float& or string
		aStream << value << space;
	  }, thePair.second);
	}
  }

  void Row::decode(std::istream &aStream){
	std::string aKey;
	std::string aValue;
	size_t dataSize;
	aStream >> entityName >> dataSize;
	for(size_t aPair = 0; aPair < dataSize; aPair++){
	  aStream >> aKey >> aValue;
	  data.insert({aKey,aValue});
	}
  }

  }
