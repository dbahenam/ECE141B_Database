//
//  Row.hpp
//  PA3
//
//  Created by rick gessner on 4/2/22.
//

#ifndef Row_hpp
#define Row_hpp

#include <stdio.h>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <memory>
#include "Attribute.hpp"
#include "BasicTypes.hpp"

//feel free to use this, or create your own version...

namespace ECE141 {

  class Row  {
  public:
	Row() : entityName("empty")  {}
    Row(const Row &aCopy);
    ~Row();
    
    Row& operator=(const Row &aRow);
    bool operator==(Row &aCopy) const;
    
      //STUDENT: What other methods do you require?
                          
    Row&                set(const std::string &aKey,
                            const Value &aValue);
	void				setName(const std::string &aName){entityName = aName;}
        
    KeyValues&          getData() {return data;}
	void				clearData(){data.clear();}
	
	StatusResult mapKeyValue(StringList &aStrList, ValueList &aValueList);
	void		 encode(std::ostream& aStream);
	void 	     decode(std::istream& aStream);
  protected:
    KeyValues           data;
	std::string 		entityName;
  };

  //-------------------------------------------
  using RowCollection = std::vector<std::unique_ptr<Row>>;
  using RowList = std::vector<Row>;

}
#endif /* Row_hpp */
