
// Copyright (C) 2018 - Vicent Giménez Alventosa
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

/** @file codeMap.cpp 
    \brief code mapping headers.
*/ 

#include "codeMap.h"

//**************************//
//      leaf functions      //
//**************************//

leaf::leaf() : maxLinks(0), nLinks(0), overload(0)
{
  name.assign("****");
}

int leaf::connect(leaf* pleaf)
{

  //Connect current leaf with leaf pointed by 'pleaf'
  //
  //return 0 if connection has been added
  //return 1 if connection already exists
  //return 2 on self connections (will not be connected)
  //
  
  //Check 'links' array dimension 
  if(nLinks >= maxLinks)
    {
      if(maxLinks <= 0)
	{
	  links = new leaf*[10];
	  maxLinks = 10;
	  nLinks = 0;
	}
      else
	{
	  //Resize array
	  leaf** paux = links;
	  maxLinks *= 2;
	  links = new leaf*[maxLinks];

	  //Copy old array to new 
	  for(unsigned int i = 0; i < nLinks; i++)
	    {
	      links[i] = paux[i];
	    }

	  //Delete old array
	  delete [] paux;
	}
    }

  //Check if this connection already exists
  for(unsigned int i = 0; i < nLinks; i++)
    if(pleaf == links[i])
      return 1;

  //Check if the connection is a self-connection
  if(pleaf == this)
    return 2;
  
  //Add connection
  links[nLinks] = pleaf;
  nLinks++;
  
  return 0;
}

void leaf::clear()
{
  //Clear arrays
  if(maxLinks > 0)
    {
      delete [] links;
    }
  maxLinks = 0;
  nLinks = 0;
  name.assign("****");
  overload = 0;
}

leaf::~leaf()
{
  clear();
}

//**************************//
//     calls functions      //
//**************************//

//Constructor

fcall::fcall()
{
  name.assign("****");
  calls = 0;
}

fcall::fcall(const fcall& c)
{
  (*this) = c;
}

fcall& fcall::operator=(const fcall& c)
{
  this->name.assign(c.name);
  this->calls = c.calls;

  return *this;
}

//Comparision overloads

//Operator <

bool fcall::operator<(const fcall& c) const
{
  //operator < used for sort function_map variables

  int value = this->name.compare(c.name);
  if(value < 0)
    {
      // The first character that does not match has a lower value in
      // "this->name". Considere "this" smaller than "c".
      return true;      
    }
  else if(value > 0)
    {
      // The first character that does not match has a lower value in
      // "c.name". Considere "c" smaller than "this".
      return false;      
    }
  else
    {
      // Both names are equal. Compare number of calls
      return (this->calls < c.calls);
    }
}

//Operator >

bool fcall::operator>(const fcall& c) const
{
  //operator < used for sort function_map variables
  
  int value = this->name.compare(c.name);
  if(value < 0)
    {
      // The first character that does not match has a lower value in
      // "this->name". Considere "this" smaller than "c".
      return false;      
    }
  else if(value > 0)
    {
      // The first character that does not match has a lower value in
      // "c.name". Considere "c" smaller than "this".
      return true;      
    }
  else
    {
      // Both names are equal. Compare number of calls
      return (this->calls > c.calls);
    }
}


//**************************//
// namespaces map functions //
//**************************//

namespace_map::namespace_map()
{
  functionsNames = 0;
  maxnfnames = 0;
  nfnames = 0;
  name.assign("****");
}

namespace_map::namespace_map(const namespace_map& c)
{
  functionsNames = 0;
  maxnfnames = 0;
  nfnames = 0;
  name.assign("****");
  
  (*this) = c;
}

int namespace_map::appendF(const char* fname)
{

  //This function append a function name to
  //function names array.
  //
  //return 0 on succes
  //return 1 if the name already exists

  //Check array space
  if(nfnames >= maxnfnames)
    {
      resizeFNames(10+2*maxnfnames);
    }

  //Check if this value already exists

  for(int i = 0; i < nfnames; i++)
    {
      if(functionsNames[i].compare(fname) == 0)
	{
	  //Exists!
	  return 1;
	}
    }
  
  //append value
  functionsNames[nfnames].assign(fname);
  nfnames++;
  return 0;
}

void namespace_map::sort()
{
  //Sorts names in 'functinsNames' array
  std::sort(functionsNames, functionsNames+nfnames, stringOrder);  
}

void namespace_map::resizeFNames(int dim)
{
  //Resize array 'functionsNames' to size 'dim'

  if(nfnames > 0)
    {  
      std::string* paux = functionsNames;
      functionsNames = 0;
      functionsNames = new std::string[dim];

      int limit =  nfnames < dim ? nfnames : dim;

      for(int i = 0; i < limit; i++)
	{
	  functionsNames[i].assign(paux[i]);
	}

      nfnames = limit;
  
      // delete old array
      delete [] paux;
    }
  else if(functionsNames != 0)
    {
      delete [] functionsNames;
      functionsNames = new std::string[dim];      
      nfnames = 0;
    }
  else
    {
      functionsNames = new std::string[dim];      
      nfnames = 0;
    }
  maxnfnames = dim;  
}

std::string namespace_map::to_string(int tabs) const
{
  // This function convert the function_map
  // to formated string.
  //
  // 'tabs' variable specifies the number of
  // inintial tabs to align the text

  std::string out;

  //Name
  out.append(tabs,'\t');
  out.append("namespace name: ");
  out.append(name);
  out.append(1,'\n');
  
  //functions that uses this namespace
  out.append(tabs+1,'\t');
  out.append("    functions: ");
  out.append(1,'\n');
  for(int i = 0; i < nfnames; i++)
    {
      out.append(tabs+2,'\t');
      out.append("               ");
      out.append(functionsNames[i]);
      out.append(1,'\n');
    }
  
  return out;
  
}

//Clear function

void namespace_map::clear()
{
  if(maxnfnames > 0)
    {
      delete [] functionsNames;
    }
  
  functionsNames = 0;

  nfnames = 0;

  maxnfnames = 0;
  
  name.assign("****"); //Default name
  
}

//Assign operator

namespace_map& namespace_map::operator=(const namespace_map& c)
{
  //Operator assign

  //Clear actual state
  clear();

  //Copy
  maxnfnames = c.maxnfnames;
  nfnames = c.nfnames;

  functionsNames = new std::string[maxnfnames];

  for(int i = 0; i < nfnames; i++)
    {
      functionsNames[i].assign(c.functionsNames[i]);
    }

  name.assign(c.name);

  return (*this);
}

//Comparison operators

bool namespace_map::operator<(const namespace_map& c) const
{
  return stringOrder(name,c.name);
}
bool namespace_map::operator>(const namespace_map& c) const
{
  return stringOrderInv(name,c.name);  
}


//Destructor

namespace_map::~namespace_map()
{
  clear();
}

//**************************//
//  function map functions  //
//**************************//

//Constructor

function_map::function_map()
{
  nNamespaces = 0;
  nCalledF = 0;

  maxNamespaces = 0;
  maxCalledF = 0;
  
  nOverload = 0;

  name.assign("****"); //Default name
  sourceFile.assign("****"); //Default source
  namespaces = 0;
  fcalls = 0;
}

function_map::function_map(const function_map& c)
{
  //Init all parameters
  nNamespaces = 0;
  nCalledF = 0;

  maxNamespaces = 0;
  maxCalledF = 0;
  
  nOverload = 0;

  name.assign("****"); //Default name
  sourceFile.assign("****"); //Default source
  namespaces = 0;
  fcalls = 0;
  
  (*this) = c; //Copy object C
}

//Resize functions

void function_map::resizeFCalls(int dim)
{
  //Resize array 'fcalls' to size 'dim'

  if(nCalledF > 0)
    {  
      fcall* paux = fcalls;
      fcalls = 0;
      fcalls = new fcall[dim];

      int limit =  nCalledF < dim ? nCalledF : dim;

      for(int i = 0; i < limit; i++)
	{
	  fcalls[i] = paux[i];
	}

      nCalledF = limit;
  
      // delete old array
      delete [] paux;
    }
  else if(fcalls != 0)
    {
      delete [] fcalls;
      fcalls = new fcall[dim];      
      nCalledF = 0;
    }
  else
    {
      fcalls = new fcall[dim];      
      nCalledF = 0;
    }
  maxCalledF = dim;  
}

