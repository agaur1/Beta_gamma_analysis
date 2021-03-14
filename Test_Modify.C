#include <RAT/DU/DSReader.hh>
#include <RAT/DU/Utility.hh>
#include <RAT/DS/Entry.hh>
#include <RAT/DS/EV.hh>
#include <RAT/DS/Run.hh>
#include <RAT/DS/UniversalTime.hh>
#include <RAT/SunUtil.hh>
#include <RAT/DataCleaningUtility.hh>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TSystemDirectory.h>
#include <TFile.h>
#include <TNtuple.h>
#include <TTree.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLegend.h>

// void  Test_Modify( const string& dirname, TString OutPutFileName = "test.root", std::string fitName = "" )
void  Test_Modify(TString dirname, TString OutPutFileName = "test.root", std::string fitName = "" )
{
    //cout<<"debug 1 " <<endl;
    // DEFINE OUTPUT ROOT FILE
    TFile *file = new TFile(OutPutFileName,"RECREATE");
    
    TCanvas *C = new TCanvas("c", "c",800,800);
    
    // DEFINE HITSTOGRAMS TO BE SAVED IN OUTPUT ROOT FILE
    TH1D* hHitTimeResiduals = new TH1D( "hHitTimeResidualsFit", "Hit time residuals using the Fit position", 1000, -500.0, 500.0 );
    TH1D *h_nhits = new TH1D("nhits","nhits",200,0,1000);
    TH1D *h_nhitscleaned = new TH1D("nhits_cleaned","nhits_cleaned",200,0,1000);
    // TH1D *h_nhitscleaned_scint = new TH1D("nhitscleaned_scint","nhitscleaned_scint",1000,0,1000);
    TH1D *h_costheta = new TH1D("costheta","costheta",50,-1,1);
    TH1D *h_dotvec = new TH1D("dotvec","dotvec",25,-1,1);
    TH1D *h_costheta_new_1 = new TH1D("costheta_new_1","costheta_new_1",25,-1,1);
    TH1D *h_avgcosthetanew_1 = new TH1D("avgcosthetanew_1","avgcosthetanew_1",25,-1,1);
    TH1D *h_costheta_new_2 = new TH1D("costheta_new_2","costheta_new_2",25,-1,1);
    TH1D *h_avgcosthetanew_2 = new TH1D("avgcosthetanew_2","avgcosthetanew_2",25,-1,1);
     TH1D *h_costheta_new_3 = new TH1D("costheta_new_3","costheta_new_3",25,-1,1);
    TH1D *h_avgcosthetanew_3 = new TH1D("avgcosthetanew_3","avgcosthetanew_3",25,-1,1);
    TH1D *h_costheta_new_4 = new TH1D("costheta_new_4","costheta_new_4",25,-1,1);
    TH1D *h_avgcosthetanew_4 = new TH1D("avgcosthetanew_4","avgcosthetanew_4",25,-1,1);

    //h_avgcosthetanew->Sumw2();
    TH1D *h_nhits_early = new TH1D("nhits_early","nhits_early",200,0,200);
    TH2D *h_theta_phi = new TH2D("h_theta_phi","theta-phi",200,-1,1,628,-3.14,3.14);
    TH2D *h_theta_phi_mc = new TH2D("h_theta_phi_mc","theta-phi_mc",200,-1,1,628,-3.14,3.14);
    TH2D *h_theta_nhit = new TH2D("h_theta_nhit","nhits_cleaned",25,-1,1,200,0,1000);
    
    TH2D *h_theta_phi_int = new TH2D("h_theta_phi_int","theta-phi_int",200,-1,1,628,-3.14,3.14);
    TH1D *h_theta_Ang_diif= new TH1D("costheta_Ang","costheta_Ang",50,0,3.14);
    TH1D *h_theta_Ang_diif_2= new TH1D("costheta_Ang_2","costheta_Ang_2",50,-1,1);
    TH2D *h_timeresidual_costheta = new TH2D("timeresidual_costheta","timeresidual_costheta",25,-1,1,100, -20.0, -2);
    

    
    int count_nhits_early=0;
    int count_costhetawhole_1 = 0;
    int countcostheta_GT0P7_1=0;
    int count_costhetanew_1=0;
    int countcostheta_new_GT0P7_1=0;
    int count_costhetawhole_2 = 0;
    int countcostheta_GT0P7_2=0;
    int count_costhetanew_2=0;
    int countcostheta_new_GT0P7_2=0;
    int count_costhetawhole_3 = 0;
    int countcostheta_GT0P7_3=0;
    int count_costhetanew_3;
    int countcostheta_new_GT0P7_3=0;
    int count_costhetawhole_4 = 0;
    int countcostheta_GT0P7_4=0;
    int count_costhetanew_4=0;
    int countcostheta_new_GT0P7_4=0;
    //double Total_event_triggered=0;
    //double Total_event_triggered_0=0;
    //double Total_event_triggered_1=0;
    //double Total_event_triggered_2=0;
    int nEvent_PassedTrigger = 0;
    vector<int> pmtID;  // vector to store PMT ID
    vector<int> GTID;   // What is GTID?
    vector<double> timeResV;  // vector to store time resolution

    //cout<<"debug 3 " <<endl;[]

    // 1. Need name of directory
    // 2. Get all root files inside the directory.
    // 3. Put main part of current code inside the for loop of all root files.

   const char *ext=".root";
   int TotalNumberOfRootFiles = 0;
   // TString dirname="/rat/Anaylysis/Partialfill/Ds_Data/PMT_BACKGROUND/";
   TSystemDirectory dir(dirname, dirname);
   TList *files = dir.GetListOfFiles(); 
   std::vector<string> ListOfAllRootFiles;
   if (files) 
   {
      TSystemFile *file;
      TString fname;
      TIter next(files);
      while ((file=(TSystemFile*)next())) 
      {
         fname = file->GetName();
         // std::cout << "file name: " << fname << std::endl;
         if (!file->IsDirectory() && fname.EndsWith(ext)) 
         {
            std::cout << "file name(inside if) " << fname << std::endl;
            ++TotalNumberOfRootFiles;
            ListOfAllRootFiles.push_back((string)(dirname+"/"+fname));
         }
     }
   }

   if (ListOfAllRootFiles.size()==0)
   {
        std::cout << "No root ffiles inside the current directory..." << std::endl;
        exit(0);
   }

    for (int RootFilesCount = 0; RootFilesCount < ListOfAllRootFiles.size(); ++RootFilesCount)
    {
        // std::cout << "\n\n============================================" << std::endl;
        std::cout<<"Reading input file: " << RootFilesCount << "\t" << ListOfAllRootFiles[RootFilesCount] << std::endl;
    }
        // If this is being done on data that does not require remote database connection
        // eg.: a simple simulation with default run number (0)
        // We can disable the remote connections:
        //
        // NOTE: Don't do this if you are using real data!!!
        // RAT::DB::Get()->SetAirplaneModeStatus(true);
        //cout<<"debug 4 " <<endl;
        RAT::DU::DSReader dsReader( ListOfAllRootFiles );
        
        // RAT::DU::Utility::Get()->GetLightPathCalculator() must be called *after* the RAT::DU::DSReader constructor.
        //cout<<"debug 5 " <<endl;
        RAT::DU::LightPathCalculator lightPath = RAT::DU::Utility::Get()->GetLightPathCalculator(); // To calculate the light's path
        //cout<<"debug 6 " <<endl;
        const RAT::DU::GroupVelocity& groupVelocity = RAT::DU::Utility::Get()->GetGroupVelocity(); // To get the group velocity
        //cout<<"debug 7 " <<endl;
        const RAT::DU::PMTInfo& pmtInfo = RAT::DU::Utility::Get()->GetPMTInfo(); // The PMT positions etc..
        
        cout<<"Total Entries (dsReader.GetEntryCount()) = " << dsReader.GetEntryCount()<<endl;

    for( size_t iEntry = 0; iEntry < dsReader.GetEntryCount(); iEntry++ )
    //for( size_t iEntry = 0; iEntry < 50; iEntry++ )
    {
        const RAT::DS::Entry& rDS = dsReader.GetEntry( iEntry );
        const TVector3 eventPosition = rDS.GetMC().GetMCParticle(0).GetPosition();
        TVector3 eventPosition_2;
        
        // clear vector that should be empty before reading each event
        pmtID.clear();
        GTID.clear();
        timeResV.clear();
        
        // cout<<eventPosition.x()<<eventPosition.y()<<eventPosition.z()<<endl;
        // h_nhits->Fill(rDS.evs.GetNhits());
        
        // Go and read further if and only if it passes the offline trigger.
        // When rDS.GetEVCount() == 0 this means it does not passed the trigger.
        //      rDS.GetEVCount() == 1 this means it passed the trigger.
        //      rDS.GetEVCount() == 2 this is some special case. For now we are not studying this.
        if (rDS.GetEVCount() != 1) continue;
        nEvent_PassedTrigger++;
        
        //std::cout << "[DEBUG] rDS.GetEVCount() = " << rDS.GetEVCount() << "\tiEntry = " << iEntry << std::endl;

        
        //h_nhits->Fill(rDS.evs.GetNhits());
        
        const RAT::DS::EV& rEV = rDS.GetEV( 0 );
        GTID.push_back(rEV.GetGTID());
        // cout<<"GTID ="<<rEV.GetGTID()<<endl;
        
        // grab the fit information
        // if(fitName == "")
        //    {
        //fitName = rEV.GetDefaultFitName();
        //fitName = rEV.GetFitNames();
        //  fitName2 = rEV.GetFitNames()[0];
        
        // cout<<"11111111"<<fitName<<endl;
        // cout << "Ankit: " << rEV.GetFitNames()[0] << endl;
        //  }
        fitName= "partialFitter";
        //TVector3 eventPosition_2;
        double eventTime;
        
        try{
            const RAT::DS::FitVertex& rVertex = rEV.GetFitResult(fitName).GetVertex(0);
            if(!(rVertex.ValidPosition() && rVertex.ValidTime()))
                continue; // fit invalid
            
            eventPosition_2 = rVertex.GetPosition();
            //eventPosition_2.SetZ(eventPosition_2.Z()-131);
            //  if(!( eventPosition_2.Mag()<5700 && eventPosition_2.Z()>0))continue;
            //cout<<eventPosition_2.x()<<eventPosition_2.y()<<eventPosition_2.z()<<endl;
            eventTime = rVertex.GetTime();
            //cout<<"Ankit"<<endl;
            // calculate time residuals
            const RAT::DS::CalPMTs& calibratedPMTs = rEV.GetCalPMTs();
            //if(rEV.GetNhitsCleaned()<90)
            h_nhits->Fill(rEV.GetNhits());
            h_nhitscleaned->Fill(rEV.GetNhitsCleaned());
            // if(rEV.GetNhitsCleaned()>300)
            // h_nhitscleaned_scint->Fill(rEV.GetNhitsCleaned());
            
            // HERE, I AM RESETTING IT TO ZERO AS (I THINK) WE NEED THIS
            //       NUMBER PER EVENT NOT FOR TOTAL EVENT.
            count_nhits_early = 0;
            for( size_t iPMT = 0; iPMT < calibratedPMTs.GetCount(); iPMT++ )
            {
                const RAT::DS::PMTCal& pmtCal = calibratedPMTs.GetPMT( iPMT );
                lightPath.CalcByPosition( eventPosition_2, pmtInfo.GetPosition( pmtCal.GetID() ) );
                TVector3 pmtCoord = pmtInfo.GetPosition( pmtCal.GetID() ); // ==>> added by sumit
                
                double distInInnerAV = lightPath.GetDistInInnerAV();
                double distInAV = lightPath.GetDistInAV();
                double distInWater = lightPath.GetDistInWater();
                const double transitTime = groupVelocity.CalcByDistance( distInInnerAV, distInAV, distInWater ); // Assumes a 400nm photon
                
                // Time residuals estimate the photon emission time relative to the event start so subtract off the transit time and eventTime
                double timeResidual = pmtCal.GetTime() - transitTime - eventTime;
                //if(timeResidual<-100 || timeResidual>-20)continue
                if(timeResidual>= -50 && timeResidual<= -20)
                {
                    count_nhits_early++;
                    //cout<<"iEntry " << iEntry<<"\tcount_nhits_early " << count_nhits_early<< endl;
                    pmtID.push_back(pmtCal.GetID());
                    timeResV.push_back(timeResidual);
                   // h_nhits_early->Fill(count_nhits_early);
                }
                hHitTimeResiduals->Fill( timeResidual); // Fill time resolution per PMT
                //h_nhits_early->Fill(count_nhits_early); 
                //h_nhits_early->Fill(count_nhits_early);
                //if(pmtID.size()>=1)
                //h_timeresidual_costheta->Fill(pmtCoord.Unit().Dot(eventPosition_2.Unit()), timeResidual);
                h_theta_nhit->Fill(pmtCoord.Unit().Dot(eventPosition_2.Unit()), rEV.GetNhitsCleaned()); // Save theta for each hit
                //cout<<timeResidual<<endl;
            }
            h_nhits_early->Fill(count_nhits_early);
        }
        catch(exception& e)
        //{std::cout<<e.what()<<"wrong fit"<<std::endl;}
        {}
        //  catch(const RAT::DS::FitCollection::NoResultError&){
        // no fit result by the name of fitName
        //     continue;
        // }
        // catch (const RAT::DS::FitResult::NoVertexError&){
        //    // no fit vertex
        //    continue;
        //  }
        // catch(const RAT::DS::FitVertex::NoValueError&){
        //    // position or time missing
        //     continue;
        // }
        // DataNotFound --> implies no fit results are present, don't catch.
        // }
        //cout<<"GTID ="<<GTID.size()<<endl;
        if(pmtID.size()==1)
        {
            TVector3 Xdiff=eventPosition_2-eventPosition;
            TVector3 Xdiff_u = Xdiff.Unit();
            
            // cout<<"event number = "<<iEntry<<endl;
            // cout<<eventPosition.x()<<"  , "<<eventPosition.y()<<"  , "<<eventPosition.z()<<endl;
            //cout<<eventPosition_2.x()<<"  , "<<eventPosition_2.y()<<"  , "<<eventPosition_2.z()<<endl;
            //const RAT::DS::EV& rEV = rDS.GetEV( iEV );
            //cout<<"GTID ="<<GTID.at(0)<<endl;
            // cout<<"cos_theta="<<cos(eventPosition.Theta())<<" ,phi = "<<eventPosition.Phi()<<endl;
            h_theta_phi_mc->Fill(cos(eventPosition.Theta()),eventPosition.Phi());
            // cout<<"cos_theta="<<cos(eventPosition_2.Theta())<<" ,phi = "<<eventPosition_2.Phi()<<endl;
            h_theta_phi_int->Fill(cos(eventPosition_2.Theta()),eventPosition_2.Phi());
            
            double sum_costheta_1=0;
           // double Total = 0;
            for (int i=0; i<pmtID.size(); i++)
            {
                //cout<<"pmt ID: "<<pmtID.at(i)<<endl;
                TVector3 pmtCoord = pmtInfo.GetPosition( pmtID.at(i) );
                TVector3 XdiffPMT = pmtCoord-eventPosition_2;
                TVector3 XdiffPMT_u= XdiffPMT.Unit();
                TVector3 XdiffPMT_cos= pmtCoord-eventPosition;
                TVector3 XdiffPMT_cos_u= XdiffPMT_cos.Unit();
               double costheta_1= pmtCoord.Unit().Dot(eventPosition_2.Unit());;
                
                sum_costheta_1+= costheta_1;
                
                h_costheta->Fill(Xdiff_u.Dot(XdiffPMT_u));
                h_dotvec->Fill(XdiffPMT_cos_u.Dot(XdiffPMT_u));
                
                h_costheta_new_1->Fill(costheta_1);
                count_costhetanew_1++;
                if(pmtCoord.Unit().Dot(eventPosition_2.Unit())>0.7)
                    countcostheta_new_GT0P7_1++;
                
                h_timeresidual_costheta->Fill(pmtCoord.Unit().Dot(eventPosition_2.Unit()), timeResV.at(i));
                
                double Ang_diif_1= fabs((pmtCoord.Theta())- (eventPosition.Theta()));
                double Ang_diif_2= cos(pmtCoord.Theta())- cos(eventPosition_2.Theta());
                //double Ang_diif_1= (pmtCoord.CosTheta())- (eventPosition.CosTheta());
                //cout<<"cos_theta="<<cos(pmtCoord.Theta())<<" ,phi = "<<pmtCoord.Phi()<<endl;
                //cout<<"x="<<pmtCoord.x()<<" ,y= "<<pmtCoord.y()<<" ,z= "<<pmtCoord.z()<<endl;
                h_theta_phi->Fill(cos(pmtCoord.Theta()),pmtCoord.Phi());
                h_theta_Ang_diif->Fill(Ang_diif_1);
                h_theta_Ang_diif_2->Fill(Ang_diif_2);
            }
            
            //  if(avg_costheta/pmtID.size()>0.7)
            h_avgcosthetanew_1->Fill(sum_costheta_1/pmtID.size());
            count_costhetawhole_1++;
            if(sum_costheta_1/pmtID.size()>0.7)
                countcostheta_GT0P7_1++;
            //break;
        }
    //}
    //std::cout << "Number of events that passed offline trigger = " << nEvent_PassedTrigger << std::endl;
    //cout << "Total_event_triggered_0 = " << Total_event_triggered_0 << endl;
    //cout << "Total_event_triggered_1 = " << Total_event_triggered_1 << endl;
    //cout << "Total_event_triggered_2 = " << Total_event_triggered_2 << endl;
    //cout <<"Total Event Count = " <<  count_costhetanew_1<< "\t, Total event (costheta > 0.7) = " << countcostheta_new_GT0P7_1 << endl;
    //cout << "Tagging efficiency = " << (countcostheta_new_GT0P7/count_costhetanew)*100.0 << endl;
   // cout <<"Total Event Count = " <<  count_costhetawhole_1<< "\t, Total event (costheta > 0.7) = " << countcostheta_GT0P7_1 << endl;
   // cout << "Tagging efficiency = " << (countcostheta_GT0P7_1/count_costhetawhole_1)*100.0 << endl;
    

    //*************************************************************
    if(pmtID.size()==2)
        {
            TVector3 Xdiff=eventPosition_2-eventPosition;
            TVector3 Xdiff_u = Xdiff.Unit();
            
            // cout<<"event number = "<<iEntry<<endl;
            // cout<<eventPosition.x()<<"  , "<<eventPosition.y()<<"  , "<<eventPosition.z()<<endl;
            //cout<<eventPosition_2.x()<<"  , "<<eventPosition_2.y()<<"  , "<<eventPosition_2.z()<<endl;
            //const RAT::DS::EV& rEV = rDS.GetEV( iEV );
            //cout<<"GTID ="<<GTID.at(0)<<endl;
            // cout<<"cos_theta="<<cos(eventPosition.Theta())<<" ,phi = "<<eventPosition.Phi()<<endl;
            h_theta_phi_mc->Fill(cos(eventPosition.Theta()),eventPosition.Phi());
            // cout<<"cos_theta="<<cos(eventPosition_2.Theta())<<" ,phi = "<<eventPosition_2.Phi()<<endl;
            h_theta_phi_int->Fill(cos(eventPosition_2.Theta()),eventPosition_2.Phi());
            
            double sum_costheta_2=0;
           // double Total = 0;
            for (int i=0; i<pmtID.size(); i++)
            {
                //cout<<"pmt ID: "<<pmtID.at(i)<<endl;
                TVector3 pmtCoord = pmtInfo.GetPosition( pmtID.at(i) );
                TVector3 XdiffPMT = pmtCoord-eventPosition_2;
                TVector3 XdiffPMT_u= XdiffPMT.Unit();
                TVector3 XdiffPMT_cos= pmtCoord-eventPosition;
                TVector3 XdiffPMT_cos_u= XdiffPMT_cos.Unit();
               double costheta_2= pmtCoord.Unit().Dot(eventPosition_2.Unit());;
                
                sum_costheta_2+= costheta_2;
                
                h_costheta->Fill(Xdiff_u.Dot(XdiffPMT_u));
                h_dotvec->Fill(XdiffPMT_cos_u.Dot(XdiffPMT_u));
                
                h_costheta_new_2->Fill(costheta_2);
                count_costhetanew_2++;
                if(pmtCoord.Unit().Dot(eventPosition_2.Unit())>0.7)
                    countcostheta_new_GT0P7_2++;
                
                h_timeresidual_costheta->Fill(pmtCoord.Unit().Dot(eventPosition_2.Unit()), timeResV.at(i));
                
                double Ang_diif_1= fabs((pmtCoord.Theta())- (eventPosition.Theta()));
                double Ang_diif_2= cos(pmtCoord.Theta())- cos(eventPosition_2.Theta());
                //double Ang_diif_1= (pmtCoord.CosTheta())- (eventPosition.CosTheta());
                //cout<<"cos_theta="<<cos(pmtCoord.Theta())<<" ,phi = "<<pmtCoord.Phi()<<endl;
                //cout<<"x="<<pmtCoord.x()<<" ,y= "<<pmtCoord.y()<<" ,z= "<<pmtCoord.z()<<endl;
                h_theta_phi->Fill(cos(pmtCoord.Theta()),pmtCoord.Phi());
                h_theta_Ang_diif->Fill(Ang_diif_1);
                h_theta_Ang_diif_2->Fill(Ang_diif_2);
            }
            
            //  if(avg_costheta/pmtID.size()>0.7)
            h_avgcosthetanew_2->Fill(sum_costheta_2/pmtID.size());
            count_costhetawhole_2++;
            if(sum_costheta_2/pmtID.size()>0.7)
                countcostheta_GT0P7_2++;
            //break;
        }
        //*************************************************************
    if(pmtID.size()==3)
        {
            TVector3 Xdiff=eventPosition_2-eventPosition;
            TVector3 Xdiff_u = Xdiff.Unit();
            
            // cout<<"event number = "<<iEntry<<endl;
            // cout<<eventPosition.x()<<"  , "<<eventPosition.y()<<"  , "<<eventPosition.z()<<endl;
            //cout<<eventPosition_2.x()<<"  , "<<eventPosition_2.y()<<"  , "<<eventPosition_2.z()<<endl;
            //const RAT::DS::EV& rEV = rDS.GetEV( iEV );
            //cout<<"GTID ="<<GTID.at(0)<<endl;
            // cout<<"cos_theta="<<cos(eventPosition.Theta())<<" ,phi = "<<eventPosition.Phi()<<endl;
            h_theta_phi_mc->Fill(cos(eventPosition.Theta()),eventPosition.Phi());
            // cout<<"cos_theta="<<cos(eventPosition_2.Theta())<<" ,phi = "<<eventPosition_2.Phi()<<endl;
            h_theta_phi_int->Fill(cos(eventPosition_2.Theta()),eventPosition_2.Phi());
            
            double sum_costheta_3=0;
           // double Total = 0;
            for (int i=0; i<pmtID.size(); i++)
            {
                //cout<<"pmt ID: "<<pmtID.at(i)<<endl;
                TVector3 pmtCoord = pmtInfo.GetPosition( pmtID.at(i) );
                TVector3 XdiffPMT = pmtCoord-eventPosition_2;
                TVector3 XdiffPMT_u= XdiffPMT.Unit();
                TVector3 XdiffPMT_cos= pmtCoord-eventPosition;
                TVector3 XdiffPMT_cos_u= XdiffPMT_cos.Unit();
               double costheta_3= pmtCoord.Unit().Dot(eventPosition_2.Unit());;
                
                sum_costheta_3+= costheta_3;
                
                h_costheta->Fill(Xdiff_u.Dot(XdiffPMT_u));
                h_dotvec->Fill(XdiffPMT_cos_u.Dot(XdiffPMT_u));
                
                h_costheta_new_3->Fill(costheta_3);
                count_costhetanew_3++;
                if(pmtCoord.Unit().Dot(eventPosition_2.Unit())>0.7)
                    countcostheta_new_GT0P7_3++;
                
                h_timeresidual_costheta->Fill(pmtCoord.Unit().Dot(eventPosition_2.Unit()), timeResV.at(i));
                
                double Ang_diif_1= fabs((pmtCoord.Theta())- (eventPosition.Theta()));
                double Ang_diif_2= cos(pmtCoord.Theta())- cos(eventPosition_2.Theta());
                //double Ang_diif_1= (pmtCoord.CosTheta())- (eventPosition.CosTheta());
                //cout<<"cos_theta="<<cos(pmtCoord.Theta())<<" ,phi = "<<pmtCoord.Phi()<<endl;
                //cout<<"x="<<pmtCoord.x()<<" ,y= "<<pmtCoord.y()<<" ,z= "<<pmtCoord.z()<<endl;
                h_theta_phi->Fill(cos(pmtCoord.Theta()),pmtCoord.Phi());
                h_theta_Ang_diif->Fill(Ang_diif_1);
                h_theta_Ang_diif_2->Fill(Ang_diif_2);
            }
            
            //  if(avg_costheta/pmtID.size()>0.7)
            h_avgcosthetanew_3->Fill(sum_costheta_3/pmtID.size());
            count_costhetawhole_3++;
            if(sum_costheta_3/pmtID.size()>0.7)
                countcostheta_GT0P7_3++;
            //break;
        }
               //*************************************************************
    if(pmtID.size()==4)
        {
            TVector3 Xdiff=eventPosition_2-eventPosition;
            TVector3 Xdiff_u = Xdiff.Unit();
            
            // cout<<"event number = "<<iEntry<<endl;
            // cout<<eventPosition.x()<<"  , "<<eventPosition.y()<<"  , "<<eventPosition.z()<<endl;
            //cout<<eventPosition_2.x()<<"  , "<<eventPosition_2.y()<<"  , "<<eventPosition_2.z()<<endl;
            //const RAT::DS::EV& rEV = rDS.GetEV( iEV );
            //cout<<"GTID ="<<GTID.at(0)<<endl;
            // cout<<"cos_theta="<<cos(eventPosition.Theta())<<" ,phi = "<<eventPosition.Phi()<<endl;
            h_theta_phi_mc->Fill(cos(eventPosition.Theta()),eventPosition.Phi());
            // cout<<"cos_theta="<<cos(eventPosition_2.Theta())<<" ,phi = "<<eventPosition_2.Phi()<<endl;
            h_theta_phi_int->Fill(cos(eventPosition_2.Theta()),eventPosition_2.Phi());
            
            double sum_costheta_4=0;
           // double Total = 0;
            for (int i=0; i<pmtID.size(); i++)
            {
                //cout<<"pmt ID: "<<pmtID.at(i)<<endl;
                TVector3 pmtCoord = pmtInfo.GetPosition( pmtID.at(i) );
                TVector3 XdiffPMT = pmtCoord-eventPosition_2;
                TVector3 XdiffPMT_u= XdiffPMT.Unit();
                TVector3 XdiffPMT_cos= pmtCoord-eventPosition;
                TVector3 XdiffPMT_cos_u= XdiffPMT_cos.Unit();
               double costheta_4= pmtCoord.Unit().Dot(eventPosition_2.Unit());;
                
                sum_costheta_4+= costheta_4;
                
                h_costheta->Fill(Xdiff_u.Dot(XdiffPMT_u));
                h_dotvec->Fill(XdiffPMT_cos_u.Dot(XdiffPMT_u));
                
                h_costheta_new_4->Fill(costheta_4);
                count_costhetanew_4++;
                if(pmtCoord.Unit().Dot(eventPosition_2.Unit())>0.7)
                    countcostheta_new_GT0P7_4++;
                
                h_timeresidual_costheta->Fill(pmtCoord.Unit().Dot(eventPosition_2.Unit()), timeResV.at(i));
                
                double Ang_diif_1= fabs((pmtCoord.Theta())- (eventPosition.Theta()));
                double Ang_diif_2= cos(pmtCoord.Theta())- cos(eventPosition_2.Theta());
                //double Ang_diif_1= (pmtCoord.CosTheta())- (eventPosition.CosTheta());
                //cout<<"cos_theta="<<cos(pmtCoord.Theta())<<" ,phi = "<<pmtCoord.Phi()<<endl;
                //cout<<"x="<<pmtCoord.x()<<" ,y= "<<pmtCoord.y()<<" ,z= "<<pmtCoord.z()<<endl;
                h_theta_phi->Fill(cos(pmtCoord.Theta()),pmtCoord.Phi());
                h_theta_Ang_diif->Fill(Ang_diif_1);
                h_theta_Ang_diif_2->Fill(Ang_diif_2);
            }
            
            //  if(avg_costheta/pmtID.size()>0.7)
            h_avgcosthetanew_4->Fill(sum_costheta_4/pmtID.size());
            count_costhetawhole_4++;
            if(sum_costheta_4/pmtID.size()>0.7)
                countcostheta_GT0P7_4++;
            //break;
        }
    } // END: event for loop
    // }// END: Vector for loop end which loops over all .root files
   //std::cout << "Number of events that passed offline trigger = " << nEvent_PassedTrigger << std::endl;
    //cout << "Total_event_triggered_0 = " << Total_event_triggered_0 << endl;
    //cout << "Total_event_triggered_1 = " << Total_event_triggered_1 << endl;
    //cout << "Total_event_triggered_2 = " << Total_event_triggered_2 << endl;
    std::cout << "Number of events that passed offline trigger = " << nEvent_PassedTrigger << std::endl;
    //cout << "Total_event_triggered_0 = " << Total_event_triggered_0 << endl;
    //cout << "Total_event_triggered_1 = " << Total_event_triggered_1 << endl;
    //cout << "Total_event_triggered_2 = " << Total_event_triggered_2 << endl;
    cout << " \n Print info for nhits_early=1\n  " << endl;
    cout <<"Total Event_1 = " <<  count_costhetanew_1<< "\t, Total Event_1 (costheta > 0.7) = " << countcostheta_new_GT0P7_1 << endl;
    cout << "Tagging Efficiency_1 = " << (((double)countcostheta_new_GT0P7_1)/((double)count_costhetanew_1))*100.0 << endl;
    cout <<"Total Event_avg_1 = " <<  count_costhetawhole_1<< "\t, Total Event_avg_1 (costheta > 0.7) = " << countcostheta_GT0P7_1 << endl;
    cout << "Tagging Efficiency_avg_1 = " << (((double)countcostheta_GT0P7_1)/((double)count_costhetawhole_1))*100.0 << endl;

    cout << " \n Print info for nhits_early=2\n  " << endl;
   
    cout <<"Total Event_2 = " <<  count_costhetanew_2<< "\t, Total Event_2 (costheta > 0.7) = " << countcostheta_new_GT0P7_2 << endl;
    cout << "Tagging Efficiency_2 = " << (((double)countcostheta_new_GT0P7_2)/((double)count_costhetanew_2))*100.0 << endl;
    cout <<"Total Event_avg_2 = " <<  count_costhetawhole_2<< "\t, Total Event_avg_2 (costheta > 0.7) = " << countcostheta_GT0P7_2 << endl;
    cout << "Tagging Efficiency_avg_2 = " << (((double)countcostheta_GT0P7_2)/((double)count_costhetawhole_2))*100.0 << endl;
    cout << "   " << endl;
    
    cout << " \n Print info for nhits_early=3\n  " << endl;

    cout <<"Total Event_3 = " <<  count_costhetanew_3<< "\t, Total Event_3 (costheta > 0.7) = " << countcostheta_new_GT0P7_3 << endl;
    cout << "Tagging Efficiency_3 = " << (((double)countcostheta_new_GT0P7_3)/((double)count_costhetanew_3))*100.0 << endl;
    cout <<"Total Event_avg_3 = " <<  count_costhetawhole_3<< "\t, Total Event_avg_3 (costheta > 0.7) = " << countcostheta_GT0P7_3 << endl;
    cout << "Tagging Efficiency_avg_3 = " << (((double)countcostheta_GT0P7_3)/((double)count_costhetawhole_3))*100.0 << endl;
    cout << "   " << endl;
    
    cout << " \n Print info for nhits_early=4\n  " << endl;

    cout <<"Total Event_4 = " <<  count_costhetanew_4<< "\t, Total Event_4 (costheta > 0.7) = " << countcostheta_new_GT0P7_4 << endl;
    cout << "Tagging Efficiency_4 = " << (((double)countcostheta_new_GT0P7_4)/((double)count_costhetanew_4))*100.0 << endl;
    cout <<"Total Event_avg_4 = " <<  count_costhetawhole_4<< "\t, Total Event_avg_4 (costheta > 0.7) = " << countcostheta_GT0P7_4 << endl;
    cout << "Tagging Efficiency_avg_4 = " << (((double)countcostheta_GT0P7_4)/((double)count_costhetawhole_4))*100.0 << endl;
    //******************************************************************
    //hHitTimeResiduals->GetYaxis()->SetTitle( "Count per 1 ns bin" );
    //hHitTimeResiduals->GetXaxis()->SetTitle( "Hit time residuals [ns]" );
    hHitTimeResiduals->Draw();
    
    //std::cout<<count_nhits_early<<std::endl;
    file->cd();//
    hHitTimeResiduals->Write();
    //TLegend leg(0.2,0.8,0.4,0.9);
    //h_nhits->Draw();
    h_nhits->Draw("hist");
    h_nhitscleaned->Draw("hist sames");
    h_nhits->GetXaxis()->SetTitle("Nhits cleaned");
    h_nhits->SetLineColor(kBlue);
    h_nhitscleaned->SetLineColor(kOrange);
    TLegend* leg = new TLegend(0.2, 0.2, .8, .8);
    //TLegend leg(0.3,0.8,0.7,0.9);
    // TLegend leg;
    leg->AddEntry("nhits","nhits");
    leg->AddEntry("nhits_cleaned","nhitscleaned");
    leg->Draw();
    
    C->SetLogy();
    C->SetGrid();
    C->SaveAs("nhits_pmt.pdf");
    h_theta_phi->SetMarkerStyle(21);
    // leg.AddEntry(h_theta_phi,"Early light");
    h_theta_phi->SetMarkerSize(2);
    h_theta_phi->SetMarkerColor(7);
    
    h_theta_phi_mc->SetMarkerStyle(23);
    h_theta_phi_mc->SetMarkerSize(3);
    h_theta_phi_mc->SetMarkerColor(9);
    //leg.AddEntry(h_theta_phi_mc,"MC Injection");
    h_theta_phi_int->SetMarkerStyle(25);
    h_theta_phi_int->SetMarkerSize(4);
    h_theta_phi_int->SetMarkerColor(3);
    // leg.AddEntry(h_theta_phi_int,"Reconstructed");
    h_theta_phi->GetXaxis()->SetTitle("Cos_theta");
    h_theta_phi->GetYaxis()->SetTitle("phi");
    h_theta_phi->Write();
    file->Write();
    
}
