//
//  Attribute.cpp
//  Assignment4
//
//  Created by rick gessner on 4/18/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#include <iostream>
#include "Attribute.hpp"

namespace ECE141 {

  Attribute::Attribute(DataTypes aType)
    : type(aType),size(0),autoIncrement(0),primary(0),nullable(1) {}
 
  Attribute::Attribute(std::string aName, DataTypes aType, uint32_t aSize)  {
    name=aName;
    type=aType;
    size=aSize;
    autoIncrement=0;
    primary=0;
    nullable=1;
  }
 
  Attribute::Attribute(const Attribute &aCopy)  {
    name=aCopy.name;
    type=aCopy.type;
    size=aCopy.size;
    autoIncrement=aCopy.autoIncrement;
    primary=aCopy.primary;
    nullable=aCopy.nullable;
  }
 
  Attribute::~Attribute()  {
  }
  Attribute Attribute::attributeParsing(Tokenizer &aTokenizer){
	Token currentToken = aTokenizer.current();
	Attribute anAttribute(DataTypes::no_type);
	anAttribute.reset();
	while(aTokenizer.more() && currentToken.data != "," && currentToken.data !=")"){
	  //std::cout << "current token: " << currentToken.data << "\n";
	  if(currentToken.type == TokenType::identifier){
		anAttribute.setName(currentToken.data);
	  }
	  else if(currentToken.type == TokenType::number){
		anAttribute.size = std::stoi(currentToken.data);
		aTokenizer.next(1);
	  }
	  else if(typesMap.count(currentToken.keyword)){
		anAttribute.type = typesMap[currentToken.keyword];
	  }
	  else if(currentToken.keyword == Keywords::auto_increment_kw){
		anAttribute.setAutoIncrement(true);
	  }
	  else if(currentToken.keyword == Keywords::not_kw){
		if(aTokenizer.peek(1).keyword == Keywords::null_kw){
		  anAttribute.setNullable(false);
		}
	  }
	  else if(currentToken.keyword == Keywords::primary_kw){
		anAttribute.setPrimaryKey(true);
	  }
	  aTokenizer.next(1);
	  currentToken = aTokenizer.current();
	}
	return anAttribute;
  }
  Attribute& Attribute::setName(std::string &aName)  {
    name = aName;
    return *this;
  }
 
  Attribute& Attribute::setDataType(DataTypes aType) {
    type=aType;
    return *this;
  }

  Attribute& Attribute::setSize(int aSize) {
    size=aSize; return *this;
  }

  Attribute& Attribute::setAutoIncrement(bool anAuto) {
    autoIncrement=anAuto; return *this;
  }

  Attribute& Attribute::setPrimaryKey(bool aPrimary) {
    primary=aPrimary; return *this;
  }

  Attribute& Attribute::setNullable(bool aNullable) {
    nullable=aNullable; return *this;
  }
  std::string Attribute::getNullable(){
	if(nullable == true){
	  return "Yes";
	}
	return "No";
  }
  std::string Attribute::getTypeName(){
	return DataNames[type];
  }
  std::string Attribute::getKey(){
	if(isPrimaryKey()){
	  return "Yes";
	}
	return "No";
  }
  bool Attribute::isValid() {
    return true;
  }
   
  Value Attribute::toValue(const std::string &aValue) const {
    //might need to do type-casting here...    
    return Value{aValue};
  }

  void Attribute::encode(std::ostream &aStream){
	std::string space = " ";
	char aType = static_cast<char>(type);
	//std::cout << name << space << aType << space << size << space << autoIncrement << space << primary << space << nullable;
	aStream << name << space << aType << space << size << space <<
	autoIncrement << space << primary << space << nullable << space;
  }
  
  void Attribute::decode(std::istream &aStream){
	std::string attribute;
	char aType;
	std::string theSize, theIncrement, thePrimary, theNullable;
	aStream >> name >>  aType;
	aStream >> theSize >> theIncrement >> thePrimary >> theNullable;
	type = static_cast<DataTypes>(aType);
	size = std::stoi(theSize);
	autoIncrement = std::stoi(theIncrement);
	primary = std::stoi(thePrimary);
	nullable = std::stoi(theNullable);
  }

}
