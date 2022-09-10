//
//  CommandProcessor.cpp
//
//  Created by rick gessner on 3/30/20
//  Copyright © 2018 rick gessner. All rights reserved.
//

#include <iostream>
#include "Application.hpp"
#include "Tokenizer.hpp"
#include <memory>
#include <algorithm>
#include <vector>
#include "Config.hpp"
#include "TestSequencer.hpp"

namespace ECE141 {
  
  Application::Application(std::ostream &anOutput)
    : CmdProcessor(anOutput), theDBProc(anOutput), theSQLProc(anOutput, theDBProc) {
  }
  
  Application::~Application() {}
  
  // USE: -----------------------------------------------------
  
  bool isKnown(Keywords aKeyword) {
    static Keywords theKnown[]=
      {
		  Keywords::quit_kw,Keywords::version_kw, Keywords::help_kw
	  };
    auto theIt = std::find(std::begin(theKnown),
                           std::end(theKnown), aKeyword);
    return theIt!=std::end(theKnown);
  }

  CmdProcessor* Application::recognizes(Tokenizer &aTokenizer) {
    if(isKnown(aTokenizer.current().keyword)) {
      return this;
    }
	CmdProcessor* DBProc = theDBProc.recognizes(aTokenizer); // check if command is for DBProcessor
	if(DBProc != nullptr){
	  return DBProc;
	}
	else{
	  CmdProcessor* SQLProc = theSQLProc.recognizes(aTokenizer); // check if command is for SQLProcessor
	  return SQLProc;
	}
	return nullptr;
  }

  StatusResult Application::run(Statement* aStatement) {
    switch(aStatement->getType()) {
      case Keywords::quit_kw:
        output << "DB::141 is shutting down\n";
        return StatusResult(ECE141::userTerminated);
        break;
      case Keywords::version_kw:
        output << "Version " << getVersion() << "\n"; break;
      case Keywords::help_kw:
        output << "Help system available\n"; break;
      default: break;
    }
    return StatusResult{Errors::noError};
  }
  
  // USE: retrieve a statement based on given text input...
  Statement* Application::makeStatement(Tokenizer &aTokenizer,
                                        StatusResult &aResult) {
    Token theToken=aTokenizer.current();
    if (isKnown(theToken.keyword)) {
      aTokenizer.next(); //skip ahead...
      return new Statement(theToken.keyword);
    }
	// check if it is command for dbProcessor
	if(true == theDBProc.recognizedCommand){
	  return theDBProc.makeStatement(aTokenizer, aResult);
	}
	else if(true == theSQLProc.getStatus()){
	  return theSQLProc.makeStatement(aTokenizer, aResult);
	}
    return nullptr;
  }

  //build a tokenizer, tokenize input, ask processors to handle...
  StatusResult Application::handleInput(std::istream &anInput){
    Tokenizer theTokenizer(anInput);
    StatusResult theResult=theTokenizer.tokenize();
    
    while (theResult && theTokenizer.more()) {
	  std::string current = theTokenizer.current().data;
	  if(current == ";"){
		  theTokenizer.next(1);
		  break;
	  }
      Config::getTimer().reset();
      if(auto *theProc=recognizes(theTokenizer)) {
        if(auto *theCmd=theProc->makeStatement(theTokenizer,theResult)) {
          theResult=theProc->run(theCmd);
          if(theResult) theTokenizer.skipIf(semicolon);
          delete theCmd;
        }
      }
      else theResult=unknownCommand;
    }
    return theResult;
  }

}
