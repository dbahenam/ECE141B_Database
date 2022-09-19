//
//  FolderView.hpp
//  Assignment2
//
//  Created by rick gessner on 2/15/21.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef FolderView_h
#define FolderView_h

#include "FolderReader.hpp"
#include "View.hpp"
#include <iomanip>
#include "Database.hpp"

namespace ECE141 {

  // USE: provide view class that lists db files in storage path...
  class FolderView : public View {
  public:
    FolderView(const char *aPath, const char *anExtension="db")
      : reader(aPath), extension(anExtension) {}
   
    virtual bool show(std::ostream &anOutput) {
      return true;
    }
    
    FolderReader  reader;
    const char    *extension;
	  //added this to fix some issues with the constructor ... it should not be used other than by child classes
	protected:
	  FolderView(): reader(""), extension("db") {}
  };
class CreateDBView : public FolderView{
public:
	CreateDBView(){}
	bool show(std::string DBName, std::ostream& anOutput){
		Database* theDB;
		double theTimeElapsed;
		Timer theTimer;
		theTimer.reset(); //start timer
		//create database
		theDB = new Database(DBName, CreateDB{});
		if(theDB != nullptr){
			theTimeElapsed = theTimer.elapsed();
			anOutput << "Query OK, 1 row affected" << "(" << theTimeElapsed << " secs)" << '\n';
			return true;
		}
		return false;
	}
};
class DropDBView : public FolderView{
public:
	DropDBView(){}
	bool show(std::string DBName, std::ostream& anOutput){
		std::string thePath = Config::getDBPath(DBName);
		if(fs::exists(thePath)){
			double theTimeElapsed;
			Timer theTimer;
			theTimer.reset(); //start timer
			//drop database
			std::remove((const char*)thePath.c_str());
			theTimeElapsed = theTimer.elapsed();
			anOutput << "Query OK, 0 rows affected " << "(" << theTimeElapsed << " secs)" << '\n';
			return true;
		}
		return false;
	}
};
class DumpDBView : public FolderView{
public:
	
	DumpDBView(Database* aDB = nullptr) : theDB(aDB) {}
  
	void setHeader(Storage& aStorage){
	  size_t count = aStorage.getBlockCount();
	  size_t i = 0;
	  while( i <= count){theHeader += section; i++;}
	  theHeader += "+";
	}
	
	void displayHeader(std::ostream& theOutput){
	  theOutput << theHeader << "\n";
	  theOutput << std::setw(width) << std::left << seperator + space + "Type" + space;
	  theOutput << std::setw(width) << std::left << seperator + space + "Id" + space;
	  theOutput << std::setw(width) << std::left << seperator + space + "Extra" + space;
	  theOutput << seperator << "\n";
	  theOutput << theHeader << "\n";
	}
	
	virtual bool show(std::ostream& anOutput, Storage &aStorage){
	  setHeader(aStorage);
	  displayHeader(anOutput);
	  char theType;
	  std::string extra = "";
	  uint32_t theID;
	  size_t aBlockNum = 0;
	  Block aBlock;
	  while(aBlockNum  < aStorage.getBlockCount()){
		aStorage.readBlock(static_cast<uint32_t>(aBlockNum), aBlock);
		theType = aBlock.header.type;
		if(theType == 'E'){
		  theID = aBlock.header.hashID;
		  extra = aBlock.header.extraID;
		}
		else if(theType == 'D'){
		  theID = aBlock.header.key;
		  extra = aBlock.header.extraID;
		}
		else{
		  theID = aBlock.header.key;
		  extra = "";
		}
		anOutput << "| " << std::left << std::setw(width-3) << theType << space;
		anOutput << "| " << std::left << std::setw(width-3) << theID << space;
		anOutput << "| " << std::left << std::setw(width-3) << extra << space << "| " << '\n';
		anOutput << theHeader << "\n";
		aBlockNum++;
	  }
	  return true;
	}
protected:
  std::string section = "+-------------------";
  size_t multiplier = 0;
  int width = 20;
  std::string theHeader = "";
  std::string seperator = "|";
  std::string space = " ";
  std::vector<std::string> attributeNames;
  Database* theDB;
};

class ShowDBView : public FolderView {
	public:
		ShowDBView(const char* aPath, const char* anExtension = "db")
			: reader(aPath), extension(anExtension) {};

