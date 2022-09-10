//
//  BlockIO.cpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#include "BlockIO.hpp"
#include <cstring>

namespace ECE141 {

  Block::Block(BlockType aType) {}

  Block::Block(const Block &aCopy) {
	*this=aCopy;
  }

  Block& Block::operator=(const Block &aCopy) {
	std::memcpy(payload, aCopy.payload, kPayloadSize);
	header=aCopy.header;
	return *this;
  }
  
  Block& Block::setType(char aChar){
	header.type = aChar;
	return *this;
  }

  Block& Block::setHashID(uint32_t aHashID){
	header.hashID = aHashID;
	return *this;
  }

  Block& Block::setKeyNum(uint32_t aKeyNum){
	header.key = aKeyNum;
	return *this;
  }
  /* Location (Block Number) of this block */
  Block& Block::setBlockID(uint32_t aBlockID){
	header.blockID = aBlockID;
	return *this;
  }
  Block& Block::setExtraID(std::string anExtraID){
	strcpy(header.extraID, anExtraID.c_str());
	return *this;
  }

  void Block::setHeader(Entity anEntity, uint32_t aBlockNum){
	header.type = 'E';
	header.hashID = anEntity.getHashID();
	header.value = aBlockNum;
  }

  StatusResult Block::write(std::ostream &aStream) {
	return StatusResult{Errors::noError};
  }

  //---------------------------------------------------

  BlockIO::BlockIO(std::iostream &aStream) : stream(aStream) {}

  // USE: write data a given block (after seek) ---------------------------------------
  StatusResult BlockIO::writeBlock(uint32_t aBlockNum, Block &aBlock) {
	static size_t theSize = sizeof(aBlock);
	stream.seekg(stream.tellg(), std::ios::beg); // move pointer to beginning
	stream.seekp(aBlockNum * theSize);
	if(stream.write((char*)&aBlock,theSize)){
		return StatusResult{noError};
	}
	return StatusResult{writeError};
  }

  // USE: write data a given block (after seek) ---------------------------------------
  StatusResult BlockIO::readBlock(uint32_t aBlockNumber, Block &aBlock) {
	static size_t theSize = sizeof(aBlock);
	stream.seekg(stream.tellg(), std::ios::beg); // move pointer to beginning
	stream.seekg(aBlockNumber * theSize);
	if(stream.read ((char*)&aBlock, theSize)) {
		return StatusResult(noError);
	}
	return StatusResult{readError};
  }

  // USE: count blocks in file ---------------------------------------
  size_t BlockIO::getBlockCount()  {
	size_t emptySize = 0;
	stream.seekg(0, std::ios::end);
	size_t fileSize = stream.tellg();
	if(fileSize < emptySize){
		return 1;
	}
	size_t numBlocks;
	size_t size = fileSize / kBlockSize;
	size_t rem = fileSize % kBlockSize;
	if(rem == 0){
		numBlocks = size;
	}
	else{
		numBlocks = size+1;
	}
	return numBlocks;
  }
}
