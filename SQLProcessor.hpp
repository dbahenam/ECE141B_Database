//
//  SQLProcessor.hpp
//  PA3
//
//  Created by rick gessner on 4/2/22.
//

#ifndef SQLProcessor_hpp
#define SQLProcessor_hpp

#include <stdio.h>
#include "CmdProcessor.hpp"
#include "Tokenizer.hpp"
#include "DBProcessor.hpp"
#include "BasicTypes.hpp"
#include "Row.hpp"

namespace ECE141 {

  class Statement;
  class DBProcessor; //define this later...
  class Entity;
  class Database;

  //Should this processor have it's own Statement types?

  class SQLProcessor : public CmdProcessor {
  public:
	//like a typedef!!!
	//declaring pointer to a member function that takes two parameters (*aProc, &aTokenize) and returns a statement pointer
	using SQL_StmtFactory = Statement* (SQLProcessor::*)(SQLProcessor* aProc,
									 Tokenizer& aTokenize);
	
	SQLProcessor(std::ostream &anOutput, DBProcessor& aDBProc);
	virtual ~SQLProcessor();
	
	CmdProcessor* recognizes(Tokenizer &aTokenizer) override;
	Statement*    makeStatement(Tokenizer &aTokenizer,
								StatusResult &aResult) override;
	
	StatusResult  run(Statement *aStmt) override;
	/* RECOGNIZE METHODS*/
	bool recognizeCreate(Tokenizer &aTokenizer);
	bool recognizeDescribe(Tokenizer &aTokenizer);
	bool recognizeDrop(Tokenizer &aTokenizer);
	bool recognizeShow(Tokenizer &aTokenizer);
	bool recognizeInsert(Tokenizer &aTokenizer);
	bool recognizeSelect(Tokenizer &aTokenizer);
	/* Final Portion*/
	bool recognizeAlter(Tokenizer& aTokenizer);
	
	/* STATEMENTS */
	Statement* CreateSQLStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenizer);
	Statement* ShowStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenizer);
	Statement* DropStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenizer);
	Statement* DescribeStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenizer);
	Statement* InsertStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenizer);
	Statement* SelectStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenizer);
	Statement* AlterStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenizer);

	//We don't DO the work here, but we can help route cmds...
	StatusResult  createTable(Entity &anEntity);
	StatusResult  describeTable(const std::string &aName);
	StatusResult  dropTable(const std::string &aName);
	StatusResult  showTables();
	
	/* Row data calls*/
	StatusResult insertRows(const std::string aTableName, RowList &aRowList);
	StatusResult selectRows( DBQuery &aQuery, RowList &aRowList);
	
	bool getStatus(){return status;}
	Database* getCurrentDB(){return theDBProc.getDatabaseInUse();}
	
  protected:
	DBProcessor& theDBProc;
	std::ostream& theOutput;
	Keywords theCommand = Keywords::unknown_kw;
	bool status = false;
  };

}
#endif /* SQLProcessor_hpp */
