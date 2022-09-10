//
//  DBStatements.hpp
//  upgraded_assignment2
//
//  Created by David Bahena Moctezuma on 4/22/22.
//

#ifndef DBStatements_hpp
#define DBStatements_hpp

#include <stdio.h>
#include "Statement.hpp"
#include "DBProcessor.hpp"

namespace ECE141 {

class DBStatement : public Statement {
	public:
		DBStatement(DBProcessor* aDBProcessor = nullptr, Keywords aStatement = Keywords::unknown_kw)
			: Statement(aStatement), theDBProc(aDBProcessor) {}
		virtual StatusResult  parse(Tokenizer& aTokenizer) {return StatusResult{};}
		virtual StatusResult  dispatchCall(){return StatusResult{};}
		DBProcessor* theDBProc;
		
	};

class CreateDBStatement : public DBStatement {
	protected:
	  Tokenizer& theTokenizer;
	public:
		CreateDBStatement(DBProcessor* aDBProcessor, Tokenizer &aTokenizer);
		virtual StatusResult  parse();
		virtual StatusResult  dispatchCall();
		DBProcessor* theDBProc;
		std::string DBName;
};

class DropDBStatement : public DBStatement {
	protected:
	  Tokenizer& theTokenizer;
	public:
		DropDBStatement(DBProcessor* aDBProcessor, Tokenizer &aTokenizer);
		virtual StatusResult  parse();
		virtual StatusResult  dispatchCall();
		DBProcessor* theDBProc;
		std::string droppedDB;
};
class ShowDBStatement : public DBStatement {
	protected:
	  Tokenizer& theTokenizer;
	public:
		ShowDBStatement(DBProcessor* aDBProcessor, Tokenizer &aTokenizer);
		virtual StatusResult  parse();
		virtual StatusResult  dispatchCall();
		DBProcessor* theDBProc;
};
class UseDBStatement : public DBStatement {
	protected:
	  Tokenizer& theTokenizer;
	public:
		UseDBStatement(DBProcessor* aDBProcessor, Tokenizer &aTokenizer);
		virtual StatusResult  parse();
		virtual StatusResult  dispatchCall();
		DBProcessor* theDBProc;
		std::string theDBName;
};
class DumpDBStatement : public DBStatement {
	protected:
	  Tokenizer& theTokenizer;
public:
	   DumpDBStatement(DBProcessor* aDBProcessor, Tokenizer &aTokenizer);
	   virtual StatusResult  parse();
	   virtual StatusResult  dispatchCall();
	   DBProcessor* theDBProc;
	   std::string theDBName;
};

	
}

#endif /* DBStatements_hpp */
