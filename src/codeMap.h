
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

/** @file codeMap.h 
    \brief code mapping headers.
*/ 

#ifndef __CODE_MAP_C_CPP__
#define __CODE_MAP_C_CPP__

#include <ctype.h>
#include <string>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <new>
#include <stdlib.h>
#include <cstdlib>

/**
 * \class leaf
 * \ingroup code-parser
 *
 * This class will be used to generate
 * dependence trees. Each leaf contain
 * a array of pointers to #leaf type
 * variables. This is basicaly a linked
 * list class. Each leaf is connected to
 * all leafs variables pointed by the
 * stored pointers in #links array.
 *
 *
 */

struct leaf
{
  friend class source_map;  
protected:
  /// Array of linked leafs.
  leaf** links;
  /// Maximum size of #links. This value can change if #links is resized.
  unsigned int maxLinks;
  /// Current number of elements in #links.
  unsigned int nLinks;   

public:
  /// Identification name of current leaf.
  std::string name;
  /// Overload number allow diferenciate between leafs with same #name.
  int overload;

  /// Create a leaf.
  leaf();             

  /** \brief Connect function
   *  \param pleaf -> Pointer to leaf that will be connected.
   *  \return Return 0 if connection has been added
   *  \return Return 1 if connection already exists
   *  \return Return 2 on self connection try (will not be connected).
   *
   *   Connect the current leaf with the
   *   leaf pointed by \a pleaf. 
   *
   */
  
  int connect(leaf*);   

  /// Remove all connections and set default values to overload an name
  void clear();

  /// Return the number of links for current leaf
  inline int getNumLinks() const{return nLinks;}  

  /** \brief Get specified link
   *  \param i -> Position to be extracted from links array.
   *  \return Return null pointer if \a i is out of range
   *  \return Return a constant pointer to specified leaf in #links on success
   *
   *   Extract a constant pointer to a leaf in #links
   *   specified by \a i. 
   *
   */
  
  inline const leaf* getLink(unsigned int i) const{    
    if(i > nLinks)
      return 0;
    return links[i];
  }

  /** \brief Check link existence
   *  \param pleaf -> pointer to check.
   *  \return Return true if \a pleaf is in #links array.
   *  \return Return false if \a pleaf is not in #links array.
   *
   *  Check the existence of pointer \a pleaf in #links array.
   *
   */
  
  inline bool checkLink(leaf* pleaf) const{
    for(unsigned int i = 0; i < nLinks; i++)
      if(links[i] == pleaf)
	return true;
    return false;
  }

  /// #leaf destructor
  ~leaf();
};

/**
 * \struct fcall
 * \ingroup code-parser
 *
 * Simple struct that stores a function name
 * and how many times this function has been called.
 *
 */


struct fcall
{
public:
  /// Function name
  std::string name;
  /// Calls to function specified by #name.
  int calls;        

  /// Constructor
  fcall();
  /// Constructor that copy argument #fcall
  fcall(const fcall&);
  /// Overload of assign operator (=)
  fcall& operator=(const fcall&);
  /// Overload of operator < (used for sorting)
  bool operator<(const fcall&) const;
  /// Overload of operator > (used for sorting)
  bool operator>(const fcall&) const;  
};

/**
 * \class namespace_map
 * \ingroup code-parser
 *
 * This class store information of one
 * namespace specified by #name.
 *
 */

class namespace_map
{
  friend class source_map;  
protected:
  /// Current maximum number of elements in #functionsNames.
  int maxnfnames;
  /// Current number of elements in #functionsNames.
  int nfnames;
  /**  Resizable array of strings that stores the function
   *   names that uses namespace specified by #name.
   */
  std::string* functionsNames;  

  /** \brief Resize array #functionsNames
   *  \param dim -> new dimension.
   *
   *  Resize array #functionsNames keeping existing
   *  elements. If \a dim is less than #nfnames, only
   *  first \a dim elements will be saved.
   */  
  void resizeFNames(int);
  
public:
  /// Store namespace name
  std::string name;

