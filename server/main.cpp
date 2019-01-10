//
// Created by andrei on 22.12.2018.
//
/* servTCPConcTh2.c - Exemplu de server TCP concurent care deserveste clientii
   prin crearea unui thread pentru fiecare client.
   Asteapta un numar de la clienti si intoarce clientilor numarul incrementat.
	Intoarce corect identificatorul din program al thread-ului.


   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include "../Tools/Write&Read.h"
#include "json.hpp"
#include <fstream>
#include <iomanip>
#include <mutex>

std::mutex mu;
/* portul folosit */
#define PORT 2908
using namespace std;
using json = nlohmann::json;

typedef struct thData {
    int idThread; //id-ul thread-ului tinut in evidenta de acest program
    int cl; //descriptorul intors de accept
} thData;

struct musics {
    string name;
    int nr_vot;
    string gen;
} music[1000];

void selectionSort(musics Vector[], int N) {
    for (int i = 0; i < N - 1; i++) {
        for (int j = i + 1; j < N; j++) {
            if (music[i].nr_vot < music[j].nr_vot) {
                musics aux = music[i];
                music[i] = music[j];
                music[j] = aux;
            }
        }
    }
}


static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
void raspunde(void *);

int main() {
    struct sockaddr_in server;    // structura folosita de server
    struct sockaddr_in from;
    int sd;        //descriptorul de socket
    pthread_t th[100];    //Identificatorii thread-urilor care se vor crea
    int i = 0;


    /* crearea unui socket */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("[server]Eroare la socket().\n");
        return errno;
    }
    /* utilizarea optiunii SO_REUSEADDR */
    int on = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    /* pregatirea structurilor de date */
    bzero(&server, sizeof(server));
    bzero(&from, sizeof(from));

    /* umplem structura folosita de server */
    /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;
    /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    /* utilizam un port utilizator */
    server.sin_port = htons(PORT);

    /* atasam socketul */
    if (bind(sd, (struct sockaddr *) &server, sizeof(struct sockaddr)) == -1) {
        perror("[server]Eroare la bind().\n");
        return errno;
    }

    /* punem serverul sa asculte daca vin clienti sa se conecteze */
    if (listen(sd, 5) == -1) {
        perror("[server]Eroare la listen().\n");
        return errno;
    }
    /* servim in mod concurent clientii...folosind thread-uri */
    while (1) {
        int client;
        thData *td; //parametru functia executata de thread
        int length = sizeof(from);

        printf("[server]Asteptam la portul %d...\n", PORT);
        fflush(stdout);

        // client= malloc(sizeof(int));
        /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
        if ((client = accept(sd, (struct sockaddr *) &from, (socklen_t *) &length)) < 0) {
            perror("[server]Eroare la accept().\n");
            continue;
        }

        /* s-a realizat conexiunea, se astepta mesajul */

        // int idThread; //id-ul threadului
        // int cl; //descriptorul intors de accept

        td = (struct thData *) malloc(sizeof(struct thData));
        td->idThread = i++;
        td->cl = client;

        pthread_create(&th[i], NULL, &treat, td);

    }//while
};

// nu ma folosit threaduri. nu stiu de lace vine asta
static void *treat(void *arg) {


    struct thData tdL;
    tdL = *((struct thData *) arg);

    printf("[thread]- %d - Asteptam mesajul...\n", tdL.idThread);
    fflush(stdout);
    pthread_detach(pthread_self());


    raspunde((struct thData *) arg);


    /* am terminat cu acest client, inchidem conexiunea */

};


