#include "OpenRelTable.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
OpenRelTableMetaInfo OpenRelTable::tableMetaInfo[MAX_OPEN];
AttrCacheEntry *createAttrCacheEntryList(int size)
{
	AttrCacheEntry *head = nullptr, *curr = nullptr;
	head = curr = (AttrCacheEntry *)malloc(sizeof(AttrCacheEntry));
	size--;
	while (size--)
	{
		curr->next = (AttrCacheEntry *)malloc(sizeof(AttrCacheEntry));
		curr = curr->next;
	}
	curr->next = nullptr;

	return head;
}

OpenRelTable::OpenRelTable() {

 
  // initialize relCache and attrCache with nullptr
  for (int i = 0; i < MAX_OPEN; ++i) {
    RelCacheTable::relCache[i] = nullptr;
    AttrCacheTable::attrCache[i] = nullptr;
    tableMetaInfo[i].free = true;
    
  }
  

 

  RecBuffer relCatBlock(RELCAT_BLOCK);

  Attribute relCatRecord[RELCAT_NO_ATTRS];
  relCatBlock.getRecord(relCatRecord, RELCAT_SLOTNUM_FOR_RELCAT);

  struct RelCacheEntry relCacheEntry;
  RelCacheTable::recordToRelCatEntry(relCatRecord, &relCacheEntry.relCatEntry);
  relCacheEntry.recId.block = RELCAT_BLOCK;
  relCacheEntry.recId.slot = RELCAT_SLOTNUM_FOR_RELCAT;

  // allocate this on the heap because we want it to persist outside this function
  RelCacheTable::relCache[RELCAT_RELID] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
  *(RelCacheTable::relCache[RELCAT_RELID]) = relCacheEntry;




  RecBuffer relCatBlock2(RELCAT_BLOCK);

  Attribute attrCatRecord[RELCAT_NO_ATTRS];
  relCatBlock2.getRecord(attrCatRecord, RELCAT_SLOTNUM_FOR_ATTRCAT);

struct RelCacheEntry relCacheEntry2;
  RelCacheTable::recordToRelCatEntry(attrCatRecord, &relCacheEntry2.relCatEntry);
  relCacheEntry2.recId.block = RELCAT_BLOCK;
  relCacheEntry2.recId.slot = RELCAT_SLOTNUM_FOR_ATTRCAT;

 
  RelCacheTable::relCache[ATTRCAT_RELID] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
  *(RelCacheTable::relCache[ATTRCAT_RELID]) = relCacheEntry2;

//////////****attrcache****///
  
  RecBuffer attrCatBlock(ATTRCAT_BLOCK);


  Attribute relCatRecord2[ATTRCAT_NO_ATTRS];

  
  


  


AttrCacheEntry *attrCacheHead = nullptr;
AttrCacheEntry *attrCachePrev = nullptr;


    for(int i =0; i<=5; i++){
    
    AttrCacheEntry *attrCacheEntry = (AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
    attrCatBlock.getRecord(relCatRecord2,i);
    AttrCacheTable::recordToAttrCatEntry(relCatRecord2,&(attrCacheEntry->attrCatEntry));
    attrCacheEntry->recId.block = ATTRCAT_BLOCK;
    attrCacheEntry->recId.slot = i;
    attrCacheEntry->next = nullptr;

    if (attrCachePrev) {
        attrCachePrev->next = attrCacheEntry;
    } else {
        attrCacheHead = attrCacheEntry;
    }

    attrCachePrev = attrCacheEntry;

    }






  // set the next field in the last entry to nullptr

  AttrCacheTable::attrCache[RELCAT_RELID] = attrCacheHead;

/* attrcache entry for attr cat*/
//Attribute attrCatRecord2[ATTRCAT_NO_ATTRS];

 
 
 
 
 
 
 
 
 
 
  //attrCatBlock.getRecord(attrCatRecord2, ATTRCAT_SLOTNUM_FOR_ATTRCAT);
  


RecBuffer attrCatBlock2(ATTRCAT_BLOCK);
Attribute attrCatRecord2[ATTRCAT_NO_ATTRS];


AttrCacheEntry *attrCacheHead2 = nullptr;
AttrCacheEntry *attrCachePrev2 = nullptr;


    for(int i =6; i<=11; i++){
    AttrCacheEntry *attrCacheEntry2 = (AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
    attrCatBlock2.getRecord(attrCatRecord2, i);
    AttrCacheTable::recordToAttrCatEntry(attrCatRecord2,&(attrCacheEntry2->attrCatEntry));
    attrCacheEntry2->recId.block = ATTRCAT_BLOCK;
    attrCacheEntry2->recId.slot = i;
    attrCacheEntry2->next = nullptr;

    if (attrCachePrev2) {
        attrCachePrev2->next = attrCacheEntry2;
    } else {
        attrCacheHead2 = attrCacheEntry2;
    }

    attrCachePrev2 = attrCacheEntry2;

    }
    AttrCacheTable::attrCache[ATTRCAT_RELID] = attrCacheHead2;
  
    tableMetaInfo[RELCAT_RELID].free = false;
    tableMetaInfo[ATTRCAT_RELID].free = false;

  
  strcpy(tableMetaInfo[RELCAT_RELID].relName,RELCAT_RELNAME);
  strcpy(tableMetaInfo[ATTRCAT_RELID].relName, ATTRCAT_RELNAME);
  
  

}



int OpenRelTable::getRelId(char relName[ATTR_SIZE]) {

  

  int i;

  for( i=0; i<MAX_OPEN; ++i)
  {
    
    if (!tableMetaInfo[i].free && strcmp(relName, tableMetaInfo[i].relName)==0)
    {
     
      break;
      
    }
  }
  
  
  if(i<12 && i>=0)
    return i;
  return E_RELNOTOPEN;
}

int OpenRelTable::getFreeOpenRelTableEntry() {

  int i;
  for(i=2; i<MAX_OPEN;i++)
  {
    if(tableMetaInfo[i].free ==true)
    {
      return i;
      
    }
  }
  
  return E_CACHEFULL;


   

 
}

int OpenRelTable::openRel(char relName[ATTR_SIZE]) 
{
	// (checked using OpenRelTable::getRelId())
	int relId = getRelId(relName);
  	if(relId >= 0){
    	// return that relation id;
		return relId;
  	}

  	// TODO: find a free slot in the Open Relation Table
    // TODO: using OpenRelTable::getFreeOpenRelTableEntry().

  	// let relId be used to store the free slot.
	relId = OpenRelTable::getFreeOpenRelTableEntry();

  	if (relId == E_CACHEFULL) 
    {
      return E_CACHEFULL;
      }
      Attribute relationName;
  strcpy(relationName.sVal, relName);
	  char relNameVal[8];
	  strcpy(relNameVal,"RelName");

  	/****** Setting up Relation Cache entry for the relation ******/

  	// search for the entry with relation name, relName, 
	// in the Relation Catalog using BlockAccess::linearSearch().
    //* Care should be taken to reset the searchIndex of the relation RELCAT_RELID
    //* before calling linearSearch().

  	// relcatRecId stores the rec-id of the relation `relName` in the Relation Catalog.
	
	RelCacheTable::resetSearchIndex(RELCAT_RELID);

  	RecId relcatRecId = BlockAccess::linearSearch(RELCAT_RELID, relNameVal, relationName, EQ);

	if (relcatRecId.block ==-1 && relcatRecId.slot==-1) {
		//! the relation is not found in the Relation Catalog
		return E_RELNOTEXIST;
	}

	// TODO: read the record entry corresponding to relcatRecId and create a relCacheEntry
	// TODO: on it using RecBuffer::getRecord() and RelCacheTable::recordToRelCatEntry().
	RecBuffer relationBuffer (relcatRecId.block);
	Attribute relationRecord [RELCAT_NO_ATTRS];
	RelCacheEntry *relCacheBuffer = nullptr;

	relationBuffer.getRecord(relationRecord, relcatRecId.slot);

	//* NOTE: make sure to allocate memory for the RelCacheEntry using malloc()
	relCacheBuffer = (RelCacheEntry*) malloc (sizeof(RelCacheEntry));
	RelCacheTable::recordToRelCatEntry(relationRecord, &(relCacheBuffer->relCatEntry));

	// update the recId field of this Relation Cache entry to relcatRecId.
	relCacheBuffer->recId.block = relcatRecId.block;
	relCacheBuffer->recId.slot = relcatRecId.slot;
	
	// use the Relation Cache entry to set the relId-th entry of the RelCacheTable.
	RelCacheTable::relCache[relId] = relCacheBuffer;	


  	/****** Setting up Attribute Cache entry for the relation ******/

	/*iterate over all the entries in the Attribute Catalog corresponding to each
	attribute of the relation relName by multiple calls of BlockAccess::linearSearch()
	care should be taken to reset the searchIndex of the relation, ATTRCAT_RELID,
	corresponding to Attribute Catalog before the first call to linearSearch().*/
	// {
	// 	let attrcatRecId store a valid record id an entry of the relation, relName,
	// 	in the Attribute Catalog.
	// 	RecId attrcatRecId;

	// 	read the record entry corresponding to attrcatRecId and create an
	// 	Attribute Cache entry on it using RecBuffer::getRecord() and
	// 	AttrCacheTable::recordToAttrCatEntry().
	// 	update the recId field of this Attribute Cache entry to attrcatRecId.
	// 	add the Attribute Cache entry to the linked list of listHead.
	// NOTE: make sure to allocate memory for the AttrCacheEntry using malloc()
	// }

	Attribute attrCatRecord[ATTRCAT_NO_ATTRS];

	// let listHead be used to hold the head of the linked list of attrCache entries.
	AttrCacheEntry *attrCacheEntry = nullptr, *head = nullptr;

	int numberOfAttributes = RelCacheTable::relCache[relId]->relCatEntry.numAttrs;
	head = createAttrCacheEntryList(numberOfAttributes);
	attrCacheEntry = head;

	RelCacheTable::resetSearchIndex(ATTRCAT_RELID);
	// while (numberOfAttributes--)
	for (int attr = 0; attr < numberOfAttributes; attr++)
	{
		// AttrCacheTable::resetSearchIndex(relId, attr);
		RecId attrcatRecId = BlockAccess::linearSearch(ATTRCAT_RELID, relNameVal, relationName, EQ);

		RecBuffer attrCatBlock(attrcatRecId.block);
		attrCatBlock.getRecord(attrCatRecord, attrcatRecId.slot);

		AttrCacheTable::recordToAttrCatEntry(
			attrCatRecord,
			&(attrCacheEntry->attrCatEntry)
		);

		attrCacheEntry->recId.block = attrcatRecId.block;
		attrCacheEntry->recId.slot = attrcatRecId.slot;

		attrCacheEntry = attrCacheEntry->next;
	}

	// set the relIdth entry of the AttrCacheTable to listHead.
	AttrCacheTable::attrCache[relId] = head;

  	/****** Setting up metadata in the Open Relation Table for the relation******/

	// update the relIdth entry of the tableMetaInfo with free as false and
	// relName as the input.
	tableMetaInfo[relId].free = false;
	strcpy(tableMetaInfo[relId].relName, relName);

  	return relId;
}

int OpenRelTable::closeRel(int relId) {
  	if (relId == RELCAT_RELID || relId == ATTRCAT_RELID) return E_NOTPERMITTED;

  	if (0 > relId || relId >= MAX_OPEN) return E_OUTOFBOUND;

  	if (tableMetaInfo[relId].free) return E_RELNOTOPEN;

	if (RelCacheTable::relCache[relId]->dirty == true) {
		/* Get the Relation Catalog entry from RelCacheTable::relCache
		Then convert it to a record using RelCacheTable::relCatEntryToRecord(). */
		Attribute relCatBuffer [RELCAT_NO_ATTRS];
		RelCacheTable::relCatEntryToRecord(&(RelCacheTable::relCache[relId]->relCatEntry), relCatBuffer);

		// declaring an object of RecBuffer class to write back to the buffer
		RecId recId = RelCacheTable::relCache[relId]->recId;
		RecBuffer relCatBlock(recId.block);

		// Write back to the buffer using relCatBlock.setRecord() with recId.slot
		relCatBlock.setRecord(relCatBuffer, RelCacheTable::relCache[relId]->recId.slot);
	}

	// free the memory allocated in the relation and attribute caches which was
	// allocated in the OpenRelTable::openRel() function
	free (RelCacheTable::relCache[relId]);
	
	// RelCacheEntry *relCacheBuffer = RelCacheTable::relCache[relId];

	//* because we are not modifying the attribute cache at this stage,
	//* write-back is not required. We will do it in subsequent
  	//* stages when it becomes needed)

	AttrCacheEntry *head = AttrCacheTable::attrCache[relId];
	AttrCacheEntry *next = head->next;

	while (true) {
		if (head->dirty)
		{
			Attribute attrCatRecord [ATTRCAT_NO_ATTRS];
			AttrCacheTable::attrCatEntryToRecord(&(head->attrCatEntry), attrCatRecord);

			RecBuffer attrCatBlockBuffer (head->recId.block);
			attrCatBlockBuffer.setRecord(attrCatRecord, head->recId.slot);
		}


		free (head);
		head = next;

		if (head == NULL) break;
		next = next->next;
	}


	// update `tableMetaInfo` to set `relId` as a free slot
	// update `relCache` and `attrCache` to set the entry at `relId` to nullptr
	tableMetaInfo[relId].free = true;
	RelCacheTable::relCache[relId] = nullptr;
	AttrCacheTable::attrCache[relId] = nullptr;

  return SUCCESS;
}






OpenRelTable::~OpenRelTable() {

    for (int i =2; i<MAX_OPEN; i++)
    {
        if(!(tableMetaInfo[i].free))
        {
            // close the relation using openRelTable::closeRel().
            OpenRelTable::closeRel(i);

        }
    }

    /**** Closing the catalog relations in the relation cache ****/

    //releasing the relation cache entry of the attribute catalog

    if (RelCacheTable::relCache[ATTRCAT_RELID]->dirty) {

        /* Get the Relation Catalog entry from RelCacheTable::relCache
        Then convert it to a record using RelCacheTable::relCatEntryToRecord(). */
        RelCatEntry relCatBuf;
        RelCacheTable::getRelCatEntry(ATTRCAT_RELID,&relCatBuf);
        union Attribute relCatRecord[RELCAT_NO_ATTRS];
        RelCacheTable::relCatEntryToRecord(&relCatBuf, relCatRecord);
        RecId recId ;
        recId.block = RelCacheTable::relCache[ATTRCAT_RELID]->recId.block;
        recId.slot = RelCacheTable::relCache[ATTRCAT_RELID]->recId.slot;

        // declaring an object of RecBuffer class to write back to the buffer
        RecBuffer relCatBlock(recId.block);

        // Write back to the buffer using relCatBlock.setRecord() with recId.slot
        relCatBlock.setRecord(relCatRecord, recId.slot);
    }
    // free the memory dynamically allocated to this RelCacheEntry
    free(RelCacheTable::relCache[ATTRCAT_RELID]);


    //releasing the relation cache entry of the relation catalog

    if(RelCacheTable::relCache[RELCAT_RELID]->dirty) {

        RelCatEntry relCatBuf;
        RelCacheTable::getRelCatEntry(RELCAT_RELID,&relCatBuf);
        union Attribute relCatRecord[RELCAT_NO_ATTRS];
        RelCacheTable::relCatEntryToRecord(&relCatBuf, relCatRecord);
        RecId recId ;
        recId.block = RelCacheTable::relCache[RELCAT_RELID]->recId.block;
        recId.slot = RelCacheTable::relCache[RELCAT_RELID]->recId.slot;

        // declaring an object of RecBuffer class to write back to the buffer
        RecBuffer relCatBlock(recId.block);

        // Write back to the buffer using relCatBlock.setRecord() with recId.slot
        relCatBlock.setRecord(relCatRecord, recId.slot);
    }
    free(RelCacheTable::relCache[RELCAT_RELID]);
    // free the memory dynamically allocated for this RelCacheEntry


    // free the memory allocated for the attribute cache entries of the
    // relation catalog and the attribute catalog
}

  
 