  /// Constructor
  namespace_map();
  /// Constructor that copy argument #namespace_map
  namespace_map(const namespace_map&);
  
  /** \brief Append a function name to #functionsNames array.
   *  \param fname -> Name to append.
   *  \return Return 0 on success.
   *  \return Return 1 if \a fname already exists in #functionsNames.
   *
   */

  int appendF(const char*);
  /// Sort #functionsNames elements alphabetically
  void sort();

  /** \brief Convert stored information to string format.
   *  \param tabs -> Number of initial tabulations.
   *  \return Return a string containing formated information.
   *
   *  Convert all stored information to plain text and stores
   *  it in returned string.
   */

  std::string to_string(int) const;

  /// Free memory where #functionsNames has been allocated and set #name to default value.
  void clear();

  /// Overload of assign operator
  namespace_map& operator=(const namespace_map&);
  /// Overload of comparision operator < (for sorting)
  bool operator<(const namespace_map&) const;
  /// Overload of comparision operator > (for sorting)
  bool operator>(const namespace_map&) const;

  /// Destructor
  ~namespace_map();
  
};

/**
 * \class function_map
 * \ingroup code-parser
 *
 * This class store information of one
 * function specified by #name, #sourceFile
 * and #nOverload.
 */

class function_map
{
  friend class source_map;
 protected:
  /// Number of namespaces used in this function.
  int nNamespaces;
  /// Number of different functions called in this function.
  int nCalledF;

  /// Maximum elements than fit in #namespaces array.
  int maxNamespaces;
  /// Maximum elements than fit in #fcalls array.
  int maxCalledF;

  /// Array with namespace names used in this function.
  std::string* namespaces;
  /// Array with pairs name/number of calls for each called function.
  fcall* fcalls;

  /** \brief Resize array #fcalls
   *  \param dim -> new dimension.
   *
   *  Resize array #fcalls keeping existing
   *  elements. If \a dim is less than #nCalledF, only
   *  first \a dim elements will be saved.
   */  
  void resizeFCalls(int);
  
 public:
  /// Function name.
  std::string name;
  /// Function source filename.
  std::string sourceFile;
  /// Overload ID. This parameter allow to diferenciate between functions with same name (overloaded).
  int nOverload;

  /// Constructor.
  function_map();
  /// Constructor that copy argument #function_map
  function_map(const function_map&);

  /** \brief Load function information
   *  \param inS -> input string.
   *  \param fname -> Name of function to extract.
   *  \param overload -> Overload to load.
   *
   *  \return Return 0 on success.
   *  \return Return -1 if the function has not been found
   *  \return Return -2 if no marks exists in input string.
   *
   *  this function load information of the specified function by
   *  \a fname and \a overload. The information extracted are 
   *  used namespaces and called functions (pairs name/ncalls).
   *
   *  Notice that source string \a inS must have been marked previously
   *  by function #definedFunc .
   */
  
  int loadFunction(std::string, const char*, int = 1);

  /** \brief Extract namespace name
   *  \param i -> Array #namespaces position.
   *
   *  \return Return a string with specified namespace name.
   *          if \a i is out of range return a empty string.
   *
   */
  std::string getNamespace(int ) const;

  /** \brief Extract called function name
   *  \param i -> Array position.
   *
   *  \return Return a string with specified called function name.
   *          if \a i is out of range return a empty string.
   *
   */  
  std::string getFName(int ) const;

  /** \brief Extract specified #fcall pair
   *  \param i -> Array #fcalls position.
   *
   *  \return Return a #fcall pair with specified called function.
   *          if \a i is out of range return a empty #fcall.
   *
   */  
  fcall getCall(int) const;

  /** \brief Append a #fcall to #fcalls array
   *  \param call
   *  \return number of elements in #fcalls on succes
   *  \return -1 if the pair already exists
   */
  int appendCall(fcall);
  
  /** \brief Append a #fcall to #fcalls array
   *  \param fname -> function name.
   *  \param ncalls -> number of calls.
   *  \return number of elements in #fcalls on succes
   *  \return -1 if the pair already exists
   */
  int appendCall(const char*, int);

