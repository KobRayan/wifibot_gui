#include <unistd.h>
#include <iostream>
#include <cmath>
#include "wifibot.h"


using namespace std;

Wifibot::Wifibot():
m_order(),
m_stop(false),
m_p_thread_send (NULL),
m_p_thread_receive(NULL),
m_p_thread_goto(NULL),
m_socket(),

speed_d(0),
speed_g(0),
battery(0.0f),
battery_pourcentage(0.0f),
position_g(0.0f),
position_deux(0.0f),
position_d(0.0f),
position_quatre(0.0f),
odo_g(0),
odo_d(0),
odo_g_prev(0),
odo_d_prev(0),
x(0.0f),
y(0.0f),
theta(0.0f),
v_g(0.0f),
v_d(0.0f),
vitesse(0.0f),
omega(0.0f),
isFirstConnected(false),
en_mouvement(false)


{
}

Wifibot::~Wifibot(){
	this->disconnect();
}

void Wifibot::stop(){
	m_order.set_order(0,0);
	cout << "Stop()" << endl;
}

void Wifibot::speed_up(){
	if (m_order.get_order_L()==m_order.get_order_R()) {
		m_order.set_order(m_order.get_order_L()+10,m_order.get_order_R()+10);
	}
	else {
		int moy=(m_order.get_order_R()+m_order.get_order_L())/2;
		m_order.set_order(moy+10,moy+10);
	}
	cout << "UP" << endl;
	cout << "Gauche : " << m_order.get_order_L() << "    Droite : " << m_order.get_order_R() << endl;
}

void Wifibot::speed_down(){
	if (m_order.get_order_L()==m_order.get_order_R()) {
		m_order.set_order(m_order.get_order_L()-10,m_order.get_order_R()-10);
	}
	else {
		int moy=m_order.get_order_R()+m_order.get_order_L()/2;
		m_order.set_order(moy-10,moy-10);
	}
	cout << "DOWN" << endl;
	cout << "Gauche : " << m_order.get_order_L() << "    Droite : " << m_order.get_order_R() << endl;
	
}

void Wifibot::turn(int direction){
	cout << "TURN" << endl;
	if (direction==+1) {
		m_order.set_order(m_order.get_order_L()*1.2,m_order.get_order_R()*0.8);
		cout << "Tourne à Droite" << endl;
		cout << "Gauche : " << m_order.get_order_L() << "    Droite : " << m_order.get_order_R() << endl;
	}
	else {
		m_order.set_order(m_order.get_order_L()*0.8,m_order.get_order_R()*1.2);
		cout << "Tourne à Gauche" << endl;
		cout << "Gauche : " << m_order.get_order_L() << "    Droite : " << m_order.get_order_R() << endl;
	}
}

void Wifibot::rotate(int direction){
	cout << "ROTATE" << endl;
	this->stop();
	usleep(1000000);
	if (direction==+1) {
		m_order.set_order(10,-10);
		cout << "Rotation à Droite" << endl;
		cout << "Gauche : " << m_order.get_order_L() << "    Droite : " << m_order.get_order_R() << endl;
	}
	else {
		m_order.set_order(-10,10);
		cout << "Rotation à Gauche" << endl;
		cout << "Gauche : " << m_order.get_order_L() << "    Droite : " << m_order.get_order_R() << endl;
	}
}

void Wifibot::run_send(){
	static int cpt;
	short crc;
	while (m_stop==false) {
		//cout << "Thread [send] : " << ++cpt << endl;
		bool speed_ctr=m_order.get_speed_ctr();
		bool sens_right=((m_order.get_order_R() >=0) ? true :false);
		short speed_right=abs(m_order.get_order_R());
		bool sens_left=((m_order.get_order_L() >=0) ? true :false);
		short speed_left=abs(m_order.get_order_L());
		
		m_output_buf[0]=0xFF;
		m_output_buf[1]=0x07;
		m_output_buf[2]=speed_left & 0xFF;
		m_output_buf[3]=(speed_left >> 8) & 0xFF;
		m_output_buf[4]=speed_right & 0xFF;
		m_output_buf[5]=(speed_right >> 8) & 0xFF;
		m_output_buf[6]=(char)(128*speed_ctr+64*sens_left+32*speed_ctr+16*sens_right+8);
		
		trame_crc[0]=(unsigned char) m_output_buf[1];
		trame_crc[1]=(unsigned char) m_output_buf[2];
		trame_crc[2]=(unsigned char) m_output_buf[3];
		trame_crc[3]=(unsigned char) m_output_buf[4];
		trame_crc[4]=(unsigned char) m_output_buf[5];
		trame_crc[5]=(unsigned char) m_output_buf[6];
		
		crc=crc16(trame_crc,6);
		m_output_buf[7] = crc & 0xFF;
		m_output_buf[8] = (crc >> 8) & 0xFF;
		
		m_socket.send(m_output_buf,9);
		
		this_thread::sleep_for(std::chrono::milliseconds(LOOP_TIME));
	}
	//cout << "Thread [send] : stop !" << endl << endl;
}	

