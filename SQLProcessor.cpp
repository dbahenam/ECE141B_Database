//
//  SQLProcessor.cpp
//  PA3
//
//  Created by rick gessner on 4/2/22.
//

#include "SQLProcessor.hpp"
#include "keywords.hpp"
#include "TestSequencer.hpp"
#include "SQLStatements.hpp"
#include "Timer.hpp"
#include <map>

namespace ECE141 {

  SQLProcessor::SQLProcessor(std::ostream &anOutput, DBProcessor& aDBProc)
	:  CmdProcessor(anOutput) , theDBProc(aDBProc), theOutput(anOutput){
  }

  SQLProcessor::~SQLProcessor() {}

  /* RECOGNIZE KEYWORDS METHODS*/
  bool SQLProcessor::recognizeCreate(Tokenizer &aTokenizer){
	static KWList createTable{Keywords::create_kw, Keywords::table_kw};
	TestSequencer theSeq(aTokenizer);
	if(aTokenizer.tokenize()){
		if(theSeq.clear().nextIs(createTable)){
			theCommand = Keywords::create_kw;
			return true;
		}
	}
	return false;
  }
  bool SQLProcessor::recognizeDescribe(Tokenizer &aTokenizer){
	static KWList describeTable{Keywords::describe_kw};
	TestSequencer theSeq(aTokenizer);
	if(aTokenizer.tokenize()){
		if(theSeq.clear().nextIs(describeTable)){
			theCommand = Keywords::describe_kw;
			return true;
		}
	}
	return false;
  }
  bool SQLProcessor::recognizeDrop(Tokenizer &aTokenizer){
	static KWList dropTable{Keywords::drop_kw, Keywords::table_kw};
	TestSequencer theSeq(aTokenizer);
	if(aTokenizer.tokenize()){
		if(theSeq.clear().nextIs(dropTable)){
			theCommand = Keywords::drop_kw;
			return true;
		}
	}
	return false;
  }
  bool SQLProcessor::recognizeShow(Tokenizer &aTokenizer){
	static KWList showTable{Keywords::show_kw, Keywords::tables_kw};
	TestSequencer theSeq(aTokenizer);
	if(aTokenizer.tokenize()){
		if(theSeq.clear().nextIs(showTable)){
			theCommand = Keywords::show_kw;
			return true;
		}
	}
	return false;
  }
  bool SQLProcessor::recognizeInsert(Tokenizer &aTokenizer){
	static KWList insertInto{Keywords::insert_kw, Keywords::into_kw};
	TestSequencer theSeq(aTokenizer);
	if(aTokenizer.tokenize()){
		if(theSeq.clear().nextIs(insertInto)){
			theCommand = Keywords::insert_kw;
			return true;
		}
	}
	return false;
  }
  bool SQLProcessor::recognizeSelect(Tokenizer &aTokenizer){
	static KWList select{Keywords::select_kw};
	TestSequencer theSeq(aTokenizer);
	if(aTokenizer.tokenize()){
		if(theSeq.clear().nextIs(select)){
			theCommand = Keywords::select_kw;
			return true;
		}
	}
	return false;
  }
  bool SQLProcessor::recognizeAlter(Tokenizer &aTokenizer){
	static KWList alterTable{Keywords::alter_kw, Keywords::table_kw};
	TestSequencer theSeq(aTokenizer);
	if(aTokenizer.tokenize()){
	  if(theSeq.clear().nextIs(alterTable)){
		theCommand = Keywords::alter_kw;
		return true;
	  }
	}
	return false;
  }

  CmdProcessor* SQLProcessor::recognizes(Tokenizer &aTokenizer) {
	if(recognizeCreate(aTokenizer) || recognizeShow(aTokenizer) ||
	   recognizeDescribe(aTokenizer) || recognizeDrop(aTokenizer) ||
	   recognizeInsert(aTokenizer) || recognizeSelect(aTokenizer) ||
	   recognizeAlter(aTokenizer)){
	  status = true;
	  return this;
	}
	return nullptr;
  }

