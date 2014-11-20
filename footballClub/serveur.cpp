#include<iostream>
#include<iomanip>
#include<string>
#include<vector>
#include<ctime>
#include<fstream>
#include<stack>
#include<winsock.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

class Club;      //Déclaration en avance pour utiliser les fonctions avant ses definitions
class Admin;    
class Player;
//Variables globales
vector<Club> cl;             //La class du récipient des clubs 
vector<Admin> ad;            //La class du récipient des personnels administratives
vector<Player> pl;           //La class du récipient des joueurs
ofstream file_out;           //Le flux pour écrire les informations qui sont dans le mémoire dans le fichier
ifstream file_in;            ///Le flux pour écrire les informations qui sont dans le fichier dans le mémoire 

SOCKET sock_con;  //Pour créer la connection
sockaddr_in addr_cli; //Pour stocker le ip et port du client
char send_buf[300];   //Envoyer le flux des données
char rece_buf[300];   //Recevoir le flux des données

//Les définitions des structures
class Admin                 //Le personnel administratif 
{
    private:
	    char club[300];        //Le nom du club que le personnel administratif appartenir à
		char name[300];       //Le nom du personnel administratif
		int age;              //L'age   
		int power;            //Capacité du personnel administratif qui a quatre niveaus: 1, 2, 3, 4
		int salary;           //salaire 
    public:
		Admin() 
		{
		    strcpy(club, "unname_club");
			strcpy(name, "unname_adm");
			age = rand()%13+18;    //Initialiser au hasard
			power = rand()%4+1;
			salary = rand()%451+50;
		}
		Admin(char name_club[], char name_ad[], int age_ad, int power_ad, int salary_ad)
		{                           
		    strcpy(club, name_club);
			strcpy(name, name_ad);
			age = age_ad;
			power = power_ad;
			salary = salary_ad;
		}
		~Admin(){}          
		char *get_club()             
		{
		    return club;
		}
		char *get_name()
		{
		    return name;
		}
		int get_age()
		{
		    return age;
		}
		int get_salary()
		{
		    return salary;
		}
		int get_power()
		{
		    return power;
		}
		int compare_club(char name_cmp[])    //Tester si "name_cmp[]" est le même avec le membre privé 
		{
		    return strcmp(club, name_cmp);
		}
		int compare_name(char name_cmp[])
		{
		    return strcmp(name, name_cmp);
		}
		void change_club(char new_club[])   
		{
		    strcpy(club, new_club);
		}
		void change_name(char new_name[])
		{
		    strcpy(name, new_name);
		}
		void change_age(int new_age)
		{
		    age = new_age;
		}
		void change_power(int new_power)
		{
		    power = new_power;
		}
		void change_salary(int new_salary)
		{
		    salary = new_salary;
		}
		void print_info()        
		{ 
		    char info[300];
			char num_to_str[300];
			strcpy(info, "#\n*************************");
			strcat(info, "\nclub:");
			strcat(info, club);
			strcat(info, "\nname:");
			strcat(info, name);
			strcat(info, "\nage:");
			itoa(age, num_to_str, 10); 
			strcat(info, num_to_str);
			strcat(info, "\npower:");
			itoa(power, num_to_str, 10); 
			strcat(info, num_to_str);
			strcat(info, "\nsalary:");
			itoa(salary, num_to_str, 10); 
			strcat(info, num_to_str);
			strcat(info, "\n*************************\n");

			sprintf(send_buf,info,inet_ntoa(addr_cli.sin_addr)); //Envoyer les informations au client
			send(sock_con,send_buf,strlen(send_buf)+1,0); 
		}
        void add_ad(char name_club[])   //Ajouter un personnel administratif
		{
		    char name_adm[300];
			int i=0, age_adm=0, power_adm=0, salary_adm=0;

			sprintf(send_buf,"please input the name of the member\n",//Attribuer les valeurs à le flux envoyé au client
					    inet_ntoa(addr_cli.sin_addr));              
   			send(sock_con,send_buf,strlen(send_buf)+1,0);  //Envoyer les information au client pour informer le client pour les opérations
			recv(sock_con,rece_buf,300,0);    //Recevoir le information du client pour l'opération après 
			strcpy(name_adm, rece_buf);
			sprintf(send_buf,"please input the age of the member\n",   //La même  méthode comme avant
					    inet_ntoa(addr_cli.sin_addr)); 
			send(sock_con,send_buf,strlen(send_buf)+1,0); 
			recv(sock_con,rece_buf,300,0);
			for(i = 0; rece_buf[i]; i++)
			    age_adm = age_adm*10 + (rece_buf[i]-'0');
			sprintf(send_buf,"please input the power of the member\n",
					    inet_ntoa(addr_cli.sin_addr)); 
			send(sock_con,send_buf,strlen(send_buf)+1,0); 
			recv(sock_con,rece_buf,300,0);
			for(i = 0; rece_buf[i]; i++)
			    power_adm = power_adm*10 + (rece_buf[i]-'0');
			sprintf(send_buf,"please input the salary of the member\n",
					    inet_ntoa(addr_cli.sin_addr)); 
			send(sock_con,send_buf,strlen(send_buf)+1,0); 
			recv(sock_con,rece_buf,300,0);
			for(i = 0; rece_buf[i]; i++)
			    salary_adm = salary_adm*10 + (rece_buf[i]-'0');

			Admin temp_ad(name_club, name_adm, age_adm, power_adm, salary_adm); //Attribuer les valeurs
			ad.push_back(temp_ad);   //Ajouter le nouveau membre dans la classe du récipient
		}
		void delete_ad(char delete_name[])      //Supprimer tout le personnel administratif d'un club
		{
		    int i=0, j=0, size_ad=0;
              
			size_ad = ad.size();     //Le nombre du personnel administratif 
			for(i = 0; i < size_ad; i++)
			{
		        if(ad[i].compare_club(delete_name) == 0)   
			    {                             
		            if(i == 0 || i == (size_ad-1))    //Si le membre est le dernier membre de ad[]  
						ad.pop_back();//Supprimer le dernier membre de ad[]
					else
					{
					    for(j = i; j < size_ad; j++) //Supprimer le membre d'itération en utilisant la méthode
						    ad[j] = ad[j+1];
					    ad.pop_back();//Supprimer le dernier membre de ad[]
					}
                    i--;
				    size_ad--; 
			    }
			}
		}
}; 
class Player     //La classe du joueur
{
    private:
		char club[300];        //Le nom du club du joueur 
		char name[300];    //le nom du joueur
		int age;              //Le age
		int power;            //Capacité du personnel administratif qui a quatre niveaus: 1, 2, 3, 4
		int salary;           //salaire 
	    int goal;             //Le nombre des buts
		int tran_fee;         //frais de transfert 
		int year;             //Durée de vie
    public:
		Player()       
		{
		    strcpy(club, "unname_club");
			strcpy(name, "unname_pla");
			age = rand()%13+18;    //Initialiser au hasard
			power = rand()%4+1; 
			salary = rand()%901+100;
			goal = rand()%81+20;
			tran_fee = rand()%1801+200;
			year = rand()%21;
		}
		Player(char name_cl[], char name_pl[], int age_pl, int power_pl, 
			   int salary_pl, int goal_pl, int tran_fee_pl, int year_pl)
		{                       
		    strcpy(club, name_cl);
			strcpy(name, name_pl);
			age = age_pl;
			power = power_pl;
			salary = salary_pl;
			goal = goal_pl;
			tran_fee = tran_fee_pl;
			year = year_pl;
		}
		~Player(){}  
		int compare_club(char name_cmp[])       
		{
		    return strcmp(club, name_cmp);
		}
		int compare_name(char name_cmp[])
		{
		    return strcmp(name, name_cmp);
		}
		void change_age(int new_age)       
		{
		    age = new_age;
		}
		void change_power(int new_power)
		{
		    power = new_power;
		}
		void change_salary(int new_salary)
		{
		    salary = new_salary;
		}
		void change_goal(int new_goal)
		{
		     goal = new_goal;
		}
		void change_tran_fee(int new_tran_fee)
		{
		    tran_fee = new_tran_fee;
		}
		void change_year(int new_year)
		{
		    year = new_year;
		}
		void change_club(char new_club[])
		{
		    strcpy(club, new_club);
		}
		void change_name(char new_name[])
		{
		    strcpy(name, new_name);
		}
		char *get_name()       
		{
		    return name;
		}
		char *get_club()
		{
		    return club;
		}
		int get_age()
		{
		    return age;
		}
		int get_salary()
		{
		    return salary;
		}
		int get_goal()
		{
		    return goal;
		}
		int get_power()
		{
		    return power;
		}
		int get_year()
		{
		    return year;
		}
		int get_tran_fee()
		{
		    return tran_fee;
		}
		void print_info()        
		{ 
		    char info[300];
			char num_to_str[300];
			strcpy(info, "#\n*************************");
			strcat(info, "\nclub:");
			strcat(info, club);
			strcat(info, "\nname:");
			strcat(info, name);
			strcat(info, "\nage:");
			itoa(age, num_to_str, 10); 
			strcat(info, num_to_str);
			strcat(info, "\npower:");
			itoa(power, num_to_str, 10); 
			strcat(info, num_to_str);
			strcat(info, "\nsalary:");
			itoa(salary, num_to_str, 10); 
			strcat(info, num_to_str);
			strcat(info, "\ngoal:");
			itoa(goal, num_to_str, 10); 
			strcat(info, num_to_str);
			strcat(info, "\ntran_fee:");
			itoa(tran_fee, num_to_str, 10); 
			strcat(info, num_to_str);
			strcat(info, "\nyear:");
			itoa(year, num_to_str, 10); 
			strcat(info, num_to_str);
			strcat(info, "\n*************************\n");

			sprintf(send_buf,info,inet_ntoa(addr_cli.sin_addr)); //Envoyer l'information au client
			send(sock_con,send_buf,strlen(send_buf)+1,0); 

		}
        void add_pl(char name_club[])       //Ajouter un joueur
		{
		    char name_pla[300], num_to_str[300];
			int age_pla=0, power_pla=0, salary_pla=0;
			int goal_pla=0, tran_fee_pla=0, year_pla=0;

	        sprintf(send_buf,"please input the name of the member\n", 
					    inet_ntoa(addr_cli.sin_addr));  //Attribuer les valeurs à le flux envoyé au client
			send(sock_con,send_buf,strlen(send_buf)+1,0);  ////Envoyer les information au client pour informer le client pour les opérations
			recv(sock_con,rece_buf,300,0);   //Recevoir le information du client pour l'opération après 
			strcpy(name_pla, rece_buf);
			sprintf(send_buf,"please input the age of the member\n",  //Le même méthode comme avant
					    inet_ntoa(addr_cli.sin_addr)); 
			send(sock_con,send_buf,strlen(send_buf)+1,0); 
			recv(sock_con,rece_buf,300,0);
			itoa(age_pla, num_to_str, 10); 
			strcpy(num_to_str, rece_buf);
			sprintf(send_buf,"please input the power of the member\n",
					    inet_ntoa(addr_cli.sin_addr)); 
			send(sock_con,send_buf,strlen(send_buf)+1,0); 
			recv(sock_con,rece_buf,300,0);
			itoa(power_pla, num_to_str, 10); 
			strcpy(num_to_str, rece_buf);
			sprintf(send_buf,"please input the salary of the member\n",
					    inet_ntoa(addr_cli.sin_addr)); 
			send(sock_con,send_buf,strlen(send_buf)+1,0); 
			recv(sock_con,rece_buf,300,0);
			itoa(salary_pla, num_to_str, 10); 
			strcpy(num_to_str, rece_buf);
			sprintf(send_buf,"please input the number of goals of the player\n",
					    inet_ntoa(addr_cli.sin_addr)); 
			send(sock_con,send_buf,strlen(send_buf)+1,0); 
			recv(sock_con,rece_buf,300,0);
			itoa(goal_pla, num_to_str, 10); 
			strcpy(num_to_str, rece_buf);
			sprintf(send_buf,"please input the transfer fee of the player\n",
					    inet_ntoa(addr_cli.sin_addr)); 
			send(sock_con,send_buf,strlen(send_buf)+1,0); 
			recv(sock_con,rece_buf,300,0);
			itoa(tran_fee_pla, num_to_str, 10); 
			strcpy(num_to_str, rece_buf);
			sprintf(send_buf,"please input the years the player has played football\n",
					    inet_ntoa(addr_cli.sin_addr)); 
			send(sock_con,send_buf,strlen(send_buf)+1,0); 
			recv(sock_con,rece_buf,300,0);
			itoa(year_pla, num_to_str, 10); 
			strcpy(num_to_str, rece_buf);

			Player temp_pla(name_club, name_pla, age_pla, power_pla, salary_pla,
				          goal_pla, tran_fee_pla, year_pla);
			pl.push_back(temp_pla);
		}
		void delete_pl(char delete_name[])      //Supprimer un joueur
		{
		    int i=0, j=0, size_pl=0;
              
			size_pl = pl.size();
			for(i = 0; i < size_pl; i++)
			{
		        if(pl[i].compare_club(delete_name) == 0)
			    {  
		            if(i == 0 || i == (size_pl-1))     //Le joueur supprimé est le dernier membre de pl[]
						pl.pop_back();//Supprimer le dernier membre de pl[]
					else
					{
					    for(j = i; j < size_pl; j++)     
						    pl[j] = pl[j+1];
					    pl.pop_back();//Supprimer le dernier membre de pl[]
					}
					i--;
				    size_pl--;
			    }
			}
		}
		void search_player()
		{
		    char search_name[300], search_club[300], info_search[300];
			char (*postfix)[20];    //L'array des pointeur pour stocker les postfix expressions 
			char (*diy_stack)[20];   //L'array bidimensionnel pour construire la pile séquentiel
			int top=-1;              //Le sommet de la pile. Si il est -1, la pile est vide. La pile initiale est vide
			char tem_str[20], str1[20], str2[20];     //Stocker temporairement la chaine
			char type[300];
			stack<char*> my_stack;
			int i=0, j=0, w=0, size=0, num=0, size_pl=0, length=0;
			int *list_player;  //Allouer le mémoire au-dessous, 1:sélectionné, 0: pas sélectionné

			size_pl = pl.size();
			list_player = new int[size_pl];
			for(i = 0; i < size_pl; i++)
				list_player[i] = 0;          //1: sélectionné; 0: pas sélectionné

			char info[300];
			strcpy(info, "please input the type of search:(1-3)\n");
			strcat(info, "1:search the player by his name\n");
			strcat(info, "2:search the player by the information\n");
			strcat(info, "3:search the player by the club he belongs to\n");
			sprintf(send_buf,info,inet_ntoa(addr_cli.sin_addr)); //Attribuer les valeurs à le flux envoyé au client
			send(sock_con,send_buf,strlen(send_buf)+1,0);  //Envoyer les information au client pour informer le client pour les opérations
			recv(sock_con,rece_buf,300,0);   //Recevoir le information du client pour l'opération après 
			strcpy(type, rece_buf);
			if(type[0] == '1')    //La première méthode de recherche en entrant le nom 
			{
		        sprintf(send_buf,"please input the name of the player you want to search\n",
					    inet_ntoa(addr_cli.sin_addr)); 
			    send(sock_con,send_buf,strlen(send_buf)+1,0); 
			    recv(sock_con,rece_buf,300,0);
			    strcpy(search_name, rece_buf);
			    for(i = 0; i < size_pl; i++)
				{
					if(pl[i].compare_name(search_name) == 0)
					{
						pl[i].print_info();
					    i = size_pl+1;
					}
				}
				if(i == size_pl)
				{
					sprintf(send_buf,"#this player doesn't exit\n",
					    inet_ntoa(addr_cli.sin_addr)); 
			        send(sock_con,send_buf,strlen(send_buf)+1,0); 
				}
			} 
			else if(type[0] == '2')     //La deuxième méthode de recherche en entrant l'expression logique
			{
			    strcpy(info, "please input the information you want to search:\n");
				strcat(info, "for example: input: ( !(@age>25) && @year>5) || @goal>100\n");
				sprintf(send_buf,info,inet_ntoa(addr_cli.sin_addr)); 
			    send(sock_con,send_buf,strlen(send_buf)+1,0); 
			    recv(sock_con,rece_buf,300,0);
			    strcpy(info_search, rece_buf);

				for(i = 0; info_search[i]; i++)       //Calculer la longueur de la cha?ne pour allouer un espace suffisant à postfix au-dessous
				{}
				postfix = new char[i][20];      //L'array bidimensionnel pour stocker l'expression postfix
				length = i;
				i = 0;
				j = 0;
				while(info_search[i])     //Parcourir l'expression infixe
				{
				    if(is_number(info_search[i]))    //Il est un opérande, pas de opérateur, et il est un chiffre
					{
					    w = 0;
						while(is_number(info_search[w+i]))     //Exporter l'opérande à l'expression postfix
						{
						    postfix[j][w] = info_search[w+i];
							w++;
						}
						postfix[j][w] = '\0';
						j++;
						i+=w;;
					}
					else
					{
					    switch(info_search[i])    //Traiter le caractère parcouru，Si il est un opérande, l'exporter à l'expression postfix
					    {
					        case '@':   //Ignorer les caractères inutiles
						        i++;
						    break;
						    case ' ':
							    i++;
						    break;
						    case 'a':
							    for(w = 0; w < 3; w++)
							        postfix[j][w] = info_search[w+i];
							    postfix[j][w] = '\0';
							    j++;
							    i+=3;     //Aller au premier caractère de la donnée prochaine
						    break;
						    case 'p':
                                for(w = 0; w < 5; w++)
							        postfix[j][w] = info_search[w+i];
							    postfix[j][w] = '\0';
							    j++;
							    i+=5;
						    break;
						    case 's':
						        for(w = 0; w < 6; w++)
							        postfix[j][w] = info_search[w+i];
							    postfix[j][w] = '\0';
							    j++;
							    i+=6;
						    break;
						    case 'g':
						        for(w = 0; w < 4; w++)
							        postfix[j][w] = info_search[w+i];
							    postfix[j][w] = '\0';
							    j++;
							    i+=4;
						    break;
						    case 't':
						        for(w = 0; w < 8; w++)
							        postfix[j][w] = info_search[w+i];
							    postfix[j][w] = '\0';
							    j++;
							    i+=8;
						    break;
						    case 'y':
						        for(w = 0; w < 4; w++)
							        postfix[j][w] = info_search[w+i];
							    postfix[j][w] = '\0';
							    j++;
							    i+=4;
						    break;
						    default:   //Il est un opérateur, pas de opérande 
						     	if(info_search[i] == '(')     //Si il est '(', le pousser dans la pile
						    	{
							    	my_stack.push("(");
							        i++;
							    }
							    else if(info_search[i] == ')') //Si il est ')', exporter les opérateurs dans la pile jusqu'à '('
							    {
							    	while(!my_stack.empty()) 
								    {
									    if(!strcmp(my_stack.top(),"(")) //Il est '(', éjecter '(', terminer le cycle
									    {
                                            my_stack.pop();
										    break;
									    }
									    else   //Il n'est pas '(', continuer à éjecter 
									    {
									        strcpy(postfix[j], my_stack.top());     
										    j++;
										    my_stack.pop();
									    }
								    }
								    i++;
							    }
							    else    //Il n'est ni '(' ni ')', il est les autres opérandes: !, ||, &&, <, >
							    {
								    if(!my_stack.empty())
								    {
								        switch(info_search[i])
									    {
							    	        case '&':
										        strcpy(tem_str, "&&");
											    i+=2;
										    break;
										    case '|':
										        strcpy(tem_str, "||");
											    i+=2;	
										    break;
										    case '!':
										        strcpy(tem_str, "!");
											    i+=1;	
										    break;
										    case '>':
										        strcpy(tem_str, ">");
											    i+=1;	
										    break;
										    case '<':
										        strcpy(tem_str, "<");
										    	i+=1;	
										    break;
										    default:
										   		i++;
										   	break;
									    }
								        if(!strcmp(my_stack.top(),"(") ||
										   priority(tem_str)>priority(my_stack.top())) 
										{          //Si le membre au sommet de la pile est '(' ou sa prioritéest inférieur à le opérande lu
											switch(tem_str[0])
											{
											    case '&':
												    my_stack.push("&&");	
												break;
												case '|':
												    my_stack.push("||");	
												break;
												case '<':
												    my_stack.push("<");	
												break;
												case '>':
												    my_stack.push(">");	
												break;
												case '!':
												    my_stack.push("!");	
												break;
												default:
												break;
											}							
										}
									    else       //Si la priorité le membre au sommet de la pile est supérieur ou égal
									   	{
									        strcpy(postfix[j], my_stack.top());   //éjecter le sommet de la pile, le mettre dans postfix[j]
										    my_stack.pop();
										    j++;
											switch(tem_str[0])
											{
											    case '&':
												    my_stack.push("&&");	
												break;
												case '|':
												    my_stack.push("||");	
												break;
												case '<':
												    my_stack.push("<");	
												break;
												case '>':
												    my_stack.push(">");	
												break;
												case '!':
												    my_stack.push("!");	
												break;
												default:
												break;
											}
									    }
								    }
								    else      //Si la pile est vide, mettre l'opérande dans la pile
									{
									    switch(info_search[i])
										{
										    case '&':
											    my_stack.push("&&");
												i+=2;
											break;
											case '|':
									            my_stack.push("||");
											    i+=2;	
										    break;
										    case '!':
										        my_stack.push("!");
											    i+=1;	
										    break;
										    case '>':
										        my_stack.push(">");
											    i+=1;	
										    break;
										    case '<':
										        my_stack.push("<");
										    	i+=1;	
										    break;
										    default:
									    		i++;
									    	break;
									    }
									}
							    }
                            break;
					    }
					}
				}
				while(!my_stack.empty())   //exporter le reste des operandes à postfix, vider la pile après 
				{
				    strcpy(postfix[j], my_stack.top());
					my_stack.pop();
					j++;
				}
				size = j;       //la taille de l'array （*postfix—）[]
				diy_stack = new char[size][20];     //créer la pile séquentielle
				for(i = 0; i < size_pl; i++)
				{
				    for(j = 0; j < size; j++)
				    {
				        strcpy(tem_str, postfix[j]);
					    if(is_number(tem_str[0]))      //Si il est chiffre, le pousser dans la pile
							strcpy(diy_stack[++top], tem_str);  //Pousser tem_str dans la pile
						else if(tem_str[0]=='a' || tem_str[0]=='p' || tem_str[0]=='s' || tem_str[0]=='g'||
							    tem_str[0]=='t' || tem_str[0]=='y')    //Pousser l'operande dans la pile
						    strcpy(diy_stack[++top], tem_str);  //Pousser tem_str dans la pile
						else if(tem_str[0] == '!')         
						{
						    strcpy(str1, diy_stack[top--]);   //Recevoir le membre au sommet àla pile, et l'éjecter
							if(!strcmp(str1, "1"))
								strcpy(str1, "0");
							else if(!strcmp(str1, "0"))
								strcpy(str1, "1");
							strcpy(diy_stack[++top], str1);    //Pousser str1 dans la pile
						}
						else if(tem_str[0]=='|' || tem_str[0]=='&')
						{
						    strcpy(str1, diy_stack[top--]);   //Recevoir le membre au sommet àla pile, et l'éjecter(top--)
							strcpy(str2, diy_stack[top--]);
							if(tem_str[0]=='&')
							{
							    if(!strcmp(str1, "1") && !strcmp(str2, "1"))
									strcpy(diy_stack[++top], "1");
								else
									strcpy(diy_stack[++top], "0");
							}
							if(tem_str[0]=='|')
							{
							    if(!strcmp(str1, "0") && !strcmp(str2, "0"))
									strcpy(diy_stack[++top], "0");
								else
									strcpy(diy_stack[++top], "1");
							}

						}
						else           //Si il est '<' ou '>'
						{
						    strcpy(str2, diy_stack[top--]);    //str2 est opérandes numériques
							strcpy(str1, diy_stack[top--]);    //str1 est opérandes de cha?nes
					        switch(str1[0])
							{
							    case 'a':
									for(w = 0; str2[w]; w++)    //transformer le cha?ne qui est un chiffre au chiffre
									    num = num*10 + (str2[w]-'0');								
								    if(!strcmp(tem_str, ">"))     //Opération logique, et pousser le résultat dans la pile
									{
									    if(pl[i].get_age() > num)
											strcpy(diy_stack[++top], "1");
										else
											strcpy(diy_stack[++top], "0");
									}
									else if(!strcmp(tem_str, "<"))
									{
									    if(pl[i].get_age() < num)
											strcpy(diy_stack[++top], "1");
										else
											strcpy(diy_stack[++top], "0");
									}
									num = 0;
								break;
								case 'p':
								    for(w = 0; str2[w]; w++)     //transformer le cha?ne qui est un chiffre au chiffre
									    num = num*10 + (str2[w]-'0');								
								    if(!strcmp(tem_str, ">"))     //Opération logique, et pousser le résultat dans la pile
									{
									    if(pl[i].get_power() > num)
											strcpy(diy_stack[++top], "1");
										else
											strcpy(diy_stack[++top], "0");
									}
									else if(!strcmp(tem_str, "<"))
									{
									    if(pl[i].get_power() < num)
											strcpy(diy_stack[++top], "1");
										else
											strcpy(diy_stack[++top], "0");
									}
									num = 0;	
								break;
								case 's':
									for(w = 0; str2[w]; w++)      //transformer le cha?ne qui est un chiffre au chiffre
									    num = num*10 + (str2[w]-'0');								
								    if(!strcmp(tem_str, ">"))    //Opération logique, et pousser le résultat dans la pile
									{
									    if(pl[i].get_salary() > num)
											strcpy(diy_stack[++top], "1");
										else
											strcpy(diy_stack[++top], "0");
									}
									else if(!strcmp(tem_str, "<"))
									{
									    if(pl[i].get_salary() < num)
											strcpy(diy_stack[++top], "1");
										else
											strcpy(diy_stack[++top], "0");
									}
									num = 0;	
								break;
								case 'g':    
									for(w = 0; str2[w]; w++)     //transformer le cha?ne qui est un chiffre au chiffre
									    num = num*10 + (str2[w]-'0');								
								    if(!strcmp(tem_str, ">"))    //Opération logique, et pousser le résultat dans la pile
									{
									    if(pl[i].get_goal() > num)
											strcpy(diy_stack[++top], "1");
										else
											strcpy(diy_stack[++top], "0");
									}
									else if(!strcmp(tem_str, "<"))
									{
									    if(pl[i].get_goal() < num)
											strcpy(diy_stack[++top], "1");
										else
											strcpy(diy_stack[++top], "0");
									}
									num = 0;	
								break;
								case 't': 
									for(w = 0; str2[w]; w++)     //transformer le cha?ne qui est un chiffre au chiffre
									    num = num*10 + (str2[w]-'0');								
								    if(!strcmp(tem_str, ">"))   //Opération logique, et pousser le résultat dans la pile
									{
									    if(pl[i].get_tran_fee() > num)
											strcpy(diy_stack[++top], "1");
										else
											strcpy(diy_stack[++top], "0");
									}
									else if(!strcmp(tem_str, "<"))
									{
									    if(pl[i].get_tran_fee() < num)
											strcpy(diy_stack[++top], "1");
										else
											strcpy(diy_stack[++top], "0");
									}
									num = 0;	
								break;
								case 'y':
									for(w = 0; str2[w]; w++)      //transformer le cha?ne qui est un chiffre au chiffre
									    num = num*10 + (str2[w]-'0');								
								    if(!strcmp(tem_str, ">"))    //Opération logique, et pousser le résultat dans la pile
									{
									    if(pl[i].get_year() > num)
											strcpy(diy_stack[++top], "1");
										else
											strcpy(diy_stack[++top], "0");
									}
									else if(!strcmp(tem_str, "<"))
									{
									    if(pl[i].get_year() < num)
											strcpy(diy_stack[++top], "1");
										else
											strcpy(diy_stack[++top], "0");
									}
									num = 0;	
								break;
							}
						}
				    }
					if(!strcmp(diy_stack[top], "1")) //Si le résultat est 1, le joueur satisfait l'expression logique 
						pl[i].print_info();          //Imprimer les informations du joueur
				}
            }
			else if(type[0] == '3')   //Le troisième méthode du recherche selon les clubs
			{
				strcpy(info, "please input the clubs the players belong to\n");
				strcat(info, "for example: input: @club=A||@club=B\n");
				sprintf(send_buf,info,inet_ntoa(addr_cli.sin_addr)); 
		    	send(sock_con,send_buf,strlen(send_buf)+1,0); 
		    	recv(sock_con,rece_buf,300,0);
		    	strcpy(info_search, rece_buf);
				i = 0;
				while(info_search[i])//parcourir l'expression du recherche
				{
				    switch(info_search[i])    
				    {
						case '@':    //ignorer les caractères inutiles
							i++;
						break;
						case '|':
							i++;
						break;
						default:
						    j=0;
							while(info_search[i]!='|')
							    search_club[j++] = info_search[i++];
							search_club[j] = '\n';
							for(j = 0; j < size_pl; j++)    //imprimer les joueurs suffisants
								if(pl[j].compare_club(search_club) == 0)
									pl[j].print_info();
						break;
					}
				}
            }
			delete []list_player;    //relacher le mémoire
		}
		bool is_number(char ch)    //tester si le caractère lu est un chiffre
		{
		    if(ch>='0' && ch <='9')
				return true;
			else
				return false;
		}
		int priority(char str[])    //déterminer la priorité des opérateurs
		{
			char ch = str[0];
			
			switch(ch)
			{
			    case '!':
					return 4;
				break;
			    case '<':
					return 3;
				break;
				case '>':
					return 3;
				break;
				case '&':
					return 2;
				break;
				case '|':
					return 1;
				break;
				default:
				break;
			}
		}
};
class Club                    //la classe du club
{
    private:
	    char name[300];      //nom
		int fund;            //fonds 
		int number;          //nombre des membres(personnel administratif et joueurs )
    public:
		Club()     //默认构造函数
		{
		    strcpy(name, "unname_club"); 
			fund = rand()%11 + 10000;     
			number = 80;
		}
		Club(char new_name[], int new_fund)   
		{
		    strcpy(name, new_name);
			fund = new_fund;
			number = 0;
		}
		~Club(){}      
		char *get_name()     
		{ 
		    return name;
		}
		int get_fund()
		{
		    return fund;
		}
		int get_number()
		{
		    return number;
		}
		int compare_name(char name_cmp[])     
		{
		    return strcmp(name, name_cmp);
		}
		void add_number()   
		{
		    number++;
		}
		void minus_number()    
		{
		    number--;
		}
		void change_name(char new_name[])  
		{
		    strcpy(name, new_name);
		}
		void change_fund(int new_fund)
		{
		    fund = new_fund;
		}
		void change_number(int new_number)
		{
		    number = new_number;
		}
		void add_fund(int new_fund)
		{
		    fund += new_fund;
		}
		void minus_fund(int new_fund)
        {
		    fund -= new_fund;
		}     
		void print_simple()   
		{
		    char info[300];
			char num_to_str[300];

			strcpy(info, "#\n****************");
			strcat(info, "\nname:");
			strcat(info, name);
			strcat(info, "\nfund:");
			itoa(fund, num_to_str, 10);  //transformer selon décimal, transformer le chiffre à un cha?ne
			strcat(info, num_to_str);
			strcat(info, "\nnumber:");
			itoa(number, num_to_str, 10); 
			strcat(info, num_to_str);
			strcat(info, "\n****************\n");
			sprintf(send_buf,info,inet_ntoa(addr_cli.sin_addr)); //Attribuer les valeurs à le flux envoyé au client
		    send(sock_con,send_buf,strlen(send_buf)+1,0);   //Envoyer les information au client pour informer le client pour les opérations
		}
		void print_info() 
		{
			int i=0, count_ad=0, count_pl=0, size_admin=0, size_player=0;

			size_admin = ad.size();
			size_player = pl.size();
			char info[300];
			char num_to_str[300];

			strcpy(info, "#\n*************************");
			strcat(info, "\nthe name of the club is:");
			strcat(info, name);
			strcat(info, "\nthe quantity of the fund of the club is:");
			itoa(fund, num_to_str, 10); //transformer selon décimal, transformer le chiffre à un cha?ne
			strcat(info, num_to_str);
			strcat(info, "\nthe number of the members the club has is:");
			itoa(number, num_to_str, 10);
			strcat(info, num_to_str);
			strcat(info, "\nthe information of the  member is following:\n");
	
			for(i = 0; i < size_admin; i++) 
			{
				if(ad[i].compare_club(name) == 0)
				{
				    ad[i].print_info();
				    count_ad++;
					Sleep(100);
				}
			}
			for(i = 0; i < size_player; i++)
			{
				if(pl[i].compare_club(name) == 0)
				{
				    pl[i].print_info();
					count_pl++;
					Sleep(100);
				}
			}
			strcat(info, "\nthe number of the administration is:");
			itoa(count_ad, num_to_str, 10);
			strcat(info, num_to_str);
			strcat(info, "\nthe number of the football player is:");
			itoa(count_pl, num_to_str, 10);
			strcat(info, num_to_str);
			sprintf(send_buf,info,inet_ntoa(addr_cli.sin_addr)); //Attribuer les valeurs à le flux envoyé au client
		    send(sock_con,send_buf,strlen(send_buf)+1,0);  //Envoyer les information au client pour informer le client pour les opérations
		}
		void add_club() 
		{
		    int i=0, size = 0, new_num=0,choice_type=0, fund_num=0;
			char new_name[300], new_fund[300];

			sprintf(send_buf,"please input the name of the new club:\n",
				    inet_ntoa(addr_cli.sin_addr)); 
		    send(sock_con,send_buf,strlen(send_buf)+1,0); 
		    recv(sock_con,rece_buf,300,0);
		    strcpy(new_name, rece_buf);
			sprintf(send_buf,"please input the fund of the club:\n",
				    inet_ntoa(addr_cli.sin_addr)); 
		    send(sock_con,send_buf,strlen(send_buf)+1,0); 
		    recv(sock_con,rece_buf,300,0);
		    strcpy(new_fund, rece_buf);

			for(i = 0; new_fund[i]; i++)
				fund_num = fund_num*10+(new_fund[i]-'0');
			Club temp(new_name, fund_num);
			cl.push_back(temp);
		}
		void delete_club(char name_delete[]) 
		{
			int size=0;

			size = cl.size();
			for(int i = 0; i < size; i++)
			{
			    if(cl[i].compare_name(name_delete) == 0)
				{
					if(i == 0 || i == (size-1))
						cl.pop_back();//Supprimer cl[size-1]
					else
					{
					    cl[i].add_number();
				        for(int j = i; j < size; j++)
						    cl[j] = cl[j+1];       
					    cl.pop_back();//Supprimer cl[size-1]
					}
				}
			}
		}
		void change_info(char name_club[])    
		{
		    int i=0, j=0, t=0, new_fund_num=0;
			int size_club=0, size_admin=0, size_player=0;
			char type[300], new_fund[300], new_name[300];
			char info[300];

		    size_club = cl.size();
			for(i = 0; i < size_club; i++)
			{
				if(cl[i].compare_name(name_club) == 0)
				{
				    strcpy(info, "please input the content you want to change(0-2):\n");
					strcat(info, "1:change the name of the club\n");
					strcat(info, "2:change the quantity of the fund of the club\n");
					strcat(info, "0:stop changing\n");
					sprintf(send_buf,info,inet_ntoa(addr_cli.sin_addr)); 
		    	    send(sock_con,send_buf,strlen(send_buf)+1,0); 
		    	    recv(sock_con,rece_buf,300,0);
		    	    strcpy(type, rece_buf);					
		
					while(type[0] != '0')
					{
					    if(type[0] == '1')    //changer le nom du club  
						{
						    size_admin = ad.size();
							size_player = pl.size();
							sprintf(send_buf,"please input the new name of the club\n",
								    inet_ntoa(addr_cli.sin_addr)); //Attribuer les valeurs à le flux envoyé au client
		    	            send(sock_con,send_buf,strlen(send_buf)+1,0);  //Envoyer les information au client pour informer le client pour les opérations
		    				recv(sock_con,rece_buf,300,0);   //Recevoir le information du client pour l'opération après 
						    strcpy(new_name, rece_buf);
							cl[i].change_name(new_name);
							for(j = 0; j < size_admin; j++)
								if(ad[j].compare_club(name_club) == 0)
									ad[j].change_club(new_name);
						    for(j = 0; j < size_player; j++)
								if(pl[j].compare_club(name_club) == 0)
									pl[j].change_club(new_name);        
						}
						else if(type[0] == '2')   //changer le fonds du club
						{
						    sprintf(send_buf,"please input the new quantity of the fund of the club\n",
								    inet_ntoa(addr_cli.sin_addr)); 
		    	            send(sock_con,send_buf,strlen(send_buf)+1,0); 
		    				recv(sock_con,rece_buf,300,0);
						    strcpy(new_fund, rece_buf);
		
							for(t=0; new_fund[t]; t++)
								new_fund_num = new_fund_num*10+(new_fund[t]-'0');
							cl[i].change_fund(new_fund_num);
						}
						strcpy(info, "please input the content you want to change(0-2):\n");
						strcat(info, "1:change the name of the club\n");
						strcat(info, "2:change the quantity of the fund of the club\n");
						strcat(info, "0:stop changing\n");
						sprintf(send_buf,info,inet_ntoa(addr_cli.sin_addr)); 
		    			send(sock_con,send_buf,strlen(send_buf)+1,0); 
		    			recv(sock_con,rece_buf,300,0);
		    			strcpy(type, rece_buf);
					}
					i = size_club;
				}
			}
		}
		void enquire_info(char name_club[])
		{
		    int size_club=0;
            int i=0;

			size_club = cl.size();
			for(i = 0; i < size_club; i++)
			{
				if(cl[i].compare_name(name_club) == 0)
				{
					cl[i].print_info();
				    i = size_club;
				}
			}
		}
		void transfer(char tran_player[], char name_club[])//transaction du joueur du club
		{
		    int i=0, j=0, size_pl=0, size_cl=0,transfer_fee=0;
			char old_club[50];

			size_pl = pl.size();
			size_cl = cl.size();         
			for(i = 0; i < size_pl; i++)
			{
				if(pl[i].compare_name(tran_player) == 0)
				{
					transfer_fee = pl[i].get_tran_fee();
					strcpy(old_club, pl[i].get_club());
                    for(j = 0; j < size_cl; j++)
			        {
				        if(cl[j].compare_name(name_club) == 0)
				        {
				            cl[j].minus_fund(transfer_fee);
							cl[j].add_number();
							pl[i].change_club(name_club);	
				        }
						if(cl[j].compare_name(old_club) == 0)
						{
						    cl[j].add_fund(transfer_fee);
						    cl[j].minus_number();
						}
			        }
				}
			}
		}
};
void menu()  
{
    int i=0, size=0;
	bool flag = true;
	char type[300],info[300], choice[300], name_club[300], new_name[300], tran_name[300];
	vector<Club> temp_club(1);    
    vector<Admin> temp_admin(1);
    vector<Player> temp_player(1);

	//1.Commencer le bibliothèque du SOCKET, la version est 2.0
	SOCKET sock_ser;  //socket
	WORD w_version_requested;
    WSADATA wsa_data;
	int err;
	w_version_requested = MAKEWORD(2, 0);  //attribuer le valeur à WORD, le premier paramètre est le numéro de version majeure
	                                         //, le deuxième paramètre est le numéro de version secondaire
    err = WSAStartup(w_version_requested, &wsa_data); //Initialiser l'environnement de socket
	if(err != 0)
	{
	    cout<<"Socket2.0 n'est pas initialisé,Exit!";
		return;
	}
	//Tester si Socket est 2.0
	if(LOBYTE(wsa_data.wVersion)!=2 || HIBYTE(wsa_data.wVersion)!=0)
	{
	    WSACleanup();  //quitter socket
		return;
	}

	//2.créer socket
	sock_ser = socket(AF_INET, SOCK_STREAM, 0); //Le premier paramètre: famille d'adresses réseau(maintenant seulement AF_INET(internet) est efficace)
	                               //Le deuxième paramètre: type de réseau protocole, SOCK_DGRAM est UDP, SOCK_STREAM est TCP
	                               //Le troisième paramètre: le protocole spécial de la famille d'adresses réseau spécifié, maintenant il n'est pas utile, et attribué 0
	if(sock_ser == INVALID_SOCKET)              
	{
	    cout<<"Socket ne peut pas être créé, Exit!";
		return;
	}
	sockaddr_in addr_ser;//l'adresse de la destination, définir l'adresse d'envoyer et recevoir les paquets des données
                     //sockaddr_in est égal de sockaddr，il peut être transformer à sockaddr, et on va faire ?à suivant
	addr_ser.sin_addr.S_un.S_addr = addr_ser.sin_addr.s_addr = htonl(INADDR_ANY); 
	    //sin_addr.s_addr est l'adresse IP
	    //transformer à ordre des octets du réseau，INADDR_ANY indique tous les IPs du client                                
	    //Envoyer les données (transformer IP à le type efficace de ulong)
	addr_ser.sin_family = AF_INET;  //sin_family est la famille d'adresse réseau，attribué AF_INET(le type: internetwork)
	addr_ser.sin_port = htons(6000);//sin_port est le port IP, on l'indiquer 6000
	bind(sock_ser,(sockaddr*)&addr_ser,sizeof(sockaddr)); //bind le socket à l'adresse
	listen(sock_ser,5);   //écoter socket
	       
	char prin_str[5000];
	strcpy(prin_str,"**************************************************************\n");
	strcat(prin_str,"* Please input the function you want to achieve(0-10):       *\n");
	strcat(prin_str,"* 1:add a new club                                           *\n");
	strcat(prin_str,"* 2:delete a club                                            *\n");
	strcat(prin_str,"* 3:change the information of a club                         *\n");
	strcat(prin_str,"* 4:enquire the infromation of the clubs                     *\n");
	strcat(prin_str,"* 5:enquire the information of the players                   *\n");
	strcat(prin_str,"* 6:add a new player                                         *\n");
	strcat(prin_str,"* 7:the transfer of the players                              *\n");
	strcat(prin_str,"* 8:print the infomation of all the clubs                    *\n");
	strcat(prin_str,"* 9:print the infomation of all the players                  *\n");
	strcat(prin_str,"* 10:print the infomation of all the administration staff    *\n");
	strcat(prin_str,"* 0:exit the system                                          *\n");
	strcat(prin_str,"**************************************************************\n");
	
	int nlen = sizeof(addr_ser);

	sock_con = accept(sock_ser,(sockaddr*)&addr_cli,&nlen);
	    //attendre la demande de la connection 
		//ce socket est utilisé pour recevoir les donnée 
	char send_print[5000];
	
	while(flag == true)
	{
	    sprintf(send_print,prin_str,inet_ntoa(addr_cli.sin_addr)); 
		    //inet_ntoa transforme addr_cli.sin_addr(IP) à la cha?ne
	        //après l'attribuer à send_str
	    send(sock_con,send_print,strlen(send_print)+1,0); 
		    //Le premier paramètre est Socket, le deuxième est le pointeur du donnée envoyé, le troisième est
	        //la longueur des données envoyés(nombre des octets); le quatrième est le type du transmission, on l'attribue 0
		recv(sock_con,rece_buf,300,0);
		switch(rece_buf[0])
	    {
	        case '1':
				switch(rece_buf[1])
				{
				    case '\0':
                        temp_club[0].add_club();
					break;
					case '0':
                        size = ad.size();
				        for(i = 0; i < size; i++)
						{
					        ad[i].print_info();
						    Sleep(100);
						}
					break;
					default:
					break;
				}
		    break;
		    case '2':
				sprintf(send_buf,"please input the name of club you want to delete\n",
					    inet_ntoa(addr_cli.sin_addr));  
				send(sock_con,send_buf,strlen(send_buf)+1,0); 
				recv(sock_con,rece_buf,300,0);
				strcpy(name_club, rece_buf);
				temp_club[0].delete_club(name_club);
				temp_admin[0].delete_ad(name_club);
                temp_player[0].delete_pl(name_club);
		    break;
		    case '3':
		        sprintf(send_buf,"please input the name of the club you want to change\n",
					    inet_ntoa(addr_cli.sin_addr)); 
				send(sock_con,send_buf,strlen(send_buf)+1,0); 
				recv(sock_con,rece_buf,300,0);
				strcpy(name_club, rece_buf);
				temp_club[0].change_info(name_club);
		    break;
		    case '4':
		        sprintf(send_buf,"please input the name of the club you want to enquire\n",
					    inet_ntoa(addr_cli.sin_addr)); 
				send(sock_con,send_buf,strlen(send_buf)+1,0); 
				recv(sock_con,rece_buf,300,0);
				strcpy(name_club, rece_buf);
				temp_club[0].enquire_info(name_club);
		    break;
		    case '5':
		        temp_player[0].search_player();
			break;
		    case '6':
				sprintf(send_buf,"please input the name of the club you want to enquire\n",
					    inet_ntoa(addr_cli.sin_addr)); 
				send(sock_con,send_buf,strlen(send_buf)+1,0); 
				recv(sock_con,rece_buf,300,0);
				strcpy(name_club, rece_buf);
				strcpy(info, "please input the type of the member you want to add\n");
				strcat(info, "1:add an adminstration staff\n");
				strcat(info, "2:add a football player\n");
				sprintf(send_buf,info,inet_ntoa(addr_cli.sin_addr)); 
				send(sock_con,send_buf,strlen(send_buf)+1,0); 
				recv(sock_con,rece_buf,300,0);
				strcpy(type, rece_buf);
				size = cl.size();
				if(type[0] == '1')
				{
	                temp_admin[0].add_ad(name_club);
			        for(int i = 0; i < size; i++)
			        {
				        if(cl[i].compare_name(name_club) == 0)
				        {
					        cl[i].add_number();
				            i = size;   
				        }
			        }
				}
				else if(type[0] == '2')
				{
					temp_player[0].add_pl(name_club);
			        for(int i = 0; i < size; i++)
			        {
				        if(cl[i].compare_name(name_club) == 0)
				        {
					        cl[i].add_number();
				            i = size;   
				        }
			        }
				}
				else
				{
					sprintf(send_buf,"sorry, you input the wrong type\n",
					    inet_ntoa(addr_cli.sin_addr)); 
				    send(sock_con,send_buf,strlen(send_buf)+1,0); 
				}
		    break;
		    case '7':
		        sprintf(send_buf,"please input the name of the player you want to transfer\n",
					    inet_ntoa(addr_cli.sin_addr)); 
				send(sock_con,send_buf,strlen(send_buf)+1,0); 
				recv(sock_con,rece_buf,300,0);
				strcpy(tran_name, rece_buf);
				sprintf(send_buf,"please input the name of the club which want the player\n",
					    inet_ntoa(addr_cli.sin_addr)); 
				send(sock_con,send_buf,strlen(send_buf)+1,0); 
				recv(sock_con,rece_buf,300,0);
				strcpy(name_club, rece_buf);
				temp_club[0].transfer(tran_name, name_club);
		    break;
			case '8':
				size = cl.size();
				for(i = 0; i < size; i++)
				{
					cl[i].print_simple();
				    Sleep(200);
				}
			break;
			case '9':
				size = pl.size();
				for(i = 0; i < size; i++)
				{
					pl[i].print_info();
					Sleep(100);
				}
			break;
		    default:
				flag = false;
		    break;
		}
		Sleep(500);
    }
	if(!closesocket(sock_con))  //fermer le socket
    {
        WSAGetLastError();     //lire les codes d'erreur
        return;
	}
	if(!closesocket(sock_ser))  
	{
	    WSAGetLastError();    
		return;
	}
	if(!WSACleanup())   //fermer le bibliothèque et quitter l'environnement de socket
	{
	    WSAGetLastError();
		return;
	}
}
void read_file()    //écrire le fichier dans le mémoire
{
    char temp, club_name[50], admin_club[50], admin_name[50], player_club[50], player_name[50];
	int i=0;
	int size_cl=0, size_ad=0, size_pl=0;
	int club_fund=0, club_number=0;
	int admin_age=0, admin_power=0, admin_salary=0;
	int player_age=0, player_power=0, player_salary=0, player_goal=0, player_tran_fee=0, player_year=0;
	vector<Club> io_club(1);
    vector<Admin> io_admin(1);
    vector<Player> io_player(1);

	file_in.open("club.txt", ios_base::in);  
	if(!file_in)
	    cout<<"can't open the file\n";

	file_in>>size_cl;
	file_in>>size_ad;
	file_in>>size_pl;
	int flag=1;
	while(flag == 1)    //lire le fichier, l'écrire dans le mémoire
	{
	    for(i = 0; i < size_cl; i++)  
		{
		    file_in>>club_name;
			io_club[0].change_name(club_name);

		    file_in>>club_fund;
			io_club[0].change_fund(club_fund);

			file_in>>club_number;
			io_club[0].change_number(club_number);
			
			cl.push_back(io_club[0]);
		}
		for(i = 0; i < size_ad; i++) 
		{
		    file_in>>admin_club;
			io_admin[0].change_club(admin_club);
			
			file_in>>admin_name;
			io_admin[0].change_name(admin_name);
			
			file_in>>admin_age;
			io_admin[0].change_age(admin_age);
			
			file_in>>admin_power;
            io_admin[0].change_power(admin_power);

			file_in>>admin_salary;
            io_admin[0].change_salary(admin_salary);

			ad.push_back(io_admin[0]);
		}
		for(i = 0; i < size_pl; i++) 
		{
		    file_in>>player_club;
			io_player[0].change_club(player_club);
			
			file_in>>player_name;
			io_player[0].change_name(player_name);

			file_in>>player_age;
			io_player[0].change_age(player_age);

            file_in>>player_power;
			io_player[0].change_power(player_power);

			file_in>>player_salary;
			io_player[0].change_salary(player_salary);

			file_in>>player_goal;
			io_player[0].change_goal(player_goal);

			file_in>>player_tran_fee;
			io_player[0].change_tran_fee(player_tran_fee);

			file_in>>player_year;
			io_player[0].change_year(player_year);

			pl.push_back(io_player[0]);
		}
		flag = 0; 
	}

	file_in.close();   
}
void write_file()      //écrire les informations dans le fichier 
{
    int i=0, size_cl=0, size_ad=0, size_pl=0;

	file_out.open("club.txt", ios_base::out); 
	if(!file_in)
		cout<<"can't open the file\n";

	size_cl = cl.size();
	size_ad = ad.size();
	size_pl = pl.size();

	file_out<<size_cl<<endl;
	file_out<<size_ad<<endl;
	file_out<<size_pl<<endl;
	//écrire les informations de mémoire dans le fichier selon le format fixe
	for(i = 0; i < size_cl; i++)
	{
		file_out<<cl[i].get_name()<<endl;
		file_out<<cl[i].get_fund()<<endl;
		file_out<<cl[i].get_number()<<endl;
	}
	for(i = 0; i < size_ad; i++)
	{
		file_out<<ad[i].get_club()<<endl;
		file_out<<ad[i].get_name()<<endl;
		file_out<<ad[i].get_age()<<endl;
		file_out<<ad[i].get_power()<<endl;
		file_out<<ad[i].get_salary()<<endl;
	}
	for(i = 0; i < size_pl; i++)
	{
		file_out<<pl[i].get_club()<<endl;
		file_out<<pl[i].get_name()<<endl;
		file_out<<pl[i].get_age()<<endl;
		file_out<<pl[i].get_power()<<endl;
		file_out<<pl[i].get_salary()<<endl;
		file_out<<pl[i].get_goal()<<endl;
		file_out<<pl[i].get_tran_fee()<<endl;
		file_out<<pl[i].get_year()<<endl;
	}
	file_out.close();
}
int main()  
{
	read_file();    //Appler le function pour écrire le fichier dans le mémoire

	srand(time(NULL));  //générer la graine aléatoire
	menu();  

	write_file();//Appler le function pour écrire les informations du mémoire  dans le fichier 

	system("pause");
	return 0;
}
