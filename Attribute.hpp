//
//  Attribute.hpp
//  Assignment4
//
//  Created by rick gessner on 4/18/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef Attribute_hpp
#define Attribute_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <optional>
#include <functional>
#include "keywords.hpp"
#include "BasicTypes.hpp"
#include "TestSequencer.hpp"

namespace ECE141 {
      
/*
  struct Property {
    Property(std::string aName, int aTableId=0) : name(aName), tableId(aTableId), desc(true) {}
    std::string     name;
    int             tableId;
    bool            desc;
  };

  using PropertyList = std::vector<Property>;
*/
//no_type = 'N', bool_type = 'B', datetime_type = 'D',
//float_type = 'F', int_type = 'I', varchar_type = 'V',

  static std::map<Keywords, DataTypes> typesMap{
	{Keywords::integer_kw, DataTypes::int_type},
	{Keywords::float_kw, DataTypes::float_type},
	{Keywords::boolean_kw, DataTypes::bool_type},
	{Keywords::varchar_kw, DataTypes::varchar_type},
	{Keywords::datetime_kw, DataTypes::datetime_type},
  };

static std::map<DataTypes, std::string> DataNames{
  {DataTypes::int_type, "int"},
  {DataTypes::float_type, "float"},
  {DataTypes::bool_type, "bool"},
  {DataTypes::varchar_type, "varchar"},
  {DataTypes::datetime_type, "datetime"},
};
  class Attribute {
  protected:
    std::string   name;
    DataTypes     type;
    uint16_t      size : 10; //max=1000
    uint16_t      autoIncrement : 1;
    uint16_t      primary : 1;
    uint16_t      nullable: 1;
    //Others?
	
  public:
          
    Attribute(DataTypes aType=DataTypes::no_type);
    Attribute(std::string aName, DataTypes aType, uint32_t aSize=0);
    Attribute(const Attribute &aCopy);
    ~Attribute();
    
    Attribute&          setName(std::string &aName);
    Attribute&          setDataType(DataTypes aType);
    Attribute&          setSize(int aSize);
    Attribute&          setAutoIncrement(bool anAuto);
    Attribute&          setPrimaryKey(bool anAuto);
    Attribute&          setNullable(bool aNullable);
    Attribute&			createAttribute(Tokenizer &aTokenizer);
    bool                isValid(); //is this Entity valid?
	
    const std::string&  getName() const {return name;}
	std::string 		getNullable();
	std::string 		getTypeName();
	std::string 		getKey();
    DataTypes           getType() const {return type;}
    size_t              getSize() const {return size;}
    Value               toValue(const std::string &aValue) const;
	
    bool                isPrimaryKey() const {return primary;}
    bool                isNullable() const {return nullable;}
    bool                isAutoIncrement() const {return autoIncrement;}    
	
	StringList getInfo();
	
	void encode(std::ostream &aStream);
	void decode(std::istream &aStream);
	
	Attribute attributeParsing(Tokenizer &aTokenizer);
    void reset(){
        name ="";
        type = DataTypes::no_type;
        size = 10; //max=1000
        autoIncrement = 1;
        primary = 1;
        nullable = 1;
    }
  };
  
  using AttributeOpt = std::optional<Attribute>;
  using AttributeList = std::vector<Attribute>;
  
}


#endif /* Attribute_hpp */
