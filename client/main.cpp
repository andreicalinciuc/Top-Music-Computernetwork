//
// Created by andrei on 22.12.2018.
//

/* cliTCPIt.c - Exemplu de client TCP
   Trimite un numar la server; primeste de la server numarul incrementat.

   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include "../Tools/Write&Read.h"
/* codul de eroare returnat de anumite apeluri */
using namespace std;
/* portul de conectare la server*/
int port;

void quit(char input[1024]);

int main(int argc, char *argv[]) {
    int sd;            // descriptorul de socket
    struct sockaddr_in server;    // structura folosita pentru conectare
    // mesajul trimis
    char input[1024];
    char recive[1024];
    long size_recive;
    int comanda;
    int type_user = 3;
    bool connected;
    /* exista toate argumentele in linia de comanda? */
    if (argc != 3) {
        printf("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
        return -1;
    }
    /* stabilim portul */
    port = atoi(argv[2]);

    /* cream socketul */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Eroare la socket().\n");
        return errno;
    }

    /* umplem structura folosita pentru realizarea conexiunii cu serverul */
    /* familia socket-ului */
    server.sin_family = AF_INET;
    /* adresa IP a serverului */
    server.sin_addr.s_addr = inet_addr(argv[1]);
    /* portul de conectare */
    server.sin_port = htons(port);

    /* ne conectam la server */

    if (connect(sd, (struct sockaddr *) &server, sizeof(struct sockaddr)) == -1) {
        perror("[client]Eroare la connect().\n");
        return errno;
    }
    citirecomanda:
    do {
        cout << "0.user" << endl;
        cout << "1.admin" << endl;
        cout << "comanda: ";
        cin >> type_user;
    } while (type_user > 1);

    string type_send;
    if (type_user == 0)
        type_send = "0";
    else
        type_send = "1";

    Write(sd, type_send);
    while (1) {
        /* citirea mesajului */


        switch (type_user) {


            case 0 : {
                top:
                cout<<endl;
                cout << "0.Inregistrare" << endl;
                cout << "1.Login" << endl;
                cout << "2.Top" << endl;
                cout << "3.Top-gen" << endl;
                cout << "4.Quit" << endl;

                fflush(stdout);
                do {
                    cout << "Introduceti numarul  comenzii: ";
                    cin >> comanda;
                    cout << endl;

                } while (comanda > 4 || comanda < 0);


                switch (comanda) {

                    case 0: {
                        char name[1024] = "\0";
                        char pass[1024] = "\0";
                        strcpy(input, "0");
                        Write(sd, input);

                        cout << "Introduceti numele contului dorit: ";
                        cin >> name;
                        Write(sd, name);

                        cout << endl << "Introduceti parola dorita: ";
                        cin >> pass;
                        Write(sd, pass);

                        goto top;

                    }

                    case 1: {
                        char name[1024] = "\0";
                        char pass[1024] = "\0";
                        strcpy(input, "1");
                        Write(sd, input);

                        cout << "Introduceti numele contului: ";
                        cin >> name;
                        Write(sd, name);

                        cout << endl << "Introduceti parola: ";
                        cin >> pass;
                        Write(sd, pass);
                        read(sd, &size_recive, sizeof(size_recive));
                        bzero(recive, size_recive + 1);
                        read(sd, recive, size_recive);
                        connected = atoi(recive);
                        switch (connected) {
                            case 0: {
                                cout << "erroare la connetare" << endl;
                                goto top;

                            }
                            case 1: {

                                cout << "Bine ai venit " << name <<"!"<<endl;
                                top_usr_connected:
                                cout<<endl;
                                cout << "2.Top" << endl;
                                cout << "3.Top-gen" << endl;
                                cout << "4.Quit" << endl;
                                cout << "5.Vote" << endl;
                                cout << "6.Comment" << endl;
                                cout << "7.Search" << endl;
                                cout << "8.Deconectare" << endl;
                                cout <<"9.Lista Melodii"<<endl;

                                do {
                                    cout << "Introduceti numarul  comenzii: ";
                                    cin >> comanda;
                                    cout << endl;

                                } while (comanda > 9 || comanda < 0);

                                switch (comanda) { //top
                                    case 2: {
                                        char nume_melodie[1024] = "\0";
                                        int nr_vot = 0;
                                        strcpy(input, "2");
                                        Write(sd, input);
                                        int size_vec = 0;
                                        read(sd, &size_vec, sizeof(size_vec));
                                        for (int i = 0; i < size_vec; i++) {

                                            if (read(sd, &size_recive, sizeof(size_recive)) <= 0) {
                                                perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                                                break;

                                            }
                                            bzero(&nume_melodie, size_recive + 1);

                                            if (read(sd, nume_melodie, size_recive) <= 0) {
                                                perror("Eroare la read() de la client.\n");
                                                break;

                                            }
                                            if (read(sd, &nr_vot, sizeof(nr_vot)) <= 0) {
                                                perror("Eroare la read() de la dimesiune la nr voturi\n");
                                                break;

                                            }
                                            cout << "Locul " << i + 1 << " " << nume_melodie << " cu un numar de "
                                                 << nr_vot << " voturi" << endl;


                                        }

                                        goto top_usr_connected;

                                    }
                                    case 3 : {
                                        char nume_melodie[1024] = "\0";
                                        char gen_send[1024] = "\0";
                                        int nr_vot = 0;
                                        strcpy(input, "3");
                                        Write(sd, input);
                                        int size_vec = 0;
                                        cout<<"Introduceti genul muzica:";
                                        cin >> gen_send;
                                        cout<<endl;
                                        Write(sd, gen_send);
                                        read(sd, &size_vec, sizeof(size_vec));
                                        for (int i = 0; i < size_vec; i++) {

                                            if (read(sd, &size_recive, sizeof(size_recive)) <= 0) {
                                                perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                                                break;

                                            }
                                            bzero(&nume_melodie, size_recive + 1);

                                            if (read(sd, nume_melodie, size_recive) <= 0) {
                                                perror("Eroare la read() de la client.\n");
                                                break;

                                            }
                                            if (read(sd, &nr_vot, sizeof(nr_vot)) <= 0) {
                                                perror("Eroare la read() de la dimesiune la nr voturi\n");
                                                break;

                                            }
                                            cout << "Locul " << i + 1 << " " << nume_melodie << "  " << nr_vot << endl;


                                        }
                                        goto top_usr_connected;

                                    }


                                    case 4: { //quit
                                        strcpy(input, "4");
                                        Write(sd, input);
                                        Close(sd);
                                        return 0;
                                    }

                                    case 5: { //votat melodie
                                        strcpy(input, "5");
                                        Write(sd, input);
                                        char name_song[1024] = "\0";
                                        cout << "Introdu numele melodiei care doresti sa o votezi: ";
                                        cin.get();
                                        cin.getline(name_song,sizeof(name_song));
                                        Write(sd, name_song);

                                        if (read(sd, &size_recive, sizeof(size_recive)) <= 0) {
                                            perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                                            break;

                                        }
                                        bzero(&input, size_recive + 1);

                                        if (read(sd, input, size_recive) <= 0) {
                                            perror("Eroare la read() de la client.\n");
                                            break;

                                        }

                                        if (strcmp(input,"0")==0)
                                        {
                                            cout << "Nu s-a votat!" << endl;
                                        }
                                        else
                                            cout << "S-a votat melodia:" << name_song << endl;
                                            cout<<endl;
                                        goto top_usr_connected;


                                    }
                                    case 6:{
                                        strcpy(input, "6");
                                        Write(sd, input);
                                        char comentariu[1024];

                                        char name_song[1024];
                                        cout<<"Introdu numele melodiei la care doresti sa lasi comentariu: ";
                                        bzero(name_song,1024);
                                        cin.get();
                                        cin.getline(name_song,sizeof(name_song));
                                        cout<<name_song<<endl;
                                        cout<<endl;
                                        Write(sd,name_song);
                                        cout<<"Introdu comentariu: ";
                                        bzero(comentariu,1024);
                                        cin.getline(comentariu,sizeof(comentariu));
                                        cout<<comentariu<<endl;

                                        cout<<endl;
                                        Write(sd,comentariu);
                                        goto top_usr_connected;



                                    }
                                    case 7:{ //search song
                                        strcpy(input, "7");
                                        Write(sd, input);
                                        char name_song[1024]="\0";
                                        char comentarii[1024]="\0";
                                        char name_sg[1024]="\0";
                                        char link_youtube[1024] = "\0";
                                        char gen[1024] = "\0";
                                        char descriere[1024] = "\0";
                                        int numar_vot=0;
                                        char comentariu[1024]="\0";
                                        cout<<"Introdu numele melodiei:";
                                        cin.get();
                                        cin.getline(name_sg,sizeof(name_sg));
                                        cout<<endl;
                                        Write(sd,name_sg);

                                        //namesong
                                        if (read(sd, &size_recive, sizeof(size_recive)) <= 0) {
                                            perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                                            break;

                                        }
                                        bzero(&name_song, size_recive + 1);

                                        if (read(sd, name_song, size_recive) <= 0) {
                                            perror("Eroare la read() de la client.\n");
                                            break;

                                        }
                                        //descriere
                                        if (read(sd, &size_recive, sizeof(size_recive)) <= 0) {
                                            perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                                            break;

                                        }
                                        bzero(descriere, size_recive + 1);

                                        if (read(sd, descriere, size_recive) <= 0) {
                                            perror("Eroare la read() de la client.\n");
                                            break;

                                        }
                                        //link
                                        if (read(sd, &size_recive, sizeof(size_recive)) <= 0) {
                                            perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                                            break;

                                        }
                                        bzero(link_youtube, size_recive + 1);

                                        if (read(sd, link_youtube, size_recive) <= 0) {
                                            perror("Eroare la read() de la client.\n");
                                            break;

                                        }
                                        //nr_voturi

                                        if (read(sd, &numar_vot, sizeof(numar_vot)) <= 0) {
                                            perror("Eroare la read() de la client.\n");
                                            break;

                                        }

                                        cout<<"Nume melodie:"<< name_song<<endl;
                                        cout<< "Link YouTube:"<< link_youtube<<endl;
                                        cout<<"Descriere:"<< descriere <<endl;
                                        cout<< "Comentarii:"<< comentarii<<endl;
                                        cout<<"Numar de voturi:"<<  numar_vot<<endl;

                                        //commentarii
                                        int nr_comm=0;
                                        read(sd,&nr_comm, sizeof(nr_comm));
                                        cout<<"Comentarii: -----------------"<<endl;
                                        cout<<nr_comm<<endl;
                                        for (int i = 0; i < nr_comm; i++) {
                                            if (read(sd, &size_recive, sizeof(size_recive)) <= 0) {
                                                perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                                                break;

                                            }
                                            bzero(comentariu, size_recive + 1);

                                            if (read(sd, comentariu, size_recive) <= 0) {
                                                perror("Eroare la read() de la client.\n");
                                                break;

                                            }
                                            cout<<"          -" <<comentariu<<endl;

                                        }
                                        cout<<"             -----------------"<<endl<<endl;


                                        //genuri
                                        int nr_genuri=0;
                                        read(sd,&nr_genuri, sizeof(nr_genuri));
                                       cout<<"Gen: -----------------"<<endl;
                                        for (int i = 0; i < nr_genuri; i++) {
                                            if (read(sd, &size_recive, sizeof(size_recive)) <= 0) {
                                                perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                                                break;

                                            }
                                            bzero(gen, size_recive + 1);

                                            if (read(sd, gen, size_recive) <= 0) {
                                                perror("Eroare la read() de la client.\n");
                                                break;

                                            }
                                            cout<<"          -" <<gen<<endl;

                                        }
                                        cout<<"     -----------------"<<endl;




                                        goto top_usr_connected;

                                    }
                                    case 8: {
                                        strcpy(input, "8");

                                        Write(sd, input);
                                        goto citirecomanda;
                                    }
                                    case 9: {  // lista membrii
                                            int size_songs= 0;
                                            strcpy(input, "9");
                                            Write(sd, input);

                                            read(sd, &size_songs, sizeof(size_songs));
                                            cout << "Lista Melodii" << endl << "-------------------" << endl;
                                            for (int i = 0; i < size_songs; i++) {
                                                if (read(sd, &size_recive, sizeof(size_recive)) <= 0) {
                                                    perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                                                    break;

                                                }
                                                bzero(&input, size_recive + 1);

                                                if (read(sd, input, size_recive) <= 0) {
                                                    perror("Eroare la read() de la client.\n");
                                                    break;

                                                }

                                                cout << i << ". " << input << endl;
                                            }
                                            cout << "-------------------";
                                            cout << endl << endl;
                                            goto  top_usr_connected;
                                        }

                                }


                            }

                        }

                    }
                    case 2: {

                        char nume_melodie[1024] = "\0";
                        int nr_vot = 0;
                        strcpy(input, "2");
                        Write(sd, input);
                        int size_vec = 0;
                        read(sd, &size_vec, sizeof(size_vec));
                        for (int i = 0; i < size_vec; i++) {

                            if (read(sd, &size_recive, sizeof(size_recive)) <= 0) {
                                perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                                break;

                            }
                            bzero(&nume_melodie, size_recive + 1);

                            if (read(sd, nume_melodie, size_recive) <= 0) {
                                perror("Eroare la read() de la client.\n");
                                break;

                            }
                            if (read(sd, &nr_vot, sizeof(nr_vot)) <= 0) {
                                perror("Eroare la read() de la dimesiune la nr voturi\n");
                                break;

                            }
                            cout << "Locul " << i + 1 << " " << nume_melodie << " cu un numar de " << nr_vot
                                 << " voturi" << endl;


                        }


                        goto top;
                    }
                    case 3 : {
                        char nume_melodie[1024] = "\0";
                        char gen_send[1024] = "\0";
                        int nr_vot = 0;
                        strcpy(input, "3");
                        Write(sd, input);
                        int size_vec = 0;
                        cin >> gen_send;
                        Write(sd, gen_send);
                        read(sd, &size_vec, sizeof(size_vec));
                        for (int i = 0; i < size_vec; i++) {

                            if (read(sd, &size_recive, sizeof(size_recive)) <= 0) {
                                perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                                break;

                            }
                            bzero(&nume_melodie, size_recive + 1);

                            if (read(sd, nume_melodie, size_recive) <= 0) {
                                perror("Eroare la read() de la client.\n");
                                break;

                            }
                            if (read(sd, &nr_vot, sizeof(nr_vot)) <= 0) {
                                perror("Eroare la read() de la dimesiune la nr voturi\n");
                                break;

                            }
                            cout << "Locul " << i + 1 << " " << nume_melodie << "  " << nr_vot << endl;


                        }
                        goto top;

                    }

                    case 4: {
                        strcpy(input, "4");
                        Write(sd, input);
                        Close(sd);
                        return 0;
                    }
                    case 8: {
                        goto citirecomanda;
                    }

                    default:
                        cout << "comanda gresita,introdua alta" << endl;
                        goto top;
                }


            }
            case 1: {
                top_adm:
                cout << "0.Inregistrare" << endl;
                cout << "1.Login" << endl;
                cout << "2.Top" << endl;
                cout << "3.Top-gen" << endl;
                cout << "4.Quit" << endl;


                fflush(stdout);
                do {
                    cout << "Introduceti numarul  comenzii: ";
                    cin >> comanda;
                    cout << comanda << endl;
                    cout << endl;

                } while (comanda > 8 || comanda < 0);

                switch (comanda) {

                    case 0: {  //inregistrare
                        char name[1024] = "\0";
                        char pass[1024] = "\0";
                        char key[1024] = "\0";
                        strcpy(input, "0");
                        Write(sd, input);

                        cout << "Introduceti numele contului dorit: ";
                        cin >> name;
                        Write(sd, name);

                        cout << endl << "Introduceti parola dorita: ";
                        cin >> pass;
                        Write(sd, pass);
                        cout << endl << "Introduceti key-ul secret: ";
                        cin >> key;
                        Write(sd, key);

                        goto top_adm;

                    }

                    case 1: {  //login
                        char name[1024] = "\0";
                        char pass[1024] = "\0";
                        char key[1024] = "\0";
                        strcpy(input, "1");
                        Write(sd, input);
                        cin.get();
                        cout << "Introduceti numele contului: ";

                        cin.getline(name, sizeof(name));
                        Write(sd, name);

                        cout << endl << "Introduceti parola: ";
                        cin.getline(pass, sizeof(pass));
                        Write(sd, pass);

                        cout << endl << "Introduceti key: ";
                        cin.getline(key, sizeof(key));
                        Write(sd, key);

                        read(sd, &size_recive, sizeof(size_recive));
                        bzero(recive, size_recive + 1);
                        read(sd, recive, size_recive);
                        connected = atoi(recive);
                        switch (connected) {
                            case 0: {
                                cout << "erroare la connetare" << endl << endl;
                                goto top_adm;

                            }
                            case 1: {

                                cout << "Bine ai venit " << name << endl;
                                top_adm_connected:

                                cout << "2.Top" << endl;
                                cout << "3.Top-gen" << endl;
                                cout << "4.Quit" << endl;
                                cout << "5.Mute user" << endl;
                                cout << "6.Unmute user" << endl;
                                cout << "7.List users" << endl;
                                cout << "8.Add song" << endl;
                                cout << "9.Deconectare" << endl;
                                cout << "10.Delete user" << endl;

                                fflush(stdout);
                                do {
                                    cout << "Introduceti numarul  comenzii: ";
                                    cin >> comanda;
                                    cout << endl;

                                } while (comanda > 10 || comanda < 0);

                                switch (comanda) {

                                    case 2: {  //top
                                        char nume_melodie[1024] = "\0";
                                        int nr_vot = 0;
                                        strcpy(input, "2");
                                        Write(sd, input);
                                        int size_vec = 0;
                                        read(sd, &size_vec, sizeof(size_vec));
                                        for (int i = 0; i < size_vec; i++) {

                                            if (read(sd, &size_recive, sizeof(size_recive)) <= 0) {
                                                perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                                                break;

                                            }
                                            bzero(&nume_melodie, size_recive + 1);

                                            if (read(sd, nume_melodie, size_recive) <= 0) {
                                                perror("Eroare la read() de la client.\n");
                                                break;

                                            }
                                            if (read(sd, &nr_vot, sizeof(nr_vot)) <= 0) {
                                                perror("Eroare la read() de la dimesiune la nr voturi\n");
                                                break;

                                            }
                                            cout << "Locul " << i + 1 << " " << nume_melodie << " cu un numar de "
                                                 << nr_vot << " voturi" << endl;


                                        }
                                        break;
                                    }

                                    case 3 : { //top gen
                                        char nume_melodie[1024] = "\0";
                                        char gen_send[1024] = "\0";
                                        int nr_vot = 0;
                                        strcpy(input, "3");
                                        Write(sd, input);
                                        int size_vec = 0;
                                        cin >> gen_send;
                                        Write(sd, gen_send);
                                        read(sd, &size_vec, sizeof(size_vec));
                                        for (int i = 0; i < size_vec; i++) {

                                            if (read(sd, &size_recive, sizeof(size_recive)) <= 0) {
                                                perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                                                break;

                                            }
                                            bzero(&nume_melodie, size_recive + 1);

                                            if (read(sd, nume_melodie, size_recive) <= 0) {
                                                perror("Eroare la read() de la client.\n");
                                                break;

                                            }
                                            if (read(sd, &nr_vot, sizeof(nr_vot)) <= 0) {
                                                perror("Eroare la read() de la dimesiune la nr voturi\n");
                                                break;

                                            }
                                            cout << "Locul " << i + 1 << " " << nume_melodie << "  " << nr_vot << endl;


                                        }
                                        break;
                                    }


                                    case 4: { //quit
                                        strcpy(input, "4");
                                        Write(sd, input);
                                        Close(sd);
                                        return 0;
                                    }
                                    case 5: {//mute
                                        strcpy(input, "5");
                                        Write(sd, input);
                                        cout << "introdu numele userului caruia doresti sa ii dai mute: ";
                                        char mute_user[1024] = "\0";
                                        cin >> mute_user;
                                        Write(sd, mute_user);

                                        break;
                                    }
                                    case 6: {//mute
                                        strcpy(input, "6");
                                        Write(sd, input);
                                        cout << "introdu numele userului caruia doresti sa ii dai unmute: ";
                                        char unmute_user[1024] = "\0";
                                        cin >> unmute_user;
                                        Write(sd, unmute_user);
                                        break;

                                    }
                                    case 7: { // lista membrii
                                        int size_memb = 0;
                                        strcpy(input, "7");
                                        Write(sd, input);

                                        read(sd, &size_memb, sizeof(size_memb));
                                        cout << "Lista membrii" << endl << "-------------------" << endl;
                                        for (int i = 0; i < size_memb; i++) {
                                            if (read(sd, &size_recive, sizeof(size_recive)) <= 0) {
                                                perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                                                break;

                                            }
                                            bzero(&input, size_recive + 1);

                                            if (read(sd, input, size_recive) <= 0) {
                                                perror("Eroare la read() de la client.\n");
                                                break;

                                            }

                                            cout << i << ". " << input << endl;
                                        }
                                        cout << "-------------------";
                                        cout << endl << endl;

                                        break;
                                    }

                                    case 8: {  //add song
                                        strcpy(input, "8");
                                        Write(sd, input);
                                        char name_song[1024] = "\0";
                                        string names;
                                        string link;
                                        string desc;
                                        char link_youtube[1024] = "\0";
                                        char gen[1024] = "\0";
                                        char descriere[1024] = "\0";
                                        char nr_gen[1024] = "\0";
                                            cin.get();
                                        cout << "Numele melodiei: ";
                                        cin.getline(name_song,sizeof(name_song));
                                        Write(sd, name_song);

                                        cout << endl << "Link youtube: ";
                                        cin.getline(link_youtube,sizeof(link_youtube));
                                        Write(sd, link_youtube);

                                        cout << "Descriere melodie: ";
                                        cin.getline(descriere,sizeof(descriere));
                                        Write(sd, descriere);
                                        fflush(stdout);

                                        cout << endl << "Numar de genuri care apartine melodia: ";
                                        cin >> nr_gen;


                                        Write(sd, nr_gen);

                                        int nr_g = atoi(nr_gen);
                                        for (int i = 0; i < nr_g; i++) {
                                            cout << "Genul melodiei: ";
                                            cin >> gen;
                                            Write(sd, gen);
                                            cout << endl;
                                        }
                                        break;
                                    }
                                    case 9: {  //deconectare
                                        strcpy(input, "9");

                                        Write(sd, input);
                                        goto citirecomanda;
                                    }

                                    case 10: {
                                        strcpy(input, "10");
                                        Write(sd, input);
                                        cout << "Introdu numele userului care doresti sa stergi: ";
                                        char unmute_user[1024] = "\0";
                                        cin >> unmute_user;
                                        Write(sd, unmute_user);
                                        break;

                                    }

                                    default:
                                        break;
                                }

                                goto top_adm_connected;


                            }

                        }

                    }
                    case 2: {
                        char nume_melodie[1024] = "\0";
                        int nr_vot = 0;
                        strcpy(input, "2");
                        Write(sd, input);
                        int size_vec = 0;
                        read(sd, &size_vec, sizeof(size_vec));
                        for (int i = 0; i < size_vec; i++) {

                            if (read(sd, &size_recive, sizeof(size_recive)) <= 0) {
                                perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                                break;

                            }
                            bzero(&nume_melodie, size_recive + 1);

                            if (read(sd, nume_melodie, size_recive) <= 0) {
                                perror("Eroare la read() de la client.\n");
                                break;

                            }
                            if (read(sd, &nr_vot, sizeof(nr_vot)) <= 0) {
                                perror("Eroare la read() de la dimesiune la nr voturi\n");
                                break;

                            }
                            cout << "Locul " << i + 1 << " " << nume_melodie << " cu un numar de " << nr_vot
                                 << " voturi" << endl;


                        }
                        break;
                    }
                    case 3 : {
                        char nume_melodie[1024] = "\0";
                        char gen_send[1024] = "\0";
                        int nr_vot = 0;
                        strcpy(input, "3");
                        Write(sd, input);
                        int size_vec = 0;
                        cin >> gen_send;
                        Write(sd, gen_send);
                        read(sd, &size_vec, sizeof(size_vec));
                        for (int i = 0; i < size_vec; i++) {

                            if (read(sd, &size_recive, sizeof(size_recive)) <= 0) {
                                perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                                break;

                            }
                            bzero(&nume_melodie, size_recive + 1);

                            if (read(sd, nume_melodie, size_recive) <= 0) {
                                perror("Eroare la read() de la client.\n");
                                break;

                            }
                            if (read(sd, &nr_vot, sizeof(nr_vot)) <= 0) {
                                perror("Eroare la read() de la dimesiune la nr voturi\n");
                                break;

                            }
                            cout << "Locul " << i + 1 << " " << nume_melodie << "  " << nr_vot << endl;


                        }
                        break;
                    }

                    case 4: {
                        strcpy(input, "4");
                        Write(sd, input);
                        Close(sd);
                        return 0;
                    }

                    default:
                        cout << "comanda gresita,introdua alta" << endl;
                        goto top;


                }
            }

        }


    }
}

