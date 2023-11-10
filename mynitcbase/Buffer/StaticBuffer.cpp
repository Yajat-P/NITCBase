#include "StaticBuffer.h"
// the declarations for this class can be found at "StaticBuffer.h"

unsigned char StaticBuffer::blocks[BUFFER_CAPACITY][BLOCK_SIZE];
struct BufferMetaInfo StaticBuffer::metainfo[BUFFER_CAPACITY];
// declare the blockAllocMap array
unsigned char StaticBuffer::blockAllocMap[DISK_BLOCKS];

StaticBuffer::StaticBuffer() {

  // copy blockAllocMap blocks from disk to buffer (using readblock() of disk)
  // blocks 0 to 3
  for(int i =0; i<=3; i++)
  {
    unsigned char buffer[BLOCK_SIZE];
    Disk::readBlock(buffer, i);
    

  for(int slot = 0; slot<BLOCK_SIZE; slot++)
  {
    blockAllocMap[(i*BLOCK_SIZE) + slot] = buffer[slot];
  }

    
  }


  // initialise all blocks as free
  for (int i =0; i<BUFFER_CAPACITY; i++) {
    metainfo[i].free = true;
    metainfo[i].dirty = false;
    metainfo[i].timeStamp = -1;
    metainfo[i].blockNum = -1;


  }
}

/*
At this stage, we are not writing back from the buffer to the disk since we are
not modifying the buffer. So, we will define an empty destructor for now. In
subsequent stages, we will implement the write-back functionality here.
*/
StaticBuffer::~StaticBuffer() 
{
  // copy blockAllocMap blocks from buffer to disk(using writeblock() of disk)

  /*iterate through all the buffer blocks,
    write back blocks with metainfo as free:false,dirty:true
    (you did this already)
  */
 for(int i =0; i <=3; i++)
 {
  unsigned char buffer[BLOCK_SIZE];
  for(int slot =0 ; slot<BLOCK_SIZE; slot++)
  {
    buffer[slot] = StaticBuffer::blockAllocMap[(i*BLOCK_SIZE)+ slot];
  }
  Disk::writeBlock(buffer, i);
 }




          for(int i=0;i<BUFFER_CAPACITY; i++)
          {
            if(  metainfo[i].free==false && metainfo[i].dirty== true)
            {
              Disk::writeBlock(blocks[i],metainfo[i].blockNum);
            }
          }


}

int StaticBuffer::getFreeBuffer(int blockNum){
    // Check if blockNum is valid (non zero and less than DISK_BLOCKS)
    // and return E_OUTOFBOUND if not valid.

    if(blockNum<0 || blockNum>DISK_BLOCKS)
    {
      return E_OUTOFBOUND;
    }
    // increase the timeStamp in metaInfo of all occupied buffers.
    for(int i=0;i<BUFFER_CAPACITY; i++)
    {
      if(metainfo[i].free ==false)
      {
        metainfo[i].timeStamp++;
      }
    }

    // let bufferNum be used to store the buffer number of the free/freed buffer.
    int bufferNum=0;
    
    // iterate through metainfo and check if there is any buffer free
  

    // if a free buffer is available, set bufferNum = index of that free buffer.
    while(bufferNum<BUFFER_CAPACITY)
    {
      if(metainfo[bufferNum].free ==true)
      {
       
        break;
      }
      bufferNum++;
    }

    // if a free buffer is not available,
    //     find the buffer with the largest timestamp
    //     IF IT IS DIRTY, write back to the disk using Disk::writeBlock()
    //     set bufferNum = index of this buffer
    if(bufferNum==BUFFER_CAPACITY)
    {
      int replacebuffernum = -1;
      int TimeStamp = -1;
      for(int i=0 ; i<BUFFER_CAPACITY; i++)
      {
        if(metainfo[i].timeStamp>TimeStamp)
        {
          TimeStamp = metainfo[i].timeStamp;
          replacebuffernum =  i;
        }
      }
      bufferNum = replacebuffernum;
    }
  if(metainfo[bufferNum].dirty ==true)
  {
    Disk::writeBlock(StaticBuffer::blocks[bufferNum], metainfo[bufferNum].blockNum);
  }

    
    // update the metaInfo entry corresponding to bufferNum with
    // free:false, dirty:false, blockNum:the input block number, timeStamp:0.
    metainfo[bufferNum].blockNum = blockNum;
    metainfo[bufferNum].free= false;
    metainfo[bufferNum].dirty= false;
    metainfo[bufferNum].timeStamp = 0;


    return bufferNum;
}

/* Get the buffer index where a particular block is stored
   or E_BLOCKNOTINBUFFER otherwise
*/
int StaticBuffer::getBufferNum(int blockNum) {
  // Check if blockNum is valid (between zero and DISK_BLOCKS)
  // and return E_OUTOFBOUND if not valid.
   if (blockNum < 0 || blockNum > DISK_BLOCKS) {
  
    return E_OUTOFBOUND;
  }

  // find and return the bufferIndex which corresponds to blockNum (check metainfo)
  for(int i  =0; i<BUFFER_CAPACITY; i++)
  {
  if (!metainfo[i].free && metainfo[i].blockNum == blockNum) {
      return i;  // Found the buffer index for the target block number
    }
  
  }

  // if block is not in the buffer
  return E_BLOCKNOTINBUFFER;
}
int StaticBuffer::setDirtyBit(int blockNum){
    // find the buffer index corresponding to the block using getBufferNum().
    int bufferNum = getBufferNum(blockNum);


    // if block is not present in the buffer (bufferNum = E_BLOCKNOTINBUFFER)
    //     return E_BLOCKNOTINBUFFER
    if(bufferNum== E_BLOCKNOTINBUFFER)
    {
      return E_BLOCKNOTINBUFFER;
    }

    // if blockNum is out of bound (bufferNum = E_OUTOFBOUND)
    //     return E_OUTOFBOUND
    else if(blockNum==E_OUTOFBOUND )
    {
      return E_OUTOFBOUND;
    }

    // else
    //     (the bufferNum is valid)
    //     set the dirty bit of that buffer to true in metainfo
    else 
    {
      metainfo[bufferNum].dirty = true;
    }

     return SUCCESS;
}
int StaticBuffer::getStaticBlockType(int blockNum){
    // Check if blockNum is valid (non zero and less than number of disk blocks)
    // and return E_OUTOFBOUND if not valid.
    if(blockNum<0 || blockNum>=DISK_BLOCKS)
    {
      return E_OUTOFBOUND;
    }

    // Access the entry in block allocation map corresponding to the blockNum argument
    // and return the block type after type casting to integer.
    int blockType = (int)blockAllocMap[blockNum];
    return blockType;
}