  /** \brief Set the number of calls of element in #fcalls array.
   *  \param call -> pair function name/number of calls.
   *  \return number of elements in #fcalls if any pair with the same name exists
   *  \return 0 on success.
   *  
   *  Overwrite number of calls of element in #fcalls with the same name of \a call. If no
   *  element has this name, \a call will be appended to #fcalls.   
   */
  int setCall(fcall);

  /** \brief Set the number of calls of element in #fcalls array.
   *  \param fname -> function name.
   *  \param ncalls -> new number of calls.
   *  \return number of elements in #fcalls if any pair with specified name exists
   *  \return 0 on success.
   *  
   *  Overwrite number of calls of element in #fcalls specified by \a fname. If no
   *  element has this name, a #fcall element will be appended with input data.   
   */  
  int setCall(const char*, int);

  
  /** \brief Remove specified element from #fcalls array
   *  \param fname -> function name to remove.
   *  \return number of elements in #fcalls on success
   *  \return -1 if no element has the specified name.
   */
  int removeCall(const char*);

  /// Free allocated memory and set default values.
  void clear();

  /** \brief Convert stored information to string format.
   *  \param tabs -> Number of initial tabulations.
   *  \return Return a string containing formated information.
   *
   *  Convert all stored information to plain text and stores
   *  it in returned string.
   */  
  std::string to_string(int = 0) const;

  /// Sort #namespaces and #fcalls elements alphabetically
  void sort();

  /// Overload for assign operator (=)
  function_map& operator=(const function_map&);
  /// Overload for comparision operator < (for sorting)
  bool operator<(const function_map&) const;
  /// Overload for comparision operator > (for sorting)
  bool operator>(const function_map&) const;

  /// return function name
  inline std::string getName() const { return name;}
  /// return source filename
  inline std::string getSource() const { return sourceFile;}
  /// return number of used namespaces
  inline int numNS() const {return nNamespaces;}
  /// return number of called functions
  inline int numFC() const {return nCalledF;}
  /// return overload value
  inline int overload() const {return nOverload;}

  /// Destructor
  ~function_map();
};

/**
 * \class source_map
 * \ingroup code-parser
 *
 * This class store information 
 * extracted directly from source
 * code files.
 */


class source_map
{
 protected:
  /// Number of #function_map stored in #functions array.
  int nFunctions;
  /// Number of different function names defined.
  int nFnames;
  /// Number of used namespace names.  
  int nNamespaces;
  /// Number of #namespace_map stored in #namespaceMaps array.  
  int nNamespaceMaps;

  /// Current maximum number of elements that fit in #functions .
  int maxFunctions;
  /// Current maximum number of elements that fit in #defFuncNames .
  int maxFnames;
  /// Current maximum number of elements that fit in #namespaces .
  int maxNamespaces;
  /// Current maximum number of elements that fit in #namespaceMaps .
  int maxNamespaceMaps;
  
  /// Array with used namespace names.
  std::string* namespaces;  
  /// Array with defined functions names.
  std::string* defFuncNames;
  /// Array with mapped functions.
  function_map* functions;
  /// Array with mapped namespaces.
  namespace_map* namespaceMaps;  

  /** \brief Resize array #defFuncNames
   *  \param dim -> new dimension.
   *
   *  Resize array #defFuncNames keeping existing
   *  elements. If \a dim is less than #nFnames, only
   *  first \a dim elements will be saved.
   */  
  void resizeFN(int);
  
  /** \brief Resize array #functions
   *  \param dim -> new dimension.
   *
   *  Resize array #functions keeping existing
   *  elements. If \a dim is less than #nFunctions, only
   *  first \a dim elements will be saved.
   */    
  void resizeF(int);
  
  /** \brief Resize array #namespaces
   *  \param dim -> new dimension.
   *
   *  Resize array #namespaces keeping existing
   *  elements. If \a dim is less than #nNamespaces, only
   *  first \a dim elements will be saved.
   */      
  void resizeNSN(int);