  /* STATEMENTS */
  Statement* SQLProcessor::CreateSQLStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenizer) {
	  Statement* createStatement = new CreateTableStatement(aSQLProc, aTokenizer);
	  return createStatement;
  }
  Statement* SQLProcessor::ShowStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenizer) {
	  Statement* showStatement = new ShowTableStatement(aSQLProc, aTokenizer);
	  return showStatement;
  }
  Statement* SQLProcessor::DropStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenizer) {
	  Statement* dropStatement = new DropTableStatement(aSQLProc, aTokenizer);
	  return dropStatement;
  }
  Statement* SQLProcessor::DescribeStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenizer) {
	  Statement* describeStatement = new DescribeTableStatement(aSQLProc, aTokenizer);
	  return describeStatement;
  }
  Statement* SQLProcessor::InsertStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenizer){
	Statement* insertStatement = new insertRowStatement(aSQLProc, aTokenizer);
	return insertStatement;
  }
  Statement* SQLProcessor::SelectStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenizer){
	Statement* selectStatement = new SelectRowStatement (aSQLProc, aTokenizer);
	return selectStatement;
  }
  Statement* SQLProcessor::makeStatement(Tokenizer &aTokenizer,
									   StatusResult &aResult) {
	static std::map<Keywords, SQL_StmtFactory> sqlFactory = {
	  {Keywords::create_kw, &SQLProcessor::CreateSQLStatement},
	  {Keywords::show_kw, &SQLProcessor::ShowStatement},
	  {Keywords::drop_kw, &SQLProcessor::DropStatement},
	  {Keywords::describe_kw, &SQLProcessor::DescribeStatement},
	  {Keywords::insert_kw, &SQLProcessor::InsertStatement},
	  {Keywords::select_kw, &SQLProcessor::SelectStatement},
	};
	Statement* theStatement = nullptr;
	if (sqlFactory.count(theCommand)){
	  theStatement = (this->*(sqlFactory[theCommand]))(this, aTokenizer); // very ugly-(preferably use ordinary function)
	  theStatement->parse();
	}
	return theStatement;
  }
  
  StatusResult  SQLProcessor::run(Statement *aStmt) {
	aStmt->dispatchCall();
	return StatusResult{noError};
  }

  /* ROUTE COMMANDS */
  StatusResult  SQLProcessor::createTable(Entity& anEntity){
	// statement creates entity object
	// Database must store entity into a block within storage system
	// that will result in the Entity object being encoded into a storageBlock
	Database* theDB = theDBProc.getDatabaseInUse();
	StatusResult theResult{noError};
	Timer timer;
	double timeElapsed;
	timer.reset();
	theResult = theDB->createTable(anEntity);
	timeElapsed = timer.elapsed();
	if(theResult.error == noError){
	  theOutput << "Query OK, 1 row affected (" << timeElapsed << "sec)\n";
	}
	return theResult;
  }

  StatusResult  SQLProcessor::describeTable(const std::string &aName){
	Database* theDB = theDBProc.getDatabaseInUse();
	theDB->describeTable(aName, theOutput);
	return StatusResult{};
  }

  StatusResult  SQLProcessor::dropTable(const std::string &aName){
	StatusResult theResult{noError};
	Database* theDB = theDBProc.getDatabaseInUse();
	theResult = theDB->dropTable(aName, theOutput);
	return theResult;
  }

  StatusResult  SQLProcessor::showTables(){
	StatusResult theResult{noError};
	Database* theDB = theDBProc.getDatabaseInUse();
	
	theResult = theDB->showTables(theOutput);
	return theResult;
  }
  StatusResult SQLProcessor::insertRows(const std::string aTableName, RowList &aRowList){
	StatusResult theResult{noError};
	Database* theDB = theDBProc.getDatabaseInUse();
	// validate table name
	Timer theTimer;
	double theTimeElapsed = 0;
	theTimer.reset();
	if(theDB->doesTableExists(aTableName)){
	  theResult = theDB->insertRows(aTableName, aRowList);
	}
	if(theResult.error == noError){
	  size_t totalRows = aRowList.size();
	  output << "Query OK, " << totalRows << " rows affected(" << theTimeElapsed << " secs)\n";
	}
	return theResult;
  }

  StatusResult SQLProcessor::selectRows(DBQuery &aQuery, RowList &aRowList){
	StatusResult theResult{noError};
	Database* theDB = theDBProc.getDatabaseInUse();
	Timer theTimer;
	double theTimeElapsed = 0;
	theResult = theDB->selectRows(aQuery, aRowList, theOutput);
	theTimeElapsed = theTimer.elapsed();
	output << aRowList.size() << " rows in set (" << theTimeElapsed << " secs)\n";
	return theResult;
  }

}
