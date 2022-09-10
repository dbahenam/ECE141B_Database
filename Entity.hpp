//
//  Entity.hpp
//  Assignment3
//
//  Created by rick gessner on 3/18/22.
//  Copyright © 2022 rick gessner. All rights reserved.
//

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include <vector>
#include <map>
#include <optional>
#include <memory>
#include <string>

#include "Attribute.hpp"
#include "BasicTypes.hpp"
#include "Helpers.hpp"

namespace ECE141 {
  
  using AttributeOpt = std::optional<Attribute>;
  using AttributeList = std::vector<Attribute>;
  using EntityVisitor = std::function<bool(Attribute&)>;

  //------------------------------------------------
  class Entity {
  public:
	Entity();
	Entity(const std::string aName, uint32_t aHashID, uint32_t aPrimaryKeyNum);
	Entity(const Entity &aCopy);
	Entity& operator=(const Entity& aCopy);
	~Entity();
	
	// map values index return (int) to DataTypes
	std::map<int, DataTypes> indexType{
	  {0, DataTypes::bool_type},
	  {1, DataTypes::int_type},
	  {2, DataTypes::float_type},
	  {3, DataTypes::varchar_type},
	};
	
	// validation methods
	bool areValidList(StringList& attributeNames, ValueList& attributeValues){return attributeNames.size() == attributeValues.size();}
	StatusResult isValidName(StringList& attributeNames);
	StatusResult isValidType(StringList& attributeNames, ValueList& attributeValues);
	
    const std::string     getName() const {return name;}
	uint32_t			  getHashID() const{ return hashID;}
    const AttributeList&  getAttributes() const {return attributes;}
    Entity&               addAttribute(const Attribute &anAttribute);
    Attribute*            getAttribute(const std::string &aName) const;
    uint32_t     		  setPrimaryKey();
	uint32_t     		  getPrimaryKey(){return primaryKeyNum;}
    uint32_t              getAutoIncr() {return autoincr;}
	bool				  setNameID(const std::string &aName);
	static size_t 				  hashString(const std::string &Name);
    void each(const std::string& aType,
        EntityVisitor& aVisitor) const {
        for (Attribute anAttribute : attributes) {
                aVisitor(anAttribute);
        }
    };
	
	void encode(std::ostream &aStream){
	  std::string space = " ";
	  aStream << name << space << hashID << space << primaryKeyNum << space << attributes.size() << space;
	  for(auto anAttr : attributes){
		anAttr.encode(aStream);
	  }
	}
	void decode(std::istream &aStream){
	  size_t numAttributes;
	  aStream >> name >> hashID >> primaryKeyNum >> numAttributes;
	  Attribute anAttr;
	  for(size_t ind = 0; ind < numAttributes; ind++){
		anAttr.decode(aStream);
		attributes.push_back(anAttr);
	  }
	}
	AttributeList   attributes;
	size_t getNumAttributes();
  protected:
        
    
    std::string     name;
	uint32_t 		hashID;
	uint32_t 		primaryKeyNum;
    uint32_t        autoincr;  //auto_increment
    std::string encoding;
	size_t 			rows = 0;
	Helpers theHelp;
  };
  
}
#endif /* Entity_hpp */
