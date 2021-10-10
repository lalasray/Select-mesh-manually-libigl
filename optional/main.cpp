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
    Eigen::MatrixXi sF = F; 
    std::cout << "Original Faces:    " << std::endl << sF << std::endl;   
    std::fstream myfile("face.txt", std::ios_base::in);

    int a;
    while (myfile >> a)
    {
        //std::cout<<a<<",";
        sF.row(j)=F.row(a) ;
        j++;
    }
    myfile.close(); //closing the file    
   unsigned int rowToRemove = j ;
   unsigned int numRows = rowToRemove;
   unsigned int numCols = sF.cols();
   if( rowToRemove < numRows )
   sF.block(rowToRemove,0,numRows-rowToRemove,numCols) = sF.block(rowToRemove+1,0,numRows-rowToRemove,numCols);

    sF.conservativeResize(numRows,numCols);
    //For later version of eigen it can be done in a single step below but not sure if libigl supports later version of eigen
   //sF = sF(Eigen::seq(0, j), Eigen::placeholders::all);
   std::cout << "Selected Faces:    " << std::endl << sF << std::endl;
   igl::opengl::glfw::Viewer viewer;
   viewer.data().set_mesh(V, sF);
   viewer.launch();
   
   igl::writeOBJ("selectedmesh.obj", V, F);
   igl::writeSTL("selectedmesh.stl", V, F);
   
   
   

}
