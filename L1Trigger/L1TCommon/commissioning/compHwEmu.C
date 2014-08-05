void compHwEmu(){

  TFile* inFile = new TFile("l1t.root");

  TH1D* rawEt = (TH1D*)inFile->Get("rawPlots/jet/et");
  TH1D* emuEt = (TH1D*)inFile->Get("simPlots/jet/et");
  TH1D* rawEta = (TH1D*)inFile->Get("rawPlots/jet/eta");
  TH1D* emuEta = (TH1D*)inFile->Get("simPlots/jet/eta");
  TH1D* rawPhi = (TH1D*)inFile->Get("rawPlots/jet/phi");
  TH1D* emuPhi = (TH1D*)inFile->Get("simPlots/jet/phi");

  TCanvas* c1 = new TCanvas("c1","");

  TLegend* leg = new TLegend(0.6,0.6,0.8,0.8);
  leg->SetFillColor(0);
  leg->AddEntry(rawEt,"Firmware", "l");
  leg->AddEntry(emuEt,"Emulator", "p");

  c1->cd();

  rawEt->GetXaxis()->SetTitle("jet Et");
  emuEt->Draw("p");
  rawEt->Draw("same");
  leg->Draw();
  c1->SaveAs("jetEt.root");
  
  TLegend* leg = new TLegend(0.6,0.6,0.8,0.8);
  leg->SetFillColor(0);
  leg->AddEntry(rawEta,"Firmware", "l");
  leg->AddEntry(emuEta,"Emulator", "p");

  rawEta->GetXaxis()->SetTitle("jet iEta");
  emuEta->Draw("p");
  rawEta->Draw("same");
  leg->Draw();
  c1->SaveAs("jetEta.root");

  TLegend* leg = new TLegend(0.6,0.6,0.8,0.8);
  leg->SetFillColor(0);
  leg->AddEntry(rawPhi,"Firmware", "l");
  leg->AddEntry(emuPhi,"Emulator", "p");

  rawPhi->GetXaxis()->SetTitle("jet iPhi");
  emuPhi->Draw("p");
  rawPhi->Draw("same");
  leg->Draw();
  c1->SaveAs("jetPhi.root");
}
