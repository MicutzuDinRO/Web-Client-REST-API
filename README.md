# Web-Client-REST-API
Nume: Micu Alexandru
Grupa: 321CC

                Tema 3 PC - Client Web. Comunicaţie cu REST API


    1. Parsare

    ** Pentru parsare am ales sa folosesc biblioteca "parson.h" (recomandata in 
    tema) deoarece e Object-Oriented adaptat pentru limbajul C si e usor de
    folosit dupa citirea documentatiei si este optimizata pe lucrul cu
    fisierele JSON.

    ** M-am gandit si la o solutie proprie care sa foloseasca RegEx, dar asta
    ar fii insemnat si o posibila hardcodare neintentionata.

********************************************************************************

    2. Requests

    ** Am modificat request-urile de tip GET si POST astfel incat sa se poata
    realiza si cu tokenul JWT emis la "enter_library" si necesar pentru
    operatiile legate de biblioteca virtuala.

    ** Functia "compute_delete_request" este aproape aceeasi ca
    "compute_get_request", exceptand faptul ca diferea tipului requestului
    trimis catre server.

********************************************************************************

    3. Comenzi

    ** Clientul transmite comenzi citite de la tastatura pana la aparitia
    comenzii "exit", atunci cand se opreste conexiunea si se inchide clientul

    ** Comenzile:
        -> register
            # Creeaza un cont nou
            # Da un request de tip POST cu username si password in payload
            # Intoarce eroare daca exista deja un cont cu username-ul respectiv
        -> login
            # Te logheaza pe un cont
            # Da un request de tip POST cu username si password in payload
            # Intoarce eroare daca nu exista deja un cont cu username-ul
            respectiv sau parola este incorecta
            # Intoarce un cookie corespunzator sesiunii
        -> enter_library
            # Te introduce in libraria virtuala
            # Da un request de tip GET cu parametru cookie-ul sesiunii
            # Intoarce eroare daca nu este deja autentificat sau daca a intrat
            deja in biblioteca
            # Intoarce un token JWT corespunzator intrarii in biblioteca
            actuala
        -> get_books
            # Intoarce lista de carti existente in biblioteca
            # Da un request de tip GET cu parametru tokenul JWT primit anterior
            # Intoarce eroare daca clientul nu are tokenul JWT
        -> get_book
            # Intoarce o carte din biblioteca proprie dupa ID-ul dat
            # Da un request de tip GET cu calea in functie de ID si cu
            parametru tokenul JWT
            # Intoarce eroare daca clientul nu are tokenul JWT sau daca nu
            exista cartea cu ID-ul dat in biblioteca utilizatorului
        -> add_book
            # Adauga o carte in biblioteca utilizatorului curent
            # Da un request de tip POST cu payload "carte" format din titlu,
            autor, gen, numar de pagini, editura si cu parametru tokenul JWT
            # Intoarce eroare daca clientul nu are tokenul JWT sau payload-ul
            are informatii lipsa sau gresite (numarul de pagini string sau mai
            mic sau egal cu 0)
        -> delete_book
            # Sterge o carte din biblioteca utilizatorului curent dupa ID
            # Da un request de tip DELETE cu calea in functie de ID si cu
            parametru tokenul JWT
            # Intoarce eroare daca cartea cu ID-ul dat nu exista in biblioteca
            utilizatorului sau acesta nu are tokenul JWT
        -> logout
            # Delogheaza utilizatorul din sesiunea curenta
            # Da un request de tip GET cu parametru cookie-ul sesiunii
            # Se sterg cookie-ul si token-ul actuale
            # Intoarce eroare daca utilizatorul e deja deconectat
        -> exit
            # Opreste executia programului si conexiunea catre server

********************************************************************************

    4. Extra

   ** M-am folosit de scheletul laboratorului 10 in mare parte a temei. Eu unul
   zic ca 90% din tema a fost intelegerea laboratorului, modificarea acestuia
   sa includa si posibilul token JWT al cererii si alegerea metodei de parsare.

   ** Am ales sa modularizez comenzile temei din cauza faptului ca se poate
   refolosi codul si intelege mai usor pe viitor, si pe seama faptului ca a
   facut debugging-ul mult mai usor :)
