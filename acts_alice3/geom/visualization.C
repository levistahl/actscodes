#include <TGeoManager.h>
#include <TGeoVolume.h>
#include <TEveManager.h> // Include TEveManager for loading geometry
#include <TGeoNode.h>
#include <TObject.h>
#include "TSeqCollection.h"
#include <TEveGeoNode.h>

#include <mutex>
#include <thread>
#include <map>
 
#include "TNamed.h"
#include "TObjArray.h"
#include "TGeoNavigator.h"


void visualization() {
  // Create an event viewer manager 
  TEveManager* gEve = TEveManager::Create(); // Might be required depending on ROOT version

  // Load the geometry from the ROOT file
  TGeoManager* gGeoManager = gEve->GetGeometry("o2sim_geometry_alice3.root");

  // Get the top-level volume 
  TGeoVolume* topVolume = gGeoManager->GetTopVolume();
  topVolume->SetVisibility(kTRUE);
  

  auto listadenodes = topVolume->GetNodes();
  
   
  Int_t numerodenodes = listadenodes->GetEntries();//create a list nodes in the form of TObjects 
  int i = 0;
  while (i < numerodenodes) { //loop for getting all nodes inside the volume
    std::string varName = "var" + std::to_string(i); //creates a variable that its name changes in each loop
    std::string detName = "det" + std::to_string(i);
    TObject* var = listadenodes->At(i); // extract the node in the TObject form
   
    TGeoNode* node = dynamic_cast<TGeoNode*>(var); //converts the TObject in TGeoNode
    const char* name = node->GetName(); //get the name of the node

    auto det = new TEveGeoTopNode(gGeoManager,node);

     //extract from geometry file the node corresponding to the name we got
    det->UseNodeTrans();
    gEve->AddGlobalElement(det);

    i++;
  
  }
  TEveViewer *ev = gEve->GetDefaultViewer();
  TGLViewer  *gv = ev->GetGLViewer();
  gv->SetGuideState(TGLUtil::kAxesOrigin, kTRUE, kFALSE, nullptr);
  gEve->FullRedraw3D(kTRUE);

}

