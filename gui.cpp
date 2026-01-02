#include "gui.h"
#include <glibmm/main.h>
//pour le formattage des nombres floatants : 
#include <sstream>
#include <iomanip>




Gui::Gui():
	m_robot (),
	m_bouton_1 ("Connexion"),
	m_bouton_up (),
	m_bouton_down (),
	m_bouton_stop (),
	m_bouton_left (),
	m_bouton_right (),
	m_bouton_rot_l (),
	m_bouton_rot_r (),
	m_entree_1 (),
	titre ("WIFIBOT CONTROL"),
	fin ("COPYRIGHT 2026")
	
	
{
	Gtk::HBox* m_hbox_1=Gtk::manage(new Gtk::HBox(false,20));
	Gtk::HBox* m_hbox_2=Gtk::manage(new Gtk::HBox(false,20));
	Gtk::HBox* m_hbox_debut=Gtk::manage(new Gtk::HBox(false,20));
	Gtk::HBox* m_hbox_fin=Gtk::manage(new Gtk::HBox(false,20));
	Gtk::VBox* m_vbox=Gtk::manage(new Gtk::VBox(false,30));
	Gtk::Table* tableau=Gtk::manage(new Gtk::Table(3,3));
	Gtk::Image* image_1=Gtk::manage(new Gtk::Image("images/robot.jpg"));
	Gtk::Image* avant=Gtk::manage(new Gtk::Image("images/icon_up.png"));
	Gtk::Image* arriere=Gtk::manage(new Gtk::Image("images/icon_down.png"));
	Gtk::Image* gauche=Gtk::manage(new Gtk::Image("images/icon_left.png"));
	Gtk::Image* droite=Gtk::manage(new Gtk::Image("images/icon_right.png"));
	Gtk::Image* rot_g=Gtk::manage(new Gtk::Image("images/icon_rot_left.png"));
	Gtk::Image* rot_d=Gtk::manage(new Gtk::Image("images/icon_rot_right.png"));
	Gtk::Image* stop=Gtk::manage(new Gtk::Image("images/stop.png"));
    
	
	m_bouton_up.set_image(*avant);
	m_bouton_down.set_image(*arriere);
	m_bouton_left.set_image(*gauche);
	m_bouton_right.set_image(*droite);
	m_bouton_rot_l.set_image(*rot_g);
	m_bouton_rot_r.set_image(*rot_d);
	m_bouton_stop.set_image(*stop);

	tableau->attach(m_bouton_up,1,2,0,1);
	tableau->attach(m_bouton_down,1,2,2,3);
	tableau->attach(m_bouton_left,0,1,1,2);
	tableau->attach(m_bouton_right,2,3,1,2);
	tableau->attach(m_bouton_rot_l,0,1,2,3);
	tableau->attach(m_bouton_rot_r,2,3,2,3);
	tableau->attach(m_bouton_stop,1,2,1,2);
	
	this->set_title("Wifibot - Audin/Kobrossly");
	this->set_position(Gtk::WIN_POS_CENTER);
	this->set_resizable(true);
    this->set_icon_from_file("images/robot_icon.jpg");


	m_entree_1.set_text("192.168.1.106");
	//m_entree_1.set_alignment(0.5);

	//m_box_1.pack_start(m_box_3);
	//m_box_1.pack_start(m_box_4);
	m_hbox_1->pack_start(m_entree_1);
	m_hbox_1->pack_start(m_bouton_1);
	

	m_hbox_debut->pack_start(titre);
	m_hbox_fin->pack_start(fin);
	m_vbox->set_spacing(10);
	m_vbox->add(*m_hbox_debut);
	m_vbox->add(*m_hbox_1);
	m_hbox_2->pack_start(*image_1);
	m_hbox_2->pack_start(*tableau);
	
	m_vbox->add(*m_hbox_2);
	
        // ===== Frame Position =====
        m_frame_position = Gtk::manage(new Gtk::Frame("Position"));
        m_vbox_position  = Gtk::manage(new Gtk::VBox(false, 10));

        m_frame_position->add(*m_vbox_position);
        // ===== Saisie déplacement X / Y =====
        Gtk::HBox* hbox_deplacement = Gtk::manage(new Gtk::HBox(false, 20));

        m_label_dx.set_text("Déplacement X (m) :");
        m_label_dy.set_text("Déplacement Y (m) :");

        m_entry_dx.set_text("0");
        m_entry_dy.set_text("0");

        m_bouton_go.set_label("Aller");

        hbox_deplacement->pack_start(m_label_dx, Gtk::PACK_SHRINK);
        hbox_deplacement->pack_start(m_entry_dx, Gtk::PACK_SHRINK);
        hbox_deplacement->pack_start(m_label_dy, Gtk::PACK_SHRINK);
        hbox_deplacement->pack_start(m_entry_dy, Gtk::PACK_SHRINK);
        hbox_deplacement->pack_start(m_bouton_go, Gtk::PACK_SHRINK);

        m_vbox_position->add(*hbox_deplacement);

        
        m_frame_position->set_shadow_type(Gtk::SHADOW_ETCHED_IN);
        m_frame_position->set_border_width(10);

        // ===== Frame Données Capteurs =====
        m_frame_data = Gtk::manage(new Gtk::Frame("Données Capteurs"));
        m_vbox_data  = Gtk::manage(new Gtk::VBox(false, 10));

        m_frame_data->add(*m_vbox_data);
        m_frame_data->set_shadow_type(Gtk::SHADOW_ETCHED_IN);
        m_frame_data->set_border_width(10);

        // Ajout des frames
        m_vbox->pack_start(*m_frame_position, Gtk::PACK_SHRINK);
        m_vbox->pack_start(*m_frame_data, Gtk::PACK_SHRINK);

	        

	Gtk::HBox* m_hbox_battery = Gtk::manage(new Gtk::HBox(false,20));
        m_label_battery.set_text("Batterie : ");
        m_entry_battery.set_editable(false);
        m_hbox_battery->pack_start(m_label_battery);
        m_hbox_battery->pack_start(m_entry_battery);
        m_vbox_data->add(*m_hbox_battery);
        
        Gtk::HBox* hbox_speed = Gtk::manage(new Gtk::HBox(false,20));
        m_label_speed_g.set_text("Vitesse Gauche :");
        m_label_speed_d.set_text("Vitesse Droite :");
        m_entry_speed_g.set_editable(false);
        m_entry_speed_d.set_editable(false);
        hbox_speed->pack_start(m_label_speed_g);
        hbox_speed->pack_start(m_entry_speed_g);
        hbox_speed->pack_start(m_label_speed_d);
        hbox_speed->pack_start(m_entry_speed_d);
        m_vbox_data->add(*hbox_speed);

        Gtk::HBox* hbox_ir = Gtk::manage(new Gtk::HBox(false,20));
        m_label_p1.set_text("Capteur IR gauche :");
        m_label_p2.set_text("Capteur IR droit  :");
      

        m_entry_p1.set_editable(false);
        m_entry_p2.set_editable(false);
       
        hbox_ir->pack_start(m_label_p1);
        hbox_ir->pack_start(m_entry_p1);
        hbox_ir->pack_start(m_label_p2);
        hbox_ir->pack_start(m_entry_p2);
     
        m_vbox_data->add(*hbox_ir);

        Gtk::HBox* hbox_odo = Gtk::manage(new Gtk::HBox(false,20));
        m_label_odo_g.set_text("Odométrie gauche :");
        m_label_odo_d.set_text("Odométrie droite :");
      

        m_entry_odo_g.set_editable(false);
        m_entry_odo_d.set_editable(false);


        hbox_odo->pack_start(m_label_odo_g);
        hbox_odo->pack_start(m_entry_odo_g);
        hbox_odo->pack_start(m_label_odo_d);
        hbox_odo->pack_start(m_entry_odo_d);

        m_vbox_data->add(*hbox_odo);


        
        Gtk::HBox* hbox_vitesse = Gtk::manage(new Gtk::HBox(false,20));
 		m_label_v_g.set_text("Vitesse gauche :");
        m_label_v_d.set_text("Vitesse droite :");
        m_label_vitesse.set_text("Vitesse :");
        m_label_omega.set_text("Omega :");

        m_entry_v_g.set_editable(false);
        m_entry_v_d.set_editable(false);
        m_entry_vitesse.set_editable(false);
        m_entry_omega.set_editable(false);

        hbox_vitesse->pack_start(m_label_v_g);
        hbox_vitesse->pack_start(m_entry_v_g);
        hbox_vitesse->pack_start(m_label_v_d);
        hbox_vitesse->pack_start(m_entry_v_d);

        hbox_vitesse->pack_start(m_label_vitesse);
        hbox_vitesse->pack_start(m_entry_vitesse);
        hbox_vitesse->pack_start(m_label_omega);
        hbox_vitesse->pack_start(m_entry_omega);

        m_vbox_data->add(*hbox_vitesse);


Gtk::HBox* hbox_position = Gtk::manage(new Gtk::HBox(false,20));
 
        m_label_x.set_text("x :");
        m_label_y.set_text("y :");
        m_label_theta.set_text("Theta :");

        m_entry_x.set_editable(false);
        m_entry_y.set_editable(false);
        m_entry_theta.set_editable(false);

        hbox_position->pack_start(m_label_x);
        hbox_position->pack_start(m_entry_x);
        hbox_position->pack_start(m_label_y);
        hbox_position->pack_start(m_entry_y);
        hbox_position->pack_start(m_label_theta);
        hbox_position->pack_start(m_entry_theta);
        //hbox_position->pack_start(m_label_indicateur);
        //hbox_position->pack_start(m_entry_indicateur);

        m_vbox_data->add(*hbox_position);




 



	

	m_vbox->add(*m_hbox_fin);	


	
	this->add(*m_vbox);
	
        Glib::signal_timeout().connect(
        sigc::mem_fun(*this, &Gui::time_out),
        400
        );

        
	this->show_all();
	
        m_bouton_go.signal_clicked().connect([this]() {

            try {
                a_x_cible = std::stoi(m_entry_dx.get_text());
                b_y_cible = std::stoi(m_entry_dy.get_text());

                std::cout << "Déplacement demandé : X=" << a_x_cible << " Y=" << b_y_cible << std::endl;
                m_robot.go_to(a_x_cible, b_y_cible);


            } catch (...) {
                std::cerr << "Erreur : entrer des entiers valides" << std::endl;
            }
        });

	m_bouton_1.signal_clicked().connect([this]() { 
		std::string adresse=m_entree_1.get_text();
		m_robot.connect(adresse);
		std::cout << "Connexion Wifibot" << std::endl;
	} );

	m_bouton_up.signal_clicked().connect([this]() { 
		m_robot.speed_up();
	} );

	m_bouton_down.signal_clicked().connect([this]() { 
		m_robot.speed_down();
	} );

	m_bouton_left.signal_clicked().connect([this]() { 
		m_robot.turn(-1);
	} );

	m_bouton_right.signal_clicked().connect([this]() { 
		m_robot.turn(+1);
	} );

	m_bouton_rot_l.signal_clicked().connect([this]() { 
		m_robot.rotate(-1);
	} );

	m_bouton_rot_r.signal_clicked().connect([this]() { 
		m_robot.rotate(+1);
	} );

	m_bouton_stop.signal_clicked().connect([this]() { 
		m_robot.stop();
	} );

}
bool Gui::time_out()
{

    m_entry_battery.set_text(format_nb(m_robot.battery) + " V ("+format_nb(m_robot.battery_pourcentage)+" %)");
    
    m_entry_speed_g.set_text(format_nb(m_robot.speed_g));
    m_entry_speed_d.set_text(format_nb(m_robot.speed_d));

    m_entry_p1.set_text(format_nb(m_robot.position_g) + " cm");
    m_entry_p2.set_text(format_nb(m_robot.position_d) + " cm");
    /*m_entry_p3.set_text(std::to_string(m_robot.position_trois) + " cm");
    m_entry_p4.set_text(std::to_string(m_robot.position_quatre) + " cm");*/

    m_entry_odo_g.set_text(std::to_string(m_robot.odo_g) + " tick");
    m_entry_odo_d.set_text(std::to_string(m_robot.odo_d) + " tick");

    m_entry_v_g.set_text(format_nb(m_robot.v_g) + " m/s");
    m_entry_v_d.set_text(format_nb(m_robot.v_d) + " m/s");
    m_entry_vitesse.set_text(format_nb(m_robot.vitesse) + " rad/s");
    m_entry_omega.set_text(format_nb(m_robot.omega) + " rad/s");

    m_entry_x.set_text(format_nb(m_robot.x) + " m");
    m_entry_y.set_text(format_nb(m_robot.y) + " m");
    m_entry_theta.set_text(format_nb(m_robot.theta) + " rad");


	//m_entry_indicateur.set_text(std::to_string(m_robot.isFirstConnected));


    // Bouton non clickable si l'ancien déplacement n'est pas fini
    m_bouton_go.set_sensitive(!m_robot.en_mouvement);


    return true;
}

Gui::~Gui()
{
	m_robot.disconnect();
}

// pour le formattage à 2 chiffres après la virgule
std::string Gui::format_nb(float value) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << value;
    return stream.str();
}