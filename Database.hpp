//
//  Database.hpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#ifndef Database_hpp
#define Database_hpp

#include <stdio.h>
#include <fstream> 
#include "Storage.hpp"
#include "Entity.hpp"
#include "Helpers.hpp"
#include "Row.hpp"
#include "DBQuery.hpp"

namespace ECE141 {
  using EntityList = std::vector<Entity>;
  class Database {
  public:
            Database(const std::string aName, CreateDB);
            Database(const std::string aName, OpenDB);
    virtual ~Database();

	StatusResult createTable(Entity &anEntity);
	bool doesTableExists(const std::string &aTableName);
	StatusResult dropTable(const std::string &aName, std::ostream& anOutput);
	StatusResult describeTable(const std::string &aName, std::ostream& anOutput);
	StatusResult showTables(std::ostream &anOutput);
    StatusResult dump(std::ostream &anOutput);
	Entity getEntity(const std::string &aName);
	StatusResult insertRows(const std::string &entityName, RowList &aRowList);
	StatusResult selectRows(DBQuery &aQuery, RowList &aRowList, std::ostream& anOutput);
	std::string getPath(){return theDBPath;}
	bool setName(std::string aName);
	std::string getName(){return name;}
	void closeStream(){stream.close();}
	/* Load Data */
	StatusResult loadEntities();
	StatusResult loadRows();
  protected:
	Storage 		theStorage;
	EntityList		DBEntities;
	RowCollection theRows;
	Helpers 		theHelp;
	std::string     name;
	std::string 	theDBPath;
    std::fstream    stream;   //low level stream used by storage
    bool            changed;
  };

}
#endif /* Database_hpp */