//run receive**************************************************************************
void Wifibot::run_receive() {
	// 1. On déclare en UNSIGNED directement. C'est la bonne pratique.
	unsigned char buf[21]; 
	short crc_receive;
	
	
	
	while (m_stop == false) {
		
		
		m_socket.receive((char*)buf, 21);
		
		
		crc_receive = crc16(buf, 19); 
		
		unsigned char crc_low = buf[19];
		unsigned char crc_high = buf[20];
		
		if (((crc_receive & 0xFF) == crc_low) && (((crc_receive >> 8) & 0xFF) == crc_high)) {
			
			// données non corrompues
			
			speed_d = (short)(buf[9] | (buf[10] << 8));
			speed_g = (short)(buf[0] | (buf[1] << 8));
			
			
			if (speed_d > 32767) speed_d -= 65536;
			if (speed_g > 32767) speed_g -= 65536;
			
			
			battery = (float)buf[2] / 10.0f; 
			battery_pourcentage = 100*battery/12.8f;
			
			
			float tension_un     = (float)(buf[3]  / 255.0f) * 3.3f;
			//float tension_deux   = (float)(buf[4]  / 255.0f) * 3.3f;
			float tension_trois  = (float)(buf[11] / 255.0f) * 3.3f;
			//float tension_quatre = (float)(buf[12] / 255.0f) * 3.3f;
			
			// fonction distance = 62.621 * x^(-1.113)
			if(tension_un>0.05) position_g = 62.621*pow(tension_un,-1.113);
			if(tension_trois>0.05) position_d  = 62.621*pow(tension_trois ,-1.113);
			// pour eviter de diviser par 0
			
			
			// 336 tics
			
			odo_g = (long)((int)(buf[5] | (buf[6] << 8) | (buf[7] << 16) | (buf[8] << 24)));
			odo_d = (long)((int)(buf[13] | (buf[14] << 8) | (buf[15] << 16) | (buf[16] << 24)));
			
			/*odo_g = (long)buf[5]        | 
			(long)buf[6] << 8   | 
			(long)buf[7] << 16  | 
			(long)buf[8] << 24;
			
			odo_d = (long)buf[13]       | 
			(long)buf[14] << 8  | 
			(long)buf[15] << 16 | 
			(long)buf[16] << 24;*/
			
			if(isFirstConnected){
				init_odo_d = odo_d;
				init_odo_g = odo_g;
				
				isFirstConnected = false;
			}
			odo_g -= init_odo_g;
			odo_d -= init_odo_d;
			
			updatePosition();
			
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(LOOP_TIME));
	}
}

//********************************************************************************************

void Wifibot::connect(string ip){
	m_socket.open(ip,PORT);
	cout << "Wifibot connect()" << endl;
	if (m_socket.is_open()) {
		
		isFirstConnected = true;
		m_p_thread_send=new thread([this]() { this->run_send(); });
		m_p_thread_receive=new thread([this]() { this->run_receive(); });
		
		
	}	 
}

void Wifibot::disconnect(){
	m_stop=true;
	m_socket.close();

	// vérifier de fermer les threads à la fermeture de la connexion/quitter le programme
    if (m_p_thread_receive != NULL) {
        if (m_p_thread_receive->joinable()) m_p_thread_receive->join();
        delete m_p_thread_receive;
        m_p_thread_receive = NULL;
    }

    if (m_p_thread_send != NULL) {
        if (m_p_thread_send->joinable()) m_p_thread_send->join();
        delete m_p_thread_send;
        m_p_thread_send = NULL;
    }

    if (m_p_thread_goto != NULL) {
        if (m_p_thread_goto->joinable()) m_p_thread_goto->join();
        delete m_p_thread_goto;
        m_p_thread_goto = NULL;
    }

	
	
}


short Wifibot::crc16(unsigned char* trame, int longueur){
	unsigned int Crc = 0xFFFF;
	unsigned int Polynome = 0xA001;
	unsigned int CptOctet = 0;
	unsigned int CptBit = 0;
	unsigned int Parity= 0;
	
	for ( CptOctet= 0 ; CptOctet < longueur ; CptOctet++)
	{
		Crc ^= *( trame + CptOctet);
		
		for ( CptBit = 0; CptBit <= 7 ; CptBit++)
		{
			Parity= Crc;
			Crc >>= 1;
			if (Parity%2 == true) Crc ^= Polynome;
		}
	}
	return(Crc);
	
}


