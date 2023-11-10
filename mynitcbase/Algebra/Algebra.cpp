#include "Algebra.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <iostream>
/* used to select all the records that satisfy a condition.
the arguments of the function are
* srcRel - the source relation we want to select from
* targetRel - the relation we want to select into. (ignore for now)
* attr - the attribute that the condition is checking
* op - the operator of the condition
* strVal - the value that we want to compare against (represented as a string)
*/
// int Algebra::select(char srcRel[ATTR_SIZE], char targetRel[ATTR_SIZE], char attr[ATTR_SIZE], int op, char strVal[ATTR_SIZE])
// {
//     int srcRelId = OpenRelTable::getRelId(srcRel); // we'll implement this later
//     if (srcRelId == E_RELNOTOPEN)
//         return E_RELNOTOPEN;

//     // get the attribute catalog entry for attr, using AttrCacheTable::getAttrcatEntry()
//     //    return E_ATTRNOTEXIST if it returns the error
//     AttrCatEntry attrCatEntry;
//     int ret = AttrCacheTable::getAttrCatEntry(srcRelId, attr, &attrCatEntry);

//     if (ret == E_ATTRNOTEXIST)
//         return E_ATTRNOTEXIST;

//     // TODO: Convert strVal (string) to an attribute of data type NUMBER or STRING 
//     int type = attrCatEntry.attrType;
//     Attribute attrVal;
//     if (type == NUMBER)
//     {
//         if (isNumber(strVal)) // the isNumber() function is implemented below
//         { 
//             attrVal.nVal = atof(strVal);
//         }
//         else
//         {
//             return E_ATTRTYPEMISMATCH;
//         }
//     }
//     else if (type == STRING)
//     {
//         strcpy(attrVal.sVal, strVal);
//     }

//     // TODO: Select records from the source relation

//     //* 1. Before calling the search function, reset the search to start from the first hit
    
//     //* using RelCacheTable::resetSearchIndex()
//     RelCacheTable::resetSearchIndex(srcRelId);

//     //* 2. get relCatEntry using RelCacheTable::getRelCatEntry()
//     RelCatEntry relCatEntry;
//     RelCacheTable::getRelCatEntry(srcRelId, &relCatEntry);

//     /************************
//     The following code prints the contents of a relation directly to the output
//     console. Direct console output is not permitted by the actual the NITCbase
//     specification and the output can only be inserted into a new relation. We will
//     be modifying it in the later stages to match the specification.
//     ************************/

//    //* 3. Printing the actual record where a match occurred
//     printf("|");
//     for (int i = 0; i < relCatEntry.numAttrs; ++i)
//     {
//         // get attrCatEntry at offset i using AttrCacheTable::getAttrCatEntry()
//         AttrCatEntry attrCatEntry;
//         AttrCacheTable::getAttrCatEntry(srcRelId, i, &attrCatEntry);

//         printf(" %s |", attrCatEntry.attrName);
//     }
//     printf("\n");

//     while (true)
//     {
//         RecId searchRes = BlockAccess::linearSearch(srcRelId, attr, attrVal, op);

//         if (searchRes.block != -1 && searchRes.slot != -1)
//         {
//             // get the record at searchRes using BlockBuffer.getRecord
//             RecBuffer blockBuffer (searchRes.block);
            
//             HeadInfo blockHeader;
//             blockBuffer.getHeader(&blockHeader);

//             Attribute recordBuffer [blockHeader.numAttrs];
//             blockBuffer.getRecord(recordBuffer, searchRes.slot);   

//             // TODO: print the attribute values in the same format as above
//             printf("|");
//             for (int i = 0; i < relCatEntry.numAttrs; ++i)
//             {
//                 AttrCacheTable::getAttrCatEntry(srcRelId, i, &attrCatEntry);
//                 if (attrCatEntry.attrType == NUMBER)
//                     printf(" %d |", (int)recordBuffer[i].nVal);
//                 else 
//                     printf(" %s |", recordBuffer[i].sVal);