  /** \brief Resize array #namespaceMaps
   *  \param dim -> new dimension.
   *
   *  Resize array #namespaceMaps keeping existing
   *  elements. If \a dim is less than #nNamespaceMaps, only
   *  first \a dim elements will be saved.
   */        
  void resizeNS(int);

 public:

  /// Constructor.
  source_map();

  /** \brief Load specified source filename
   *  \param filename -> Source filename.
   *  \return Return 0 on success.
   *  \return Return -1 if file can't be opened.
   *  \return Return -2 if an error clearing the code occurs.
   *  \return Return -3 if there are not any function definition
   *          in specified source file.
   *
   *  Clear all current #source_map using
   *  function #clear and process the specified
   *  \a filename to create a new map.
   */          
  int loadSource(const char*);

  /** \brief Append specified source filename.
   *  \param filename -> Source filename.
   *  \return Return 0 on success.
   *  \return Return -1 if file can't be opened.
   *  \return Return -2 if an error clearing the code occurs.
   *  \return Return -3 if there are not any function definition
   *          in specified source file.
   *
   *  Map the source file specified by \a filename and
   *  merge the created map to the previous one stored in
   *  current #source_map.
   */          
  int appendSource(const char*);

  /** \brief Convert stored information to string format.
   *  \param tabs -> Number of initial tabulations.
   *  \return Return a string containing formated information.
   *
   *  Convert all stored information to plain text and stores
   *  it in returned string.
   */    
  std::string to_string(int = 0) const;

  /** \brief Extract namespace name.
   *  \param i -> Array #namespaces position.
   *
   *  \return Return specified namespace name.
   *          if \a i is out of range return a empty string.
   *
   */    
  std::string getNamespaceName(int) const;
  
  /** \brief Extract defined function name.
   *  \param i -> Array #defFuncNames position.
   *
   *  \return Return specified namespace name.
   *          if \a i is out of range return a empty string.
   *
   */      
  std::string getFName(int) const;  

  /** \brief Remove specified #function_map.
   *  \param functionName -> function name.
   *  \param sourceFile -> source filename.
   *  \param overload -> overload value.
   *
   *  \return Return 0 on success.
   *  \return Return -1 if specified function doesn't exists
   *
   *  Remove first #function_map that match with specified input
   *  parameters from #functions array. If \a sourceFile is set to
   *  null pointer and/or \a overload is set to -1 the function don't
   *  worries about this respectives variables to make the match.
   *  
   */        
  int removeF(const char*, const char* = 0, int = -1);

  /** \brief Remove specified #function_map.
   *  \param i -> Array  position.
   *
   *  \return Return 0 on success.
   *  \return Return -1 if \a i is out of range
   *
   *  Remove the element \a i of array #functions.
   */          
  int removeF(int);

  /// Updates used namespaces and defined function names.
  void update();

  /// Free allocated memory and set all varaibles to default values
  void clear();

  /** \brief Get specified #function_map pointer from #functions array.
   *  \param functionName -> function name.
   *  \param sourceFile -> source filename.
   *  \param overload -> overload value.
   *
   *  \return Return null pointer if specified #function_map doesn't exists.
   *  \return Return constant pointer to specified #function_map in #functions array
   *
   *  If \a sourceFile is a null pointer and/or \a overload is < 0, the 
   *  respectively variables are ignored to perform the match. The pointer
   *  to the first matching element will be returned.
   */         
  const function_map* getFunction(const char*, const char* = 0, int = -1) const;

  /** \brief Get specified #function_map pointer from #functions array.
   *  \param i -> Array position.
   *
   *  \return Return null pointer if \a i is out of range.
   *  \return Return constant pointer to specified #function_map in #functions array
   *
   */           
  const function_map* getFunction(int) const;

  /** \brief Get specified #namespace_map pointer from #namespaceMaps array.
   *  \param namespaceName -> namespace name.
   *
   *  \return Return null pointer if the name of any element matches with \a namespaceName.
   *  \return Return constant pointer to specified #namespace_map in #namespaceMaps array
   *
   */             
  const namespace_map* getNamespace(const char*) const;