void raspunde(void *arg) {
    long size_recive = 0;
    long size_send = 0;
    int i = 0;
    char recive[1024];
    int size = 0;
    struct thData tdL;
    tdL = *((struct thData *) arg);
    char type_user[1024];


    citireusr:
    if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
        printf("[Thread %d]\n", tdL.idThread);
        perror("Eroare la read() de la dimesiune\n");


    }
    bzero(&type_user, size_recive + 1);

    if (read(tdL.cl, &type_user, size_recive) <= 0) {
        printf("[Thread %d]\n", tdL.idThread);
        perror("Eroare la read() de la client.\n");

    }
    int usr = atoi(type_user);

    switch (usr) {
        case 0: {
            cout << "Suntem in user" << endl;
            while (1) {
                size_recive = 0;
                size_send = 0;

                using json = nlohmann::json;
                ifstream file("../file.json");
                json j;
                file >> j;


                if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                    printf("[Thread %d]\n", tdL.idThread);
                    perror("Eroare la read() de la dimesiune\n");
                    break;


                }
                bzero(&recive, size_recive + 1);

                if (read(tdL.cl, &recive, size_recive) <= 0) {
                    printf("[Thread %d]\n", tdL.idThread);
                    perror("Eroare la read() de la client.\n");
                    break;

                }

                cout << "Am primit comanda " << recive << " de la [Thread]: " << tdL.idThread << endl;
                int comanda = atoi(recive);

                switch (comanda) {
                    case 0: {
                        char name[1024] = "\0";
                        char pass[1024] = "\0";
                        if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                            break;

                        }
                        bzero(&name, size_recive + 1);

                        if (read(tdL.cl, &name, size_recive) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la client.\n");
                            break;

                        }


                        if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la dimesiune la Inregistrare-pass\n");
                            break;

                        }
                        bzero(&pass, size_recive + 1);

                        if (read(tdL.cl, &pass, size_recive) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la client.\n");
                            break;

                        }

                        mu.lock();
                        json x;
                        x["name"] = name;
                        x["pass"] = pass;
                        x["drept de vot"] = 1;
                        j["users"].push_back(x);
                        ofstream o("../file.json");
                        o << setw(4) << j << endl;
                        mu.unlock();
                        cout << "Am inregistrat pe -" << name << "- cu parola -" << pass << "-" << endl;
                        break;
                    }
                    case 1: {
                        char name[1024] = "\0";
                        char pass[1024] = "\0";
                        if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                            break;

                        }
                        bzero(&name, size_recive + 1);

                        if (read(tdL.cl, &name, size_recive) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la client.\n");
                            break;

                        }

                        if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la dimesiune la Inregistrare-pass\n");
                            break;

                        }
                        bzero(&pass, size_recive + 1);

                        if (read(tdL.cl, &pass, size_recive) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la client.\n");
                            break;

                        }
                        string ok = "0";
                        ifstream file("../file.json");
                        json login = json::parse(file);
                        for (auto logi:login["users"]) {
                            string usersname = logi["name"];
                            string password = logi["pass"];
                            if (pass == password && name == usersname) {
                                ok = "1";
                                break;
                            }

                        }
                        Write(tdL.cl, ok);
                        break;

                    }
                    case 2: { //top
                        mu.lock();
                        ifstream file("../file.json");
                        json login = json::parse(file);
                        int j = 0;
                        for (auto musc:login["songs"]) {

                            music[j].name = musc["name"];
                            music[j].nr_vot = musc["numar de voturi"];
                            j++;
                        }


                        selectionSort(music, j);
                        write(tdL.cl, &j, sizeof(j));

                        for (int x = 0; x < j; x++) {
                            Write(tdL.cl, music[x].name);
                            write(tdL.cl, &music[x].nr_vot, sizeof(music[x].nr_vot));
                        }
                        mu.unlock();
                        break;
                    }


                    case 3: {  //top gen
                        mu.lock();
                        ifstream file("../file.json");
                        json login = json::parse(file);
                        int j = 0;
                        char gen_recive[1024] = "\0";
                        if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la dimesiune la Inregistrare-key\n");
                            break;

                        }
                        bzero(&gen_recive, size_recive + 1);

                        if (read(tdL.cl, &gen_recive, size_recive) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la client.\n");
                            break;

                        }
                        for (auto musc:login["songs"]) {

                            int size_gen;
                            size_gen = musc["genuri"]["gen"].size();
                            for (int i = 0; i < size_gen; i++) {
                                if (musc["genuri"]["gen"][i] == gen_recive) {
                                    music[j].name = musc["name"];
                                    music[j].nr_vot = musc["numar de voturi"];
                                    j++;
                                }
                            }

                        }


                        selectionSort(music, j);
                        write(tdL.cl, &j, sizeof(j));

                        for (int x = 0; x < j; x++) {
                            Write(tdL.cl, music[x].name);
                            write(tdL.cl, &music[x].nr_vot, sizeof(music[x].nr_vot));
                        }
                        mu.unlock();
                        break;
                    }

                    case 4: {  //quit
                        Close((intptr_t) arg);
                        cout << "Am terminat conexiunea cu  [Threadul]: " << tdL.idThread << endl;
                        break;

                    }
                    case 5: { //vote
                        char name_song[1024] = "\0";

                        if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la dimesiune\n");
                            break;


                        }
                        bzero(&name_song, size_recive + 1);

                        if (read(tdL.cl, &name_song, size_recive) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la client.\n");
                            break;

                        }
                        mu.lock();
                        ifstream file("../file.json");
                        json login = json::parse(file);

                        int j = login["songs"].size();
                        cout << i << endl;
                        string votat = "0";
                        for (int i = 0; i < j; i++) {
                            if (login["songs"][i]["name"] == name_song) {
                                int x = login["songs"][i]["numar de voturi"];
                                x++;
                                login["songs"][i]["numar de voturi"] = x;
                                votat = "1";
                            }


                        }
                        ofstream o("../file.json");
                        o << setw(4) << login << endl;
                        mu.unlock();
                        Write(tdL.cl, votat);
                        break;


                    }
                    case 6 : {
                        char name_song[1024] = "\0";
                        char commentariu[1024] = "\0";

                        if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                            break;

                        }
                        bzero(&name_song, size_recive + 1);

                        if (read(tdL.cl, &name_song, size_recive) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la client.\n");
                            break;

                        }

                        if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la dimesiune la Inregistrare-pass\n");
                            break;

                        }
                        bzero(&commentariu, size_recive + 1);

                        if (read(tdL.cl, &commentariu, size_recive) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la client.\n");
                            break;

                        }
                        cout << commentariu << endl;

                        mu.lock();

                        ifstream file("../file.json");
                        json songs = json::parse(file);
                        json q;
                        int j = songs["songs"].size();
                        cout << j << endl;
                        for (int x = 0; x < j; x++) {
                            if (songs["songs"][x]["name"] == name_song) {

                                songs["songs"][x]["comentariu"].push_back(commentariu);
                                cout << songs["songs"][x]["comentariu"] << endl;

                            }

                        }
                        ofstream o("../file.json");
                        o << setw(4) << songs << endl;
                        mu.unlock();
                        break;

                    }

                    case 7: { //search song
                        string name_song;
                        string link_youtube;
                        string gen;
                        string descriere;
                        string nr_gen;
                        string comm;
                        char name_song_search[1024];
                        string comentarii;
                        int numar_vot = 0;

                        if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la dimesiune -nume melodie\n");
                            break;

                        }
                        bzero(name_song_search, size_recive + 1);

                        if (read(tdL.cl, name_song_search, size_recive) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la client.\n");
                            break;

                        }

                        ifstream file("../file.json");
                        json song = json::parse(file);
                        int j = song["songs"].size();
                        for (int x = 0; x < j; x++) {
                            if (song["songs"][x]["name"] == name_song_search) {
                                name_song = song["songs"][x]["name"];
                                Write(tdL.cl, name_song);

                                descriere = song["songs"][x]["descriere"];
                                Write(tdL.cl, descriere);


                                link_youtube = song["songs"][x]["link"];
                                Write(tdL.cl, link_youtube);


                                numar_vot = song["songs"][x]["numar de voturi"];
                                write(tdL.cl, &numar_vot, sizeof(numar_vot));
                                cout << song["songs"][x]["comentariu"].size()<<endl;
                                cout << song["songs"][x]["comentariu"]<<endl;

                                int nr_com=song["songs"][x]["comentariu"].size();
                                write(tdL.cl,&nr_com, sizeof(nr_com));
                                for (int i = 0; i < nr_com; i++) {
                                    comm=song["songs"][x]["comentariu"][i];
                                    cout<<comm<<endl;
                                    Write(tdL.cl,comm);

                                }



                                int nr_gen = song["songs"][x]["genuri"]["gen"].size();
                                write(tdL.cl, &nr_gen, sizeof(nr_gen));
                                for (int i = 0; i < nr_gen; i++) {
                                    gen = song["songs"][x]["genuri"]["gen"][i];
                                    Write(tdL.cl, gen);

                                }


                            }

                        }


                        break;
                    }

                    case 8: //deconectare
                        goto citireusr;


                    case 9: {
                        ifstream file("../file.json");
                        json login = json::parse(file);
                        string usr, pre;
                        int i = login["songs"].size();
                        write(tdL.cl, &i, sizeof(i));
                        for (auto logi:login["songs"]) {
                            pre = logi["name"];
                            Write(tdL.cl, pre);

                        }

                        break;
                    }
                    default:
                        break;
                }
                if (comanda == 4)
                    break;

            }
            break;

        }
        case 1: {
            cout << "Suntem in admin" << endl;

            while (1) {
                size_recive = 0;

                using json = nlohmann::json;
                ifstream file("../file.json");
                json j;
                file >> j;


                if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                    printf("[Thread %d]\n", tdL.idThread);
                    perror("Eroare la read() de la dimesiune\n");
                    break;


                }
                bzero(&recive, size_recive + 1);

                if (read(tdL.cl, &recive, size_recive) <= 0) {
                    printf("[Thread %d]\n", tdL.idThread);
                    perror("Eroare la read() de la client.\n");

                }

                cout << "Am primit comanda " << recive << " de la [Threadul]:" << tdL.idThread << endl;
                int comanda = atoi(recive);

                switch (comanda) {
                    case 0: {
                        char name[1024] = "\0";
                        char pass[1024] = "\0";
                        char key[1024] = "\0";

                        if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                            break;

                        }
                        bzero(&name, size_recive + 1);

                        if (read(tdL.cl, &name, size_recive) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la client.\n");
                            break;

                        }


                        if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la dimesiune la Inregistrare-pass\n");
                            break;

                        }
                        bzero(&pass, size_recive + 1);

                        if (read(tdL.cl, &pass, size_recive) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la client.\n");
                            break;

                        }

                        if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la dimesiune la Inregistrare-key\n");
                            break;

                        }
                        bzero(&key, size_recive + 1);

                        if (read(tdL.cl, &key, size_recive) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la client.\n");
                            break;

                        }

                        mu.lock();
                        json x;
                        x["name"] = name;
                        x["pass"] = pass;
                        x["key"] = key;
                        j["admins"].push_back(x);
                        ofstream o("../file.json");
                        o << setw(4) << j << endl;
                        mu.unlock();
                        cout << "Am inregistrat pe " << name << " cu parola " << pass << " si cu key-ul " << key
                             << endl;

                        break;
                    }
                    case 1: {
                        char name[1024] = "\0";
                        char pass[1024] = "\0";
                        char key[1024] = "\0";
                        if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                            break;

                        }
                        bzero(&name, size_recive + 1);

                        if (read(tdL.cl, &name, size_recive) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la client.\n");
                            break;

                        }

                        if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la dimesiune la Inregistrare-pass\n");
                            break;

                        }
                        bzero(&pass, size_recive + 1);

                        if (read(tdL.cl, &pass, size_recive) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la client.\n");
                            break;

                        }


                        if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la dimesiune la Inregistrare-key\n");
                            break;

                        }
                        bzero(&key, size_recive + 1);

                        if (read(tdL.cl, &key, size_recive) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la client.\n");
                            break;

                        }

                        string ok = "0";
                        ifstream file("../file.json");
                        json login = json::parse(file);
                        for (auto logi:login["admins"]) {
                            string usersname = logi["name"];
                            string password = logi["pass"];
                            string keys = logi["key"];
                            if (pass == password && name == usersname) {
                                if (key == keys) {
                                    ok = "1";
                                    break;
                                }


                            }

                        }
                        Write(tdL.cl, ok);
                        break;

                    }

                    case 2: { // top
                        mu.lock();
                        ifstream file("../file.json");
                        json login = json::parse(file);
                        int j = 0;
                        for (auto musc:login["songs"]) {

                            music[j].name = musc["name"];
                            music[j].nr_vot = musc["numar de voturi"];
                            j++;
                        }


                        selectionSort(music, j);
                        write(tdL.cl, &j, sizeof(j));

                        for (int x = 0; x < j; x++) {
                            Write(tdL.cl, music[x].name);
                            write(tdL.cl, &music[x].nr_vot, sizeof(music[x].nr_vot));
                        }
                        mu.unlock();
                        break;
                    }


                    case 3: {
                        mu.lock();
                        ifstream file("../file.json");
                        json login = json::parse(file);
                        int j = 0;
                        char gen_recive[1024] = "\0";
                        if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la dimesiune la Inregistrare-key\n");
                            break;

                        }
                        bzero(&gen_recive, size_recive + 1);

                        if (read(tdL.cl, &gen_recive, size_recive) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la client.\n");
                            break;

                        }
                        for (auto musc:login["songs"]) {

                            int size_gen;
                            size_gen = musc["genuri"]["gen"].size();
                            for (int i = 0; i < size_gen; i++) {
                                if (musc["genuri"]["gen"][i] == gen_recive) {
                                    music[j].name = musc["name"];
                                    music[j].nr_vot = musc["numar de voturi"];
                                    j++;
                                }
                            }

                        }


                        selectionSort(music, j);
                        write(tdL.cl, &j, sizeof(j));

                        for (int x = 0; x < j; x++) {
                            Write(tdL.cl, music[x].name);
                            write(tdL.cl, &music[x].nr_vot, sizeof(music[x].nr_vot));
                        }
                        mu.unlock();
                        break;
                    }

                    case 4: {
                        Close((intptr_t) arg);
                        cout << "Am terminat conexiunea cu  [Threadul]: " << tdL.idThread << endl;
                        break;
                    }

                    case 5: {
                        char mute_usr[1024] = "\0";
                        if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la dimesiune la mute\n");
                            break;

                        }
                        bzero(&mute_usr, size_recive + 1);

                        if (read(tdL.cl, &mute_usr, size_recive) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la client.\n");
                            break;

                        }
                        cout << mute_usr << endl;
                        mu.lock();

                        ifstream file("../file.json");
                        json user = json::parse(file);
                        int j = user["users"].size();
                        for (int x = 0; x < j; x++) {
                            if (user["users"][x]["name"] == mute_usr) {
                                cout << "am dat mute la " << user["users"][x]["name"] << endl;
                                user["users"][x]["drept de vot"] = 0;

                            }

                        }
                        ofstream o("../file.json");
                        o << setw(4) << user << endl;
                        mu.unlock();


                        break;
                    }

                    case 6: {
                        char mute_usr[1024] = "\0";
                        if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la dimesiune la mute\n");
                            break;

                        }
                        bzero(&mute_usr, size_recive + 1);

                        if (read(tdL.cl, mute_usr, size_recive) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la client.\n");
                            break;

                        }
                        cout << mute_usr << endl;
                        mu.lock();

                        ifstream file("../file.json");
                        json user = json::parse(file);
                        int j = user["users"].size();
                        for (int x = 0; x < j; x++) {
                            if (user["users"][x]["name"] == mute_usr) {
                                cout << "am dat unmute la " << user["users"][x]["name"] << endl;
                                user["users"][x]["drept de vot"] = 1;


                            }

                        }
                        ofstream o("../file.json");
                        o << setw(4) << user << endl;
                        mu.unlock();


                        break;
                    }

                    case 7: {
                        ifstream file("../file.json");
                        json login = json::parse(file);
                        string usr, pre;
                        int i = login["users"].size();
                        write(tdL.cl, &i, sizeof(i));
                        for (auto logi:login["users"]) {
                            pre = logi["name"];
                            Write(tdL.cl, pre);

                        }

                        break;
                    }

                    case 8: {
                        char name_song[1024] = "\0";
                        char link_youtube[1024] = "\0";
                        char gen[1024] = "\0";
                        char descriere[1024] = "\0";
                        char nr_gen[1024] = "\0";


                        //nume melodie
                        if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                            break;

                        }
                        bzero(&name_song, size_recive + 1);

                        if (read(tdL.cl, &name_song, size_recive) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la client.\n");
                            break;

                        }


                        //link youtube
                        if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                            break;

                        }
                        bzero(&link_youtube, size_recive + 1);

                        if (read(tdL.cl, &link_youtube, size_recive) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la client.\n");
                            break;

                        }

                        //descriere melodie

                        if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                            break;

                        }
                        bzero(&descriere, size_recive + 1);

                        if (read(tdL.cl, &descriere, size_recive) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la client.\n");
                            break;

                        }

                        //numar de genuri muzicale
                        if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                            break;

                        }
                        bzero(&nr_gen, size_recive + 1);

                        if (read(tdL.cl, &nr_gen, size_recive) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la client.\n");
                            break;

                        }

                        int nrx = atoi(nr_gen);

                        mu.lock();
                        json x;
                        for (int i = 0; i < nrx; i++) {

                            if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                                printf("[Thread %d]\n", tdL.idThread);
                                perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                                break;

                            }
                            bzero(&gen, size_recive + 1);

                            if (read(tdL.cl, &gen, size_recive) <= 0) {
                                printf("[Thread %d]\n", tdL.idThread);
                                perror("Eroare la read() de la client.\n");
                                break;

                            }

                            x["genuri"]["gen"][i] = gen;
                        }
                        x["name"] = name_song;
                        x["link"] = link_youtube;
                        x["descriere"] = descriere;
                        x["numar de voturi"] = 0;
                        x["comentarii"] = "-";


                        j["songs"].push_back(x);
                        ofstream o("../file.json");
                        o << setw(4) << j << endl;
                        mu.unlock();


                        break;
                    }
                    case 9: {
                        goto citireusr;
                    }
                    case 10: {
                        char mute_usr[1024] = "\0";
                        if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la dimesiune la mute\n");
                            break;

                        }
                        bzero(&mute_usr, size_recive + 1);

                        if (read(tdL.cl, mute_usr, size_recive) <= 0) {
                            printf("[Thread %d]\n", tdL.idThread);
                            perror("Eroare la read() de la client.\n");
                            break;

                        }
                        mu.lock();

                        ifstream file("../file.json");
                        json user = json::parse(file);
                        int j = user["users"].size();
                        for (int x = 0; x < j; x++) {
                            if (user["users"][x]["name"] == mute_usr) {
                                cout << "Threadul [" << tdL.idThread << "] " << "a sters pe "<< user["users"][x]["name"] << endl;
                                user["users"].erase(x);
                            }

                        }
                        ofstream o("../file.json");
                        o << setw(4) << user << endl;
                        mu.unlock();


                        break;
                    }
                    default:
                        break;
                }
                if (comanda == 4 || comanda == 15)
                    break;

            }
            break;
        }
        default:
            break;
    }
}
