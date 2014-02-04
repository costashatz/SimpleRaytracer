#include "FreeImage.h"
#include <iostream>
#include "readfile.h"
#include <time.h>

using namespace std ; 

int main(int argc, char* argv[]) {
	if(argc < 2)
	{
		cout<<"Usage: Example scene_file"<<endl;
		return 1;
	}
	string file = string(argv[1]);
  	FreeImage_Initialise();
	srand((unsigned)time(0));
	//clock_t t = clock();
	timespec t, t2;
	clock_gettime(CLOCK_REALTIME, &t);
	Scene sc = Scene();
	readfile(file.c_str(),&sc);
	sc.render();
	clock_gettime(CLOCK_REALTIME, &t2);
	//t = clock()-t;
	cout<<"Done!\n";
	//cout<<"It took "<<(float(t)/CLOCKS_PER_SEC)<<" secs!\n";
	cout<<"It took "<<float(t2.tv_sec-t.tv_sec)<<" secs!\n";
	FreeImage_DeInitialise();
	return 0;
}
