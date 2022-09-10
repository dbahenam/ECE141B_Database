//
//  ValueType.h
//  assignment6_upgraded
//
//  Created by David Bahena Moctezuma on 8/24/22.
//

#ifndef ValueType_h
#define ValueType_h
#include "Entity.hpp"
#include "BasicTypes.hpp"
namespace ECE141{

  using typeFactory = DataTypes (*)(const std::string& aStr);

  //methods to find DataType from string
  DataTypes isBool(const std::string& aStr){
	if(aStr == "true" || aStr == "false"){
	  return DataTypes::bool_type;
	}
	return DataTypes::no_type;
  }
  DataTypes isInt(const std::string& aStr){
	if(aStr.find_first_not_of("0123456789") == std::string::npos){
	  return DataTypes::int_type;
	}
	return DataTypes::no_type;
  }
  DataTypes isFloat(const std::string& aStr){
	if(aStr.find_first_not_of("0123456789.") == std::string::npos){
	  return DataTypes::float_type;
	}
	return DataTypes::no_type;
  }
  DataTypes isVarchar(const std::string& aStr){
	if(aStr.find_first_not_of("0123456789.-") != std::string::npos){
	  return DataTypes::varchar_type;
	}
	return DataTypes::no_type;
  }
  DataTypes isDateTime(const std::string& aStr){
	return DataTypes::no_type;
  }
  static std::map<int, typeFactory> typeMap = {
	{0, isBool},
	{1, isInt},
	{2, isFloat},
	{3, isVarchar},
	{4, isDateTime}
  };

  struct ValueType{
	ValueType(){}
	// get type
	DataTypes getType(const std::string& aStr){
	  int length = static_cast<int>(DataTypes::Count) - 1; // subtract no type
	  DataTypes theType = DataTypes::no_type;
	  for(int i = 0; i < length; i++){
		theType = typeMap[i](aStr);
		if(theType != DataTypes::no_type){
		  return theType;
		}
	  }
	  return theType;
	}
	
	//convert DataType to value(bool, int, float, string)
	ValueOpt getValue(const DataTypes& aType, const std::string& aStr){
	  // call map to get datatype from string
	  if(aType == DataTypes::no_type){return std::nullopt;}
	  Value theValue;
	  switch(aType){
		case DataTypes::int_type:
		  theValue = std::stoi(aStr);
		  return theValue;
		case DataTypes::float_type:
		  theValue = std::stof(aStr);
		  return theValue;
		case DataTypes::bool_type:
		  if(aStr == "true"){
			theValue = true;
		  }
		  else{
			theValue = false;
		  }
		  return theValue;
		case DataTypes::varchar_type:
		  theValue = aStr;
		  return theValue;
		default:
		  break;
	  }
	  return std::nullopt;
	}
  };
}

#endif /* ValueType_h */
