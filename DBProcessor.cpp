//
//  DBProcessor.cpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#include "DBProcessor.hpp"
#include <map>
#include <fstream>
#include "Config.hpp"
#include "DBProcessor.hpp"
#include "FolderView.hpp"
#include "FolderReader.hpp"
#include "Database.hpp"
#include "Application.hpp"
#include "Helpers.hpp"
#include "DBStatements.hpp"
#include "TestSequencer.hpp"
#include "keywords.hpp"

namespace ECE141 {

  //like a typedef!!!
  //declaring pointer to an ordinary function that takes two parameters (*aProc, &aTokenize) and returns a statement pointer
  using StmtFactory = Statement* (*)(DBProcessor *aProc,
                                   Tokenizer &aTokenize);

  DBProcessor::DBProcessor(std::ostream &anOutput)
    : CmdProcessor(anOutput), theOutput(anOutput) {
  }
  
  DBProcessor::~DBProcessor() {
  }
//--------- Recognize commands

bool DBProcessor::recognizeCreate(Tokenizer &aTokenizer){
	static KWList createDB{Keywords::create_kw, Keywords::database_kw};
	TestSequencer theSeq(aTokenizer);
	if(aTokenizer.tokenize()){
		if(theSeq.clear().nextIs(createDB)){
			theCommand = Keywords::create_kw;
			return true;
		}
	}
	return false;
}
bool DBProcessor::recognizeDrop(Tokenizer &aTokenizer){
	static KWList dropDB{Keywords::drop_kw, Keywords::database_kw};
	TestSequencer theSeq(aTokenizer);
	if(aTokenizer.tokenize()){
		if(theSeq.clear().nextIs(dropDB)){
			theCommand = Keywords::drop_kw;
			return true;
		}
	}
	return false;
}
bool DBProcessor::recognizeShow(Tokenizer &aTokenizer){
	static KWList showDB{Keywords::show_kw, Keywords::databases_kw};
	TestSequencer theSeq(aTokenizer);
	if(aTokenizer.tokenize()){
		if(theSeq.clear().nextIs(showDB)){
			theCommand = Keywords::show_kw;
			return true;
		}
	}
	return false;
}
bool DBProcessor::recognizeUse(Tokenizer &aTokenizer){
	static KWList useDB{Keywords::use_kw};
	TestSequencer theSeq(aTokenizer);
	if(aTokenizer.tokenize()){
		if(theSeq.clear().nextIs(useDB)){
			theCommand = Keywords::use_kw;
			return true;
		}
	}
	return false;
}
bool DBProcessor::recognizeDump(Tokenizer &aTokenizer){
	static KWList dumpDB{Keywords::dump_kw, Keywords::database_kw};
	TestSequencer theSeq(aTokenizer);
	if(aTokenizer.tokenize()){
		if(theSeq.clear().nextIs(dumpDB)){
			theCommand = Keywords::dump_kw;
			if(aTokenizer.current().data == ";"){
			  return false;
			}
			return true;
		}
	}
	return false;
}


//---------

  //CmdProcessor interface ...
  CmdProcessor* DBProcessor::recognizes(Tokenizer &aTokenizer) {
	if(recognizeCreate(aTokenizer) ||
	   recognizeDrop(aTokenizer) ||
	   recognizeShow(aTokenizer) ||
	   recognizeUse(aTokenizer) ||
	   recognizeDump(aTokenizer)){
		  recognizedCommand = true; // recognized command
		  return this;
	}
    return nullptr;
  }

	Statement* CreateStatement(DBProcessor* aDBProc, Tokenizer &aTokenizer) {
		Statement* createStatement = new CreateDBStatement(aDBProc, aTokenizer);
		return createStatement;
	}
	Statement* DropStatement(DBProcessor* aDBProc, Tokenizer &aTokenizer) {
		Statement* dropStatement = new DropDBStatement(aDBProc, aTokenizer);
		return dropStatement;
	}
	Statement* ShowStatement(DBProcessor* aDBProc, Tokenizer &aTokenizer){
		Statement* showStatement = new ShowDBStatement(aDBProc, aTokenizer);
		return showStatement;
	}
	Statement* UseStatement(DBProcessor* aDBProc, Tokenizer &aTokenizer){
		Statement* useStatement = new UseDBStatement(aDBProc, aTokenizer);
		return useStatement;
	}
	Statement* DumpStatement(DBProcessor* aDBProc, Tokenizer &aTokenizer){
		Statement* dumpStatement = new DumpDBStatement(aDBProc, aTokenizer);
		return dumpStatement;
	}
	
  StatusResult DBProcessor::run(Statement *aStmt) {
	aStmt->dispatchCall();
    return runResult;
  }

  // USE: retrieve a statement based on given text input...
  Statement* DBProcessor::makeStatement(Tokenizer &aTokenizer,
                                        StatusResult &aResult) {
	static std::map<Keywords, StmtFactory> dbFactory = {
	  {Keywords::create_kw, CreateStatement},
	  {Keywords::drop_kw, DropStatement},
	  {Keywords::show_kw, ShowStatement},
	  {Keywords::use_kw, UseStatement},
	  {Keywords::dump_kw, DumpStatement},
	};
	Statement* theStatement = nullptr;
	if (dbFactory.count(theCommand)){
	  theStatement = dbFactory[theCommand](this, aTokenizer);
	  theStatement->parse();
	}
    return theStatement; // a statement that will be returned
	// the statement will have a dispatchCall that will
	// perform an action depending on theCommand
  }
  
  //-------------------------------------------------------


  bool DBProcessor::dbExists(const std::string &aDBName) {
    return false;
  }

  StatusResult DBProcessor::createDatabase(const std::string &aName) {
	std::string theDBPath = Config::getDBPath(aName);
	CreateDBView theCreateView;
	theCreateView.show(aName, output);
	return StatusResult{noError};
  }
  
  StatusResult DBProcessor::showDatabases() const {
    //make a view, load with DB names from storage folder...
	  std::string filePath = Config::getStoragePath();
	  ShowDBView dbView( ( filePath.c_str() ) , "db");
	  dbView.show(output);
	  return StatusResult{ noError };
  }

  // USE: call this to perform the dropping of a database (remove the file)...
  StatusResult DBProcessor::dropDatabase(const std::string &aName) {
	DropDBView theView;
	if(theView.show(aName, output)){
		return StatusResult{noError};
	}
    return StatusResult{unknownDatabase};
  }

  // USE: DB dump all storage blocks
  StatusResult DBProcessor::dumpDatabase(const std::string &aName)  {
	if (std::filesystem::exists(Config::getDBPath(aName))) {    //if the database exists
	  if(currentDB == nullptr){
		  currentDB = new Database(aName, OpenDB{});
	  }
	  if(currentDB->getName() != aName){
		  delete currentDB;
		  currentDB = nullptr;
		  currentDB = new Database(aName, OpenDB{});
	  }
	}
	currentDB->dump(output);
    return StatusResult{ECE141::unknownDatabase};
  }

  // USE: call DB object to be loaded into memory...
  StatusResult DBProcessor::useDatabase(const std::string &aName) {
	StatusResult theResult{noError};
	std::string thePath = Config::getDBPath(aName);
	if(currentDB != nullptr){
	  if(currentDB->getPath() == thePath){
		output << "Database " << aName << " in use\n";
		return theResult;
	  }
	}
	else if(fs::exists(thePath)){
		currentDB = new Database(aName, OpenDB());
		output << "Database changed\n";
		return theResult;
	}
	else{
	  theResult = unknownDatabase;
	  output << "Unknown database\n";
	}
    return theResult;
  }

}