//Load information function

int function_map::loadFunction(std::string inS, const char* fname, int overload)
{
  //This function will search and extract information
  //for the specified function name (fname) defined in
  //source stored in 'inS'. To handle overload functions,
  //an overload paramter can be specified.
  //
  //A overload equal to 1 will load the first function
  //definition match.
  //
  //return 0 on succes.
  //return -1 if the function has not been found
  //return -2 if no marks exists in input string
  //
  
  //Clear function
  clear();
  
  //Load data

  //Namespaces
  nNamespaces = usedNamespaces(inS, namespaces, fname, overload);
  
  if(nNamespaces < 0)
    {
      //Function not found or source file
      //not marked previously
      int returnv = nNamespaces; //Save return value before clear call
      clear();
      return returnv;
    }
  maxNamespaces = nNamespaces;
  
  //Called functions
  nCalledF = calledFunc(inS, fcalls, fname, overload);

  if(nCalledF < 0)
    {
      //Function not found or source file
      //not marked previously
      int returnv = nCalledF; //Save return value before clear call
      clear();
      return returnv;
    }

  maxCalledF = nCalledF;

  //Store specified overload
  nOverload = overload;

  //Store function name
  name.assign(fname);
  
  return 0;
}

//Acces functions

std::string function_map::getNamespace(int i) const
{
  std::string aux;
  if(i >= 0 && i < nNamespaces)
    {
      aux.assign(namespaces[i]);
    }
  else{aux.clear();}
  
  return aux;
}

std::string function_map::getFName(int i) const
{
  std::string aux;
  if(i >= 0 && i < nCalledF)
    {
      aux.assign(fcalls[i].name);
    }
  else{aux.clear();}
  
  return aux;
}

fcall function_map::getCall(int i) const
{
  if(i >= 0 && i < nCalledF)
    {
      return fcalls[i];
    }
  
  return fcall();  
}

//Append functions

int function_map::appendCall(fcall call)
{
  //This function append a call pair function/number of call to
  //the 'fcalls' array. The addition will not be performed if
  //specified pair already exists.
  //
  //return number of function calls on succes
  //return -1 if the pair already exists

  return appendCall(call.name.c_str(), call.calls);
  
}

int function_map::appendCall(const char* fname, int ncalls)
{
  //This function append a call pair function/number of calls to
  //the 'fcalls' array. The addition will not be performed if
  //specified pair already exists.
  //
  //return number of function calls on succes
  //return -1 if the pair already exists

  //Check if this name already exists
  for(int i = 0; i < nCalledF; i++)
    {
      if(fcalls[i].name.compare(fname) == 0)
	{
	  return -1;
	}
    }

  //Check call array size
  if(nCalledF >= maxCalledF)
    {
      int size;
      if(maxCalledF == 0){size = 10;}
      else{size = 2*maxCalledF;}
      resizeFCalls(size); //Enlarge array
    }

  //Store new function call
  fcalls[nCalledF].name.assign(fname);
  fcalls[nCalledF].calls = ncalls;
  nCalledF++;
  
  return nCalledF;
}

//Set functions

int function_map::setCall(fcall call)
{
  //This function set a call pair function/number of 
  //the 'fcalls' array. If the specified pair doesn't exists
  //will be appendet.
  //
  //return 0 on succes
  //return number of function calls if the pair doesn't exist

  return setCall(call.name.c_str(), call.calls);
  
}

int function_map::setCall(const char* fname, int ncalls)
{
  //This function set a call pair function/number of 
  //the 'fcalls' array. If the specified pair doesn't exists
  //will be appendet.
  //
  //return 0 on succes
  //return number of function calls if the pair doesn't exist

  //Check if this name already exists
  for(int i = 0; i < nCalledF; i++)
    {
      if(fcalls[i].name.compare(fname) == 0)
	{
	  fcalls[i].calls = ncalls;
	  return 0;
	}
    }

  return appendCall(fname, ncalls);
}

int function_map::removeCall(const char* fname)
{
  //This function remove a call pair function/number of 
  //the 'fcalls' array.
  //
  //return number of function calls on succes
  //return -1 if the pair doesn't exist

  //Check if this name already exists
  for(int i = 0; i < nCalledF; i++)
    {
      if(fcalls[i].name.compare(fname) == 0)
	{
	  //Found, move all following calls
	  for(int j = i; j < nCalledF-1; j++)
	    {
	      fcalls[j] = fcalls[j+1];
	    }
	  nCalledF--;
	  return 0;
	}
    }

  return -1;
}


//Clear function

void function_map::clear()
{
  if(maxNamespaces > 0)
    {
      delete [] namespaces;
    }
  if(maxCalledF > 0)
    {
      delete [] fcalls;
    }
  
  nNamespaces = 0;
  nCalledF = 0;

  maxNamespaces = 0;
  maxCalledF = 0;
  
  nOverload = 0;

  name.assign("****"); //Default name
  sourceFile.assign("****"); //Default source
  
}

//To string function

std::string function_map::to_string(int tabs) const
{
  // This function convert the function_map
  // to formated string.
  //
  // 'tabs' variable specifies the number of
  // inintial tabs to align the text

  std::string out;

  //Name
  out.append(tabs,'\t');
  out.append("function name: ");
  out.append(name);
  out.append(1,'\n');

  //source file
  out.append(tabs+1,'\t');
  out.append("  source file: ");
  out.append(sourceFile);
  out.append(1,'\n');

  //overload
  out.append(tabs+1,'\t');
  out.append("     overload: ");

  char overloadChar[10];
  sprintf(overloadChar,"%d",nOverload);
  
  out.append(overloadChar);
  out.append(1,'\n');
  
  //namespaces used
  out.append(tabs+1,'\t');
  out.append("   namespaces: ");
  out.append(1,'\n');
  for(int i = 0; i < nNamespaces; i++)
    {
      out.append(tabs+2,'\t');
      out.append("               ");
      out.append(namespaces[i]);
      out.append(1,'\n');
    }

  //functions called
  out.append(tabs+1,'\t');
  out.append("        calls: ");
  out.append(1,'\n');
  for(int i = 0; i < nCalledF; i++)
    {
      out.append(tabs+2,'\t');

      int auxL = fcalls[i].name.length();
      int remaining = 15-auxL;

      if(remaining > 0)
	{
	  out.append(remaining,' ');
	}
      out.append(fcalls[i].name);
      out.append(": ");

      char callsChar[30];
      sprintf(callsChar,"%20d",fcalls[i].calls);
      
      out.append(callsChar);
      out.append(1,'\n');
    }

  return out;
  
}

//Acces functions

//Sort function

void function_map::sort()
{
  //This function sort function and namespace names in function_map

  //Namespaces
  std::sort(namespaces, namespaces+nNamespaces, stringOrder);

  //Function calls
  std::sort(fcalls,fcalls+nCalledF);
}

//Operator =

function_map& function_map::operator=(const function_map& c)
{  
  //Clear local function_map
  clear();

  //Copy all variables
  nNamespaces    = c.nNamespaces;
  nCalledF       = c.nCalledF;

  maxNamespaces  = c.maxNamespaces;
  maxCalledF     = c.maxCalledF;
  
  nOverload      = c.nOverload;

  //Handle dinamic arrays and pointers

  //namespaces
  namespaces = 0;
  if(maxNamespaces > 0)
    {
      namespaces = new std::string[maxNamespaces];
      for(int i = 0; i < nNamespaces; i++)
	{
	  namespaces[i].assign(c.namespaces[i]);
	}
    }
  
  //fcalls
  fcalls = 0;
  if(maxCalledF > 0)
    {
      fcalls = new fcall[maxCalledF];
      for(int i = 0; i < nCalledF; i++)
	{
	  fcalls[i] = c.fcalls[i];
	}
    }

  //Handle strings
  name.assign(c.name);
  sourceFile.assign(c.sourceFile);
  
  return (*this);
}

