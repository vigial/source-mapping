


/*
  Example of code to map 1
 */



void f1();  // { Comment 1 )(
void f2(int arg1, void* arg2); // } Comment 2 (
float f3(float arg1, int arg3, (float* f)(int arg1, int arg2)); // Comment 3
float f4(int arg1, int arg2); // { Comment 4


void f1 (){

  using namespace NS1;
  //"}" '}' ""
  
  const float b = 32.2;
  float a = pow(sqrt(23) + NS2::b,3);
  a = sqrt(a);
  //Auto call
  f1();
  
  f2();
  f3();
  f3();
  f3();

  f4();
  f1();  
}

void f2   (int arg1, void* arg2)

{
  using namespace NS2;
  
  const char string[] = "})(f1()";
  
  float a = sqrt  (arg1);

  int aInt = (int)a;
  int cInt = int(a*3);
  
  float b  = f3  (a, arg1, f(aInt, cInt));
  
}



float f3(float arg1, int arg3, (void* f)(int argf1, int argf2))
{
  /*
    s******************************************
    s******************************************
    s******************************************
    s******************************************
    s******************************************
    s******************************************
    s******************************************
    s******************************************
     Big comment f(f(2),3)
   */

  using namespace NS1;
  
  return f((int)arg1, arg3);
}

float f4(int arg1, int arg2){

  if(arg1 > arg2)
    {
      while(arg1 > arg2)
	{
	  arg1--;
	}
    }
  else if(arg1 < arg2)
    {
      do{
	arg1++;
      }while(arg1 < arg2)
    }
  else
    {
      arg1 = -arg2;
    }
  return (float)arg1;
}
