
/* 
   This example uses "graphviz" lib to
   plot the created dot graph
 */

#include <stdio.h>
#include <ctype.h>
#include <string>

#include <graphviz/gvc.h>
#include "../../src/codeMap.h"

int main (int argc, char** argv)
{
  if(argc < 2)
    {
      printf("Usage: %s path/to/file \n",argv[0]);
      return 0;
    }

  source_map map;

  //Append all specified source files
  for(int i = 1; i < argc; i++)
    {
      printf("Appending source %s...\n",argv[i]);
      fflush(stdout);
      int err = map.appendSource(argv[i]);
      if(err != 0)
	{
	  printf("Error (%d) appending source %s.",err,argv[i]);
	  return -1;
	}
      printf("Done!\n");
      fflush(stdout);
    }

  //Sort map alphabeticaly
  printf("Sorting...\n");
  map.sort();
  printf("Done!\n");
  
  //print map
  FILE* fmap = fopen("map.dep","w");
  fprintf(fmap,"%s\n",map.to_string().c_str());
  fclose(fmap);

  //Create tree
  printf("Creating tree...\n");
  leaf* tree = 0;
  int nleafs = 0;
  map.tree(nleafs, tree);

  printf("Done!\n");
  printf("Number of leafs: %d\n",nleafs);

  //Print tree in "dot" format
  printf("Print dot format...\n");
  unsigned int nprop = 1;
  const char* prop[] = {"fixedsize=\"false\""};
  tree2dot(tree, nleafs, "dependencies.dot", nprop, prop); //+1 to avoid head node
  printf("Done!\n");

  
  //*************//
  //* Plot tree *//
  //*************//

  printf("Plot tree graph...\n");
  GVC_t* gvc = 0;
  graph_t* g = 0;

  int argc2    = 5;
  char* argv2[] = {"dot","-Tps","dependencies.dot","-o","dependencies.ps"};
  
  gvc = gvContext();

  gvParseArgs(gvc, argc2, argv2);
  
  g = gvNextInputGraph(gvc);
  
  gvLayoutJobs(gvc, g);
  gvRenderJobs(gvc, g);

  gvFreeContext(gvc);

  printf("Done!\n");
  
  return 0;
}