//Operator <

bool function_map::operator<(const function_map& c) const
{
  //operator < used for sort function_map variables

  int value = this->name.compare(c.name);
  if(value < 0)
    {
      // The first character that does not match has a lower value in
      // "this->name". Considere "this" smaller than "c".
      return true;      
    }
  else if(value > 0)
    {
      // The first character that does not match has a lower value in
      // "c.name". Considere "c" smaller than "this".
      return false;      
    }
  else
    {
      // Both names are equal. Compare source files
      value = this->sourceFile.compare(c.sourceFile);
      if(value < 0)
	{
	  // The first character that does not match has a lower value in
	  // "this->sourceFile". Considere "this" smaller than "c".
	  return true;      
	}
      else if(value > 0)
	{
	  // The first character that does not match has a lower value in
	  // "c.sourceFile" or both are equal. Don't considere "this" smaller than "c".
	  return false;      
	}
      else
	{
	  //Check overload
	  return (this->nOverload < c.nOverload);
	}
    }
}

//Operator >

bool function_map::operator>(const function_map& c) const
{
  //operator > used for sort function_map variables

  int value = this->name.compare(c.name);
  if(value < 0)
    {
      // The first character that does not match has a lower value in
      // "this->name". Considere "this" smaller than "c".
      return false;      
    }
  else if(value > 0)
    {
      // The first character that does not match has a lower value in
      // "c.name". Considere "c" smaller than "this".
      return true;      
    }
  else
    {
      // Both names are equal. Compare source files
      value = this->sourceFile.compare(c.sourceFile);
      if(value < 0)
	{
	  // The first character that does not match has a lower value in
	  // "this->sourceFile". Considere "this" smaller than "c".
	  return false;      
	}
      else if (value > 0)
	{
	  // The first character that does not match has a lower value in
	  // "c.sourceFile" or both are equal. Don't considere "this" smaller than "c".
	  return true;      
	}
      else
	{
	  //Check overload
	  return (this->nOverload > c.nOverload);
	}
    }
}

//Destructor

function_map::~function_map()
{
  clear();
}


//**************************//
//   source map functions   //
//**************************//

//Constructor

source_map::source_map()
{
  nNamespaces = 0;
  nFunctions = 0;
  nFnames = 0;
  nNamespaceMaps = 0;

  maxFunctions = 0;
  maxFnames = 0;
  maxNamespaces = 0;
  maxNamespaceMaps = 0;
  
  namespaces = 0;
  defFuncNames = 0;
  functions = 0;
  namespaceMaps = 0;

}

//Load function

int source_map::loadSource(const char* filename)
{
  // This function create a map from specified
  // filename file. The map will be cleared
  // before the new load.
  //
  // return 0 on succes
  // return -1 if file can't be opened
  // return -2 if an error clearing the code occurs
  // return -3 if there are not any function definition
  //           in specified source file
  //

  //Clear map
  clear();
  
  return appendSource(filename);
}

//Append map function

int source_map::appendSource(const char* filename)
{
  // This function create a map from specified
  // filename file. The new map will be appended
  // to existing maps.
  //
  // return 0 on succes
  // return -1 if file can't be opened
  // return -2 if an error clearing the code occurs
  // return -3 if there are not any function definition
  //           in specified source file
  //

  //Read and store the file text
  std::string text;
  if(readFile(filename, text) != 0)
    {
      //Error reading the specified filename
      return -1;
    }

  //Clear code from strings and comments
  int errClear = clearCode(text);
  if(errClear == -5)
    {
      return -2;
    }

  //Extract defined function names
  std::string* auxDF = 0;
  int readF = definedFunc(text,auxDF);

  if(readF == 0)
    {
      //No function definitions in specified source file
      return -3;
    }
  
  //Calculate total names number of defined functions
  int totalDF = nFnames + readF;
  int oldnFnames = nFnames;
  if(totalDF > maxFnames)
    {
      //Resize arrays to save new values
      resizeFN(2*totalDF);
    }

  //Save readed names
  for(int i = 0; i < readF; i++)
    {
      //Check if this name already exists
      if(nameMatch(defFuncNames,nFnames,auxDF[i].c_str()) < 0)
	{
	  //Store new function name
	  defFuncNames[nFnames].assign(auxDF[i]);
	  nFnames++;
	}
    }

  //Check the remaining space in
  //'functions' array
  if(nFunctions == maxFunctions)
    {
      resizeF(maxFunctions+2*readF);
    }
  
  //Extract information of each function
  //defined in this source file
  for(int i = 0; i < readF; i++)
    {
      int overload = 1;
      //Load all overloads of defined 'i' function
      while(functions[nFunctions].loadFunction(text, auxDF[i].c_str(), overload) == 0)
	{	  
	  //Succesfull function load

	  //Store source filename
	  functions[nFunctions].sourceFile.assign(filename);
	  
	  //Increment overload counter and number of functions
	  overload++;
	  nFunctions++;

	  //Check the remaining space in
	  //'functions' array
	  if(nFunctions == maxFunctions)
	    {
	      //Duplicate space
	      resizeF(2*maxFunctions);
	    }
	}
    }

  //Check namespaces array remaining space
  if(nNamespaces >= maxNamespaces)
    {
      resizeNSN(maxNamespaces+10);      
    }

  if(nNamespaceMaps >= maxNamespaceMaps)
    {
      resizeNS(maxNamespaceMaps+10);
    }
  
  //Extract used namespaces from defined functions
  for(int i = oldnFnames; i < nFunctions; i++)
    {
      for(int j = 0; j < functions[i].nNamespaces; j++)
	{
	  if(nameMatch(namespaces,nNamespaces,functions[i].namespaces[j].c_str()) < 0)
	    {
	      //This namespace is not included
	      namespaces[nNamespaces].assign(functions[i].namespaces[j]);
	      nNamespaces++;

	      namespaceMaps[nNamespaceMaps].name.assign(functions[i].namespaces[j]);
	      namespaceMaps[nNamespaceMaps].appendF(functions[i].name.c_str());
	      nNamespaceMaps++;
		
	      //Check the remaining space in
	      //'namespaces' array
	      if(nNamespaces == maxNamespaces)
		{
		  //Duplicate space
		  resizeNSN(2*maxNamespaces);
		}
	      if(nNamespaceMaps >= maxNamespaceMaps)
		{
		  resizeNS(2*maxNamespaceMaps);
		}
	    }
	  else
	    {
	      //This namespace already exists, find it
	      for(int k = 0; k < nNamespaceMaps; k++)
		{
		  if(namespaceMaps[k].name.compare(functions[i].namespaces[j]) == 0)
		    {
		      //Check if this namespace contains already this function
		      bool found = false;
		      for(int l = 0; l < namespaceMaps[k].nfnames; l++)
			{
			  if(namespaceMaps[k].functionsNames[l].compare(functions[i].name) == 0)
			    {
			      found = true;
			      break;
			    }
			}
		      //If not found, append function name
		      if(!found)
			{
			  namespaceMaps[k].appendF(functions[i].name.c_str());
			}
		      
		      break;
		    }
		}
	    }
	}
    }
  
  //Free auxiliar array memory 
  delete [] auxDF;
  return 0;
}

//To string function

std::string source_map::to_string(int tabs) const
{
  //Convert all source map to text format and
  //return this text as string.
  //
  // 'tabs' specifies the number of initial
  // tabulations '\t' to use
  //

  std::string out;

  //Functions names
  out.append(tabs,'\t');
  out.append("Defined functions names: ");
  out.append(1,'\n');
  for(int i = 0; i < nFnames; i++)
    {
      out.append(tabs+1,'\t');
      out.append("                         ");
      out.append(defFuncNames[i]);
      out.append(1,'\n');
    }

  //Namespace names
  out.append(tabs,'\t');
  out.append("        Used namespaces: ");
  out.append(1,'\n');
  for(int i = 0; i < nNamespaces; i++)
    {
      out.append(tabs+1,'\t');
      out.append("                         ");
      out.append(namespaces[i]);
      out.append(1,'\n');
    }

  //Namespaces
  out.append(tabs,'\t');
  out.append("         Namespace maps: ");
  out.append(1,'\n');
  for(int i = 0; i < nNamespaceMaps; i++)
    {
      out.append(namespaceMaps[i].to_string(tabs+3));
      out.append(1,'\n');
    }
  
  //Functions
  out.append(tabs,'\t');
  out.append("              Functions: ");
  out.append(1,'\n');
  for(int i = 0; i < nFnames; i++)
    {
      out.append(functions[i].to_string(tabs+3));
      out.append(1,'\n');
    }

  //Return output string
  return out;
}

