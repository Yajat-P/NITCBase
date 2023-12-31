#include "AttrCacheTable.h"

#include <cstring>

AttrCacheEntry *AttrCacheTable::attrCache[MAX_OPEN];

//* returns the attrOffset-th attribute for the relation corresponding to relId
//* NOTE: this function expects the caller to allocate memory for `*attrCatBuf`
int AttrCacheTable::getAttrCatEntry(int relId, int attrOffset, AttrCatEntry *attrCatBuf)
{
    //! check if 0 <= relId < MAX_OPEN and return E_OUTOFBOUND otherwise
    if (relId < 0 || relId >= MAX_OPEN)
        return E_OUTOFBOUND;

    //! check if attrCache[relId] == nullptr and return E_RELNOTOPEN if true
    if (attrCache[relId] == nullptr)
        return E_RELNOTOPEN;

    // traverse the linked list of attribute cache entries
    for (AttrCacheEntry *entry = attrCache[relId]; entry != nullptr; entry = entry->next)
    {
        if (entry->attrCatEntry.offset == attrOffset)
        {
            // TODO: copy entry->attrCatEntry to *attrCatBuf and return SUCCESS;
            strcpy(attrCatBuf->relName, entry->attrCatEntry.relName);
            strcpy(attrCatBuf->attrName, entry->attrCatEntry.attrName);

            attrCatBuf->attrType = entry->attrCatEntry.attrType;
            attrCatBuf->primaryFlag = entry->attrCatEntry.primaryFlag;
            attrCatBuf->rootBlock = entry->attrCatEntry.rootBlock;
            attrCatBuf->offset = entry->attrCatEntry.offset;

            return SUCCESS;
        }
    }

    //! there is no attribute at this offset
    return E_ATTRNOTEXIST;
}

//* returns the attribute with name `attrName` for the relation corresponding to relId
//* NOTE: this function expects the caller to allocate memory for `*attrCatBuf`
int AttrCacheTable::getAttrCatEntry(int relId, char attrName[ATTR_SIZE], AttrCatEntry *attrCatBuf)
{

    
    if (relId < 0 || relId >= MAX_OPEN)
        return E_OUTOFBOUND;

    //! check if attrCache[relId] == nullptr and return E_RELNOTOPEN if true
    if (attrCache[relId] == nullptr)
        return E_RELNOTOPEN;

    
    for (AttrCacheEntry *entry = attrCache[relId]; entry != nullptr; entry = entry->next)
    {
        if (strcmp(entry->attrCatEntry.attrName, attrName) == 0) // matching the name
        {
            // TODO: copy entry->attrCatEntry to *attrCatBuf and return SUCCESS;
            strcpy(attrCatBuf->relName, entry->attrCatEntry.relName);
            strcpy(attrCatBuf->attrName, entry->attrCatEntry.attrName);

            attrCatBuf->attrType = entry->attrCatEntry.attrType;
            attrCatBuf->primaryFlag = entry->attrCatEntry.primaryFlag;
            attrCatBuf->rootBlock = entry->attrCatEntry.rootBlock;
            attrCatBuf->offset = entry->attrCatEntry.offset;

            return SUCCESS;
        }
    }


    //! no attribute with name attrName for the relation
    return E_ATTRNOTEXIST;
}

/*  
    * Converts a attribute catalog record to AttrCatEntry struct
    * We get the record as Attribute[] from the BlockBuffer.getRecord() function.
    * This function will convert that to a struct AttrCatEntry type.
*/
void AttrCacheTable::recordToAttrCatEntry(union Attribute record[ATTRCAT_NO_ATTRS],
                                          AttrCatEntry *attrCatEntry)
{
    strcpy(attrCatEntry->relName, record[ATTRCAT_REL_NAME_INDEX].sVal);
    strcpy(attrCatEntry->attrName, record[ATTRCAT_ATTR_NAME_INDEX].sVal);

    attrCatEntry->attrType = record[ATTRCAT_ATTR_TYPE_INDEX].nVal;
    attrCatEntry->primaryFlag = record[ATTRCAT_PRIMARY_FLAG_INDEX].nVal;
    attrCatEntry->rootBlock = record[ATTRCAT_ROOT_BLOCK_INDEX].nVal;
    attrCatEntry->offset = record[ATTRCAT_OFFSET_INDEX].nVal;

    // copy the rest of the fields in the record to the attrCacheEntry struct
}