//                 // std:: cout << " " << (attrCatEntry.attrType == NUMBER ? 
//                                         // recordBuffer[i].nVal : recordBuffer[i].sVal) << " |" ;
//             }
//             printf("\n");
//         }
//         else // (all records over)
//             break;
//     }

//     return SUCCESS;
// }

// will return if a string can be parsed as a floating point number
inline bool isNumber(char *str)
{
    int len;
    float ignore;
    /*
      sscanf returns the number of elements read, so if there is no float matching
      the first %f, ret will be 0, else it'll be 1

      %n gets the number of characters read. this scanf sequence will read the
      first float ignoring all the whitespace before and after. and the number of
      characters read that far will be stored in len. if len == strlen(str), then
      the string only contains a float with/without whitespace. else, there's other
      characters.
    */
    int ret = sscanf(str, "%f %n", &ignore, &len);
    return ret == 1 && len == strlen(str);
}

int Algebra::insert(char relName[ATTR_SIZE], int nAttrs, char record[][ATTR_SIZE]){
    // if relName is equal to "RELATIONCAT" or "ATTRIBUTECAT"
    if (strcmp(relName, RELCAT_RELNAME) == 0 || strcmp(relName, ATTRCAT_RELNAME) == 0)
    return E_NOTPERMITTED;

    // get the relation's rel-id using OpenRelTable::getRelId() method
    int relId = OpenRelTable::getRelId(relName);

    // if relation is not open in open relation table, return E_RELNOTOPEN
    // (check if the value returned from getRelId function call = E_RELNOTOPEN)
    if (relId < 0 || relId >= MAX_OPEN) return E_RELNOTOPEN;

    // get the relation catalog entry from relation cache
    // (use RelCacheTable::getRelCatEntry() of Cache Layer)
    RelCatEntry relCatBuffer;
    RelCacheTable::getRelCatEntry(relId, &relCatBuffer);

    // if relCatEntry.numAttrs != numberOfAttributes in relation,
    if (relCatBuffer.numAttrs != nAttrs) return E_NATTRMISMATCH;

    // let recordValues[numberOfAttributes] be an array of type union Attribute
    Attribute recordValues[nAttrs];

    // TODO: Converting 2D char array of record values to Attribute array recordValues 
    // iterate through 0 to nAttrs-1: (let i be the iterator)
    for (int attrIndex = 0; attrIndex < nAttrs; attrIndex++)
    {
        // get the attr-cat entry for the i'th attribute from the attr-cache
        // (use AttrCacheTable::getAttrCatEntry())
        AttrCatEntry attrCatEntry;
        AttrCacheTable::getAttrCatEntry(relId, attrIndex, &attrCatEntry);

        int type = attrCatEntry.attrType;
        if (type == NUMBER)
        {
            // if the char array record[i] can be converted to a number
            // (check this using isNumber() function)
            if (isNumber(record[attrIndex]))
            {
                /* convert the char array to numeral and store it
                   at recordValues[i].nVal using atof() */
                recordValues[attrIndex].nVal = atof (record[attrIndex]);
            }
            else
                return E_ATTRTYPEMISMATCH;
        }
        else if (type == STRING)
        {
            // copy record[i] to recordValues[i].sVal
            strcpy((recordValues[attrIndex].sVal), record[attrIndex]);
        }
    }

    // insert the record by calling BlockAccess::insert() function
    // let retVal denote the return value of insert call
    int ret = BlockAccess::insert(relId, recordValues);

    return ret;
}
int Algebra::select(char srcRel[ATTR_SIZE], char targetRel[ATTR_SIZE], char attr[ATTR_SIZE], int op, char strVal[ATTR_SIZE]) {
    // get the srcRel's rel-id (let it be srcRelid), using OpenRelTable::getRelId()
    // if srcRel is not open in open relation table, return E_RELNOTOPEN
    int srcRelid = OpenRelTable::getRelId(srcRel);
    if(srcRelid==E_RELNOTOPEN)
    {
        return E_RELNOTOPEN;
    }

    // get the attr-cat entry for attr, using AttrCacheTable::getAttrCatEntry()
    // if getAttrcatEntry() call fails return E_ATTRNOTEXIST
    AttrCatEntry attrCatBuf;
    int ret = AttrCacheTable::getAttrCatEntry(srcRelid, attr, &attrCatBuf);
    if(ret!=SUCCESS)
    {
        return ret;
    }


    /*** Convert strVal to an attribute of data type NUMBER or STRING ***/

    Attribute attrVal;
    int type = attrCatBuf.attrType;

    if (type == NUMBER)
    {
        // if the input argument strVal can be converted to a number
        // (check this using isNumber() function)
        if (isNumber(strVal))
        {
            // convert strVal to double and store it at attrVal.nVal using atof()
            attrVal.nVal = atof(strVal);
        }
        else
        {
            return E_ATTRTYPEMISMATCH;
        }
    }
    else if (type == STRING)
    {
        // copy strVal to attrVal.sVal
        strcpy(attrVal.sVal, strVal);
    }

    /*** Creating and opening the target relation ***/
    // Prepare arguments for createRel() in the following way:
    // get RelcatEntry of srcRel using RelCacheTable::getRelCatEntry()
    RelCatEntry relcatBuf;
    RelCacheTable::getRelCatEntry(srcRelid,&relcatBuf);
    int src_nAttrs = relcatBuf.numAttrs ;


    /* let attr_names[src_nAttrs][ATTR_SIZE] be a 2D array of type char
        (will store the attribute names of rel). */
    char attr_names[src_nAttrs][ATTR_SIZE];
    // let attr_types[src_nAttrs] be an array of type int
    int attr_types[src_nAttrs];
    

    /*iterate through 0 to src_nAttrs-1 :
        get the i'th attribute's AttrCatEntry using AttrCacheTable::getAttrCatEntry()
        fill the attr_names, attr_types arrays that we declared with the entries
        of corresponding attributes
    */
   for(int i=0; i<src_nAttrs; i++)
   {
    AttrCatEntry attrCatBuf;
    AttrCacheTable::getAttrCatEntry(srcRelid, i,&attrCatBuf);
    strcpy(attr_names[i], attrCatBuf.attrName);
    attr_types[i] = attrCatBuf.attrType;
   }


    /* Create the relation for target relation by calling Schema::createRel()
       by providing appropriate arguments */
    // if the createRel returns an error code, then return that value.
    ret =Schema::createRel(targetRel, src_nAttrs, attr_names,attr_types);
    if(ret!=SUCCESS)
    {
        return ret;
    }
    

    /* Open the newly created target relation by calling OpenRelTable::openRel()
       method and store the target relid */
    /* If opening fails, delete the target relation by calling Schema::deleteRel()
       and return the error value returned from openRel() */
    ret = OpenRelTable::openRel(targetRel);
    if (ret<0 || ret>=MAX_OPEN)
    {
        Schema::deleteRel(targetRel);
        return ret;
    }
    int targetrelId = OpenRelTable::getRelId(targetRel);
    
   
    

    /*** Selecting and inserting records into the target relation ***/
    /* Before calling the search function, reset the search to start from the
       first using RelCacheTable::resetSearchIndex() */
    


    Attribute record[src_nAttrs];

    /*
        The BlockAccess::search() function can either do a linearSearch or
        a B+ tree search. Hence, reset the search index of the relation in the
        relation cache using RelCacheTable::resetSearchIndex().
        Also, reset the search index in the attribute cache for the select
        condition attribute with name given by the argument `attr`. Use
        AttrCacheTable::resetSearchIndex().
        Both these calls are necessary to ensure that search begins from the
        first record.
    */
   
    RelCacheTable::resetSearchIndex(srcRelid);
    AttrCacheTable::resetSearchIndex(srcRelid, attr);

    // read every record that satisfies the condition by repeatedly calling
    // BlockAccess::search() until there are no more records to be read
    //cout<<"hello";
    while (BlockAccess::search(srcRelid, record, attr, attrVal,op)==SUCCESS) {

        // ret = BlockAccess::insert(targetRelId, record);
        int ret  =  BlockAccess::insert(targetrelId, record);

        // if (insert fails) {
        //     close the targetrel(by calling Schema::closeRel(targetrel))
        //     delete targetrel (by calling Schema::deleteRel(targetrel))
        //     return ret;
        // }

        if (ret!=SUCCESS)
        {
            Schema::closeRel(targetRel);
            Schema::deleteRel(targetRel);
            return ret;
        }
    }

    // Close the targetRel by calling closeRel() method of schema layer
    Schema::closeRel(targetRel);

    return SUCCESS;
}











