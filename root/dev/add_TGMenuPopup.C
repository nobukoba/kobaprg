void add_TGMenuPopup(){
  TGPopupMenu * popupmenu = ((TRootCanvas*)gPad->GetCanvas()->GetCanvasImp())->GetMenuBar()->GetPopup("File");
  TGMenuEntry * entry_pri     = popupmenu->GetEntry("Print...");
  popupmenu->AddEntry("Print in landscape...", 100, 0, 0, entry_pri);
  TGMenuEntry * entry_pri_por = popupmenu->GetEntry("Print in portrait...");
  Int_t id = entry_pri->GetEntryId();
  popupmenu->DeleteEntry(entry_pri);
  popupmenu->AddEntry("Print...", id, 0, 0, entry_pri_por);
  return;
}
