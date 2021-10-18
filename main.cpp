#include <igl/readOBJ.h>
#include <igl/writeOBJ.h>
#include <igl/writeSTL.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/unproject_onto_mesh.h>
#include <iostream>
#include <fstream>  

Eigen::MatrixXd V1, V2;
Eigen::MatrixXi F1, F2;

bool key_down(igl::opengl::glfw::Viewer& viewer, unsigned char key, int modifier)
{
  std::cout<<"Key: "<<key<<" "<<(unsigned int)key<<std::endl;
  if (key == '1')
  {
    
    viewer.data().clear();
    viewer.data().set_mesh(V1, F1);
    viewer.core().align_camera_center(V1,F1);
  }
  else if (key == '2')
  {
    viewer.data().clear();
    viewer.data().set_mesh(V2, F2);
    viewer.core().align_camera_center(V2,F2);
   }

  return false;
}

void removeRow(Eigen::MatrixXi& matrix, unsigned int rowToRemove)
{
unsigned int numRows = matrix.rows()-1;
unsigned int numCols = matrix.cols();
if( rowToRemove < numRows )
matrix.block(rowToRemove,0,numRows-rowToRemove,numCols) = matrix.block(rowToRemove+1,0,numRows-rowToRemove,numCols);
matrix.conservativeResize(numRows,numCols);
}

int main(int argc, char *argv[])
{


  // Mesh with per-face color
  Eigen::MatrixXd V, C;
  Eigen::MatrixXi F;
  // Load a mesh in OBJ format
  igl::readOBJ(argv[1], V, F);
  
  int j = 0;
  int i = 0;
  Eigen::MatrixXi sF = F;
  Eigen::MatrixXi uF = F;
  
  std::cout << "Original Faces:    " << std::endl << sF << std::endl; 
/*
  Print the vertices and faces matrices
  std::cout << "Vertices: " << std::endl << V << std::endl;
  std::cout << "Faces:    " << std::endl << F << std::endl;
*/

  // Initialize white
  C = Eigen::MatrixXd::Constant(F.rows(),3,1);
  igl::opengl::glfw::Viewer viewer;
  viewer.callback_key_down = &key_down;
  viewer.callback_mouse_down =
    [&V,&F,&C, &sF, &uF, &j](igl::opengl::glfw::Viewer& viewer, int, int)->bool
  {
    int fid;
    Eigen::Vector3f bc;
    // Cast a ray in the view direction starting from the mouse position
    double x = viewer.current_mouse_x;
    double y = viewer.core().viewport(3) - viewer.current_mouse_y;
    if(igl::unproject_onto_mesh(Eigen::Vector2f(x,y), viewer.core().view,
      viewer.core().proj, viewer.core().viewport, V, F, fid, bc))
    {
      // paint hit red
      C.row(fid)<<1,0,0;
      viewer.data().set_colors(C);
      std::cout<<"Selected Face ID is :"<<fid<<std::endl;
      std::ofstream outfile ("face.txt", std::ios_base::app);
      outfile<<fid<<std::endl;
      outfile.close();
      std::cout<<"Vertices IDs are :"<<F.row(fid)<<std::endl;
      for (int i = 0; i < 3; i++)
      {
      std::cout<<"Cordinates of vertice "<<F(fid,i)<<" are :"<<V.row(F(fid,i))<<std::endl;
      }
      
      
      
      sF.row(j)=F.row(fid) ;
      j++;
      removeRow(uF,fid);
     for (int k = 0; k < (F.rows() -j) ; k++)
      { 
      unsigned int rowToRemove = sF.rows();
      unsigned int numRows = sF.rows()-1;
      unsigned int numCols = sF.cols();
      if( rowToRemove < numRows )
      sF.block(rowToRemove,0,numRows-rowToRemove,numCols) = sF.block(rowToRemove+1,0,numRows-rowToRemove,numCols);
      sF.conservativeResize(numRows,numCols); 

      }
      std::cout << "selected:    " << std::endl << sF << std::endl;
      std::cout << "Unselected:    " << std::endl << uF << std::endl;
      
      igl::writeOBJ("selectedmesh.obj", V, sF);
      igl::writeSTL("selectedmesh.stl", V, sF);
      igl::writeOBJ("Unselectedmesh.obj", V, uF);
      igl::writeSTL("Unselectedmesh.stl", V, uF);
   
      igl::readOBJ("selectedmesh.obj", V1, F1);
      igl::readOBJ("Unselectedmesh.obj", V2, F2);
   
    
      return true;
    }
    return false;
    
  };
  std::cout<<R"(Usage:[click]  Pick face on shape)";
  // Show mesh
  viewer.data().set_mesh(V, F);
  viewer.data().set_colors(C);
  viewer.data().show_lines = false;
  viewer.launch();
}
