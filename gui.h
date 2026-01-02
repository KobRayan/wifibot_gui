#pragma once
#include "wifibot.h"
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/frame.h>
#include <gtkmm/box.h>
#include <gtkmm/entry.h>
#include <gtkmm/table.h>
#include <gtkmm/label.h>
#include <iostream>
#include <sstream>
#include <iomanip>




class Gui : public Gtk::Window 
{
private:
	bool time_out();
	int a_x_cible;
    int b_y_cible;
    Gtk::Label m_label_dx;
    Gtk::Label m_label_dy;
    Gtk::Entry m_entry_dx;
    Gtk::Entry m_entry_dy;
    Gtk::Button m_bouton_go;
    Gtk::Frame* m_frame_position;
    Gtk::VBox*  m_vbox_position;
    Gtk::Frame* m_frame_data;
    Gtk::VBox*  m_vbox_data;

    Wifibot m_robot;
    Gtk::Button m_bouton_1;
    Gtk::Button m_bouton_up;
    Gtk::Button m_bouton_down;
    Gtk::Button m_bouton_stop;
    Gtk::Button m_bouton_left;
    Gtk::Button m_bouton_right;
    Gtk::Button m_bouton_rot_l;
    Gtk::Button m_bouton_rot_r;
    Gtk::Entry m_entree_1;
    Gtk::Label titre;
    Gtk::Label fin;

	// ********************************** DONNEES DES CAPTEURS
    Gtk::Label m_label_battery;
    Gtk::Entry m_entry_battery;
    
    Gtk::Label m_label_speed_g;
    Gtk::Entry m_entry_speed_g;
    Gtk::Label m_label_speed_d;
    Gtk::Entry m_entry_speed_d;
    Gtk::Label m_label_p1;
    Gtk::Entry m_entry_p1;
    Gtk::Label m_label_p2;
    Gtk::Entry m_entry_p2;
    /*Gtk::Label m_label_p3;
    Gtk::Entry m_entry_p3;
    Gtk::Label m_label_p4;
    Gtk::Entry m_entry_p4;*/
    Gtk::Label m_label_odo_g;
    Gtk::Entry m_entry_odo_g;
    Gtk::Label m_label_odo_d;
    Gtk::Entry m_entry_odo_d;

    // Vitesses, X, Y, THETA, VITESSE ANGULAIRE
    Gtk::Label m_label_v_g;
    Gtk::Entry m_entry_v_g;
    Gtk::Label m_label_v_d;
    Gtk::Entry  m_entry_v_d;
    Gtk::Label m_label_omega;
    Gtk::Entry m_entry_omega;
    Gtk::Label m_label_vitesse;
    Gtk::Entry  m_entry_vitesse;
    Gtk::Label m_label_x;
    Gtk::Entry m_entry_x;
    Gtk::Label m_label_y;
    Gtk::Entry  m_entry_y;
    Gtk::Label m_label_theta;
    Gtk::Entry  m_entry_theta;

   // Gtk::Label m_label_indicateur;
   // Gtk::Entry  m_entry_indicateur;
   std::string format_nb(float value);








    

public:
	Gui();
	~Gui();


};
