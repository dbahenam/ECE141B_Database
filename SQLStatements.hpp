//
//  SQLStatements.hpp
//  assignment3
//
//  Created by David Bahena Moctezuma on 4/20/22.
//

#ifndef SQLStatements_hpp
#define SQLStatements_hpp

#include <stdio.h>
#include "Statement.hpp"
#include "SQLProcessor.hpp"
#include "Entity.hpp"
#include "Attribute.hpp"
#include "ParseHelper.hpp"
#include "Row.hpp"
#include "DBQuery.hpp"

namespace ECE141 {

class SQLStatement : public Statement {
	public:
		SQLStatement(SQLProcessor* aSQLProc = nullptr, Keywords aStatement = Keywords::unknown_kw)
			: Statement(aStatement), theSQLProc(aSQLProc) {}
		virtual StatusResult  parse(Tokenizer& aTokenizer) {return StatusResult{};}
		virtual StatusResult  dispatchCall(){return StatusResult{};}
		SQLProcessor* theSQLProc;
	};

class CreateTableStatement : public SQLStatement {
	protected:
	  Tokenizer& theTokenizer;
	public:
		CreateTableStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenizer);
		virtual StatusResult  parse();
		virtual StatusResult  dispatchCall();
		SQLProcessor* theSQLProc;
		Entity theEntity;
		Attribute theAttribute;
};

class ShowTableStatement : public SQLStatement {
	protected:
	  Tokenizer& theTokenizer;
	public:
		ShowTableStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenizer);
		virtual StatusResult  parse();
		virtual StatusResult  dispatchCall();
		SQLProcessor* theSQLProc;
		Entity* theEntity;
};

class DropTableStatement : public SQLStatement {
	protected:
	  Tokenizer& theTokenizer;
	public:
		DropTableStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenizer);
		virtual StatusResult  parse();
		virtual StatusResult  dispatchCall();
		SQLProcessor* theSQLProc;
		Entity* theEntity;
		std::string tableName;
};

class DescribeTableStatement : public SQLStatement {
	protected:
	  Tokenizer& theTokenizer;
	public:
		DescribeTableStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenizer);
		virtual StatusResult  parse();
		virtual StatusResult  dispatchCall();
		SQLProcessor* theSQLProc;
		Entity theEntity;
		std::string tableName;
};
class insertRowStatement : public SQLStatement {
	protected:
	  Tokenizer& theTokenizer;
	public:
		insertRowStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenizer);
		virtual StatusResult  parse();
		virtual StatusResult  dispatchCall();
		SQLProcessor* theSQLProc;
		std::string theTableName;
		StringList theKeys;
		ValueList theValues;
		RowList theRows;
};

class SelectRowStatement : public SQLStatement {
	protected:
	  Tokenizer& theTokenizer;
	public:
		SelectRowStatement(SQLProcessor* aSQLProc, Tokenizer& aTokenzier);
		virtual StatusResult  parse();
		virtual StatusResult  dispatchCall();
		SQLProcessor* theSQLProc;
		std::string theTableName;
		DBQuery theQuery;
		RowList theRows;
};
  /* Final Portion */
//  class AlterTableStatement : public SQLStatement{
//  public:
//  AlterTableStatement(SQLProcessor* aSQLProc);
//  virtual StatusResult parse();
//  virtual StatusResult dispatchCall();
//  SQLProcessor* theSQLProc;
//  Keywords alterCommand = Keywords::unknown_kw;
//  std::string theTable;
//  };
  /* ------------- */
}
#endif /* SQLStatements_hpp */
