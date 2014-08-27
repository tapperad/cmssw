void compHwEmu(){

  TFile* inFile = new TFile("l1t.root");

  TH1D* rawEt = (TH1D*)inFile->Get("rawPlots/jet/et");
  TH1D* emuEt = (TH1D*)inFile->Get("simPlots/jet/et");
  TH1D* rawEta = (TH1D*)inFile->Get("rawPlots/jet/eta");
  TH1D* emuEta = (TH1D*)inFile->Get("simPlots/jet/eta");
  TH1D* rawPhi = (TH1D*)inFile->Get("rawPlots/jet/phi");
  TH1D* emuPhi = (TH1D*)inFile->Get("simPlots/jet/phi");

  TH1D* rawEtTot = (TH1D*)inFile->Get("rawPlots/sum/et");
  TH1D* emuEtTot = (TH1D*)inFile->Get("simPlots/sum/et");
  TH1D* rawEtxTot = (TH1D*)inFile->Get("rawPlots/sum/et1");
  TH1D* emuEtxTot = (TH1D*)inFile->Get("simPlots/sum/et1");
  TH1D* rawEtyTot = (TH1D*)inFile->Get("rawPlots/sum/et2");
  TH1D* emuEtyTot = (TH1D*)inFile->Get("simPlots/sum/et2");

  TH1D* rawHtTot = (TH1D*)inFile->Get("rawPlots/sum/ht");
  TH1D* emuHtTot = (TH1D*)inFile->Get("simPlots/sum/ht");

  TH1D* rawMetPhi = (TH1D*)inFile->Get("rawPlots/sum/metphi");
  TH1D* emuMetPhi = (TH1D*)inFile->Get("simPlots/sum/metphi");
  TH1D* rawMet = (TH1D*)inFile->Get("rawPlots/sum/met");
  TH1D* emuMet = (TH1D*)inFile->Get("simPlots/sum/met");

  TCanvas* c1 = new TCanvas("c1","");

  TLegend* leg = new TLegend(0.6,0.6,0.8,0.8);
  leg->SetFillColor(0);
  leg->AddEntry(rawEt,"Firmware", "l");
  leg->AddEntry(emuEt,"Emulator", "p");

  c1->cd();

  emuEt->GetXaxis()->SetTitle("jet Et");
  emuEt->Draw("p");
  rawEt->Draw("same");
  leg->Draw();
  c1->SaveAs("jetEt.root");
  
  TLegend* leg = new TLegend(0.6,0.6,0.8,0.8);
  leg->SetFillColor(0);
  leg->AddEntry(rawEta,"Firmware", "l");
  leg->AddEntry(emuEta,"Emulator", "p");

  emuEta->GetXaxis()->SetTitle("jet iEta");
  emuEta->Draw("p");
  rawEta->Draw("same");
  leg->Draw();
  c1->SaveAs("jetEta.root");

  TLegend* leg = new TLegend(0.6,0.6,0.8,0.8);
  leg->SetFillColor(0);
  leg->AddEntry(rawPhi,"Firmware", "l");
  leg->AddEntry(emuPhi,"Emulator", "p");

  emuPhi->GetXaxis()->SetTitle("jet iPhi");
  emuPhi->Draw("p");
  rawPhi->Draw("same");
  leg->Draw();
  c1->SaveAs("jetPhi.root");

  TLegend* leg = new TLegend(0.6,0.6,0.8,0.8);
  leg->SetFillColor(0);
  leg->AddEntry(rawEtTot,"Firmware", "l");
  leg->AddEntry(emuEtTot,"Emulator", "p");

  c1->cd();

  emuEtTot->GetXaxis()->SetTitle("Total Et");
  emuEtTot->Draw("p");
  rawEtTot->Draw("same");
  leg->Draw();
  c1->SaveAs("etTot.root");

  TLegend* leg = new TLegend(0.6,0.6,0.8,0.8);
  leg->SetFillColor(0);
  leg->AddEntry(rawEtxTot,"Firmware", "l");
  leg->AddEntry(emuEtxTot,"Emulator", "p");

  c1->cd();

  emuEtxTot->GetXaxis()->SetTitle("Met X");
  emuEtxTot->Draw("p");
  rawEtxTot->Draw("same");
  leg->Draw();
  c1->SaveAs("etXTot.root");

  TLegend* leg = new TLegend(0.6,0.6,0.8,0.8);
  leg->SetFillColor(0);
  leg->AddEntry(rawEtyTot,"Firmware", "l");
  leg->AddEntry(emuEtyTot,"Emulator", "p");

  c1->cd();

  emuEtyTot->GetXaxis()->SetTitle("Met Y");
  emuEtyTot->Draw("p");
  rawEtyTot->Draw("same");
  leg->Draw();
  c1->SaveAs("etYTot.root");

  TLegend* leg = new TLegend(0.6,0.6,0.8,0.8);
  leg->SetFillColor(0);
  leg->AddEntry(rawHtTot,"Firmware", "l");
  leg->AddEntry(emuHtTot,"Emulator", "p");

  c1->cd();

  emuHtTot->GetXaxis()->SetTitle("Total Ht");
  emuHtTot->Draw("p");
  rawHtTot->Draw("same");
  leg->Draw();
  c1->SaveAs("htTot.root");

  TLegend* leg = new TLegend(0.6,0.6,0.8,0.8);
  leg->SetFillColor(0);
  leg->AddEntry(rawMet,"Firmware", "l");
  leg->AddEntry(emuMet,"Emulator", "p");

  c1->cd();

  emuMet->GetXaxis()->SetTitle("Met");
  emuMet->Draw("p");
  rawMet->Draw("same");
  leg->Draw();
  c1->SaveAs("met.root");

  TLegend* leg = new TLegend(0.6,0.6,0.8,0.8);
  leg->SetFillColor(0);
  leg->AddEntry(rawMetPhi,"Firmware", "l");
  leg->AddEntry(emuMetPhi,"Emulator", "p");

  c1->cd();

  emuMetPhi->GetXaxis()->SetTitle("Met Phi");
  emuMetPhi->Draw("p");
  rawMetPhi->Draw("same");
  leg->Draw();
  c1->SaveAs("metPhi.root");

}