int AttrCacheTable::getSearchIndex(int relId, char attrName[ATTR_SIZE], IndexId *searchIndex) {

  if(relId<0 || relId>=MAX_OPEN) {
    return E_OUTOFBOUND;
  }

  if(attrCache[relId]== nullptr) {
    return E_RELNOTOPEN;
  }

  for(AttrCacheEntry * attrCatBuf = attrCache[relId]; attrCatBuf!=nullptr; attrCatBuf= attrCatBuf->next)
  {
    if(strcmp(attrCatBuf->attrCatEntry.attrName, attrName)== 0)
    {
      //copy the searchIndex field of the corresponding Attribute Cache entry
      //in the Attribute Cache Table to input searchIndex variable.
      searchIndex->block = attrCatBuf->searchIndex.block;
      searchIndex->index = attrCatBuf->searchIndex.index;

      return SUCCESS;
    }
  }

  return E_ATTRNOTEXIST;
}
int AttrCacheTable::getSearchIndex(int relId, int attrOffset, IndexId *searchIndex) {

  if(relId<0 || relId>=MAX_OPEN) {
    return E_OUTOFBOUND;
  }

  if(attrCache[relId]== nullptr) {
    return E_RELNOTOPEN;
  }

  for(AttrCacheEntry * attrCatBuf = attrCache[relId]; attrCatBuf!=nullptr; attrCatBuf= attrCatBuf->next)
  {
    if(attrCatBuf->attrCatEntry.offset == attrOffset)
    {
      //copy the searchIndex field of the corresponding Attribute Cache entry
      //in the Attribute Cache Table to input searchIndex variable.
      searchIndex->block = attrCatBuf->searchIndex.block;
      searchIndex->index = attrCatBuf->searchIndex.index;

      return SUCCESS;
    }
  }

  return E_ATTRNOTEXIST;
}
int AttrCacheTable::setSearchIndex(int relId, char attrName[ATTR_SIZE], IndexId *searchIndex) {


 if(relId<0 || relId>=MAX_OPEN) {
    return E_OUTOFBOUND;
  }
  if(attrCache[relId]== nullptr) {
    return E_RELNOTOPEN;
  }


   for(AttrCacheEntry * attrCatBuf = attrCache[relId]; attrCatBuf!=nullptr; attrCatBuf= attrCatBuf->next)
  {
    if(strcmp(attrCatBuf->attrCatEntry.attrName, attrName)== 0)
    {
      //copy the searchIndex field of the corresponding Attribute Cache entry
      //in the Attribute Cache Table to input searchIndex variable.
      attrCatBuf->searchIndex.block=searchIndex->block;
      attrCatBuf->searchIndex.index=searchIndex->index;

      return SUCCESS;
    }
  }

  return E_ATTRNOTEXIST;
}
int AttrCacheTable::setSearchIndex(int relId, int attrOffset, IndexId *searchIndex) {


 if(relId<0 || relId>=MAX_OPEN) {
    return E_OUTOFBOUND;
  }
  if(attrCache[relId]== nullptr) {
    return E_RELNOTOPEN;
  }


   for(AttrCacheEntry * attrCatBuf = attrCache[relId]; attrCatBuf!=nullptr; attrCatBuf= attrCatBuf->next)
  {
    if(attrCatBuf->attrCatEntry.offset == attrOffset)
    {
      //copy the searchIndex field of the corresponding Attribute Cache entry
      //in the Attribute Cache Table to input searchIndex variable.
      attrCatBuf->searchIndex.block=searchIndex->block;
      attrCatBuf->searchIndex.index=searchIndex->index;

      return SUCCESS;
    }
  }

  return E_ATTRNOTEXIST;
}
int AttrCacheTable::resetSearchIndex(int relId, char attrName[ATTR_SIZE]) {

  // declare an IndexId having value {-1, -1}
  IndexId indId;
  indId.block = -1;
  indId.index = -1;
  // set the search index to {-1, -1} using AttrCacheTable::setSearchIndex
  int ret = AttrCacheTable::setSearchIndex(relId, attrName,&indId);
  // return the value returned by setSearchIndex
  return ret;
}
int AttrCacheTable::resetSearchIndex(int relId, int attrOffset) {

  // declare an IndexId having value {-1, -1}
  IndexId indId;
  indId.block = -1;
  indId.index = -1;
  // set the search index to {-1, -1} using AttrCacheTable::setSearchIndex
  int ret = AttrCacheTable::setSearchIndex(relId, attrOffset,&indId);
  // return the value returned by setSearchIndex
  return ret;
}
int AttrCacheTable::setAttrCatEntry(int relId, char attrName[ATTR_SIZE], AttrCatEntry *attrCatBuf) {

  if(relId<0 || relId>=MAX_OPEN) {
    return E_OUTOFBOUND;
  }

  if(attrCache[relId]==nullptr) {
    return E_RELNOTOPEN;
  }

  for(AttrCacheEntry *attrCacheBuf= attrCache[relId]; attrCacheBuf!=nullptr; attrCacheBuf= attrCacheBuf->next)
  {
    if(strcmp(attrCacheBuf->attrCatEntry.attrName, attrName)==0)
    {
      // copy the attrCatBuf to the corresponding Attribute Catalog entry in
      // the Attribute Cache Table.
      strcpy(attrCacheBuf->attrCatEntry.attrName, attrCatBuf->attrName);
      strcpy(attrCacheBuf->attrCatEntry.relName, attrCatBuf->relName);

      attrCacheBuf->attrCatEntry.attrType= attrCatBuf->attrType;
      attrCacheBuf->attrCatEntry.offset= attrCatBuf->offset;
      attrCacheBuf->attrCatEntry.primaryFlag= attrCatBuf->primaryFlag;
      attrCacheBuf->attrCatEntry.rootBlock = attrCatBuf->rootBlock;
      
      

      // set the dirty flag of the corresponding Attribute Cache entry in the
      // Attribute Cache Table.
      attrCacheBuf->dirty = true;

      return SUCCESS;
    }
  }

  return E_ATTRNOTEXIST;
}
int AttrCacheTable::setAttrCatEntry(int relId, int attrOffset, AttrCatEntry *attrCatBuf) {

  if(relId<0 || relId>=MAX_OPEN) {
    return E_OUTOFBOUND;
  }

  if(attrCache[relId]==nullptr) {
    return E_RELNOTOPEN;
  }

  for(AttrCacheEntry *attrCacheBuf= attrCache[relId]; attrCacheBuf!=nullptr; attrCacheBuf= attrCacheBuf->next)
  {
    if(attrCacheBuf->attrCatEntry.offset == attrOffset)
    {
      // copy the attrCatBuf to the corresponding Attribute Catalog entry in
      // the Attribute Cache Table.
      strcpy(attrCacheBuf->attrCatEntry.attrName, attrCatBuf->attrName);
      strcpy(attrCacheBuf->attrCatEntry.relName, attrCatBuf->relName);

      attrCacheBuf->attrCatEntry.attrType= attrCatBuf->attrType;
      attrCacheBuf->attrCatEntry.offset= attrCatBuf->offset;
      attrCacheBuf->attrCatEntry.primaryFlag= attrCatBuf->primaryFlag;
      attrCacheBuf->attrCatEntry.rootBlock = attrCatBuf->rootBlock;
      
      

      // set the dirty flag of the corresponding Attribute Cache entry in the
      // Attribute Cache Table.
      attrCacheBuf->dirty = true;

      return SUCCESS;
    }
  }

  return E_ATTRNOTEXIST;
}

void AttrCacheTable::attrCatEntryToRecord(AttrCatEntry *attrCatEntry, union Attribute record[ATTRCAT_NO_ATTRS])
{
        strcpy(record[ATTRCAT_ATTR_NAME_INDEX].sVal, attrCatEntry->attrName);
        strcpy(record[ATTRCAT_REL_NAME_INDEX].sVal, attrCatEntry->relName);

        record[ATTRCAT_OFFSET_INDEX].nVal = (int)attrCatEntry->offset;
        record[ATTRCAT_PRIMARY_FLAG_INDEX].nVal = (int)attrCatEntry->primaryFlag;
        record[ATTRCAT_ROOT_BLOCK_INDEX].nVal = (int)attrCatEntry->rootBlock;
        record[ATTRCAT_ATTR_TYPE_INDEX].nVal = (int)attrCatEntry->attrType;




}