  /** \brief Get specified #namespace_map pointer from #namespaceMaps array.
   *  \param i -> Array position.
   *
   *  \return Return null pointer if \a i is out of range.
   *  \return Return constant pointer to specified #namespace_map in #namespaceMaps array
   *
   */           
  const namespace_map* getNamespace(int) const;

  /** \brief Extract function names that uses all specified namespaces.
   *  \param namesNS -> Array with namespace names.
   *  \param nNS -> Number of specified namespaces.
   *  \param outF -> String array with names of functions that passed the filter.
   *  \param nF -> Input : Maximum number of elements that fit in \a outF
   *               Output: Number of functions that passed the filter.
   *
   *  \return Return 0 on success.
   *  \return Return -1 if there are more possible matches than specified \a nF value.
   *  \return Return -2 if some namespace doesn't exists.
   *
   *
   *  Find all functions that uses all specified namespaces and store the corresponding
   *  function names in string array \a outF.
   *
   */           
  int filterFunctions(const char**, const int, std::string*, int&) const;

  
  /// Sort #namespaces and #fcall elements alphabetically
  void sort();

  /// Return number of namespaces (#nNamespaces)
  inline int numNS() const {return nNamespaces;}
  /// Return number of defined function names (#nFnames)
  inline int numFN() const {return nFnames;}
  /// Return number of defined functions (#nFunctions)
  inline int numF() const {return nFunctions;}

  /** \brief Create a dependence function tree.
   *  \param nleafs -> Number of output #leaf in \a tree.
   *  \param tree -> Array of #leaf.
   *
   *  \return Return \a nleafs on succes.
   *  \return Return -1 if map has not any defined function
   *
   *  Fill \a tree array with connected #leaf according to
   *  called functions in each defined function. Called functions
   *  that are not defined will be ignored in the tree.
   */             
  int tree(int&, leaf*&) const;

  /// Destructor
  ~source_map();
  
};

/// Return true if input string has only blank chars. Return false otherwise.
bool blankString(const char* string);

/** 
 *  \param c -> Input character.
 *
 *  \return Return 1 if is in number range (0,1,2,3,4,5,6,7,8,9)
 *  \return Return 2 if is in upercase range
 *  \return Return 3 if is in downcase range
 *  \return Return 4 if is '_'
 *  \return Return 0 if is out of all ranges
 *
 *  Check if input character \a c is a valid character
 *  for C/C++ identifier (function names, variable names...).
 *  Also, return the range type (number, upercase, downcase...)
 */             
int inRange(char c);

/** \brief Store file plain text
 *  \param filename -> File filename to read.
 *  \param store -> Outpu string.
 *
 *  \return Return 0 on success.
 *  \return Return -1 if can't open file.
 *
 *  Reads the file specified by \a filename and store
 *  the corresponding plain text in \a store.
 */
int readFile(const char* filename, std::string& store);

/** \brief Remove text interval
 *  \param inS -> input text.
 *  \param pos -> initial position.
 *  \param limit -> limiting string.
 *  \param eraseLimit -> Enable/disable limit erasing.
 *  \param scapped -> Enable/disable considere scapped characters for limits.
 *
 *  \return Return 0 if the limiting string has been found.
 *  \return Return 1 if the limiting string has not been found.
 *  \return Return -1 if the limiting string has non valid length.
 *
 *  Search in \a inS string from position \a pos+1 until end of string the
 *  specified limiting string \a limit. All characters in this interval will
 *  be removed. If \a eraseLimit is true, the limiting string will be removed
 *  too. If \a scapped is true, \a limit will be considered as valid limit.
 *  Otherwise, if is false, \a limit will not be a valid \a limit match.
 *
 *  The first position to be erased is \a pos.
 *
 */
int removeText(std::string& inS, std::size_t pos, const char* limit, bool eraseLimit = true, bool scapped = false);

