void Overlap()
{
	gStyle->SetOptStat(0);
	TCanvas *C= new TCanvas("All", "All", 800,800);
	C->SetLogy(1);
	//C->SetLogx(1);
    //TPad *pad = new TPad("pad","pad",0.03,0.62,0.50,0.92,32);

   //  pad->cd();
    //pad->SetLogy();
   // pad->SetGridy();
   // gPad->SetLogx(1);


	TFile f1("myfiles_mc_1_k40_avg_1.root");
	TFile f2("myfiles_mc_1_k40_avg_2.root");
	TFile f3("myfiles_mc_1_k40_avg_3.root");
	TFile f4("myfiles_mc_1_k40_avg_4.root");
	//TFile f5("WaterProtonDecay_Exwater.root");
	//TFile f6("WaterProtonDecay.root");
	//TFile f7("WaterTl_AV.root");
	//TFile f8("WaterTl_Exwater.root");
	//TFile f9("WaterTl_PMT.root");
	//TFile f10("Rawdata.root");

    
	//TH1F *h1 = (TH1F*)f1.Get("qpdtProbability");
	TH1F *h1 = (TH1F*)f1.Get("avgcosthetanew");
	//h1->Sumw2();
	h1->Scale(1/h1->Integral());
	//h1->Rebin(6);
	//TH1F *h1 = (TH1F*)f1.Get("qpdtQMax");
	//TH1F *h2 = (TH1F*)f2.Get("qpdtProbability");
	TH1F *h2 = (TH1F*)f2.Get("avgcosthetanew");
	//h2->Sumw2();

	h2->Scale(1/h2->Integral());
	//h2->Rebin(6);
	//TH1F *h2 = (TH1F*)f2.Get("qpdtQMax");
    //TH1F *h3 = (TH1F*)f3.Get("qpdtProbability");
	TH1F *h3 = (TH1F*)f3.Get("avgcosthetanew");
	//h3->Sumw2();

   h3->Scale(1/h3->Integral());
	//h3->Rebin(6);
	//TH1F *h3 = (TH1F*)f3.Get("qpdtQMax");
	//TH1F *h4 = (TH1F*)f4.Get("qpdtProbability");
	TH1F *h4 = (TH1F*)f4.Get("avgcosthetanew");
	//h4->Sumw2();
	h4->Scale(1/h4->Integral());
	
	//h4->Rebin(6);
	//TH1F *h4 = (TH1F*)f4.Get("qpdtQMax");
	//TH1F *h5 = (TH1F*)f5.Get("qpdtProbability");
	//TH1F *h5 = (TH1F*)f5.Get("second");
	//TH1F *h5 = (TH1F*)f5.Get("qpdtQMax");
	//TH1F *h6 = (TH1F*)f6.Get("qpdtProbability");
	//TH1F *h6 = (TH1F*)f6.Get("second");
	//TH1F *h6 = (TH1F*)f6.Get("qpdtQMax");
	//TH1F *h7 = (TH1F*)f7.Get("qpdtProbability");
	//TH1F *h7 = (TH1F*)f7.Get("second");
	//TH1F *h7 = (TH1F*)f7.Get("qpdtQMax");
	//TH1F *h8 = (TH1F*)f8.Get("qpdtProbability");
	//TH1F *h8 = (TH1F*)f8.Get("second");
	//TH1F *h8 = (TH1F*)f8.Get("qpdtQMax");
	//TH1F *h9 = (TH1F*)f9.Get("qpdtProbability");
	//TH1F *h9 = (TH1F*)f9.Get("second");
	//TH1F *h9 = (TH1F*)f9.Get("qpdtQMax");
	//TH1F *h10 = (TH1F*)f10.Get("qpdtProbability");
	//TH1F *h10 = (TH1F*)f10.Get("qpdtProbability");
	//TH1F *h10 = (TH1F*)f10.Get("qpdtQMax");
   //h1->GetYaxis()->SetRangeUser(0.02,0.16);
	h1->Draw("hist");
	//h1->GetYaxis()->SetRangeUser(0.1,1.0);
	h1->GetYaxis()->SetRangeUser(0.01,1.5);

	//h1->GetXaxis()->SetLimits(0.1,1);

	h1->SetStats(0);
	h2->Draw("histsame");
	h2->SetStats(0);
	h3->Draw("histsame");
	h3->SetStats(0);
	h4->Draw("histsame");
	h4->SetStats(0);
	//h5->Draw("samehist");
	//h5->SetStats(0);
	//h6->Draw("samehist");
	//h6->SetStats(0);
	//h7->Draw("samehist");
//	h7->SetStats(0);
	//h8->Draw("samehist");
	//h8->SetStats(0);
	//h9->Draw("samehist");
	//h9->SetStats(0);
	//h10->Draw("same");
	//h10->SetStats(0);

	h1->GetXaxis()->SetTitle("Cos(#theta)");
	
   h1->SetLineColor(1);
   h2->SetLineColor(2);
   h3->SetLineColor(3);
   h4->SetLineColor(4);
  // h5->SetLineColor(5);
   //h6->SetLineColor(6);
  // h7->SetLineColor(7);
  //h8->SetLineColor(8);
  // h9->SetLineColor(46);
    //h10->SetLineColor(28);


   TLegend leg(0.2,0.8,0.4,0.9);
   // TLegend leg(0.3,0.7,0.7,0.9);
   leg.AddEntry(h1,"Earlynhit #geq 1");
   leg.AddEntry(h2,"Earlynhit #geq 2");
  leg.AddEntry(h3,"Earlynhit #geq 3");
   leg.AddEntry(h4,"Earlynhit #geq 4");
   //leg.AddEntry(h5,"WaterProtonDecay_Exwater");
   //leg.AddEntry(h6,"WaterProtonDecay");
  //leg.AddEntry(h7,"WaterTl_AV");
  //// leg.AddEntry(h8,"WaterTl_Exwater");
  // leg.AddEntry(h9,"WaterTl_PMT");
   //leg.AddEntry(h10,"Rawdata");


   //leg.AddEntry("nhitscleaned","nhits with flag fitValid");
   leg.Draw();
   C->Update();
	
	
   C->SaveAs("avgcosthetanew_K40_log.pdf");
   C->SaveAs("Overlap_nrmlised_newlog.root");

}