//Acces functions

std::string source_map::getNamespaceName(int i) const
{
  std::string aux;
  if(i >= 0 && i < nNamespaces)
    {
      aux.assign(namespaces[i]);
    }
  else{aux.clear();}
  
  return aux;
}

std::string source_map::getFName(int i) const
{
  std::string aux;
  if(i >= 0 && i < nFnames)
    {
      aux.assign(defFuncNames[i]);
    }
  else{aux.clear();}
  
  return aux;
}

const function_map* source_map::getFunction(const char* functionName, const char* sourceFile, int overload) const
{
  //Return a inmutable pointer for the specified function.
  
  for(int i = 0; i < nFunctions; i++)
    {
      //Check function name
      if(functions[i].name.compare(functionName) == 0)
	{
	  //Check if source file has been specified for match
	  if(sourceFile == 0)
	    {
	      //Don't care of source name, return first function name match
	      return &functions[i];
	    }

	  //Check source filename
	  if(functions[i].sourceFile.compare(sourceFile) == 0)
	    {
	      //Check if overload has been specified for match
	      if(overload <= 0)
		{
		  //Don't care of overload number, return first function
		  //that are matching in name and source file.
		  return &functions[i];		  
		}

	      //Check overload number
	      if(functions[i].nOverload == overload)
		{
		  return &functions[i];
		}
	    }
	}
    }
  return 0;
}

const function_map* source_map::getFunction(int i) const
{
  //Return a inmutable pointer for the specified function.
  
  if(i >= 0 && i < nFunctions)
    {
      return &functions[i];
    }
  return 0;
}

//Acces namespace

const namespace_map* source_map::getNamespace(const char* namespaceName) const
{
  //Return a inmutable pointer for the specified namespace object.

  //return null pointer if namespace name has not been found
  
  for(int i = 0; i < nNamespaceMaps; i++)
    {
      if(namespaceMaps[i].name.compare(namespaceName) == 0)
	{
	  return &namespaceMaps[i];
	}
    }
  return 0;
}

const namespace_map* source_map::getNamespace(int i) const
{
  //Return a inmutable pointer for the specified function.
  
  if(i >= 0 && i < nNamespaceMaps)
    {
      return &namespaceMaps[i];
    }
  return 0;
}


//Remove functions

int source_map::removeF(const char* functionName, const char* sourceFile, int overload)
{
  //This function removes one or more function
  //stored in the source map. The removed functions
  //are which has been stored in position 'i'.
  //
  //return 0 on succes
  //return -1 if specified function doesn't exists

  bool found = false;
  for(int i = 0; i < nFunctions; i++)
    {
      //Check function name
      if(functions[i].name.compare(functionName) == 0)
	{
	  //Check if source file has been specified for match
	  if(sourceFile == 0)
	    {
	      //Don't care of source name remove this function
	      for(int j = i; j < nFunctions-1; j++)
		{
		  functions[j] = functions[j+1];
		}
	      nFunctions--;
	      i--;
	      found = true;
	      continue;
	    }

	  //Check source filename
	  if(functions[i].sourceFile.compare(sourceFile) == 0)
	    {
	      //Check if overload has been specified for match
	      if(overload <= 0)
		{
		  //Don't care of overload number,
		  //remove this function
		  for(int j = i; j < nFunctions-1; j++)
		    {
		      functions[j] = functions[j+1];
		    }
		  nFunctions--;
		  i--;
		  found = true;
		  continue;
		}

	      //Check overload number
	      if(functions[i].nOverload == overload)
		{
		  for(int j = i; j < nFunctions-1; j++)
		    {
		      functions[j] = functions[j+1];
		    }
		  nFunctions--;
		  i--;
		  found = true;
		}
	    }
	}
    }

  //Check if the specified function has
  //been found.
  if(found)
    {
      //Update function and namespaces names
      update();
      return 0;
    }
  else
    {
      return -1;
    }
}

int source_map::removeF(int i)
{
  //This function removes a function stored
  //in the source map. The removed function
  //is which has been stored in position 'i'.
  //
  //return 0 on succes
  //return -1 if 'i' is out of range

  if(i >= 0 && i < nFunctions)
    {
      for(int j = i; j < nFunctions-1; j++)
	{
	  functions[j] = functions[j+1];
	}
      
      //Update defined function names and
      //used namespaces
      update();
      return 0;
    }
  return -1;
}

//Update function

void source_map::update()
{
  //This function updates used namespaces
  //and defined function names

  //Clear namespace maps
  for(int i = 0; i < nNamespaceMaps; i++)
    {
      namespaceMaps[i].clear();
    }
  nNamespaceMaps = 0;
  
  nNamespaces = 0;
  nFnames = 0;

  for(int i = 0; i < nFunctions; i++)
    {
      //Check namespaces
      for(int j = 0; j < functions[i].nNamespaces; j++)
	{
	  if(nameMatch(namespaces, nNamespaces, functions[i].namespaces[j].c_str()) < 0)
	    {
	      //This namespace is not in the array
	      //Check array sizes 
	      if(nNamespaces >= maxNamespaces)
		{
		  //Resize namespaces array
		  resizeNSN(5+maxNamespaces*2);
		}
	      if(nNamespaceMaps >= maxNamespaceMaps)
		{
		  resizeNS(10+maxNamespaceMaps*2);
		}
	      
	      
	      //Add this namespace
	      namespaces[nNamespaces].assign(functions[i].namespaces[j]);
	      nNamespaces++;

	      namespaceMaps[nNamespaceMaps].name.assign(functions[i].namespaces[j]);
	      namespaceMaps[nNamespaceMaps].appendF(functions[i].name.c_str());
	      nNamespaceMaps++;
	    }
	  else
	    {
	      //This namespace already exists, find it
	      for(int k = 0; k < nNamespaceMaps; k++)
		{
		  if(namespaceMaps[k].name.compare(functions[i].namespaces[j]) == 0)
		    {
		      //Check if this namespace contains already this function
		      bool found = false;
		      for(int l = 0; l < namespaceMaps[k].nfnames; l++)
			{
			  if(namespaceMaps[k].functionsNames[l].compare(functions[i].name) == 0)
			    {
			      found = true;
			      break;
			    }
			}
		      //If not found, append function name
		      if(!found)
			{
			  namespaceMaps[k].appendF(functions[i].name.c_str());
			}
		      break;
		    }
		}
	    }
	}

      //Check existence of function name
      if(nameMatch(defFuncNames, nFnames, functions[i].name.c_str()) < 0)
	{
	  //Check array size
	  if(nFnames <= maxFnames)
	    {
	      resizeFN(5+maxFnames*2);
	    }

	  //Add this function name
	  defFuncNames[nFnames].assign(functions[i].name);
	  nFnames++;
	}
    }
}

//Clear function

void source_map::clear()
{
  if(maxNamespaces > 0)
    {
      delete [] namespaces;
    }
  if(maxFnames > 0)
    {
      delete [] defFuncNames;
    }
  if(maxFunctions > 0)
    {
      delete [] functions;
    }

  nNamespaces = 0;
  nFunctions = 0;
  nFnames = 0;

  maxFunctions = 0;
  maxFnames = 0;
  maxNamespaces = 0;
  
}

//Sort function

