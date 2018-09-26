void select_counting_room_a_printer(){
  gEnv->SetValue("Print.Printer","RCC4000A");
  std::cout << "Selected printer: RCC4000A at Ring Counting Room" << std::endl;
  std::cout << "NB. This config is valid only in this settion." << std::endl;
  return;
}
