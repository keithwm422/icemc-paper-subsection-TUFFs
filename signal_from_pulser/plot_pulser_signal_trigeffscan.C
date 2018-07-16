// This will plot the signal from the pulser used in the trigger efficiency scan of ANITA-4
#include <iostream>
using namespace std;

void plot_pulser_signal_trigeffscan()
{

//
  TCanvas *c1 = new TCanvas ("C1","c1", 800,800);
  TFile * _file0= TFile::Open("data/TriggerEfficiencyScanPulser_anita4_33dB_avg_trimmed.root");
// get the graph
  _file0->cd();
  TGraph *g = (TGraph*) _file0->Get("Phisector_3_33dBCh1_trimmed");
// change to nanoseconds from seconds
    Int_t nPoints  = g->GetN();
    Double_t *newx = g->GetX();
    Double_t *newy = g->GetY();
    for (int i=0;i<nPoints;i++){
    // change time axis s to ns
      newx[i]=newx[i]*1E9;
    }
      *g = TGraph(nPoints,newx,newy);
// end change to ns

  g->GetXaxis()->SetTitle("Time [ns]");
//  g->GetYaxis()->SetTitle("Magnitude");
  g->SetTitle("Signal from pulser");
  gStyle->SetOptStat(0);
  g->Draw("Al");
  c1->Draw();
  TString filename = Form("pulser_signal_for_trigeffscan.png");
  c1->SaveAs(filename);


}