int Algebra::project(char srcRel[ATTR_SIZE], char targetRel[ATTR_SIZE]) {

    int srcRelId = OpenRelTable::getRelId(srcRel);

    // if srcRel is not open in open relation table, return E_RELNOTOPEN
    if(srcRelId==E_RELNOTOPEN)
    {
        return E_RELNOTOPEN;
    }

    // get RelCatEntry of srcRel using RelCacheTable::getRelCatEntry()
    RelCatEntry relCatBuf;
    RelCacheTable::getRelCatEntry(srcRelId, &relCatBuf);

    // get the no. of attributes present in relation from the fetched RelCatEntry.
    int numAttrs = relCatBuf.numAttrs;

    // attrNames and attrTypes will be used to store the attribute names
    // and types of the source relation respectively
    char attrNames[numAttrs][ATTR_SIZE];
    int attrTypes[numAttrs];

    /*iterate through every attribute of the source relation :
        - get the AttributeCat entry of the attribute with offset.
          (using AttrCacheTable::getAttrCatEntry())
        - fill the arrays `attrNames` and `attrTypes` that we declared earlier
          with the data about each attribute
    */
   for(int i=0; i<numAttrs; i++)
   {
    AttrCatEntry attrCatBuf;
    AttrCacheTable::getAttrCatEntry(srcRelId, i,&attrCatBuf);
    strcpy(attrNames[i], attrCatBuf.attrName);
    attrTypes[i] = attrCatBuf.attrType;
   }


    /*** Creating and opening the target relation ***/

    // Create a relation for target relation by calling Schema::createRel()
    int retval = Schema::createRel(targetRel,numAttrs, attrNames,attrTypes);

    // if the createRel returns an error code, then return that value.
    if(retval!=SUCCESS)
    {
        return retval;
    }

    // Open the newly created target relation by calling OpenRelTable::openRel()
    // and get the target relid
    // If opening fails, delete the target relation by calling Schema::deleteRel() of
    // return the error value returned from openRel().
    int open = OpenRelTable::openRel(targetRel);
    if(open<0 || open>=MAX_OPEN)
    {
        return open;
    }
    int targetRelId = OpenRelTable::getRelId(targetRel);


    /*** Inserting projected records into the target relation ***/

    // Take care to reset the searchIndex before calling the project function
    // using RelCacheTable::resetSearchIndex()
    RelCacheTable::resetSearchIndex(srcRelId);

    Attribute record[numAttrs];


    while (BlockAccess::project(srcRelId, record)==SUCCESS)
    {
        // record will contain the next record

        // ret = BlockAccess::insert(targetRelId, proj_record);
        int ret = BlockAccess::insert(targetRelId, record);

        if (ret!=SUCCESS) {
            // close the targetrel by calling Schema::closeRel()
            Schema::closeRel(targetRel);
            // delete targetrel by calling Schema::deleteRel()
            Schema::deleteRel(targetRel);
            // return ret;
            return ret;
        }
    }

    // Close the targetRel by calling Schema::closeRel()
    Schema::closeRel(targetRel);

    return SUCCESS;
}
int Algebra::project(char srcRel[ATTR_SIZE], char targetRel[ATTR_SIZE], int tar_nAttrs, char tar_Attrs[][ATTR_SIZE]) {

    int srcRelId = OpenRelTable::getRelId(srcRel);

    // if srcRel is not open in open relation table, return E_RELNOTOPEN
    if(srcRelId == E_RELNOTOPEN)
    {
        return E_RELNOTOPEN;

    }

    // get RelCatEntry of srcRel using RelCacheTable::getRelCatEntry()
    RelCatEntry srcRelCatEntry;
    RelCacheTable::getRelCatEntry(srcRelId, &srcRelCatEntry);

    // get the no. of attributes present in relation from the fetched RelCatEntry.
    int src_nAttrs = srcRelCatEntry.numAttrs;

    // declare attr_offset[tar_nAttrs] an array of type int.
    // where i-th entry will store the offset in a record of srcRel for the
    // i-th attribute in the target relation.
    int attr_offset[tar_nAttrs];

    // let attr_types[tar_nAttrs] be an array of type int.
    // where i-th entry will store the type of the i-th attribute in the
    // target relation.
    int attr_types[tar_nAttrs];


    /*** Checking if attributes of target are present in the source relation
         and storing its offsets and types ***/
    

    /*iterate through 0 to tar_nAttrs-1 :
        - get the attribute catalog entry of the attribute with name tar_attrs[i].
        - if the attribute is not found return E_ATTRNOTEXIST
        - fill the attr_offset, attr_types arrays of target relation from the
          corresponding attribute catalog entries of source relation
    */
   for(int i=0;i < tar_nAttrs;i++)
   {
    AttrCatEntry attrCatBuf;
    int ret = AttrCacheTable::getAttrCatEntry(srcRelId, tar_Attrs[i], &attrCatBuf);
    if(ret==E_ATTRNOTEXIST)
    {
        return E_ATTRNOTEXIST;
    }
    attr_offset[i] = attrCatBuf.offset;
    attr_types[i] = attrCatBuf.attrType;
   }


    /*** Creating and opening the target relation ***/

    // Create a relation for target relation by calling Schema::createRel()
    int retval  = Schema::createRel(targetRel, tar_nAttrs, tar_Attrs, attr_types);

    // if the createRel returns an error code, then return that value.
    if(retval!=SUCCESS)
    {
        return retval;
    }

    // Open the newly created target relation by calling OpenRelTable::openRel()
    // and get the target relid
    int targetRelId  = OpenRelTable::openRel(targetRel);

    // If opening fails, delete the target relation by calling Schema::deleteRel()
    // and return the error value from openRel()
    if(targetRelId<0 || targetRelId>=MAX_OPEN)
    {
        Schema::deleteRel(targetRel);
        return targetRelId;
    }


    /*** Inserting projected records into the target relation ***/

    // Take care to reset the searchIndex before calling the project function
    // using RelCacheTable::resetSearchIndex()
    RelCacheTable::resetSearchIndex(srcRelId);


    Attribute record[src_nAttrs];

    while (BlockAccess::project(srcRelId, record)==SUCCESS) {
        // the variable `record` will contain the next record

        Attribute proj_record[tar_nAttrs];

        //iterate through 0 to tar_attrs-1:
        //    proj_record[attr_iter] = record[attr_offset[attr_iter]]
        for(int i=0; i<tar_nAttrs;i++)
        {
            proj_record[i]= record[attr_offset[i]];
        }

        int ret = BlockAccess::insert(targetRelId, proj_record);

        if (ret!=SUCCESS) {
            // close the targetrel by calling Schema::closeRel()
            Schema::closeRel(targetRel);
            // delete targetrel by calling Schema::deleteRel()
            Schema::deleteRel(targetRel);
            // return ret;
            return ret;
        }
    }

    // Close the targetRel by calling Schema::closeRel()
    Schema::closeRel(targetRel);

    // return SUCCESS.
    return SUCCESS;
}