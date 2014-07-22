void compHwEmu(){

  TFile* inFile = new TFile("l1t.root");

  TH1D* rawEt = (TH1D*)inFile->Get("rawPlots/jet/et");
  TH1D* emuEt = (TH1D*)inFile->Get("simPlots/jet/et");
  TH1D* rawEta = (TH1D*)inFile->Get("rawPlots/jet/eta");
  TH1D* emuEta = (TH1D*)inFile->Get("simPlots/jet/eta");
  TH1D* rawPhi = (TH1D*)inFile->Get("rawPlots/jet/phi");
  TH1D* emuPhi = (TH1D*)inFile->Get("simPlots/jet/phi");

  TCanvas* c1 = new TCanvas("c1","");

  c1->cd();

  rawEt->GetXaxis()->SetTitle("jet Et");
  emuEt->Draw("p");
  rawEt->Draw("same");

  c1->SaveAs("jetEt.png");
  
  rawEta->GetXaxis()->SetTitle("jet iEta");
  emuEta->Draw("p");
  rawEta->Draw("same");
  c1->SaveAs("jetEta.png");

  rawPhi->GetXaxis()->SetTitle("jet iPhi");
  emuPhi->Draw("p");
  rawPhi->Draw("same");
  c1->SaveAs("jetPhi.png");
}