void Wifibot::updatePosition()
{
	
	
	float phi_d = (odo_d * 2.0f * PI) / TICK_IN_REVOLUTION;
	float phi_g = (odo_g * 2.0f * PI) / TICK_IN_REVOLUTION;
	
	float phi_d_prev = (odo_d_prev * 2.0f * PI) / TICK_IN_REVOLUTION;
	float phi_g_prev = (odo_g_prev * 2.0f * PI) / TICK_IN_REVOLUTION;
	
	//vitesses gauches et droites
	v_d = (phi_d - phi_d_prev) * DIAMETRE_ROUE / (2*LOOP_TIME_F);
	v_g = (phi_g - phi_g_prev) * DIAMETRE_ROUE / (2*LOOP_TIME_F);
	
	
	vitesse = (v_d + v_g) / 2.0f;
	omega = (v_d - v_g) / (2.0f *L_CHASSIS);
	
	// pour passer à l'interation i+1 de la boucle
	odo_d_prev = odo_d;
	odo_g_prev = odo_g;
	
	
	
	// 5 - mise à jour de la pose
	theta += omega * LOOP_TIME_F;
	x += vitesse * cos(theta) * LOOP_TIME_F;
	y += vitesse * sin(theta) * LOOP_TIME_F;
	
	if (theta > PI) theta -= 2 * PI;
	else if (theta < -PI) theta += 2 * PI;
}

void Wifibot::go_to(double dx, double dy)
{
	
	if (en_mouvement) return;
	// SI L'ANCIENNE POSITION N'EST PAS ATTEINTE, NE PAS CREER UN NOUVEAU THREAD
	
	if (m_p_thread_goto != NULL) {
		if (m_p_thread_goto->joinable()) {
            m_p_thread_goto->join(); 
        } // o n join le thread avant de le supprimer
		delete m_p_thread_goto;
		m_p_thread_goto = NULL;
	}
	
	double x_cible = x + dx;
	double y_cible = y + dy;
	//double distance = sqrt(dx*dx + dy*dy); //ancinenne
	
	en_mouvement = true;
	// ajouter x_cible and y_cible au capture list [this, x_cible, y_cible] pour ne pas perdre ces variables quand la fonction arrête d'être appelée
	m_p_thread_goto = new thread([this, x_cible, y_cible]() { 
		this->deplacement_global(x_cible, y_cible); 
	});
	
	
	
}

void Wifibot::deplacement(double x_cible, double y_cible)
{
	
	//double distance;
	
	const double erreur = 0.005;   
	//const double erreur_AXIS = 0.001;
	
	bool position_x_atteinte = false;
	
	double x_relatif = x_cible - x;
	bool direction_avant = (x_relatif >= 0); // direction de mvt initiale
	int direction_sign = direction_avant ? 1 : -1; //operation pour faire xor
	// * *************** GERER AVANT/ARRIERE
		
		if (x_relatif >=0)
		{
			speed_up();
			speed_up();
		}
		else{
			speed_down();
			speed_down();
		}
	while (!position_x_atteinte)
	{
		
		
		x_relatif = x_cible - x;
		
		if (abs(x_relatif)<erreur){
			stop();
			position_x_atteinte = true;
		}
		
		
		// (direction_avant && x_relatif < 0 ou !direction_avant && x_relatif > 0
		// pour que si il dépasse, on s'arrête
		// est exactement un xor entre direction_avant et x relatif (en logique binaire)
		// en cas normal les deux variables sont de même signe sauf si on dépasse la cible

		else if (direction_sign * x_relatif < 0 ) {
             stop(); // On s'arrête immédiatement même si on n'est pas pile dessus
             position_x_atteinte = true;
             cout << "Cible depassée (freinage d'urgence)" << endl;
        }
       

		usleep(LOOP_TIME); // 200 ms
	}
	en_mouvement = false; // Position atteinte okayy
	cout << "Deplacement achevé." << endl;
	
}

void Wifibot::deplacement_global(double x_cible, double y_cible) {
    
    // 1. Calcul de l'angle et de la distance à atteindre
    double dx = x_cible - x;
    double dy = y_cible - y;
    double angle_desire = atan2(dy, dx);
    double distance_restante = sqrt(dx*dx + dy*dy);
    
    // ********************** PHASE ALIGNEMENT
    // On tourne tant qu'on n'est pas aligné (à 0.05 radian près par exemple)
    while (abs(angle_desire - theta) > 0.05) {
        
       
        double erreur_angle = angle_desire - theta;
        // Normalisation entre -PI et PI
        while (erreur_angle > PI) erreur_angle -= 2*PI;
        while (erreur_angle < -PI) erreur_angle += 2*PI;

        if (erreur_angle > 0) {
           m_order.set_order(-10,10);
        } else {
            m_order.set_order(10,-10);
        }
        usleep(LOOP_TIME); // 50ms 
    }
    stop(); 
    usleep(200000); // Stabilisation


    // ************** PHASE AVANCER
    while (distance_restante > 0.05) { // Tant qu'on est à plus de 5cm
        
        dx = x_cible - x;
        dy = y_cible - y;
        distance_restante = sqrt(dx*dx + dy*dy);
        
        
        m_order.set_order(20,20);
        
        usleep(LOOP_TIME);
    }

    stop(); 
    en_mouvement = false;
    cout << "Arrivé en (" << x << ", " << y << ")" << endl;
}