/** \brief Clear code
 *  \param inS -> Input string with source code plain text.
 *
 *  \return Return the number of removed text segments on success.
 *  \return Return -1 if some string delimited by "" are  not closed.
 *  \return Return -2 if some string delimited by '' are  not closed.
 *  \return Return -3 if some string delimited by // are  not closed.
 *  \return Return -4 if some string delimited by /* * / are  not closed.
 *  \return Return -5 on error in #removeText function.
 *
 *  Remove all hard code strings in \a inS (text between "" or '') and
 *  all comments (// and /* * /) in C/C++ style.
 */
int clearCode(std::string& inS);


/** \brief Extract and mark defined functions
 *  \param inS -> Input string with source code plain text without any hard code string or comment.
 *  \param pnames -> Output array of strings with defined functions names.
 *
 *  \return Return number of extracted functions.
 *
 *  Extract all defined function names in input string \a inS that contains
 *  a C/C++ cleared code (without hard code string nor comments). To ensure
 *  that no remaining comments or hard code strings are in \a inS use function
 *  #clearCode.
 *
 *  Also, mark defined functions in input string \a inS. Then, this string 
 *  can be used as input for other functions in this library. \a pnames will
 *  be allocated to store data.
 *
 */
int definedFunc(std::string& inS, std::string*& pnames);

/** \brief Obtain position of corresponding closing key.
 *
 *  \param inS -> Input string.
 *  \param init -> Initial position.
 *
 *  \return Return the position of corresponding open/close key on success
 *  \return Return std::string::npos if character on \a init position is not a open/close key
 *  \return Return std::string::npos if no close key has been found for specified open key
 *  \return Return std::string::npos if no open key has been found for specified close key
 *
 *  Find the corresponding close (open) key for the open (close) key in position \a init.
 *  Return his position or std::string::npos if can't found that key.
 */
std::size_t closeKey(std::string& inS, std::size_t init);

/// Return true if input string is an statment (if,else,for...)
bool checkStatment(std::string& string);

/** \brief Find specified name in a strings array.
 *
 *  \param sarray -> Array of strings.
 *  \param dim -> \a sarray dimension.
 *  \param name -> string to find.
 *
 *  \return Return the position of element in \a sarray that match with \a name.
 *  \return Return -1 if no element match with \a name. 
 */
int nameMatch(std::string* sarray, int dim, const char* name);

/** \brief Find next called function.
 *
 *  \param inS -> Input code string.
 *  \param init -> Initial position.
 *  \param name -> Found function name.
 *  \param begin -> function beginning.
 *
 *  \return Parameter \a begin will be filled with the position of
 *          the first identifier name character of the extracted function.
 *  \return Parameter \a name will be filled with the found function name.
 *  \return Return the end function position on success (position of closing ')')
 *  \return Return std::string::npos if the function has not a close key ')'
 *  \return Return std::string::npos if not found any function.
 *
 *  This function find next function in \a inS starting from \a init position.
 *  Extract the first character position of function identifier and store it
 *  in \a begin and return the corresponding function close key ')'. On no
 *  function found or no close key found, the function return a std::string::npos.
 *
 */
std::size_t extractFunction(std::string& inS, std::size_t init, std::string& name, std::size_t& begin);

/** \brief Extract all called functions in the specified defined function.
 *
 *  \param inS -> Input code in plain text
 *  \param fcalls -> Pointer of #fcall elements that will store function calls.
 *  \param fname  -> Defined function name.
 *  \param overload -> Overload value for defined function to extract.
 *
 *  \return Return -1 if founction not found.
 *  \return Return -2 if no marks exist in \a inS.
 *  \return Return number of extracted functions on success.
 *
 *  Extracts function names of all called functions by function
 *  \a fname with specified \a overload. \a inS must be a cleared
 *  and marked source code. Use functions #clearCode and #definedFunc
 *  to ensure that. \a fcalls will be allocated to store data.
 *
 */
int calledFunc(std::string& inS, fcall*& fcalls, const char* fname, int overload = 1);