void source_map::sort()
{
  //This function sort function and namespace names in function_map
  
  //Namespaces names
  std::sort(namespaces, namespaces+nNamespaces, stringOrder);
  
  //Function names
  std::sort(defFuncNames, defFuncNames+nFnames, stringOrder);

  //Function array
  std::sort(functions,functions+nFunctions);

  //Namespaces array
  std::sort(namespaceMaps,namespaceMaps+nNamespaceMaps);
  
  //Finally, sort each function and namespace
  for(int i = 0; i < nFunctions; i++)
    {
      functions[i].sort();
    }
  for(int i = 0; i < nNamespaceMaps; i++)
    {
      namespaceMaps[i].sort();
    }
}

//Filter functions

int source_map::filterFunctions(const char** namesNS, const int nNS, std::string* outF, int& nF) const
{
  //This function return function names that uses all
  //specified namespace names.

  // namesNS -> array with the names of namespaces to filter
  //     nNS -> number of namespaces to filter
  //    outF -> array of strings containing filtered function names
  //      nF -> in: Maximum number of matches, out: Number of matches

  //
  // return  0 on succes
  // return -1 if there are more possible matches than specified nF value.
  // return -2 if some namespace doesn't exists
  

  for(int i = 0; i < nNS; i++) //Iterate all specified namespaces
    {
      //Find specified namespace
      int posNS = -1;
      for(int k = 0; k < nNamespaceMaps; k++)
	{
	  if(namespaceMaps[k].name.compare(namesNS[i]) == 0)
	    {
	      posNS = k;
	      break;
	    }
	}

      if(posNS < 0){return -2;}  //Check if namespace name has been found

      if(i == 0) //Is the first namespace, append all his functions
	{
	  //Check output array size
	  if(nF < namespaceMaps[posNS].nfnames){return -1;}
	  
	  //Store actual number of filtered functions
	  nF = namespaceMaps[posNS].nfnames;

	  if(nF == 0){return 0;}

	  //Store function names
	  for(int j = 0; j < nF; j++)
	    {
	      outF[j].assign(namespaceMaps[posNS].functionsNames[j]);
	    }
	}
      else
	{
	  //Is not the first namespace, all function names in outF 
	  //that are not in he current namespace's function list must be removed.

	  //Store number of functions that uses this namespace
	  int NSnfunc = namespaceMaps[posNS].nfnames;

	  //Check all possible filtered functions in outF
	  for(int j = nF-1; j >= 0; j--) 
	    {
	      //Search outF functions in this namespace
	      int pos = nameMatch(namespaceMaps[posNS].functionsNames, NSnfunc, outF[j].c_str());

	      if(pos < 0)
		{
		  //This function don't uses current namespace,
		  //remove it.
		  for(int k = j; k < nF-1; k++)
		    {
		      outF[k].assign(outF[k+1]);
		    }
		  nF--; //Reduce the number of possible matches
		}	     
	    }
	  //Check for remaining possible function matches
	  if(nF == 0)
	    {
	      return 0;
	    }
	}
    }
  
  return 0;
}
//Resize functions

void source_map::resizeFN(int dim)
{
  //Resize array defFuncNames to size 'dim'

  if(nFnames > 0)
    {
      std::string* paux = defFuncNames;
      defFuncNames = 0;
      defFuncNames = new std::string[dim];
      
      int limit =  nFnames < dim ? nFnames : dim;
      
      for(int i = 0; i < limit; i++)
	{
	  defFuncNames[i].assign(paux[i]);
	}
      
      nFnames = limit;
      
      // delete old array
      delete [] paux;
    }
  else if(defFuncNames != 0)
    {
      delete [] defFuncNames;
      defFuncNames = new std::string[dim];      
      nFnames = 0;
    }
  else
    {
      defFuncNames = new std::string[dim];            
      nFnames = 0;
    }
  maxFnames = dim;
}

void source_map::resizeF(int dim)
{
  //Resize array 'functions' to size 'dim'

  if(nFunctions > 0)
    {  
      function_map* paux = functions;
      functions = 0;
      functions = new function_map[dim];

      int limit =  nFunctions < dim ? nFunctions : dim;

      for(int i = 0; i < limit; i++)
	{
	  functions[i] = paux[i];
	}

      nFunctions = limit;
  
      // delete old array
      delete [] paux;
    }
  else if(functions != 0)
    {
      delete [] functions;
      functions = new function_map[dim];      
      nFunctions = 0;
    }
  else
    {
      functions = new function_map[dim];      
      nFunctions = 0;
    }
  maxFunctions = dim;  
}

void source_map::resizeNS(int dim)
{
  //Resize array 'functions' to size 'dim'

  if(nNamespaceMaps > 0)
    {  
      namespace_map* paux = namespaceMaps;
      namespaceMaps = 0;
      namespaceMaps = new namespace_map[dim];

      int limit =  nNamespaceMaps < dim ? nNamespaceMaps : dim;

      for(int i = 0; i < limit; i++)
	{
	  namespaceMaps[i] = paux[i];
	}

      nNamespaceMaps = limit;
  
      // delete old array
      delete [] paux;
    }
  else if(namespaceMaps != 0)
    {
      delete [] namespaceMaps;
      namespaceMaps = new namespace_map[dim];      
      nNamespaceMaps = 0;
    }
  else
    {
      namespaceMaps = new namespace_map[dim];      
      nNamespaceMaps = 0;
    }
  maxNamespaceMaps = dim;  
}

void source_map::resizeNSN(int dim)
{
  //Resize array namespaces to size 'dim'

  if(nNamespaces > 0)
    {
      std::string* paux = namespaces;
      namespaces = 0;
      namespaces = new std::string[dim];

      int limit =  nNamespaces < dim ? nNamespaces : dim;

      for(int i = 0; i < limit; i++)
	{
	  namespaces[i].assign(paux[i]);
	}

      nNamespaces = limit;
  
      // delete old array
      delete [] paux;
    }
  else if(namespaces != 0)
    {
      delete [] namespaces;
      namespaces = new std::string[dim];      
      nNamespaces = 0;
    }
  else
    {
      namespaces = new std::string[dim];            
      nNamespaces = 0;
    }
  maxNamespaces = dim;
}


int source_map::tree(int& nleafs, leaf*& tree) const
{
  //This function create a dependence function tree
  //
  //return number of leafs on succes
  //return -1 if map has not any defined function
  //
  //
  
  //Check if there are some functions in the map
  if(nFunctions <= 0)
    return -1;
  
  //Allocate memory for one leaf for each function
  tree = new leaf[nFunctions+1];
  nleafs = nFunctions;

  //Initialize all leafs names and overloads
  for(int i = 0; i < nFunctions; i++)
    {
      tree[i].name.assign(functions[i].name);
      tree[i].overload = functions[i].nOverload;
    }
  
  //Connect each defined function with all his dependences
  for(int i = 0; i < nFunctions; i++)
    {
      //Iterate for all dependencies
      for(int j = 0; j < functions[i].nCalledF; j++)
	{
	  //Extract dependence name
	  std::string dName(functions[i].fcalls[j].name);

	  //Check the name of all leafs
	  for(int k = 0; k < nFunctions; k++)
	    {
	      if(dName.compare(tree[k].name) == 0)
		{
		  //Connect to this leaf
		  tree[i].connect(&tree[k]);
		}
	    }
	}
    }

  return nleafs;
  
}

//Destructor

source_map::~source_map()
{
  clear();
}

//**********************//
//  Auxiliar functions  //
//**********************//

bool blankString(const char* string)
{
  // Check if a string has only blank chars
  int length = strlen(string);
  for(int i = 0; i < length; i++)
    {
      if(!isblank(string[i]))
	{
	  return false;
	}
    }
  return true;
}

int readFile(const char* filename, std::string& store)
{
  // This function reads the entire file with path "filename"
  // and store it in "store".
  //
  // return 0 on succes or -1 if can't open file
  //
  
  FILE* fin = 0;
  fin = fopen(filename,"r");
  if(fin == 0)
    {
      return -1;
    }

  char line[10000];

  //Clear output string
  store.clear();
  
  while(fgets(line,10000,fin) != NULL)
    {
      store.append(line);
    }

  //Close file
  fclose(fin);
  
  return 0;
}

