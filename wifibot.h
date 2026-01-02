#pragma once
#include <thread>
#include <atomic> // pour que le thread et le giu partagent
// l'attribut en_mouvement pour que le cpu verifie à chaque iteration et ne pas faire des problèmes bizzares d'optimisation et donc non lecture constante du bool
#include "order.h"
#include "socket.h"


#define LOOP_TIME 200
#define PORT 15020

#define PI 3.14159265358979323846f

#define LOOP_TIME_F 0.2f				// en s
#define TICK_IN_REVOLUTION 336.0f	
#define DIAMETRE_ROUE 0.14f				// en m
#define L_CHASSIS 0.165f				// en m
//largeur = 41, et sans les roues 25 et donc 2L = 33

class Wifibot {
private:
	Order m_order;
	std::thread* m_p_thread_send;
	std::thread* m_p_thread_receive;
	std::thread* m_p_thread_goto;

	bool m_stop;
	Socket_TCP m_socket;
	char m_output_buf[9];
	unsigned char trame_crc[6];

	
	

public:
	Wifibot();
	~Wifibot();
	void stop();
	void speed_up();
	void speed_down();
	void turn(int direction);
	void rotate(int direction);
	void connect(std::string ip);
	void disconnect();
	void go_to(double dx, double dy);
	void deplacement(double x_cible, double y_cible);
	void deplacement_global(double x_cible, double y_cible);

		bool isFirstConnected;
	

	short speed_d;
	short speed_g;

	float battery;
	float battery_pourcentage;

	// Capteurs IR en cm
    float position_g;
    float position_deux;
    float position_d;
    float position_quatre;

      	
    long odo_g;
    long odo_d;
    // odometrie est juste un compte de revolution et donc c'est un long
    // tandis que la vitesse, la position et l'angle sont tous des flotants

    long odo_g_prev;
    long odo_d_prev;

     
	// La position (x,y) est sockée en m
    float x, y, theta;
    float v_g, v_d, vitesse, omega;
	// on pouvait encore utiliser std::atomic pour x et y

	// gêrer lecture et ecriter de en_mouvement en meme temps sans corrompre la donnée
	std::atomic<bool> en_mouvement;


private:	
	void run_send();
	void run_receive();
	short crc16(unsigned char*,int);
	void updatePosition();
	long init_odo_g;
	long init_odo_d;


};

