// this code finds the values of the channle indices for fVOlts that give all zero in time to show they are the gps channels and not antennas

#include <iostream>
#include <fstream>
using namespace std;




void which_gps_chans()
{ 

  TFile * _file0= TFile::Open("/home/keith/anitaBuildTool/components/anitaMagicDisplay/macros/thermal_noise/run3_tuffB_8/SimulatedAnitaEventFile3.root");
  TFile * _file1= TFile::Open("/home/keith/anitaBuildTool/components/anitaMagicDisplay/macros/thermal_noise/run3_tuffB_9/SimulatedAnitaEventFile3.root");

  _file0->cd();
  TTree *t_A=(TTree*)_file0->Get("eventTree");
  _file1->cd();
  TTree *t_B=(TTree*)_file1->Get("eventTree");
  cout << " got here " << endl;
  double fVolts[108][260];
  double fVolts_1[108][260];

  UsefulAnitaEvent* event= NULL;
  UsefulAnitaEvent* event_1= NULL;
  t_A->SetBranchAddress("event",&event);
  t_B->SetBranchAddress("event",&event_1);
//  t_A->SetBranchAddress("fVolts[108][260]",&fVolts);
// loop over all events

    for(int i=0;i<41;i++){
    t_A->GetEntry(i);
    t_B->GetEntry(i);

    cout << "got here " << endl;

    for(int k=0;k<108;k++){
      for(int j=0; j<260;j++){
        fVolts[k][j] = event->fVolts[k][j];
        fVolts_1[k][j] = event_1->fVolts[k][j];
      }
      if(fVolts[k][10]==0) cout << k << " is a gps channel" << endl;
      if(fVolts_1[k][10]==0) cout << k << " is a gps channel" << endl;
    }
  }
  delete event;
  delete event_1;
  delete t_B;
  delete t_A;
  _file0->Close();
  _file1->Close();

}
