#include <iostream>
#include <fstream>
#include <string>
#include <igl/readOBJ.h>
#include <igl/writeOBJ.h>
#include <igl/writeSTL.h>
#include <igl/opengl/glfw/Viewer.h>


using namespace std;

int main (int argc, char *argv[])
{
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    // Load a mesh in OFF format
    igl::readOBJ(argv[1], V, F);
    int j = 0;
    Eigen::MatrixXi sF;
    std::fstream myfile("face.txt", std::ios_base::in);

    int a;
    while (myfile >> a)
    {
        std::cout<<a<<",";
        //sF.row(j)<<F.row(a) ;
        j++;
    }
    myfile.close(); //closing the file    
    
   igl::opengl::glfw::Viewer viewer;
   viewer.data().set_mesh(V, F);
   viewer.launch();

}
