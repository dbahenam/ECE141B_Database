//
//  Storage.cpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//


#include <sstream>
#include <cmath>
#include <cstdlib>
#include <optional>
#include <cstring>
#include "Storage.hpp"
#include "Config.hpp"

namespace ECE141 {

  //storage class?
  Storage::Storage(std::iostream& aStream) : BlockIO(aStream) {
  }
  bool Storage::isAvailable(const std::string &aName){
	size_t anID = theHelp.hashString(aName.c_str());
	for(Block aBlock : storageBlocks){
	  if(anID == aBlock.header.hashID){
		return false;
	  }
	}
	return true;
  }
  bool Storage::each(Block& aBlock, uint32_t aBlockNum) {
	  if(readBlock(aBlockNum,aBlock).error == noError){
		  return true;
	  }
	  return false;
  }
  void Storage::setIndex(){
	uint32_t counter = 0;
	for(Block aBlock : storageBlocks){
	  if(aBlock.header.type == 'F'){
		index = counter;
	  }
	  counter++;
	}
	index++;
  }

  uint32_t Storage::getBlockID(){
	uint32_t count = 0;
	for(Block aBlock : storageBlocks){
	  if(aBlock.header.type == 'F'){
		return count;
	  }
	  count++;
	}
	count = static_cast<uint32_t>(storageBlocks.size());
	return count;
  }


}

