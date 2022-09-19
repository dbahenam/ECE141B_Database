//
//  Storage.hpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#ifndef Storage_hpp
#define Storage_hpp

#include <string>
#include <fstream>
#include <iostream>
#include <deque>
#include <stack>
#include <optional>
#include <functional>
#include "BlockIO.hpp"
#include "Errors.hpp"
#include "Helpers.hpp"

namespace ECE141 {
  using blockList = std::vector<Block>;
  using numberList = std::vector<uint32_t>;

  struct CreateDB {}; //tags for db-open modes...
  struct OpenDB {};

    //not required -- but we discussed in class...
  class Storable{
  public:
    virtual StatusResult  encode(std::ostream &anOutput)=0;
    virtual StatusResult  decode(std::istream &anInput)=0;
  };

using BlockList = std::deque<uint32_t>;
using BlockVisitor = std::function<bool(const Block&, uint32_t)>;
//struct BlockIterator{
//  virtual bool each(const BlockVisitor)=0;
//};

  // USE: A storage class, might be helpful...
  class Storage : public BlockIO {
  public:
	  Storage(std::iostream& aStream);
	  bool isAvailable(const std::string &aName);
	  StatusResult freeBlock(uint32_t &aHashID);
	  //bool each(Block& aBlock, uint32_t aBlockNum);
	  bool each(const BlockVisitor& aVisitor); // fix
	  uint32_t getBlockID();
	
	  blockList 	storageBlocks;
	  numberList 	blockNumbers;
	  void 			setIndex();
	  uint32_t 		getIndex(){return index;}
  protected:
	  uint32_t 		index = 0;
	  Helpers theHelp;
  };

}


#endif /* Storage_hpp */
