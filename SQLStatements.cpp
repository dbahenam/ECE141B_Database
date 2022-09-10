//
//  SQLStatements.cpp
//  assignment3
//
//  Created by David Bahena Moctezuma on 4/20/22.
//

#include "SQLStatements.hpp"
#include "keywords.hpp"
#include "Tokenizer.hpp"

namespace ECE141{

  CreateTableStatement::CreateTableStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenizer) : theSQLProc(aSQLProc), theTokenizer(aTokenizer){}
  StatusResult CreateTableStatement::parse(){
	  //Create Table test1
	  //(id int NOT NULL auto_increment primary key,
	  //first_name varchar(50) NOT NULL, last_name varchar(50));
	  StatusResult theResult{noError};
	  while(theTokenizer.current().type != TokenType::identifier){
		theTokenizer.next(1);
	  }
	  std::string tableName = theTokenizer.current().data;
	  theEntity.setNameID(tableName);
	  theTokenizer.next(1);
	  while(theTokenizer.more() && theTokenizer.current().data != ";"){
		//std::cout << aTokenizer.current().data << "\n";
		theAttribute = theAttribute.attributeParsing(theTokenizer);
		theEntity.addAttribute(theAttribute);
		theTokenizer.next(1);
	  }
	  return theResult;
  }

  StatusResult  CreateTableStatement::dispatchCall(){
	  return theSQLProc->createTable(theEntity);
  }

  ShowTableStatement::ShowTableStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenizer) : theSQLProc(aSQLProc), theTokenizer(aTokenizer){}
  StatusResult ShowTableStatement::parse(){
	//Show Tables;
	StatusResult theResult{noError};
	theTokenizer.next(2); // move past show tables;
	return theResult;
  }
//create table test1(id int NOT NULL auto_increment primary key, first_name varchar(50) NOT NULL, last_name varchar(50))
  StatusResult ShowTableStatement::dispatchCall(){
	return theSQLProc->showTables();
  }

  DropTableStatement::DropTableStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenizer) : theSQLProc(aSQLProc), theTokenizer(aTokenizer){}
  StatusResult DropTableStatement::parse(){
	// drop table <tableName>
	StatusResult theResult{noError};
	theTokenizer.next(2); // move past drop table
	tableName = theTokenizer.current().data; // get table name
	theTokenizer.next(1); // move past table name
	return theResult;
  }
  StatusResult DropTableStatement::dispatchCall(){
	return theSQLProc->dropTable(tableName);
  }

  DescribeTableStatement::DescribeTableStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenizer) : theSQLProc(aSQLProc), theTokenizer(aTokenizer) {}
  StatusResult DescribeTableStatement::parse(){
	// describe <tableName>
	StatusResult theResult{noError};
	tableName = theTokenizer.current().data; // get table name
	theTokenizer.next(1); // move past name
	return theResult;
  }
  StatusResult DescribeTableStatement::dispatchCall(){
	return theSQLProc->describeTable(tableName);
  }

  insertRowStatement::insertRowStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenizer) : theSQLProc(aSQLProc), theTokenizer(aTokenizer){}
  StatusResult insertRowStatement::parse(){
	// Insert Into tableName ('first_name', 'last_name', 'email')
	// Values ('David', 'He', 'dyhe@ucsd.edu')
	TokenSequencer theSeq(theTokenizer);
	StatusResult theResult{noError};
	// parse through command to get string list  of attribute names
	//std::cout << aTokenizer.current().data << std::endl;
	theTableName = theTokenizer.current().data;
	theTokenizer.next(2);
	
	/* Get Entity from table name to do validation */
	Database* currentDB = theSQLProc->getCurrentDB();
	Entity theEntity = currentDB->getEntity(theTableName);
	
	//std::cout << aTokenizer.current().data << std::endl;
	ParseHelper parseHelper(theTokenizer);
	parseHelper.parseKeyList(theKeys);
	//std::cout << aTokenizer.current().data << std::endl;
	theTokenizer.skipIf(')');
	//validate key list as acceptable field values
	
	
	if(theTokenizer.current().keyword == Keywords::values_kw){
	  theTokenizer.next(2); // skip past values and (
	  Row aRow;
	  while(theTokenizer.more() && theTokenizer.current().data!= ";"){
		theResult = parseHelper.parseValueList(theValues);
		theResult = theEntity.isValidName(theKeys);
		if(theResult == noError){
		  theResult = theEntity.isValidType(theKeys, theValues);
		}
		if(theResult == noError){
		  if(theValues.size() != 0){
			aRow.mapKeyValue(theKeys,theValues);
			theRows.push_back(aRow);
		  }
		  aRow.clearData();
		  theValues.clear();
		  theTokenizer.next(1);
		}
	  }
		
	}
	return theResult;
  }
  StatusResult insertRowStatement::dispatchCall(){
	return theSQLProc->insertRows(theTableName, theRows);
  }
  SelectRowStatement::SelectRowStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenizer) : theSQLProc(aSQLProc), theTokenizer(aTokenizer){}
  StatusResult SelectRowStatement::parse(){
	StatusResult theResult{noError};
	//SELECT * from Users;
	//Select * from Users where first_name="Anirudh"
	//Select last_name, email from Users Order By age;
	//Select last_name, email from Users Limit 5;
	ParseHelper parseHelper(theTokenizer);

	if(parseHelper.parseStar()){
	  // validate table name
	  theTableName = theTokenizer.current().data;
	  theTokenizer.next();
	  if(theTokenizer.current().data == ";"){
		Database* currentDB = theSQLProc->getCurrentDB();
		Entity theEntity = currentDB->getEntity(theTableName);
		theQuery.setEntity(theEntity);
		theQuery.setTableName(theTableName);
	  }
	  else{
		parseHelper.parseClause(theTokenizer.current().keyword);
	  }
	}
	else if(false ){
	  
	}
	return theResult;
  }
  StatusResult SelectRowStatement::dispatchCall(){
	return theSQLProc->selectRows(theQuery, theRows);
  }
  /* Final Portion */
//  AlterTableStatement::AlterTableStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenizer) : theSQLProc(aSQLProc), th{}
//  StatusResult AlterTableStatement::parse(){
//	//ALTER TABLE Books add(drop) pub_year varchar(4);
//	//Query Ok, 20 rows affected (0.00123 secs)
//	
//	StatusResult theResult{noError};
//	theTable = aTokenizer.current().data;
//	aTokenizer.next(1);
//	ParseHelper theHelp(aTokenizer);
//	Token currentToken = aTokenizer.current();
//	alterCommand = currentToken.keyword;
//	if(currentToken.keyword != Keywords::add_kw || currentToken.keyword != Keywords::drop_kw){
//	  theResult = syntaxError;
//	  alterCommand = Keywords::unknown_kw;
//	}
//	return theResult;
//  }
//  StatusResult AlterTableStatement::dispatchCall(){
//	StatusResult theResult{noError};
//	
//	return theResult;
//  }
  /* ------------- */
}
