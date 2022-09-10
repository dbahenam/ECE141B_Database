//
//  BlockIO.hpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#ifndef BlockIO_hpp
#define BlockIO_hpp

#include <stdio.h>
#include <iostream>
#include <functional>
#include "Errors.hpp"
#include "Entity.hpp"

namespace ECE141 {

  enum class BlockType {
	data_block='D',
	free_block='F',
	//other types?
	unknown_block='U',
	meta_block='M',
	entity_block='E'
  };

  //a small header that describes the block...
  struct BlockHeader {
   
	BlockHeader(BlockType aType=BlockType::data_block)
	  : type(static_cast<char>(aType)) {}

	BlockHeader(const BlockHeader &aCopy) {
	  *this=aCopy;
	}
		
	void empty() {
	  type=static_cast<char>(BlockType::free_block);
	}
	
	BlockHeader& operator=(const BlockHeader &aCopy) {
	  type=aCopy.type;
	  key = aCopy.key;
	  value = aCopy.value;
	  hashID = aCopy.hashID;
	  blockID = aCopy.blockID;
	  strcpy(extraID, aCopy.extraID);
	  return *this;
	}
   
	char      type;     //char version of block type
	uint32_t key; // primary key
	uint32_t value; // value = blocknum;
	uint32_t hashID; // from table hash id
	uint32_t blockID; // block number 
	//other properties?
	char extraID[32]; // table name
  };

  const size_t kBlockSize = 1024;
  const size_t kPayloadSize = kBlockSize - sizeof(BlockHeader);
  
  //block .................
  class Block {
  public:
	Block(BlockType aType=BlockType::data_block);
	Block(const Block &aCopy);
	
	Block& operator=(const Block &aCopy);
   
	Block& setType(char aChar);
	Block& setHashID(uint32_t aHashID);
	Block& setKeyNum(uint32_t aKeyNum);
	Block& setBlockID(uint32_t aBlockID);
	Block& setExtraID(std::string anExtraID);
	void   setHeader(Entity anEntity, uint32_t aBlockNum);
	StatusResult write(std::ostream &aStream);
	BlockHeader   header;
	char          payload[kPayloadSize];
  };

  //------------------------------

  class BlockIO {
  public:
	
	BlockIO(std::iostream &aStream);
	
	size_t            getBlockCount();
	
	virtual StatusResult  readBlock(uint32_t aBlockNumber, Block &aBlock);
	virtual StatusResult  writeBlock(uint32_t aBlockNumber, Block &aBlock);
	
  protected:
	std::iostream &stream;
  };

}


#endif /* BlockIO_hpp */
