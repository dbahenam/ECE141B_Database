//
//  Database.cpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#include <string>
#include <iostream>
#include <iomanip>
#include <map>
#include "Database.hpp"
#include "Config.hpp"
#include "FolderView.hpp"

namespace ECE141 {
  
  Database::Database(const std::string aName, CreateDB)
    : name(aName), changed(true), theStorage(stream) {
	std::string thePath = Config::getDBPath(name);
	if(!fs::exists(thePath)){
		theDBPath = thePath;
	}
	//stream.clear(); // clear flags
	stream.open(theDBPath.c_str(), std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
	stream.seekg(0, std::ios::end);
	//std::cout << "filesize at create: " << stream.tellg();
	stream.close();
    //create for read/write
  }

  Database::Database(const std::string aName, OpenDB)
    : name(aName), theStorage(stream), changed(false) {
      theDBPath = Config::getDBPath(name);
	  stream.open(theDBPath.c_str(), std::ios::binary | std::ios::in | std::ios::out);
	  stream.seekg(0, std::ios::beg);
	  loadEntities();
  }

  Database::~Database() {
    if(changed) {
      //stuff to save?
    }
  }
  StatusResult Database::loadEntities(){
	StatusResult theResult{noError};
	int count = 0;
	Block theBlock;
	Entity theEntity;
	
	Row theRow;
	std::stringstream aStream;
	/* load data for storage */
	while(count < theStorage.getBlockCount()){
	  theStorage.readBlock(count, theBlock);
	  theStorage.storageBlocks.push_back(theBlock);
	  aStream.write(theBlock.payload, sizeof(theBlock));
	  if(theBlock.header.type == 'E'){
		theEntity.decode(aStream);
		DBEntities.push_back(theEntity);
	  }
	  else if(theBlock.header.type == 'D'){
		theRow.decode(aStream);
		theRows.push_back(std::unique_ptr<Row>(new Row(theRow)));
	  }
	  aStream.str("");
	  count++;
	}
	
	return theResult;
  }
  StatusResult Database::createTable(Entity &anEntity){
	StatusResult theResult{noError};
	if(!theStorage.isAvailable(anEntity.getName())){
	  theResult.error = tableExists;
	  return theResult;
	}
	/* set block header information */
	Block theBlock;
	uint32_t theHashID = anEntity.getHashID();
	uint32_t theKey = anEntity.setPrimaryKey();
	uint32_t theBlockID = theStorage.getBlockID();
	std::string extraID = anEntity.getName();
	theBlock.setType('E').setBlockID(theBlockID).setHashID(theHashID).setKeyNum(theKey).setExtraID(extraID);
	
	/* encode entity */
	std::stringstream data;
	anEntity.encode(data);
	std::string entityData = data.str();
	
	/* store block */
	std::memcpy(&theBlock.payload, entityData.c_str(), entityData.size());
	theStorage.writeBlock(theBlockID, theBlock);
	theStorage.storageBlocks.push_back(theBlock);
	
	Entity theEntity(anEntity);
	DBEntities.push_back(theEntity);
	return StatusResult{noError};
  }

  bool Database::doesTableExists(const std::string &aTableName){
	for(Entity anEntity : DBEntities){
	  if(anEntity.getName() == aTableName){return true;}
	}
	return false;
  }

  StatusResult Database::dropTable(const std::string &aName, std::ostream& anOutput){
	// theStorage.drop table
	StatusResult theResult{noError};
	Timer timer;
	double timeElapsed;
	timer.reset();
	size_t numRows = 0;
	for(std::vector<Entity>::iterator it = DBEntities.begin(); it != DBEntities.end(); ++it){
	  if(it->getName() == aName){
		  DBEntities.erase(it);
		  numRows++;
		  break;
	  }
	}
	timeElapsed = timer.elapsed();
	if(theResult.error == noError){
	  anOutput << "Query OK, " << numRows <<  " rows affected (" << timeElapsed << "sec)\n";
	}
	return theResult;
  }

  StatusResult Database::showTables(std::ostream &anOutput){
	StatusResult theResult{noError};
	std::string header = "+----------+";
	anOutput << header << "\n";
	std::string bar = "|";
	Timer timer;
	double timeElapsed;
	timer.reset();
	anOutput << bar << "Tables" << bar << "\n";
	for(std::vector<Entity>::iterator it = DBEntities.begin(); it != DBEntities.end(); ++it){
	  anOutput << bar << it->getName() << bar << "\n";
	}
	anOutput << header << "\n";
	timeElapsed = timer.elapsed();
	size_t rows = DBEntities.size();
	anOutput << rows << " rows in set (" << timeElapsed << "sec)\n";
	return theResult;
  }

  StatusResult Database::describeTable(const std::string &aName,std::ostream& anOutput){
	StatusResult theResult{noError};
	Timer timer;
	double timeElapsed;
	std::string bar = "|";
	std::string dashes = "--------";
	std::string plus = "+";
	std::string header = plus+dashes+plus+dashes+dashes+plus+dashes+plus+dashes+plus+dashes+plus+dashes+plus;
	std::cout << header << "\n";
	std::cout << bar << " Field" << std::right << std::setw(3);
	std::cout << bar << " Type" << std::right << std::setw(12);
	std::cout << bar << " Null" << std::right << std::setw(4);
	std::cout << bar << " Key" << std::right << std::setw(5);
	std::cout << bar << " Default" << std::right << std::setw(1);
	std::cout << bar << " Extra" << std::right << std::setw(3) << bar << "\n";
	std::cout << header << "\n";
	timer.reset();
	size_t counter = 0;
	for(Entity anEntity : DBEntities){
	  if(anEntity.getName() == aName){
		//call view
		for(Attribute anAttr : anEntity.attributes){
		  anOutput << std::left << std::setw(4) << "| " << anAttr.getName() << std::right << std::setw(6);
		  anOutput << std::left << std::setw(4) << "| " << anAttr.getTypeName() << std::right << std::setw(15);
		  anOutput << std::left << std::setw(4) << "| " << anAttr.getNullable() << std::right << std::setw(8);
		  anOutput << std::left << std::setw(4) << "| " << anAttr.getKey() << std::right << std::setw(8) << "\n";
		  counter++;
		}
	  }
	}
	timeElapsed = timer.elapsed();
	//size_t rows = DBEntities.size();
	anOutput << counter << " rows in set (" << timeElapsed << ")" << "\n";
	return theResult;
  }

  Entity Database::getEntity(const std::string &aName){
	Entity theEntity;
	for(Entity anEntity : DBEntities){
	  if (anEntity.getName() == aName){
		return anEntity;
	  }
	}
	return theEntity;
  }
  StatusResult Database::insertRows(const std::string &entityName, RowList &aRowList){
	StatusResult theResult{noError};
	Entity theEntity = getEntity(entityName);
	for (auto& aRow : aRowList) {
		aRow.setName(entityName);
		Block theBlock(BlockType::data_block);
	  
		/* set block header information */
		uint32_t theHashID = theEntity.getHashID();
		uint32_t theKey = theEntity.setPrimaryKey();
		uint32_t theBlockID = theStorage.getBlockID();
		std::string extraID = theEntity.getName();
		theBlock.setType('D').setBlockID(theBlockID).setHashID(theHashID).setKeyNum(theKey).setExtraID(extraID);
	
		/* encode Row information and store it */
		std::stringstream data;
		aRow.encode(data);
		std::string theRowInfo = data.str();
		std::memcpy(&theBlock.payload, theRowInfo.c_str(), theRowInfo.size());
		theStorage.storageBlocks.push_back(theBlock);
		theStorage.writeBlock(theBlockID, theBlock);
	}
	return theResult;
  }

  StatusResult Database::selectRows(DBQuery &aQuery, RowList &aRowList, std::ostream& anOutput){
	StatusResult theResult{noError};
	Entity theEntity = aQuery.getEntity();
	uint32_t theHashID = 0;
	for(Block &aBlock : theStorage.storageBlocks){
	  theHashID = aBlock.header.hashID;
	  if(aBlock.header.type == 'D' && theHashID == theEntity.getHashID()){
		Block theBlock;
		theResult = theStorage.readBlock(aBlock.header.blockID, theBlock);
		std::stringstream output;
		output.write(theBlock.payload,sizeof(theBlock.payload));
		std::string test_output = output.str(); // debug
		std::stringstream input(output.str());
		Row aRow;
		aRow.decode(input);
		aRowList.push_back(aRow);
	  }
	}
	//aQuery.filter
	TabularView aSelectView(anOutput, aRowList);
	aSelectView.show(aRowList);
	return theResult;
  }

	
  // USE: Call this to dump the db for debug purposes...
  StatusResult Database::dump(std::ostream &anOutput) {
	  //we print the view? temporary view:
	StatusResult theResult{noError};
	DumpDBView theView(this);
	if(theView.show(anOutput, theStorage)){
	  return theResult;
	}
	else{theResult.error = unknownError;}
	return theResult;
  }

  bool Database::setName(std::string aName){
	  if(aName != name){
		  name = aName;
		  return true;
	  }
	  return false;
  }
}
