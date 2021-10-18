#include <igl/readOBJ.h>
#include <igl/writeOBJ.h>
#include <igl/writeSTL.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/unproject_onto_mesh.h>
#include <iostream>
#include <fstream>  

// Mesh and Vertices
Eigen::MatrixXd V1, V2;
Eigen::MatrixXi F1, F2;


//Function to change the visualized mesh on pressing button
bool key_down(igl::opengl::glfw::Viewer& viewer, unsigned char key, int modifier)
	
	{
	
	  std::cout<<"Key: "<<key<<" "<<(unsigned int)key<<std::endl;
	  if (key == '1')
	  
	  {
    	    
    	    //Clear the screen
	    viewer.data().clear();
	    //Visualize the mesh
	    viewer.data().set_mesh(V1, F1);
	    //Center the camera arounf the mesh
	    viewer.core().align_camera_center(V1,F1);
	  
	  }
	  else if (key == '2')
	  
	  {
	    
	    //Clear the screen
	    viewer.data().clear();
	    //Visualize the mesh
	    viewer.data().set_mesh(V2, F2);
	    //Center the camera arounf the mesh
	    viewer.core().align_camera_center(V2,F2);
	   
	   }

	  return false;
	
	}


//Function to remove a desired row from the eigen Face matrix
void removeRow(Eigen::MatrixXi& matrix, unsigned int rowToRemove)
	
	{
	
	//Resize after removing the row
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
 	  //Create a sF Matrix with size F but all elements as zero 
 	  Eigen::MatrixXi sF = Eigen::MatrixXi::Zero(F.rows(), F.cols());
 	  //Create a su Matrix with size F and all elements of F
 	  Eigen::MatrixXi uF = F;
 	  //Print the vertices and faces matrices
 	  /*
	  std::cout << "Vertices: " << std::endl << V << std::endl;
	  std::cout << "Faces:    " << std::endl << F << std::endl;
	  */
	  // Initialize white
	  C = Eigen::MatrixXd::Constant(F.rows(),3,1);
 	  //Call Viewer
 	  igl::opengl::glfw::Viewer viewer;
 	  //Enable Keyboard click event
 	  viewer.callback_key_down = &key_down;
 	  //Enable mouse click event
 	  viewer.callback_mouse_down = [&V,&F,&C, &sF, &uF, &j](igl::opengl::glfw::Viewer& viewer, int, int)->bool
 	  	
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
      					//Print face ID
      					std::cout<<"Selected Face ID is :"<<fid<<std::endl;
      					// Write to face ID to face.txt file
      					std::ofstream outfile ("face.txt", std::ios_base::app);
      					outfile<<fid<<std::endl;
      					outfile.close();
      					//Print the vertices associated with Face ID
      					std::cout<<"Vertices IDs are :"<<F.row(fid)<<std::endl;
      					//Print the coordinates of vertices associated with face ID
      					for (int i = 0; i < 3; i++)
      						
      						{
      							
      							std::cout<<"Cordinates of vertice "<<F(fid,i)<<" are :"<<V.row(F(fid,i))<<std::endl;
      						
      						}
      					
      					//Put the selected Face in the jth row of sF
      					sF.row(j)=F.row(fid) ;
      					j++;
      					//Replace the selected row value to 0,0,0 in uF
      					uF.row(fid)<<0,0,0;
      					//removeRow(uF,fid);
      					//Print Selected and Unselected Faces
      					/*
      					std::cout << "selected:    " << std::endl << sF << std::endl;
      					std::cout << "Unselected:    " << std::endl << uF << std::endl;
      					*/
      					//Create selected mesh object file
      					igl::writeOBJ("selectedmesh.obj", V, sF);
      					//Create selected mesh STL file
      					igl::writeSTL("selectedmesh.stl", V, sF);
      					//Create unselected mesh object file
      					igl::writeOBJ("Unselectedmesh.obj", V, uF);
      					//Create unselected mesh STL file
      					igl::writeSTL("Unselectedmesh.stl", V, uF);
      					
      					//Load selected mesh object file   
      					igl::readOBJ("selectedmesh.obj", V1, F1);
      					//Load unselected mesh object file   
      					igl::readOBJ("Unselectedmesh.obj", V2, F2);
   
    
      					return true;
    				
    				}
    				
    			return false;
    
  		};
  		
  		//Print Extra Info on terminal
  		std::cout<<R"(Usage:[click]  Pick face on shape)";
  		std::cout<<R"(Usage:[Keyboard-1]  Visualize selected faces)";
  		std::cout<<R"(Usage:[Keyboard-2]  Visualize unselected faces)";
  		// Visualize the default mesh
  		viewer.data().set_mesh(V, F);
  		viewer.data().set_colors(C);
  		//Dont show edge lines
  		viewer.data().show_lines = false;
  		viewer.launch();
  		
		}