int removeText(std::string& inS, std::size_t pos, const char* limit, bool eraseLimit ,bool scapped)
{
  // This function removes text in 'inS' string from 'pos'
  // to next first 'limit' string match. 'scapped' option
  // enables or disable matches with scapped characters (\limit).
  //
  // The limiting chars will be found from pos+1 until end of string. 
  //
  // If the limiting char is not found, the function will remove
  // all chars until end of string.
  //
  // return  0 if the limiting char has been found
  // return  1 if the limiting char has not been found
  // return -1 for non valid limit length

  std::size_t found;
  int limitLength = strlen(limit);
  if(limitLength <= 0)
    {
      return -1;
    }
  
  if(scapped)
    {
      //Scapped chars allowed for check
      found = inS.find(limit,pos+1);
    }
  else
    {
      found = pos;
      //Scapped chars not allowed to check
      while(found != std::string::npos)
	{	  
	  found = inS.find(limit,found+1);
      
	  if(found != std::string::npos && inS[found-1] != '\\')
	    {
	      //Found limiting char with no previous slash
	      break;
	    }
	  else
	    {
	      found += limitLength-1;
	    }
	}
    }

  //Remove from pos to found
  int rValue = 1;
  if(found != std::string::npos)
    {
      if(eraseLimit)
	{
	  found += limitLength;
	}
      rValue = 0;
    }
  
  inS.erase(pos, found-pos);
  return rValue;  
  
}

int clearCode(std::string& inS)
{
  // This function removes all hard code strings
  // in input string (text between "" or '') and
  // comments (// and /* */)
  //
  // 
  // return the number of removed text segments
  // return -1 if some string delimited by "" are not closed
  // return -2 if some string delimited by '' are not closed
  // return -3 if some comment delimited by // are not closed
  // return -4 if some comment delimited by /* */ are not closed
  // return -5 on error in 'removeText' function

  const char* opening1 = "\"";
  const char* opening2 = "'";
  const char* opening3 = "//";
  const char* opening4 = "/*";
  
  const char* limit1 = "\"";
  const char* limit2 = "'";
  const char* limit3 = "\n";
  const char* limit4 = "*/";
  
  int nremoved = 0;
  std::size_t pos = 0;
  while(pos != std::string::npos)
    {
      std::size_t found1 = inS.find(opening1,pos);
      std::size_t found2 = inS.find(opening2,pos);
      std::size_t found3 = inS.find(opening3,pos);
      std::size_t found4 = inS.find(opening4,pos);

      if(found1 == std::string::npos && found2 == std::string::npos && found3 == std::string::npos && found4 == std::string::npos)
	{
	  pos = std::string::npos;
	}
      else if(found1 < found2 && found1 < found3 && found1 < found4)
	{
	  if( found1 == 0 || inS[found1-1] != '\\' )
	    {
	      // Remove until next non slashed "
	      int err = removeText(inS, found1, limit1, true, false);
	      if(err < 0)
		{
		  printf("removeText:Error: Invalid limit array\n");
		  return -5;
		}
	      else if(err == 1)
		{
		  printf("clearCode:warning: Non limited string (\"__\")\n");
		  nremoved++;
		  return -1;
		}
	      nremoved++;	    
	      pos = found1 == 0 ? 0 : found1-1;	  
	    }
	  else
	    {
	      //Find on next position
	      pos = found1+1;
	    }
	}
      else if(found2 < found3 && found2 < found4)
	{
	  if( found2 == 0 || inS[found2-1] != '\\' )
	    {
	      // Remove until next non slashed '
	      int err = removeText(inS, found2, limit2, true, false);
	      if(err < 0)
		{
		  printf("removeText:Error: Invalid limit array\n");
		  return -5;
		}
	      else if(err == 1)
		{
		  printf("clearCode:warning: Non limited char ('__')\n");
		  nremoved++;
		  return -2;
		}
	      nremoved++;
	      pos = found2 == 0 ? 0 : found2-1;	  
	    }
	  else
	    {
	      //Find on next position
	      pos = found2+1;
	    }
	}
      else if(found3 < found4)
	{
	  if( found3 == 0 || inS[found3-1] != '\\' )
	    {
	      // Remove until next non slashed \n
	      int err = removeText(inS, found3, limit3, false, false);
	      if(err < 0)
		{
		  printf("removeText:Error: Invalid limit array\n");
		  return -5;
		}
	      else if(err == 1)
		{
		  printf("clearCode:warning: Non limited comment (//__\\n)\n");
		  nremoved++;
		  return -3;
		}
	      nremoved++;
	      pos = found3 == 0 ? 0 : found3-1;	  
	    }
	  else
	    {
	      //Find on next position
	      pos = found3+1;
	    }
	}
      else if( found4 == 0 || inS[found4-1] != '\\' )
	{	  
	  // Remove until next */ with or without slash
	  int err = removeText(inS, found4, limit4, true, true);
	  if(err < 0)
	    {
	      printf("removeText:Error: Invalid limit array\n");	      
	      return -5;
	    }
	  else if(err == 1)
	    {
	      printf("clearCode:warning: Non limited comment (/*__*/n)\n");
	      nremoved++;
	      return -4;
	    }
	  nremoved++;
	  pos = found4 == 0 ? 0 : found4-1;	  
	}
      else
	{
	  //Find on next position
	  pos = found4+1;	  
	}
    }
  return nremoved;
}

int definedFunc(std::string& inS, std::string*& pnames)
{
  // This function extract all defined function names in
  // input string in C/C++ format.
  //
  // The input string must has not any comment or strings
  // using "" and ''. Use function clearCode to ensure
  // that.
  //
  // Extracted functions will be marked in the input string to
  // simplify future search.
  //
  // return number of extracted functions
  //

  const char* mark = "%00%";
  int maxDim = 30;
  pnames = new std::string[30];
  
  int nfunctions = 0;
  std::size_t pos = 0;
  while(pos != std::string::npos)
    {
      //Find next '('
      std::size_t posPar = inS.find('(',pos);
      if(posPar == std::string::npos)
	{
	  // Any '(' found
	  break;
	}
      
      //Find position where '(' closes
      std::size_t posParClose = closeKey(inS,posPar);
      if(posParClose == std::string::npos)
	{
	  // Previous '(' is not closed
	  break;
	}
      
      //Find next '{' position
      std::size_t posCor = inS.find('{',posParClose);
      if(posCor == std::string::npos)
	{
	  break;
	}
      
      //Check if there are some ';' between ')' and '{'
      std::size_t posSColon = inS.find(';',posParClose);
      
      if(posSColon < posCor)
	{
	  // Is not a function implementation
	  pos = posSColon;	  
	}
      else
	{
	  // Is a function implementation
	  // Extract function identifier
	  std::size_t begin;
	  pos = extractFunction(inS,posPar,pnames[nfunctions],begin);
	  
	  if(pos != std::string::npos)
	    {
	      std::size_t shift = 0; //Stores the shift produced by marks insertion
	      //Check if is a statement: 'if', 'while', 'do', 'for', 'else'...
	      if(!checkStatment(pnames[nfunctions]))
		{
		  //Insert marks at the beginning.
		  inS.insert(begin,mark);
		  shift = strlen(mark);
		  
		  // Add this function name if has not already been added.
		  if(nameMatch(pnames, nfunctions, pnames[nfunctions].c_str()) < 0)
		    {
		      //Is not in array 
		      nfunctions++;
		      
		      //Check array size
		      if(nfunctions >= maxDim)
			{
			  //Resize array
			  std::string* auxp = pnames;
			  pnames = 0;
			  pnames = new std::string[2*maxDim];
			  
			  for(int i = 0; i < maxDim; i++)
			    {
			      pnames[i] = auxp[i];
			    }
			  delete [] auxp;
			  maxDim = 2*maxDim;
			}
		    }
		}
	      //Find corresponding close key '}' position
	      pos = closeKey(inS, posCor + shift);
	    }
	}
    }

  return nfunctions;
}

