//
//  DBStatements.cpp
//  upgraded_assignment2
//
//  Created by David Bahena Moctezuma on 4/22/22.
//

#include "DBStatements.hpp"
#include "keywords.hpp"
namespace ECE141{
	CreateDBStatement::CreateDBStatement(DBProcessor* aDBProc, Tokenizer &aTokenzier) : theDBProc(aDBProc), theTokenizer(aTokenzier){}
	StatusResult CreateDBStatement::parse(){
		//CREATE DATABASE DBNAME
		StatusResult theResult{noError};
		theTokenizer.next(2); // move past CREATE DATABASE
		DBName = theTokenizer.current().data; // name of DATABASE
		theTokenizer.next(1);
		return theResult;
	}
	StatusResult  CreateDBStatement::dispatchCall(){
		return theDBProc->createDatabase(DBName);
	}

	DropDBStatement::DropDBStatement(DBProcessor* aDBProc, Tokenizer &aTokenizer) : theDBProc(aDBProc), theTokenizer(aTokenizer){}
	StatusResult DropDBStatement::parse(){
		// Drop Database DBName
		StatusResult theResult{noError};
		theTokenizer.next(2); // move past DROP DATABASE
		droppedDB = theTokenizer.current().data; // name of dropped database
		theTokenizer.next(1);
		return theResult;
	}
	StatusResult  DropDBStatement::dispatchCall(){
		return theDBProc->dropDatabase(droppedDB);
	}
	ShowDBStatement::ShowDBStatement(DBProcessor* aDBProc, Tokenizer &aTokenizer) : theDBProc(aDBProc), theTokenizer(aTokenizer){}
	StatusResult ShowDBStatement::parse(){
		// Show Databases
		StatusResult theResult{noError};
		theTokenizer.next(2); // move past DROP DATABASE
		theTokenizer.next(1);
		return theResult;
	}
	StatusResult  ShowDBStatement::dispatchCall(){
		return theDBProc->showDatabases();
	}
	UseDBStatement::UseDBStatement(DBProcessor* aDBProc, Tokenizer &aTokenizer) : theDBProc(aDBProc), theTokenizer(aTokenizer){}
	StatusResult UseDBStatement::parse(){
		StatusResult theResult{noError};
		theDBName = theTokenizer.current().data;
		theTokenizer.next(1);
		return theResult;
	}
	StatusResult  UseDBStatement::dispatchCall(){
		return theDBProc->useDatabase(theDBName);
	}
	DumpDBStatement::DumpDBStatement(DBProcessor* aDBProc, Tokenizer &aTokenizer) : theDBProc(aDBProc), theTokenizer(aTokenizer){}
	StatusResult DumpDBStatement::parse(){
		// dump database {db-name}
		StatusResult theResult{noError};
		theTokenizer.next(2); // move past dump DATABASE
		theDBName = theTokenizer.current().data; // database to be dumped
		theTokenizer.next(1);
		return theResult;
	}
	StatusResult  DumpDBStatement::dispatchCall(){
		return theDBProc->dumpDatabase(theDBName);
	}
}
