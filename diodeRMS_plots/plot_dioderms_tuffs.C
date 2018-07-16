// This will plot the dioderms per channel for all configs of Tuffs 
#include <iostream>
using namespace std;

void plot_dioderms_tuffs()
{
/*
  double x[96];
  for (int i =0; i<96; i++){
    x[i]=(double)(i);
  }
*/
// declare the variable arrays for the didoerms in the branches



  double diode_all[672];
//
  TCanvas *c1 = new TCanvas ("C1","c1", 800,800);
  TFile * _file0= TFile::Open("/home/keith/icemc_branching/icemc/run3/SimulatedAnitaTruthFile3.root");
// get the tree
  _file0->cd();
  TTree *t_B=(TTree*)_file0->Get("triggerSettingsTree");
//Set Branch Address and fill the diode array
  t_B->SetBranchAddress("dioderms",&diode_all);
//  t_B->SetBranchAddress("dioderms",&diode_B);
//  t_C->SetBranchAddress("dioderms",&diode_C);
//  t_G->SetBranchAddress("dioderms",&diode_G);
//  t_O->SetBranchAddress("dioderms",&diode_O);
//  t_P->SetBranchAddress("dioderms",&diode_P);

// make hist for each configuration
TH1F *hA = new TH1F("diodermsA", "diodermsA", 25, 0, 1e-12);
TH1F *hB = new TH1F("diodermsB", "diodermsB", 25, 0, 1e-12);
TH1F *hC = new TH1F("diodermsC", "diodermsC", 25, 0, 1e-12);
TH1F *hG = new TH1F("diodermsG", "diodermsG", 25, 0, 1e-12);
TH1F *hO = new TH1F("diodermsO", "diodermsO", 25, 0, 1e-12);
TH1F *hP = new TH1F("dioderms", "RMS of the diodes", 25, 0, 1e-12);

  hA->SetFillStyle(3003);
  hA->SetFillColor(kBlack);
  hA->SetLineColor(1);
  hB->SetFillStyle(3003);
  hB->SetFillColor(kRed);
  hB->SetLineColor(2);
  hC->SetFillStyle(3003);
  hC->SetFillColor(kGreen);
  hC->SetLineColor(3);
  hG->SetFillStyle(3003);
  hG->SetFillColor(kBlack);
  hG->SetLineColor(1);
  hO->SetFillStyle(3003);
  hO->SetFillColor(kBlue);
  hO->SetLineColor(4);
  hP->SetFillStyle(3003);
  hP->SetFillColor(kMagenta);
  hP->SetLineColor(6);
/*
  hA->SetLineColor(2);
  hB->SetLineColor(3);
  hC->SetLineColor(4);
  hG->SetLineColor(5);
  hO->SetLineColor(6);
  hP->SetLineColor(7);
*/


// fill the diode_all array with all the dioderms values
long int entries= t_B->GetEntries();
  for(int j =0; j<entries; j++){
    t_B->GetEntry(j);
//    t_A->GetEntry(j);
//    t_B->GetEntry(j);
//    t_C->GetEntry(j);
//    t_G->GetEntry(j);
//    t_O->GetEntry(j);
//    t_P->GetEntry(j);
  }
//

// fill histograms
  for(int i=0; i<96; i++){
    hA->Fill(diode_all[7*i]);
    hB->Fill(diode_all[(7*i)+1]);
    hC->Fill(diode_all[(7*i)+2]);
    hG->Fill(diode_all[(7*i)+3]);
    hO->Fill(diode_all[(7*i)+4]);
    hP->Fill(diode_all[(7*i)+5]);
  }


// edit order of drawing to make some of lines more visible, the hatches in fillStyle above help. Also these line widths might help too
  hP->SetLineWidth(2);
  hB->SetLineWidth(2);
  hC->SetLineWidth(2);
  hG->SetLineWidth(2);
  hO->SetLineWidth(2);
  hA->SetLineWidth(2);

// draw ABCP
  hP->Draw("");
  hC->Draw("Same");
//  hG->Draw("CSame");
//  hO->Draw("CSame");
  hB->Draw("Same");
  hA->Draw("Same");
  TLegend *leg = new TLegend(0.7,0.7,0.9,0.9);
  leg->SetHeader("Configuration Label","C");
  leg->AddEntry(hA,"A");
  leg->AddEntry(hB,"B");
  leg->AddEntry(hC,"C");
  leg->AddEntry(hP,"P");
  leg->Draw("SAME");
  gStyle->SetOptStat(0);
  c1->Draw();
  TString filename = Form("diode_rms_histogram_ABCP.png");
  c1->SaveAs(filename);

// Draw B,G, and O configurations because of the similarities
  hG->SetFillStyle(0);
  hB->SetFillStyle(0);
  hO->SetFillStyle(0);
  hG->Draw("C");
  hO->Draw("CSAME");
  hB->Draw("CSAME");
  TLegend *leg1 = new TLegend(0.7,0.7,0.9,0.9);
  leg1->SetHeader("Configuration Label","C");
  leg1->AddEntry(hB,"B");
  leg1->AddEntry(hG,"G");
  leg1->AddEntry(hO,"O");
  leg1->Draw("SAME");
  c1->Update();
  filename=Form("diode_rms_histogram_lines_BGO.png");
  c1->SaveAs(filename);
// maybe just normalize the histograms and zoom in on their peaks?
  double norm_1 = 1.0;
  double scale_1 = norm_1/(hA->Integral());
  hA->Scale(scale_1);
  scale_1= norm_1/(hB->Integral());
  hB->Scale(scale_1);
  scale_1= norm_1/(hC->Integral());
  hC->Scale(scale_1);
  scale_1= norm_1/(hG->Integral());
  hG->Scale(scale_1);
  scale_1= norm_1/(hO->Integral());
  hO->Scale(scale_1);
  scale_1= norm_1/(hP->Integral());
  hP->Scale(scale_1);
// thicker lines for clarity
  hP->SetLineWidth(2);
  hB->SetLineWidth(2);
  hC->SetLineWidth(2);
  hG->SetLineWidth(2);
  hO->SetLineWidth(2);
  hA->SetLineWidth(2);
// set fills again
  hG->SetFillStyle(3003);
  hB->SetFillStyle(3003);
  hO->SetFillStyle(3003);
// draw G line different color
  hG->SetLineColor(15);
  hG->SetFillColor(kGray);
  hP->Draw("HIST");
  hC->Draw("HISTSame");
  hG->Draw("HISTSame");
  hO->Draw("HISTSame");
  hA->Draw("HISTSame");
  hB->Draw("HISTSame");

/*  hA->Fit("gaus","","P");
  hB->Fit("gaus","","same");
  hC->Fit("gaus","","same");
  hG->Fit("gaus","","same");
  hO->Fit("gaus","","same");
  hP->Fit("gaus","","same");
*/
/*  hP->Draw();
  hB->Draw("Same");
  hC->Draw("Same");
  hG->Draw("Same");
  hO->Draw("Same");
  hA->Draw("Same");
*/
  leg->AddEntry(hG,"G");
  leg->AddEntry(hO,"O");

  leg->Draw("Same");
  filename=Form("diode_rms_normalized_histograms_ABCGOP.png");
  c1->Update();
  c1->SaveAs(filename);


}