int usedNamespaces(std::string& inS, std::string*& pnames, const char* fname, int overload)
{
  // This function extract names of all namespaces used by
  // function 'fname' in input string in C/C++ format.
  //
  //
  // The input string must has not any comment or strings
  // using "" and ''. Use function clearCode to ensure
  // that.
  //
  // return -1 if function not found
  // return -2 if no marks exists in 'inS'
  // return number of extracted namespaces on succes


  const char* defineMark = "%00%";

  // Check if 'inS' has already been marked

  if(inS.find(defineMark,0) == std::string::npos){return -2;}
  
  int maxDim = 30;
  pnames = new std::string[30];

  // Search the specified function overload
  // Extract position of first '{' (beginning of function definition)
  std::size_t pos = posDefinedFunc(inS, fname, overload);
  std::size_t endf;
  if(pos != std::string::npos)
    {
      //Extract end of function (position of close '}')
      endf = closeKey(inS, pos);
    }
  else
    {
      // This function/overload doesn't exists
      delete [] pnames;
      return -1;
    }

  int nNamespaces = 0;
  while(pos < endf)
    {
      //Find next namespace
      std::size_t posBegin;
      pos = extractNamespace(inS,pos,pnames[nNamespaces],posBegin);
      
      if(posBegin < endf)
	{
	  // This namespace is in actual function definition
	  // Add this namespace name if has not already been added.
	  int arrayPos = nameMatch(pnames, nNamespaces, pnames[nNamespaces].c_str());
	  if(arrayPos < 0)
	    {
	      //Is not in the array 
	      nNamespaces++;
		  
	      //Check array size
	      if(nNamespaces >= maxDim)
		{
		  //Resize array
		  std::string* auxp = pnames;
		  pnames = 0;
		  pnames = new std::string[2*maxDim];
			
		  for(int i = 0; i < maxDim; i++)
		    {
		      pnames[i] = auxp[i];
		    }
		  delete [] auxp;
		  maxDim = 2*maxDim;
		}
	    }
	  pos += 2;  // Skip found namespace possible ::
	}
      else
	{
	  pos = endf;
	}
    }

  return nNamespaces;  
  
}

int calledFunc(std::string& inS, fcall*& fcalls, const char* fname, int overload)
{
  //Wrapper to allow the use of 'calls' struct with
  //'calledFunc' function

  std::string* pnames = 0;
  int* counts = 0;
  int value = calledFunc(inS, pnames, counts, fname, overload);

  //Store values in struct array
  if(value > 0)
    {
      fcalls = new fcall[value];
      for(int i = 0; i < value; i++)
	{
	  fcalls[i].name.assign(pnames[i]);
	  fcalls[i].calls = counts[i];
	}
    }
  
  if(value >= 0)
    {
      delete [] pnames;
      free(counts);
    }
  return value;
}

int calledFunc(std::string& inS, std::string*& pnames, int*& counts, const char* fname, int overload)
{
  // This function extract names of all functions called by
  // function 'fname' in input string in C/C++ format.
  //
  //
  // The input string must has not any comment or strings
  // using "" and ''. Use function clearCode to ensure
  // that.
  //
  // return -1 if function not found
  // return -2 if no marks exists in 'inS'
  // return number of extracted functions on succes

  const char* defineMark = "%00%";

  // Check if 'inS' has already been marked

  if(inS.find(defineMark,0) == std::string::npos){return -2;}
  
  int maxDim = 30;
  pnames = new std::string[maxDim];
  counts = (int*) malloc(sizeof(int)*maxDim);

  // Search the specified function overload
  // Extract position of first '{' (beginning of function definition)
  std::size_t pos = posDefinedFunc(inS, fname, overload);
  std::size_t endf;
  if(pos != std::string::npos)
    {
      //Extract end of function (position of close '}')
      endf = closeKey(inS, pos);
    }
  else
    {
      // This function/overload doesn't exists
      delete [] pnames;
      free(counts);
      return -1;
    }
    
  
  int nfunctions = 0;
  while(pos < endf)
    {
      //Find next function
      std::size_t posBegin;
      extractFunction(inS,pos,pnames[nfunctions],posBegin);
      
      if(posBegin < endf)
	{
	  // This function is in actual function definition
	  // Check if is a statment: if, else if, switch...
	  if(!checkStatment(pnames[nfunctions]))
	    {
	      // Add this function name if has not already been added.
	      int arrayPos = nameMatch(pnames, nfunctions, pnames[nfunctions].c_str());
	      if(arrayPos < 0)
		{
		  //Is not in the array 
		  counts[nfunctions] = 1;
		  nfunctions++;
		  
		  //Check array size
		  if(nfunctions >= maxDim)
		    {
		      //Resize array
		      std::string* auxp = pnames;
		      int* auxpInt = counts;
		      pnames = 0;
		      counts = 0;
		      pnames = new std::string[2*maxDim];
		      counts = (int*) malloc(sizeof(int)*2*maxDim);
			
		      for(int i = 0; i < maxDim; i++)
			{
			  pnames[i] = auxp[i];
			  counts[i] = auxpInt[i];
			}
		      delete [] auxp;
		      maxDim = 2*maxDim;
		    }
		}
	      else
		{
		  counts[arrayPos]++;
		}
	    }
	  pos = inS.find('(',posBegin)+1;  // Skip found function open parenthesis '('
	}
      else
	{
	  pos = endf;
	}
    }

  return nfunctions;
}

int nameMatch(std::string* sarray, int dim, const char* name)
{
  // This function check if 'name' exists in the string array
  // 'sarray' of dimension 'dim' and return its position.
  //
  // if the name is not found, the return value will be -1.
  //
  
  for(int i = 0; i < dim; i++)
    {
      if(sarray[i].compare(name) == 0)
	{
	  return i;
	}
    }
  return -1;
}

std::size_t extractNamespace(std::string& inS, std::size_t init, std::string& name, std::size_t& begin)
{
  // This function return the name of next used namespace in 'inS' string.
  // 'init' mark the first position to search.
  //
  // returns the end namespace position (position of "::" or ';' for "using namespace").
  // variable 'begin' will store first position of identifier name

  std::size_t posNS = inS.find(" namespace ",init);
  std::size_t pos2D = inS.find("::",init);

  if(posNS == std::string::npos && pos2D == std::string::npos)
    {
      begin = std::string::npos;
      return std::string::npos;
    }
  
  if(posNS < pos2D)
    {
      //Found first a " namespace " string
      size_t firstPos = inS.find(' ', posNS+3);
      begin = firstPos;
      //Extract string after " namespace "
      char auxNamespace[400];
      int nread;
      sscanf(inS.c_str() + firstPos, " %s%n",auxNamespace,&nread);
      name.assign(auxNamespace);

      //Check for end ';'
      size_t posSC = name.find(';');
      if(posSC != std::string::npos)
	{
	  name.erase(posSC,1);
	}
      return firstPos+nread;
    }
  else
    {
      //Found first a "::"
      // Search identifier init
      int cont = 0;  // Count number of positions between identificator init and '('
      std::size_t idEnd = pos2D-1; //Store end position of namespace identifier
      while(1)
	{
	  if(inRange(inS[idEnd-cont]) > 0)
	    {
	      cont++;
	    }
	  else
	    {
	      //End of namespace identifier
	      break;
	    }
	}

      if(cont > 0)
	{
	  std::size_t idBegin = idEnd - (cont-1);
	  begin = idBegin;
	  name.assign(inS,idBegin,idEnd-idBegin+1);
  
	  return pos2D;
	}
      else
	{
	  // Call function itself to find next possible namespace
	  return extractNamespace(inS,pos2D+1,name,begin);
	}
      
    }
}