		bool show(std::ostream& anOutput) {
			Timer aTimer;
			size_t countDB = 0;
			//aTimer.reset();
			std::string dashes = "-----------";
			std::string spaces = "           ";
			size_t numRows = 0;

			//we can use this to make sure strings are aligned later:
			size_t stringLengths = 20;

			anOutput << "+ --------" << dashes << " +\n";
			anOutput << "| Database" << spaces << " |\n";
			anOutput << "+ --------" << dashes << " +\n";
			std::string theExtension = "." + std::string(extension);
			//using lambdas ... we created the function that prints aFilename
			//then passed this function to each directory using each for it to print the directory's name
			(this->reader).each(theExtension, [&](const std::string& aFilename) {
				anOutput << "| "<< aFilename << spaces << " |\n";
				countDB++;
				return true;
				});
			anOutput << "+ --------" << dashes << " +\n";
			anOutput << countDB << " rows in set ("<< aTimer.elapsed() <<"sec)" << '\n';
			
			return true;
		}
		FolderReader  reader;
		const char* extension;
};
/* Lazy Loaded View, Dynamically changing with Entity as Delagate*/
class TableDescriptionView : public FolderView{
  public:
	TableDescriptionView(Entity& anEntity, std::ostream& anOutput) : theOutput(anOutput), theEntity{anEntity}{}
	std::vector<int> findLengths(AttributeList& aList){
	  std::vector<int> lengths;
	  std::vector<std::string> temp;
	  int max;
	  for(int i = 0; i < attributeField.size(); i++){
		max = 0;
		for(int j = 0; j < aList.size(); j++){
		  if(aList[j].getInfo()[i].size() > max){
			max = aList[j].getInfo()[i].size();
		  }
		}
		if(max < attributeField.size()){max = attributeField.size() + 1;}
		lengths.push_back(max);
	  }
	  return lengths;
	}
	void setHeader(){
	  sections = findLengths(theEntity.attributes);
	  int filler = 3;
	  for(int i = 0; i < sections.size(); i++){
		if(i == sections.size() - 1){
		  theHeader << std::setfill('-') << plus << std::setw(sections[i] + filler + 1);
		}
		else{
		  theHeader << std::setfill('-') << plus << std::setw(sections[i] + filler);
		}
	  }
	  theHeader << "+\n";
	  theOutput << theHeader.str();
	  for(int i = 0; i < sections.size(); i++){
		if(i == sections.size() -1){
		  theOutput << std::setfill(' ') << seperator << space << attributeField[i] << space << std::setw(sections[i] - attributeField[i].size()) << space;
		}
		else{
		  theOutput << std::setfill(' ') << seperator << space << attributeField[i] << space << std::setw(sections[i] - attributeField[i].size()) << space;
		}
	  }
	  theOutput << "|\n" << theHeader.str();
	}
  void show(){
	setHeader();
	std::string aField;
	int filler = 1;
	for(int i = 0; i < theEntity.attributes.size(); i++){
	  for(int j = 0; j < sections.size(); j++){
		aField = theEntity.attributes[i].getInfo()[j];
		theOutput << seperator << space << aField << std::setw(sections[j] + filler - aField.size()) << space;
	  }
	  theOutput << "|\n";
	}
	theOutput << theHeader.str();
  }
  protected:
	std::ostream& theOutput;
	Entity& theEntity;
	std::vector<std::string> attributeField = {"Field", "Type", "NULL", "Key"}; // can add autoincrement, extra
	std::vector<int> sections;
	std::string plus = "+";
	std::string seperator = "|";
	std::string space = " ";
	std::stringstream theHeader;
	int width = 17;
};

  class TabularView : public FolderView{ // select rows
  public:
	TabularView(std::ostream& anOutput, RowList &aRowList) : theOutput(anOutput), theRows(aRowList){}
	
	void setHeader(){
	  for(auto& aRow : theRows){
		for(const auto& thePair : aRow.getData()){
		  attributeNames.push_back(thePair.first);
		  multiplier++;
		}
		break;
	  }
	  size_t i = 0;
	  while( i < multiplier){theHeader += section; i++;}
	  theHeader += "+";
	}
	
	void displayHeader(){
	  theOutput << theHeader << "\n";
	  for(auto& name : attributeNames){
		theOutput << std::setw(width) << std::left << seperator + space + name + space;
	  }
	  theOutput << seperator << "\n";
	  theOutput << theHeader << "\n";
	}
	
	virtual bool show(RowList &aRowList){
	  setHeader();
	  displayHeader();
	  for(auto& aRow : aRowList){
		for(auto& thePair : aRow.getData()){
		  std::visit([&](auto&& value) {
			theOutput << seperator + space;
			theOutput << std::setw(width-3) << std::left <<  value;
			theOutput << space;
		  }, thePair.second);
		}
		theOutput << seperator << "\n";
	  }
	  theOutput << theHeader << "\n";
	  return true;
	}
  protected:
	std::ostream& theOutput;
	RowList theRows;
	std::string section = "+-------------------";
	size_t multiplier = 0;
	int width = 20;
	std::string theHeader = "";
	std::string seperator = "|";
	std::string space = " ";
	std::vector<std::string> attributeNames;
	
  };

}

#endif /* FolderView_h */
