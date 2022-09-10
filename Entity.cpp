//
//  Entity.cpp
//  PA3
//
//  Created by rick gessner on 3/2/22.
//

#include <stdio.h>
#include <vector>
#include <optional>
#include <memory>
#include <string>
#include "Entity.hpp"

namespace ECE141 {

  Entity::Entity() : name{""}, hashID(0), primaryKeyNum(0) {}

  Entity::Entity(const std::string aName, uint32_t aHashID, uint32_t aPrimaryKeyNum) :
	name{aName}, hashID{aHashID}, primaryKeyNum{aPrimaryKeyNum} {}

  Entity::Entity(const Entity &aCopy){
	*this = aCopy;
  }
  Entity& Entity::operator=(const Entity &aCopy) {
    name = aCopy.name;
	hashID = aCopy.hashID;
	primaryKeyNum = aCopy.primaryKeyNum;
    autoincr=aCopy.autoincr;
	attributes = aCopy.attributes;
	return *this;
  }

  Entity::~Entity() {
    //std::cout << "~Entity()\n";
  }

  //validation methods
  StatusResult Entity::isValidName(StringList& attributeNames){
	for(auto& aName : attributeNames){
	  for(auto& anAttr: attributes){
		if(aName == anAttr.getName()){
		  return StatusResult{noError};
		}
	  }
	}
	return StatusResult{unknownAttribute};
  }

  StatusResult Entity::isValidType(StringList& attributeNames, ValueList& attributeValues){
	int index = 0;
	// loop through each name, check that it exists as an attribute name
	// if it does exist as an attribute name, check that the value associated
	// with that name is of the same type as the the attribute's type
	for(auto& aName : attributeNames){
	  for(auto& anAttr : attributes){
		if(aName == anAttr.getName()){
		  int pos = attributeValues[index].index();
		  DataTypes aType = DataTypes::no_type;
		  aType = indexType[pos];
		  if(aType != anAttr.getType()){
			return StatusResult{unknownType};
		  }
		  else{
			break;
		  }
		}
	  }
	  index++;
	}
	return StatusResult{noError};
  }


  Entity& Entity::addAttribute(const Attribute &anAttribute) {
    if(!getAttribute(anAttribute.getName())) {
      attributes.push_back(anAttribute);
    }
	rows++;
    return *this;
  }
//
  size_t Entity::hashString( const std::string &Name){
	return static_cast<size_t>(Helpers::hashString(Name.c_str()));
  }

//
  bool Entity::setNameID(const std::string &aName){
	// conditions to fail?
	name = aName;
	hashID = static_cast<uint32_t>(theHelp.hashString(name.c_str()));
	return true;
  }
  size_t Entity::getNumAttributes(){
	return attributes.size();
  }
  Attribute* Entity::getAttribute(const std::string &aName) const {
      for (Attribute anAttribute : attributes) {
          if (anAttribute.getName() == aName) {
              return &anAttribute;
          }
      }

    return nullptr;
  }
  
  uint32_t Entity::setPrimaryKey() {
	  primaryKeyNum++;
	  return primaryKeyNum;
  }

  // USE: ask the entity for name of primary key (may not have one...)
  // make method for this?
}
