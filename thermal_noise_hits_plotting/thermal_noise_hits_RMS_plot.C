// This will calculate the RMS for each channel from outputs of icemc with thermal noise hits and then plot all of those RMS's in a histogram.
// Files needed include the data in the root file "rmsRun200_TUFF_260_375_0.root" and 2 SimulatedAnitaEventFile.roots (one with thermal noise factor at 0.8 and one at 0.9). In these simulations, should use the same configuration (B). 
// input file for these simulations should have Min Bias:ON, Skip Neutrinos:ON, Thermal Noise Factor: 0.8 or 0.9, Zero Signal:ON, TUFFs:ON, CW:OFF, and then hard code the tuff configuration in balloon.cc to always return 1, or tuffIndex=1 s.t. configuration B is only always choice. 
#include <iostream>
#include <fstream>
using namespace std;

void thermal_noise_hits_RMS_plot()
{
  cout << "starting up" << endl;
  // opening file to write histograms to for the data on thermal noise hits
  TFile * _file_out = new TFile("thermal_noise_hits_RMS_hists.root","RECREATE");
  TCanvas *c1 = new TCanvas ("C1","c1", 800,800);
  TFile * _file2= TFile::Open("/home/keith/anitaBuildTool/components/anitaMagicDisplay/macros/thermal_noise/rmsRun200_TUFF_260_375_0.root");
  _file2->cd();
  TString histname = Form("ant");
  // histogram setup
  TH1D *h = new TH1D ("RMS","RMS from data", 200, 0.0, 100.0);
  h->SetMarkerStyle(34);
  h->SetLineWidth(3);
  h->SetLineColor(1);
  TH1D *h_1= 0;
  // for loop to combine the histograms together
  for(int i=0;i<96;i++){
    histname = Form("ant%d" , i);
    _file2->GetObject(histname, h_1);
    h->Add(h_1);
  } // end for loop over i for adding histograms together
  // edit the histogram to be normalized
  double norm_1 = 1;
  double scale_1 = norm_1/(h->Integral());
  h->Scale(scale_1);
//  h->Draw("HIST");
//  c1->Draw();
//  TString filename = Form("RMS_andrew_sum.png");
//  c1->SaveAs(filename);


// end the data section

// begin the simulated thermal noise hits section
  // set up histograms and labels
  TH1D *h_mean = new TH1D("MEAN","fVolts Mean simulation 0.8", 50, -20., 20.);
  TH1D *h_rms = new TH1D("RMS_0.8","fVolts RMS simulation 0.8", 60, 0.0, 30.);
  TH1D *h_rms_1 = new TH1D("RMS","fVolts RMS Histogram (Normalized)", 60, 0.0, 30.);
  TH1D *h_std = new TH1D("Standard Deviation","fVolts Standard Deviation simulation 0.8", 50, -20., 20.);
  h_mean->SetMarkerStyle(34);
  h_mean->SetLineWidth(3);
  h_mean->SetLineColor(1);
  h_std->SetMarkerStyle(34);
  h_std->SetLineWidth(3);
  h_std->SetLineColor(3);
  h_rms->SetMarkerStyle(34);
  h_rms->SetLineWidth(3);
  h_rms->SetLineColor(2);
  h_rms_1->SetMarkerStyle(34);
  h_rms_1->SetLineWidth(3);
  h_rms_1->SetLineColor(4);
   // open simulated thermal noise hits files
  TFile * _file0= TFile::Open("/home/keith/anitaBuildTool/components/anitaMagicDisplay/macros/thermal_noise/run3_tuffB_8/SimulatedAnitaEventFile3.root");
  TFile * _file1= TFile::Open("/home/keith/anitaBuildTool/components/anitaMagicDisplay/macros/thermal_noise/run3_tuffB_9/SimulatedAnitaEventFile3.root");
// make trees for accessing the events
  _file0->cd();
  TTree *t_A=(TTree*)_file0->Get("eventTree");
  _file1->cd();
  TTree *t_B=(TTree*)_file1->Get("eventTree");
  //define arrays to hold the events fVolts info.
  double fVolts[108][260];
  double fVolts_1[108][260];
  double fVolts_only_antennas[96][260]={0};
  double fVolts_only_antennas_1[96][260]={0};
// access the events
  UsefulAnitaEvent* event= NULL;
  UsefulAnitaEvent* event_1= NULL;
  t_A->SetBranchAddress("event",&event);
  t_B->SetBranchAddress("event",&event_1);

// not needed but extra //  t_A->SetBranchAddress("fVolts[108][260]",&fVolts);

// loop over all events
    for(int i=0;i<41;i++){
    t_A->GetEntry(i);
    t_B->GetEntry(i);

    cout << "got event " << i << endl;
    // set new arrays to events' arrays of fVolts
    for(int k=0;k<108;k++){
      for(int j=0; j<260;j++){
        fVolts[k][j] = event->fVolts[k][j];
        fVolts_1[k][j] = event_1->fVolts[k][j];
      } // end j
    }// end k

// make a new array of the simulated thermal noise hits that do not include the 0's from the channels that are not antennas.
  int channel_index=0;
  for(int l=0;l<108;l++){
    //if l is not any of these channels then we want to make a copy of it and increment the channel index
    if(l!=8 && l!=17 && l!=26 && l!=35 && l!=44 && l!=53 && l!=62 && l!=71 && l!=80 && l!=89 && l!=98 && l!=107){
      for(int m=0; m<260;m++){
        fVolts_only_antennas[channel_index][m]=fVolts[l][m];
        fVolts_only_antennas_1[channel_index][m]=fVolts_1[l][m];
      }//end m
        channel_index++;
    }//end if
    else continue;
  }//end l
  cout << "Channel index should be 95 and it is " << channel_index << endl;

  double fVolts_mean[96];
  double fVolts_sum =0.0;
  double fVolts_sum_1 =0.0;
// calc Mean
    for(int l=0;l<96;l++){
      fVolts_sum=0.0;
      for (int j=0;j<260;j++){
        fVolts_sum+=fVolts_only_antennas[l][j];
      }
      fVolts_mean[l]=fVolts_sum/260.0;
      //cout << "channel " << l << " has mean " << fVolts_sum/260.0 << endl;
    }
// Calc std
  double fVolts_std[96];

    for(int i=0; i<96; i++){
      fVolts_sum=0.0;
      for(int k=0;k<260;k++){
        fVolts_sum += (fVolts_only_antennas[i][k]-fVolts_mean[i])*(fVolts_only_antennas[i][k]-fVolts_mean[i]);
      }
      fVolts_std[i]=sqrt(fVolts_sum/260.0);
 //     cout << "channel " << i << " has std " << sqrt(fVolts_sum/260.0) << endl;
    }

// Calc RMS
  double fVolts_rms[96];
  double fVolts_rms_1[96];

    for(int i=0; i<96; i++){
      fVolts_sum=0.0;
      fVolts_sum_1=0.0;
      for(int k=0;k<260;k++){
        fVolts_sum += (fVolts_only_antennas[i][k])*(fVolts_only_antennas[i][k]);
        fVolts_sum_1 += (fVolts_only_antennas_1[i][k])*(fVolts_only_antennas_1[i][k]);

      }
      fVolts_rms[i]=sqrt(fVolts_sum/260.0);
      fVolts_rms_1[i]=sqrt(fVolts_sum_1/260.0);
    }

// fill histogram

    for(int k=0;k<96;k++){
      h_mean->Fill(fVolts_mean[k]);
      h_std->Fill(fVolts_std[k]);
      h_rms->Fill(fVolts_rms[k]);
      h_rms_1->Fill(fVolts_rms_1[k]);
 //     if(i==0){
   //     h_rms_1->Fill(fVolts_rms[k]);
  //    }

    }
    cout << "rms is " << fVolts_rms[0] << endl;
}// end if i for events

  double norm = 1;
  double scale = norm/(h_mean->Integral());
  h_mean->Scale(scale);
  scale = norm/(h_std->Integral());
  h_std->Scale(scale);
  scale = norm/(h_rms->Integral());
  h_rms->Scale(scale);
  scale = norm/(h_rms_1->Integral());
  h_rms_1->Scale(scale);


  h_mean->Draw("HIST");
  c1->Draw();
  TString filename = Form("RMS_fVolts_mean_only_antennas.png");
  c1->SaveAs(filename);

  // write out the histograms to rootfile
  _file_out->cd();
  h->Write("Data");
  h_rms_1->Write("Sim8");
  h_rms->Write("Sim9");
  gStyle->SetOptStat(0);
  h_rms_1->SetAxisRange(0.0, 0.16, "Y");
  h_rms_1->Draw("HIST");
  h_rms->Draw("sameHIST");
  h->Draw("sameHIST");

  leg = new TLegend(0.7,0.5,0.9,0.7);

  leg->AddEntry(h,"data");
  leg->AddEntry(h_rms,"Simulation 0.8");
  leg->AddEntry(h_rms_1,"Simulation 0.9");
  leg->Draw("same");

  c1->Draw();
  filename = Form("RMS_fVolts_rms_only_antennas.png");
  c1->SaveAs(filename);


  h_std->Draw("HIST");
  c1->Draw();
  filename = Form("RMS_fVolts_std_only_antennas.png");
  c1->SaveAs(filename);

  _file_out->Close();
  _file0->Close();
  _file1->Close();

}