/** \brief Extract all called functions in the specified defined function.
 *
 *  \param inS -> Input code in plain text
 *  \param pnames -> Pointer that will store called function names
 *  \param counts -> Pointer that will store the number of times each function has been called.
 *  \param fname  -> Defined function name.
 *  \param overload -> Overload value for defined function to extract.
 *
 *  \return Return -1 if founction not found.
 *  \return Return -2 if no marks exist in \a inS.
 *  \return Return number of extracted functions on success.
 *
 *  Extracts function names of all called functions by function
 *  \a fname with specified \a overload. \a inS must be a cleared
 *  and marked source code. Use functions #clearCode and #definedFunc
 *  to ensure that. \a pnames and \a counts will be allocated to store
 *  data.
 *
 */
int calledFunc(std::string& inS, std::string*& pnames, int*& counts, const char* fname, int overload = 1);

/** \brief Find the position of specified defined function.  
 *
 *  \param inS -> Input code in plain text.
 *  \param name -> Defined function name.
 *  \param overload -> Overload value of defined function.
 *
 *  \return Return position of character '{' that marks the 
 *          beginning of function definition.
 *  \return Return std::string::npos on function not found.
 *
 *  Search the definition of function specified by \a name and \a overload
 *  and return the position where definition begin (on character '{').
 *  \a inS must be a cleared and marked source code. 
 *  Use functions #clearCode and #definedFunc to ensure that.
 *
 */
std::size_t posDefinedFunc(std::string& inS, const char* name, int overload = 1);

/** \brief Extract used namespace names in defined function.  
 *
 *  \param inS -> Input code in plain text.
 *  \param pnames -> Pointer that will store used namespace names.
 *  \param fname -> Defined function name.
 *  \param overload -> Overload value of defined function.
 *
 *  \return Return number of extracted namespaces on success.
 *  \return Return -1 if function not found.
 *  \return Return -2 if no marks exist in \a inS.
 *
 *  Store the names of all namespaces used in the defined function
 *  specified by \a fname and \a overload. The extracted names will
 *  be stored in \a pnames. \a pnames will be allocated to store this data.
 *  \a inS must be a cleared and marked source code. 
 *  Use functions #clearCode and #definedFunc to ensure that.
 *
 */
int usedNamespaces(std::string& inS, std::string*& pnames, const char* fname, int overload = 1);

/** \brief Find next used namespace 
 *
 *  \param inS -> Input code in plain text.
 *  \param init -> Init position to search.
 *  \param name -> Extracted namespace name.
 *  \param begin -> Beginning position of namespace identifier.
 *
 *  \return \a begin parameter will be filled with the first position of namespace identifier. 
 *  \return Return the end of namespace position (position of '::' or ';' for "using namespace")
 *  \return Both, return and \a begin will be set to "std::string::npos" on no namespace found.
 *
 *  From position \a init in \a inS string, find the next use of a namespace. This use can
 *  be a "using namespace" statment or a "namespace_identifier::variable" form.
 */
std::size_t extractNamespace(std::string& inS, std::size_t init, std::string& name, std::size_t& begin);

/// Return true if string \a a is alphabetically previous to \a b and false otherwise.
bool stringOrder(const std::string& a, const std::string& b);

/// Return true if string \a a is alphabetically later to \a b and false otherwise.
bool stringOrderInv(const std::string& a, const std::string& b);

/** \brief Extract a "dot" format file from input dependence tree
 *  
 *  \param tree -> Array with interconnected leafs (#leaf)
 *  \param nleafs -> Number of leafs in \a tree.
 *  \param filename -> Output file filename.
 *  \param nprop -> Number of "dot" properties
 *  \param prop -> Array of strings where each element is a dot property.
 *
 *  \return Return 0 on success.
 *  \return Return -1 if can't open output file.
 *
 *  Create a file with <a href="https://en.wikipedia.org/wiki/DOT_(graph_description_language)">dot</a> format from input interconnected \a tree of #leaf elements. Properties specified will
 *  be appended at the beginning of the dot graph.
 */
int tree2dot(leaf* tree, int nleafs, const char* filename, unsigned int nprop = 0, const char** prop = NULL);

#endif