std::size_t extractFunction(std::string& inS, std::size_t init, std::string& name, std::size_t& begin)
{
  // This function return the name of next function in 'inS' string.
  // 'init' mark the first position to search. The function will extract
  // function name corresponding to next '(' found.
  //
  // returns the end function position (position of ')').
  // variable 'begin' will be filled by the first position of identifier name
  // returns std::string::npos if the function has not a close key ')'
  // returns std::string::npos if not found any function

  std::size_t pos = init;

  std::size_t posPar = inS.find('(',pos);

  if(posPar == std::string::npos) //No function found
    {
      begin = std::string::npos;
      return posPar;
    }
  
  std::size_t closePos = closeKey(inS,pos);

  // Search identifier init
  int cont = 0;  // Count number of positions between identificator init and '('
  bool namespaceFound = false; //Control if namespace has been specified
  bool blankSkiped = false; //Control if whitespaces has been skiped
  std::size_t idEnd = posPar-1; //Store end position of function identifier
  while(1)
    {
      if(inRange(inS[idEnd-cont]) > 0)
	{
	  blankSkiped = true;
	  cont++;
	}
      else if(!blankSkiped && (isblank(inS[idEnd-cont]) || inS[idEnd-cont] == '\n'))
	{
	  idEnd--; //Skip blank character
	}
      else if(blankSkiped && !namespaceFound && (int)idEnd-cont-1 >= 0)
	{
	  if(inS[idEnd-cont] == ':' && inS[idEnd-cont-1] == ':')
	    {
	      namespaceFound = true;
	      cont += 2;
	    }
	  else
	    {
	      //End of function identifier
	      break;
	    }
	}
      else
	{
	  //End of function identifier
	  break;
	}
    }

  if(cont > 0)
    {
      std::size_t idBegin = idEnd - (cont-1);
      begin = idBegin;
      name.assign(inS,idBegin,idEnd-idBegin+1);
  
      return closePos;
    }
  else
    {
      // Call function itself to find next possible function
      return extractFunction(inS,posPar+1,name,begin);
    }  
}

std::size_t posDefinedFunc(std::string& inS, const char* name, int overload)
{
  // Return the position of the begining of function definition (on character '{')
  // The function name must be marked by 'definedFunc'.
  // If is not found will return 'std::string::npos'
  
  const char* defineMark = "%00%";

  int nfound = 0;
  std::size_t pos = 0;
  while(pos != std::string::npos)
    {
      std::size_t pos1 = inS.find(defineMark,pos);
      pos = std::string::npos;

      if(pos1 != std::string::npos)
	{	  
	  std::size_t pos2 = inS.find('(',pos1);

	  if(pos2 != std::string::npos)
	    {
	      std::size_t nameBegin = pos1 + strlen(defineMark);
	      std::string auxName = inS.substr(nameBegin, pos2-nameBegin);

	      //Check if auxName has some whitespace at end
	      std::size_t posWS = auxName.find(' ');
	      if(posWS != std::string::npos)
		{
		  //Erase whitespaces
		  auxName.erase(posWS);
		}
	      
	      std::size_t posCor = inS.find('{',pos2);
	      if(auxName.compare(name) == 0)
		{
		  // This is the desired function,
		  nfound++;
		  if(nfound == overload)
		    {
		      //This is the specified definition overload
		      return posCor;
		    }
		}
	      pos = closeKey(inS, posCor);
	    }
	}
    }
  return std::string::npos;
}

bool checkStatment(std::string& string)
{
  // This function check if input string is
  // an statment or not

  const int nStatements = 14;
  const char* statements [] = {
    "if",
    "else",
    "for",
    "else if",
    "while",
    "do",
    "switch",
    "case",
    "do while",
    "break",
    "continue",
    "goto",
    "default",
    "return"
  };

  for(int i = 0; i < nStatements; i++)
    {
      if(string.compare(statements[i]) == 0)
	return true;
    }
  return false;
}

std::size_t closeKey(std::string& inS, std::size_t init)
{
  // This function returns the position where the key in
  // position 'init' closes with the corresponding close key.
  //
  // For example, if a '(' is at position 'init', the returned
  // value will be the position where '(' close. If the 'init'
  // position is close side key (')') the function will find
  // the corresponding '(' from right to left.
  //
  // return the position of corresponding open/close key on success
  // return std::string::npos if char on 'init' position is not a open/close key
  // return std::string::npos if no close key has been found for specified open key
  // return std::string::npos if no open key has been found for specified close key
  
  const int nKeys = 3;
  const char openKeys[] = {'(','[','{'};
  const char closeKeys[] = {')',']','}'};

  bool isOpenKey;
  int id = -1;

  for(int i = 0; i < nKeys; i++)
    {
      if(inS[init] == openKeys[i])
	{
	  id = i;
	  isOpenKey = true;
	  break;
	}
      if(inS[init] == closeKeys[i])
	{
	  id = i;
	  isOpenKey = false;
	  break;
	}      
    }

  if(id < 0){return std::string::npos;}

  if(isOpenKey)
    {
      //Is a open key, find the corresponding close key
      std::size_t nextClose = 1;
      std::size_t nextOpen = 0;

      std::size_t pos = init;
      
      while(nextClose > nextOpen)
	{
	  nextClose = inS.find(closeKeys[id],pos+1);
	  nextOpen  = inS.find(openKeys[id],pos+1);

	  if(nextClose == std::string::npos)
	    {
	      return std::string::npos; //Corresponding close key not found
	    }
	  
	  if(nextOpen < nextClose)
	    {
	      // Another open key found before close key. Skyp this enclosure
	      pos = closeKey(inS, nextOpen);
	      if(pos == std::string::npos){ return std::string::npos;} //Corresponding close key not found
	    }
	}
      return nextClose;
    }
  else
    {
      //Is a close key, find the corresponding open key
      std::size_t prevClose = 1;
      std::size_t prevOpen = 0;

      std::size_t pos = init;
      
      while(prevClose > prevOpen)
	{
	  prevClose = inS.rfind(closeKeys[id],pos-1);
	  prevOpen  = inS.rfind(openKeys[id],pos-1);

	  if(prevOpen == std::string::npos)
	    {
	      return std::string::npos; //Corresponding open key not found
	    }
	  
	  if(prevOpen < prevClose)
	    {
	      // Another close key found before open key. Skyp this enclosure
	      pos = closeKey(inS, prevClose);
	      if(pos == std::string::npos){ return std::string::npos;} //Corresponding open key not found
	    }
	}
      return prevOpen;
    }
}

int inRange(char c)
{
  // Check if character 'c' is in acceptable
  // identifier range.
  //
  // return 1 if is in number range (0,1,2,3,4,5,6,7,8,9)
  // return 2 if is in upercase range
  // return 3 if is in downcase range
  // return 4 if is '_'
  // return 0 if is out of all ranges
  
  const int nIntervals = 4;
  const int charLimits[] = {
    48,57,   //Numbers
    65,90,   //Upcase
    97,122,  //Downcase
    95,95    // '_'
  };

  for(int i = 0; i < nIntervals; i++)
    {
      if(c >= charLimits[2*i] && c <= charLimits[2*i+1])
	{
	  return i+1;
	}
    }
  return 0;
}

bool stringOrder(const std::string& a, const std::string& b)
{
  // Compare two string to sort them alphabetically
  //
  
  if(a.compare(b) < 0)
    {
      return true;
    }
  return false;
}

bool stringOrderInv(const std::string& a, const std::string& b)
{
  // Compare two string to sort them alphabetically
  //
  
  if(a.compare(b) > 0)
    {
      return true;
    }
  return false;
}

int tree2dot(leaf* tree, int nleafs, const char* filename, unsigned int nprop, const char** prop)
{
  
  FILE* ftree = 0;
  
  ftree = fopen(filename,"w");
  if(ftree == 0)
    return -1;
  
  fprintf(ftree, "digraph G {\n");

  if(nprop > 0 && prop != 0)
    {
      for(unsigned int i = 0; i < nprop; i++)
	{
	  fprintf(ftree, "%s;\n",prop[i]);	  
	}
    }
  
  for(int i = 0; i < nleafs; i++)
    {
      
      int nlinks = tree[i].getNumLinks();
      for(int j = 0; j < nlinks; j++)
	{
	  const leaf* link = tree[i].getLink(j);
	  fprintf(ftree,"     %s -> %s;\n",tree[i].name.c_str(),link->name.c_str());
	}
    }      
  
  fprintf(ftree, "}\n");
  fclose(ftree);

  return 0;
}
