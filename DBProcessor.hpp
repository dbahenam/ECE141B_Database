//
//  DBProcessor.hpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#ifndef DBProcessor_hpp
#define DBProcessor_hpp

#include <stdio.h>
#include "CmdProcessor.hpp"
#include "Tokenizer.hpp"
#include "Database.hpp"


namespace ECE141 {
  
  class DBProcessor : public CmdProcessor {
  public:
    
    DBProcessor(std::ostream &anOutput);
    ~DBProcessor();
    
      //cmd processor interface...
    CmdProcessor* recognizes(Tokenizer &aTokenizer) override;
    Statement*    makeStatement(Tokenizer &aTokenizer,
                                StatusResult &aResult) override;
    StatusResult  run(Statement *aStmt) override;
               
      //dbProcessor interface...
    StatusResult  createDatabase(const std::string &aName);
    StatusResult  dumpDatabase(const std::string &aName);
    StatusResult  dropDatabase(const std::string &aName);
    StatusResult  showDatabases() const;
    StatusResult  useDatabase(const std::string &aName);
	// recognize commands
	bool recognizeCreate(Tokenizer &aTokenizer);
	bool recognizeDrop(Tokenizer &aTokenizer);
	bool recognizeShow(Tokenizer &aTokenizer);
	bool recognizeUse(Tokenizer &aTokenizer);
	bool recognizeDump(Tokenizer &aTokenizer);
	//
    bool          dbExists(const std::string &aDBName);
	Database*     getDatabaseInUse(){return currentDB;};
	std::ostream &theOutput;
	Keywords theCommand= Keywords::unknown_kw;
	bool recognizedCommand = false;
	StatusResult runResult{noError};
  protected:

    //stuff?
    Database* currentDB = nullptr;
	
    
  };

}
#endif /* DBProcessor_hpp */
