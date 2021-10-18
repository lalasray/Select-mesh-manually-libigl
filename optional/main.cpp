#include <iostream>
#include <fstream>
#include <string>
#include <igl/readOBJ.h>
#include <igl/writeOBJ.h>
#include <igl/writeSTL.h>
#include <igl/opengl/glfw/Viewer.h>


using namespace std;

Eigen::MatrixXd V, V1, V2;
Eigen::MatrixXi F, F1, F2;


bool key_down(igl::opengl::glfw::Viewer& viewer, unsigned char key, int modifier)
{
  std::cout<<"Key: "<<key<<" "<<(unsigned int)key<<std::endl;
  if (key == '1')
  {
    // Clear should be called before drawing the mesh
    viewer.data().clear();
    // Draw_mesh creates or updates the vertices and faces of the displayed mesh.
    // If a mesh is already displayed, draw_mesh returns an error if the given V and
    // F have size different than the current ones
    viewer.data().set_mesh(V, F);
    viewer.core().align_camera_center(V,F);
  }
  else if (key == '2')
  {
    viewer.data().clear();
    viewer.data().set_mesh(V1, F1);
  }
  else if (key == '3')
  {
    viewer.data().clear();
    viewer.data().set_mesh(V2, F2);

  }

  return false;
}

int main (int argc, char *argv[])
{
    using namespace Eigen;
    
    // Load a mesh in OFF format
    igl::readOBJ(argv[1], V, F);
    int j = 0;
    int i = 0;
    Eigen::MatrixXi sF = F;
    Eigen::MatrixXi uF = F;
    std::cout << "Original Faces:    " << std::endl << sF << std::endl;   
    std::fstream myfile("face.txt", std::ios_base::in);

    int a;
    while (myfile >> a)
    {
        //std::cout<<a<<",";
        sF.row(j)=F.row(a) ;
        unsigned int urowToRemove = a ;
        unsigned int unumRows = uF.rows()-1;
        unsigned int unumCols = uF.cols();
        if( urowToRemove < unumRows )
        uF.block(urowToRemove,0,unumRows-urowToRemove,unumCols) = uF.block(urowToRemove+1,0,unumRows-urowToRemove,unumCols);
        uF.conservativeResize(unumRows,unumCols);
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
   std::cout << "Unselected Faces:    " << std::endl << uF << std::endl;

  
   igl::writeOBJ("selectedmesh.obj", V, sF);
   igl::writeSTL("selectedmesh.stl", V, sF);
   igl::writeOBJ("Unselectedmesh.obj", V, uF);
   igl::writeSTL("Unselectedmesh.stl", V, uF);
   
   igl::readOBJ("selectedmesh.obj", V1, F1);
   igl::readOBJ("Unselectedmesh.obj", V2, F2);
   
   std::cout<<R"(
   1 Switch to Original mesh
   2 Switch to Selected mesh
   3 Switch to Unselected mesh
   )";
   igl::opengl::glfw::Viewer viewer;
   viewer.callback_key_down = &key_down;
   viewer.data().set_mesh(V, F);

   viewer.launch();
   
   
   

}
